CC=arm-linux-gcc
all: build install
build:
	$(CC) -g -static init.c -o init
	$(CC) -g  kernel_log.c init2.c -o init2
clean:
	rm -f init init2
install:
	cp -f init ../ramdisk/init 
	cp -f init2 ../rootfs

