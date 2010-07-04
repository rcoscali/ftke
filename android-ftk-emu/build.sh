#!/bin/bash

export ADNROID_SDK_IMAGES_DIR=/work/android/sdk/android-sdk-linux_86/platforms/android-7/images

if [ ! -d $ADNROID_SDK_IMAGES_DIR ]
then
	echo "Please set the env ADNROID_SDK_IMAGES_DIR first."
	exit 0
fi

if [ ! -d rootfs ]
then
	tar xf rootfs.tar.gz 
fi

rm -rf ramdisk
mkdir ramdisk

cd init && make; cd -

cd ramdisk && find . | cpio -c -o > ../initrd; cd -
gzip initrd && mv initrd.gz ramdisk.img

./mkyaffs2image rootfs system.img

for f in ramdisk.img system.img
do
	mv -fv $f $ADNROID_SDK_IMAGES_DIR
done

echo "done"

