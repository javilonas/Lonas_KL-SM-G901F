#!/bin/bash
#
# Build Script: Javilonas, 14-05-2015
# Javilonas <admin@lonasdigital.com>
#

start_time=`date +'%d/%m/%y %H:%M:%S'`
echo "#################### Eliminando Restos ####################"
./clean.sh > /dev/null 2>&1

# Rutas
export ROOTFS_PATH="/home/lonas/Kernel_Lonas/Lonas_KL-SM-G901F/ramdisk"
export TOOLCHAIN="/home/lonas/Kernel_Lonas/toolchains/arm-eabi-4.8/bin/arm-eabi-"
export TOOLBASE="/home/lonas/Kernel_Lonas/Lonas_KL-SM-G901F/buildtools"

echo "#################### Preparando Entorno ####################"
export KERNELDIR="/home/lonas/Kernel_Lonas/Lonas_KL-SM-G901F"

export KERNEL_VERSION="Lonas-KL-0.1"
export VERSION_KL="SM-G901F"
export REVISION="RTM"

export KBUILD_BUILD_VERSION="1"

export ARCH=arm
export SUBARCH=arm
export CROSS_COMPILE=$TOOLCHAIN

export TARGET_ARCH=arm


if [ ! -d output ]; then
        mkdir -p output
fi

cp arch/arm/configs/lonas_defconfig $KERNELDIR/output/.config

. $KERNELDIR/output/.config

