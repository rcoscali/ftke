CC=arm-linux-gcc

all: build install

build:
	$(CC) -g -static init.c -o init

clean:
	rm -f init

install:
	cp -f init ../ramdisk/init 

