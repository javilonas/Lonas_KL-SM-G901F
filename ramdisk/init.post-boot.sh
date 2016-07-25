#!/system/bin/sh
#
# Copyright (c) 2016 Javier Sayago <admin@lonasdigital.com>
# Contact: javilonas@esp-desarrolladores.com
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOG_FILE=/data/kernel_init.log

PATH=/sbin:/system/sbin:/system/bin:/system/xbin
export PATH

# Initial
mount -o remount,rw -t auto /
mount -t rootfs -o remount,rw rootfs
mount -o remount,rw -t auto /system
mount -o remount,rw /data
mount -o remount,rw /cache

if [ -e $LOG_FILE ]; then
	rm $LOG_FILE
fi

if [ ! -f $LOG_FILE ]; then
	touch $LOG_FILE
fi

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) Init post-boot.." | tee -a $LOG_FILE

if [ -f /system/xbin/busybox ]; then
	chown 0:2000 /system/xbin/busybox
	chmod 0755 /system/xbin/busybox
	/system/xbin/busybox --install -s /system/xbin
	ln -s /system/xbin/busybox /sbin/busybox
	ln -s /system/xbin/busybox /system/bin/busybox
	sync
fi

# Set environment and create symlinks: /bin, /etc, /lib, and /etc/mtab
set_environment ()
{
	# create /bin symlinks
	if [ ! -e /bin ]; then
		/system/xbin/busybox ln -s /system/bin /bin
	fi

	# create /etc symlinks
	if [ ! -e /etc ]; then
		/system/xbin/busybox ln -s /system/etc /etc
	fi

	# create /lib symlinks
	if [ ! -e /lib ]; then
		/system/xbin/busybox ln -s /system/lib /lib
	fi

	# symlink /etc/mtab to /proc/self/mounts
	if [ ! -e /system/etc/mtab ]; then
		/system/xbin/busybox ln -s /proc/self/mounts /system/etc/mtab
	fi
}

if [ -x /system/xbin/busybox ]; then
	set_environment
fi

# protect init from oom
echo "-1000" > /proc/1/oom_score_adj

# set sysrq to 2 = enable control of console logging level
echo "2" > /proc/sys/kernel/sysrq

sleep 0.9s

# Intelli_thermal
if [ -e /sys/module/intelli_thermal/parameters/enabled ]; then
    chmod -h 0777 /sys/module/intelli_thermal/parameters/enabled
    echo "Y" > /sys/module/intelli_thermal/parameters/enabled
    chmod -h 0666 /sys/module/intelli_thermal/parameters/enabled
fi

# Intelli_Plug
if [ -e /sys/module/intelli_plug/parameters/intelli_plug_active ]; then
    chmod -h 0777 /sys/module/intelli_plug/parameters/intelli_plug_active
    echo "1" > /sys/module/intelli_plug/parameters/intelli_plug_active
    chmod -h 0666 /sys/module/intelli_plug/parameters/intelli_plug_active
fi

# Simple GPU algorithm.
if [ -e /sys/module/simple_gpu_algorithm/parameters/simple_gpu_activate ]; then
    chmod -h 0777 /sys/module/simple_gpu_algorithm/parameters/simple_gpu_activate
    echo "1" > /sys/module/simple_gpu_algorithm/parameters/simple_gpu_activate
    chmod -h 0666 /sys/module/simple_gpu_algorithm/parameters/simple_gpu_activate
fi

sleep 0.5s

# Init permission
/res/ext/permission.sh & > /dev/null 2>&1

# Init Supersu daemon and Supolicy
/res/ext/supolicy.sh & > /dev/null 2>&1

sleep 0.3s

# Init KILL
/res/ext/killing.sh & > /dev/null 2>&1

# Init SQlite
/res/ext/sqlite.sh & > /dev/null 2>&1

# Init Zipalign
/res/ext/zipalign.sh & > /dev/null 2>&1

# Init Wifi Sleeper
/res/ext/Wifi_sleeper.sh & > /dev/null 2>&1

# Init Kernel Sleepers
/res/ext/Kernel_sleepers.sh & > /dev/null 2>&1

# Init Default tweaks buildprop
/res/ext/buildprop.sh & > /dev/null 2>&1

# Init Gps
/res/ext/gps.sh & > /dev/null 2>&1

sleep 0.2s

# Init Kernel panic
#/res/ext/panic.sh & > /dev/null 2>&1

# Init tweaks memory
/res/ext/tweak_memory.sh & > /dev/null 2>&1

sleep 0.3s

# Init debug level off
/res/ext/debug_level.sh & > /dev/null 2>&1

# Fast Charge
if [ -e /sys/kernel/fast_charge/force_fast_charge ]; then
    chown system.system /sys/kernel/fast_charge/force_fast_charge
    chmod -h 0777 /sys/kernel/fast_charge/force_fast_charge
    echo "1" > /sys/kernel/fast_charge/force_fast_charge
    chmod -h 0666 /sys/kernel/fast_charge/force_fast_charge
fi

#Dynamic fsync
if [ -e /sys/kernel/dyn_fsync/Dyn_fsync_active ]; then
    chmod 0777 /sys/kernel/dyn_fsync/Dyn_fsync_active
    echo "1" > /sys/kernel/dyn_fsync/Dyn_fsync_active
    chmod 0664 /sys/kernel/dyn_fsync/Dyn_fsync_active
fi

# Init Tweaks for batery (intelli thermal)
/res/ext/intelli_tweaks.sh & > /dev/null 2>&1

# Activate simple GPU alogarithm
echo "4" > /sys/module/simple_gpu_algorithm/parameters/simple_laziness
echo "6000" > /sys/module/simple_gpu_algorithm/parameters/simple_ramp_threshold

sleep 0.5s

# Init LMK CHECK
/res/ext/lmk_check.sh & > /dev/null 2>&1

# Now wait for the rom to finish booting up
# (by checking for any android process)
while ! pgrep android.process.acore ; do
	sleep 2
done

sync

stop thermal-engine

# init.d support
/res/ext/initd.sh & > /dev/null 2>&1

start thermal-engine

sleep 0.2s

sync

# Force Disable Mpdecision
stop mpdecision

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) End post-boot Kernel script is working !!!.." | tee -a $LOG_FILE

mount -o remount,ro -t auto /
mount -t rootfs -o remount,ro rootfs
mount -o remount,ro -t auto /system
mount -o remount,rw /data
mount -o remount,rw /cache
