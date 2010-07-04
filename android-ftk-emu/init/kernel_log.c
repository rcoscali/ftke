#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>

static int s_log_fd = -1;
#define LOG_BUF_MAX 512

void log_init(void)
{
	static const char *name = "/dev/__kmsg__";
	if (mknod(name, S_IFCHR | 0600, (1 << 8) | 11) == 0) {
		s_log_fd = open(name, O_WRONLY);
		fcntl(s_log_fd, F_SETFD, FD_CLOEXEC);
		unlink(name);
	}

	return;
}

void log_write(int level, const char *fmt, ...)
{
	va_list ap;
	char buf[LOG_BUF_MAX];

	if (s_log_fd < 0) 
	{
	    log_init();
	}

	if (s_log_fd < 0) 
	{
	    printf("Init kernel log failed.\n");
	    return;
    }

	va_start(ap, fmt);
	vsnprintf(buf, LOG_BUF_MAX, fmt, ap);
	buf[LOG_BUF_MAX - 1] = 0;
	va_end(ap);
	write(s_log_fd, buf, strlen(buf));

	return;
}