echo "#################### Aplicando Permisos correctos ####################"
chmod 644 $ROOTFS_PATH/*.rc
chmod 750 $ROOTFS_PATH/init*
chmod 640 $ROOTFS_PATH/fstab*
chmod 644 $ROOTFS_PATH/default.prop
chmod 771 $ROOTFS_PATH/data
chmod 755 $ROOTFS_PATH/dev

if [ -f $ROOTFS_PATH/system/lib/modules ]; then
        chmod 755 $ROOTFS_PATH/system/lib/modules
        chmod 644 $ROOTFS_PATH/system/lib/modules/*
fi

chmod 755 $ROOTFS_PATH/proc
chmod 750 $ROOTFS_PATH/sbin
chmod 750 $ROOTFS_PATH/sbin/*
if [ -f $ROOTFS_PATH/res/ext/99SuperSUDaemon ]; then
        chmod 755 $ROOTFS_PATH/res/ext/99SuperSUDaemon
fi
chmod 755 $ROOTFS_PATH/sys
chmod 755 $ROOTFS_PATH/system
chmod 750 $ROOTFS_PATH/sbin/adbd*
chmod 750 $ROOTFS_PATH/sbin/healthd

find . -type f -name '*.h' -exec chmod 644 {} \;
find . -type f -name '*.c' -exec chmod 644 {} \;
find . -type f -name '*.py' -exec chmod 755 {} \;
find . -type f -name '*.sh' -exec chmod 755 {} \;
find . -type f -name '*.pl' -exec chmod 755 {} \;

echo "#################### Deleting Previous Build ####################"
make ARCH=arm CROSS_COMPILE=$TOOLCHAIN -j`grep 'processor' /proc/cpuinfo | wc -l` mrproper
make ARCH=arm CROSS_COMPILE=$TOOLCHAIN -j`grep 'processor' /proc/cpuinfo | wc -l` clean

find -name '*.ko' -exec rm -rf {} \;
rm -rf $KERNELDIR/arch/arm/boot/zImage > /dev/null 2>&1
rm -rf $KERNELDIR/arch/arm/boot/zImage-dtb > /dev/null 2>&1
rm -rf $KERNELDIR/arch/arm/boot/Image > /dev/null 2>&1
rm -rf $KERNELDIR/arch/arm/boot/dt.img > /dev/null 2>&1
rm -rf $KERNELDIR/arch/arm/boot/*.img > /dev/null 2>&1
rm -rf $KERNELDIR/arch/arm/boot/dts/*.dtb > /dev/null 2>&1
rm -rf $KERNELDIR/arch/arm/boot/dts/*.reverse.dts > /dev/null 2>&1

rm -rf $KERNELDIR/output/arch/arm/boot/zImage > /dev/null 2>&1
rm -rf $KERNELDIR/output/arch/arm/boot/zImage-dtb > /dev/null 2>&1
rm -rf $KERNELDIR/output/arch/arm/boot/Image > /dev/null 2>&1
rm -rf $KERNELDIR/output/arch/arm/boot/dt.img > /dev/null 2>&1
rm -rf $KERNELDIR/output/arch/arm/boot/*.img > /dev/null 2>&1
rm -rf $KERNELDIR/output/arch/arm/boot/dts/*.dtb > /dev/null 2>&1
rm -rf $KERNELDIR/output/arch/arm/boot/dts/*.reverse.dts > /dev/null 2>&1

rm $KERNELDIR/zImage > /dev/null 2>&1
rm $KERNELDIR/zImage-dtb > /dev/null 2>&1
rm $KERNELDIR/boot.dt.img > /dev/null 2>&1
rm $KERNELDIR/boot.img > /dev/null 2>&1
rm $KERNELDIR/*.ko > /dev/null 2>&1
rm $KERNELDIR/*.img > /dev/null 2>&1

rm $KERNELDIR/output/zImage > /dev/null 2>&1
rm $KERNELDIR/output/zImage-dtb > /dev/null 2>&1
rm $KERNELDIR/output/boot.dt.img > /dev/null 2>&1
rm $KERNELDIR/output/boot.img > /dev/null 2>&1
rm $KERNELDIR/output/*.ko > /dev/null 2>&1
rm $KERNELDIR/output/*.img > /dev/null 2>&1

echo "#################### Make defconfig ####################"

make ARCH=arm CROSS_COMPILE=$TOOLCHAIN -C $(pwd) O=output apq8084_sec_defconfig VARIANT_DEFCONFIG=apq8084_sec_kccat6_eur_defconfig DEBUG_DEFCONFIG=apq8084_sec_userdebug_defconfig TIMA_DEFCONFIG=tima_defconfig DMVERITY_DEFCONFIG=dmverity_defconfig SELINUX_LOG_DEFCONFIG=selinux_log_defconfig SELINUX_DEFCONFIG=selinux_defconfig

#make ARCH=arm CROSS_COMPILE=$TOOLCHAIN -C $(pwd) O=output lonas_defconfig

make -j`grep 'processor' /proc/cpuinfo | wc -l` ARCH=arm CROSS_COMPILE=$TOOLCHAIN -C $(pwd) O=output

cp -f output/arch/arm/boot/Image $(pwd)/arch/arm/boot/zImage

echo "#################### Build Ramdisk ####################"

if [ ! -d $ROOTFS_PATH/system/lib/modules ]; then
        mkdir -p $ROOTFS_PATH/system/lib/modules
fi

find . -name '*.ko' -exec cp -av {} $ROOTFS_PATH/system/lib/modules/ \;
chmod 644 $ROOTFS_PATH/system/lib/modules/*
${CROSS_COMPILE}strip --strip-unneeded $ROOTFS_PATH/system/lib/modules/*

find $ROOTFS_PATH -name .EMPTY_DIRECTORY -exec rm -rf {} \;

cd ramdisk
find . | fakeroot cpio -o -H newc | gzip > ../ramdisk.img
cd ..

echo "#################### Generar nueva dt image ####################"
$TOOLBASE/dtbTool -o output/arch/arm/boot/dt.img -s 4096 -p output/scripts/dtc/ output/arch/arm/boot/dts/
chmod a+r output/arch/arm/boot/dt.img

echo "#################### Generar nuevo boot.img ####################"
$TOOLBASE/mkbootimg --kernel output/arch/arm/boot/zImage --ramdisk ramdisk.img --cmdline 'console=null androidboot.hardware=qcom user_debug=23 msm_rtb.filter=0x3b7 dwc3_msm.cpu_to_affin=1' --base 0x00000000 --pagesize 4096 --kernel_offset 0x00008000 --ramdisk_offset 0x02600000 --tags_offset 0x02400000 --dt output/arch/arm/boot/dt.img -o boot.img

echo "Started  : $start_time"
echo "Finished : `date +'%d/%m/%y %H:%M:%S'`"
find . -name "boot.img"
find . -name "*.ko"

echo "#################### Preparando flasheables ####################"

cp boot.img $KERNELDIR/releasetools/zip
cp boot.img $KERNELDIR/releasetools/tar

cd $KERNELDIR
cd releasetools/zip
zip -0 -r $KERNEL_VERSION-$REVISION$KBUILD_BUILD_VERSION-$VERSION_KL.zip *
cd ..
cd tar
tar cf $KERNEL_VERSION-$REVISION$KBUILD_BUILD_VERSION-$VERSION_KL.tar boot.img && ls -lh $KERNEL_VERSION-$REVISION$KBUILD_BUILD_VERSION-$VERSION_KL.tar

echo "#################### Eliminando restos ####################"

rm $KERNELDIR/releasetools/zip/boot.img > /dev/null 2>&1
rm $KERNELDIR/releasetools/tar/boot.img > /dev/null 2>&1

echo "#################### Terminado ####################"
