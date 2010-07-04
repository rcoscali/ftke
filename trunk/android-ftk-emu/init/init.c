/**
 * A simple init.
 * Copyright(C) 2010 Li XianJing <xianjimli@gmail.com>
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int init_dirs(void)
{
	umask(0);
	mkdir("/dev", 0755);
	mkdir("/system", 0777);
	mkdir("/dev/block", 0755);

	return 0;
}

int mount_filesystem(void)
{
	int ret = 0;
	ret = mount("/dev/block/mtdblock0", "/system", "yaffs2", 0, NULL);
	printf("mount /dev/block/mtdblock0 ret = %d errno=%d\n", ret, errno);

	return 0;
}

int init_dev_nodes(void)
{
	int ret = 0;
    ret = mknod("/dev/block/mtdblock0", 0755|S_IFBLK, makedev(31, 0));
	printf("mknod /dev/block/mtdblock0 ret = %d errno=%d\n", ret, errno);

    return 0;
}

int init_env(void)
{
	// setenv("LD_LIBRARY_PATH", "/lib:/usr/lib:/usr/local/lib:/opt/lib", 1);

	return 0;
}

int do_init(void)
{
	int ret = 0;
	init_dirs();
	init_dev_nodes();
	mount_filesystem();
	
	ret = chroot("/system");
	
	return 0;
}

int startup_init2(const char* name)
{
	struct stat st = {0};
	int ret = stat(name, &st);
	
	if(ret != 0)
	{
		return -1;
	}

	printf("starting the second init: %s.\n", name);

    if(fork() == 0)
    {
		ret = execl(name, name);
    }

    return ret;
}

int switch_root(void)
{
	int ret = startup_init2("/opt/bin/desktop");

	if(ret < 0)
	{
		ret = startup_init2("/init2");
	}

	return 0;
}

int main(int argc, char* argv[])
{
	do_init();

	switch_root();

	while(1)
	{
		sleep(1);
	}

	return 0;
}

