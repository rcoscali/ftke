#!/bin/bash

if [ ! -d /usr/local/openmoko/arm/ ]
then
	echo "No openmoko toolchain installed, install it automatically."
	wget http://www.limodev.cn/download/openmoko-toolchain.tar.gz
	tar xf openmoko-toolchain.tar.gz -C /
fi
	
export PATH=/usr/local/openmoko/arm/bin:$PATH
export ADNROID_SDK_IMAGES_DIR=/work/android/sdk/android-sdk-linux_86/platforms/android-7/images

if [ ! -d $ADNROID_SDK_IMAGES_DIR ]
then
	echo "Please set the env ADNROID_SDK_IMAGES_DIR first."
	exit 0
fi

rm -rf ramdisk
mkdir ramdisk

cd init && make; cd - >/dev/null

echo "Generating ramdisk..."

cd ramdisk && find . | cpio -c -o > ../initrd; cd - >/dev/null
gzip initrd && mv initrd.gz ramdisk.img

echo "Generating system.img..."
if [ ! -d rootfs ]
then
	tar xf rootfs.tar.gz 
fi
./mkyaffs2image rootfs system.img

echo "Copying files..."
for f in ramdisk.img system.img
do
	echo "  $f --> $ADNROID_SDK_IMAGES_DIR"
	cp -f $f $ADNROID_SDK_IMAGES_DIR
done

echo "Done"

