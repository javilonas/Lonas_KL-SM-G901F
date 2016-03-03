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

PATH=/sbin:/system/sbin:/system/bin:/system/xbin
export PATH

# Inicio
mount -o remount,rw -t auto /
mount -t rootfs -o remount,rw rootfs
mount -o remount,rw -t auto /system
mount -o remount,rw /data
mount -o remount,rw /cache

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

#Set CPU Min Frequencies
echo 300000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq
echo 300000 > /sys/devices/system/cpu/cpu1/cpufreq/scaling_min_freq
echo 300000 > /sys/devices/system/cpu/cpu2/cpufreq/scaling_min_freq
echo 300000 > /sys/devices/system/cpu/cpu3/cpufreq/scaling_min_freq

#Set CPU Max Frequencies
echo 2649600 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq
echo 2649600 > /sys/devices/system/cpu/cpu1/cpufreq/scaling_max_freq
echo 2649600 > /sys/devices/system/cpu/cpu2/cpufreq/scaling_max_freq
echo 2649600 > /sys/devices/system/cpu/cpu3/cpufreq/scaling_max_freq

# barry_allen governor
chown -R system:system /sys/devices/system/cpu/cpu0/cpufreq/barry_allen
chmod -R 0666 /sys/devices/system/cpu/cpu0/cpufreq/barry_allen
chmod 0755 /sys/devices/system/cpu/cpu0/cpufreq/barry_allen
chown -R system:system /sys/devices/system/cpu/cpu1/cpufreq/barry_allen
chmod -R 0666 /sys/devices/system/cpu/cpu1/cpufreq/barry_allen
chmod 0755 /sys/devices/system/cpu/cpu1/cpufreq/barry_allen
chown -R system:system /sys/devices/system/cpu/cpu2/cpufreq/barry_allen
chmod -R 0666 /sys/devices/system/cpu/cpu2/cpufreq/barry_allen
chmod 0755 /sys/devices/system/cpu/cpu2/cpufreq/barry_allen
chown -R system:system /sys/devices/system/cpu/cpu3/cpufreq/barry_allen
chmod -R 0666 /sys/devices/system/cpu/cpu3/cpufreq/barry_allen
chmod 0755 /sys/devices/system/cpu/cpu3/cpufreq/barry_allen

# Asegurar Governor for Default
chown system system /sys/devices/system/cpu/cpufreq/barry_allen/timer_rate
chmod 0660 /sys/devices/system/cpu/cpufreq/barry_allen/timer_rate
chown system system /sys/devices/system/cpu/cpufreq/barry_allen/timer_slack
chmod 0660 /sys/devices/system/cpu/cpufreq/barry_allen/timer_slack
chown system system /sys/devices/system/cpu/cpufreq/barry_allen/min_sample_time
chmod 0660 /sys/devices/system/cpu/cpufreq/barry_allen/min_sample_time
chown system system /sys/devices/system/cpu/cpufreq/barry_allen/hispeed_freq
chmod 0660 /sys/devices/system/cpu/cpufreq/barry_allen/hispeed_freq
chown system system /sys/devices/system/cpu/cpufreq/barry_allen/target_loads
chmod 0660 /sys/devices/system/cpu/cpufreq/barry_allen/target_loads
chown system system /sys/devices/system/cpu/cpufreq/barry_allen/go_hispeed_load
chmod 0660 /sys/devices/system/cpu/cpufreq/barry_allen/go_hispeed_load
chown system system /sys/devices/system/cpu/cpufreq/barry_allen/above_hispeed_delay
chmod 0660 /sys/devices/system/cpu/cpufreq/barry_allen/above_hispeed_delay
chown system system /sys/devices/system/cpu/cpufreq/barry_allen/boost
chmod 0660 /sys/devices/system/cpu/cpufreq/barry_allen/boost
chown system system /sys/devices/system/cpu/cpufreq/barry_allen/boostpulse
chown system system /sys/devices/system/cpu/cpufreq/barry_allen/input_boost
chmod 0660 /sys/devices/system/cpu/cpufreq/barry_allen/input_boost
chown system system /sys/devices/system/cpu/cpufreq/barry_allen/boostpulse_duration
chmod 0660 /sys/devices/system/cpu/cpufreq/barry_allen/boostpulse_duration
chown system system /sys/devices/system/cpu/cpufreq/barry_allen/io_is_busy
chmod 0660 /sys/devices/system/cpu/cpufreq/barry_allen/io_is_busy

chmod 777 /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
echo "barry_allen" > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor # CPU0
chmod -h 0664 /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
chmod 777 /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
echo "barry_allen" > /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor # CPU1
chmod -h 0664 /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
chmod 777 /sys/devices/system/cpu/cpu2/cpufreq/scaling_governor
echo "barry_allen" > /sys/devices/system/cpu/cpu2/cpufreq/scaling_governor # CPU2
chmod -h 0664 /sys/devices/system/cpu/cpu2/cpufreq/scaling_governor
chmod 777 /sys/devices/system/cpu/cpu3/cpufreq/scaling_governor
echo "barry_allen" > /sys/devices/system/cpu/cpu3/cpufreq/scaling_governor # CPU3
chmod -h 0664 /sys/devices/system/cpu/cpu3/cpufreq/scaling_governor

chmod 777 /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
write /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor "barry_allen"
chmod -h 0664 /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
chmod 777 /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
write /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor "barry_allen"
chmod -h 0664 /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
chmod 777 /sys/devices/system/cpu/cpu2/cpufreq/scaling_governor
write /sys/devices/system/cpu/cpu2/cpufreq/scaling_governor "barry_allen"
chmod -h 0664 /sys/devices/system/cpu/cpu2/cpufreq/scaling_governor
chmod 777 /sys/devices/system/cpu/cpu3/cpufreq/scaling_governor
write /sys/devices/system/cpu/cpu3/cpufreq/scaling_governor "barry_allen"
chmod -h 0664 /sys/devices/system/cpu/cpu3/cpufreq/scaling_governor

sleep 0.5s

sync

#Supersu
/system/xbin/daemonsu --auto-daemon &

# Allow untrusted apps to read from debugfs (mitigate SELinux denials)
/system/xbin/supolicy --live \
	"allow untrusted_app debugfs file { open read getattr }" \
	"allow untrusted_app sysfs_lowmemorykiller file { open read getattr }" \
	"allow untrusted_app sysfs_devices_system_iosched file { open read getattr }" \
	"allow untrusted_app persist_file dir { open read getattr }" \
	"allow debuggerd gpu_device chr_file { open read getattr }" \
	"allow netd netd capability fsetid" \
	"allow netd { hostapd dnsmasq } process fork" \
	"allow { system_app shell } dalvikcache_data_file file write" \
	"allow { zygote mediaserver bootanim appdomain }  theme_data_file dir { search r_file_perms r_dir_perms }" \
	"allow { zygote mediaserver bootanim appdomain }  theme_data_file file { r_file_perms r_dir_perms }" \
	"allow system_server { rootfs resourcecache_data_file } dir { open read write getattr add_name setattr create remove_name rmdir unlink link }" \
	"allow system_server resourcecache_data_file file { open read write getattr add_name setattr create remove_name unlink link }" \
	"allow system_server dex2oat_exec file rx_file_perms" \
	"allow mediaserver mediaserver_tmpfs file execute" \
	"allow drmserver theme_data_file file r_file_perms" \
	"allow zygote system_file file write" \
	"allow atfwd property_socket sock_file write" \
	"allow untrusted_app sysfs_display file { open read write getattr add_name setattr remove_name }" \
	"allow debuggerd app_data_file dir search"

#/res/ext/killing.sh

sleep 0.3s

sync

# Detectar si existe el directorio en /system/etc y si no la crea. - by Javilonas
#
if [ ! -d "/system/etc/init.d" ] ; then
mount -o remount,rw -t auto /system
mkdir /system/etc/init.d
chmod 0755 /system/etc/init.d/*
mount -o remount,ro -t auto /system
fi

# Iniciar SQlite
#/res/ext/sqlite.sh

# Iniciar Zipalign
#/res/ext/zipalign.sh

# Iniciar Wifi Sleeper
#/res/ext/Wifi_sleeper.sh

# Iniciar Kernel Sleepers
#/res/ext/Kernel_sleepers.sh

# Kernel panic setup
if [ -e /proc/sys/kernel/panic_on_oops ]; then 
	echo "0" > /proc/sys/kernel/panic_on_oops
fi
if [ -e /proc/sys/kernel/panic ]; then 
	echo "0" > /proc/sys/kernel/panic
fi
if [ -e /proc/sys/vm/panic_on_oom ]; then 
	 echo "0" > /proc/sys/vm/panic_on_oom
fi

# Tweaks (Javilonas)
echo "5" > /proc/sys/vm/laptop_mode
echo "8" > /proc/sys/vm/page-cluster

# Carga Rápida
echo "1" > /sys/kernel/fast_charge/force_fast_charge

sync

sleep 0.2s

# Fix permisos
chmod 0664 /sys/module/lowmemorykiller/parameters/minfree
chmod 0664 /sys/module/lowmemorykiller/parameters/adj

sync

sleep 0.2s

echo "50" > /sys/module/zswap/parameters/max_pool_percent

sleep 0.5s

sync

# IO_tweak
LOOP=`ls -d /sys/block/loop* 2>/dev/null`
RAM=`ls -d /sys/block/ram* 2>/dev/null`
MMC=`ls -d /sys/block/mmc* 2>/dev/null`
ZSWA=`ls -d /sys/block/vnswap* 2>/dev/null`
for j in $LOOP $RAM $MMC $ZSWA
do 
	if [ -e $j/queue/rotational ]; then
		echo "0" > $j/queue/rotational
	fi
	if [ -e $j/queue/iostats ]; then
		echo "0" > $j/queue/iostats
	fi
	if [ -e $j/queue/nr_requests ]; then
		echo "1024" > $j/queue/nr_requests
	fi
	if [ -e $j/queue/read_ahead_kb ]; then
		echo "2048" > $j/queue/read_ahead_kb
	fi
	if [ -e $j/bdi/read_ahead_kb ]; then
		echo "2048" > $j/bdi/read_ahead_kb
        fi
done

echo "2048" > /sys/devices/virtual/bdi/179:0/read_ahead_kb

sleep 0.5s

sync

stop thermal-engine
/system/xbin/busybox run-parts /system/etc/init.d
start thermal-engine

sync

sleep 0.2s


busy=/sbin/busybox;

# lmk tweaks for fewer empty background processes
minfree=7628,9768,11909,14515,16655,20469;
lmk=/sys/module/lowmemorykiller/parameters/minfree;
minboot=`cat $lmk`;
while sleep 1; do
  if [ `cat $lmk` != $minboot ]; then
    [ `cat $lmk` != $minfree ] && echo $minfree > $lmk || exit;
  fi;
done &

sleep 0.5s

sync

# Debug level
if [ -e /sys/module/lowmemorykiller/parameters/debug_level ]; then
    chmod 0777 /sys/module/lowmemorykiller/parameters/debug_level
    echo "0" > /sys/module/lowmemorykiller/parameters/debug_level
    chmod 0644 /sys/module/lowmemorykiller/parameters/debug_level
  fi

# enlarger seeder
chmod 0777 /proc/sys/kernel/random/read_wakeup_threshold
echo "256" > /proc/sys/kernel/random/read_wakeup_threshold
chmod 0644 /proc/sys/kernel/random/read_wakeup_threshold

# Tweaks Memory
chmod 0777 /proc/sys/kernel/random/write_wakeup_threshold
echo "1376" > /proc/sys/kernel/random/write_wakeup_threshold
chmod 0644 /proc/sys/kernel/random/write_wakeup_threshold

chmod 0777 /proc/sys/vm/vfs_cache_pressure
echo "200" /proc/sys/vm/vfs_cache_pressure
chmod 0644 /proc/sys/vm/vfs_cache_pressure

chmod 0777 /proc/sys/vm/min_free_kbyte
echo "8192" /proc/sys/vm/min_free_kbytes
chmod 0644 /proc/sys/vm/min_free_kbyte

chmod 0777 /proc/sys/vm/dirty_expire_centisecs
echo "300" /proc/sys/vm/dirty_expire_centisecs
chmod 0644 /proc/sys/vm/dirty_expire_centisecs

chmod 0777 /proc/sys/vm/dirty_writeback_centisecs
echo "1500" /proc/sys/vm/dirty_writeback_centisecs
chmod 0644 /proc/sys/vm/dirty_writeback_centisecs

sleep 0.5s

sync

# Now wait for the rom to finish booting up
# (by checking for any android process)
while ! pgrep android.process.acore ; do
  sleep 2
done

# Google play services wakelock fix
sleep 40
su -c "pm enable com.google.android.gms/.update.SystemUpdateActivity"
su -c "pm enable com.google.android.gms/.update.SystemUpdateService"
su -c "pm enable com.google.android.gms/.update.SystemUpdateService$ActiveReceiver"
su -c "pm enable com.google.android.gms/.update.SystemUpdateService$Receiver"
su -c "pm enable com.google.android.gms/.update.SystemUpdateService$SecretCodeReceiver"
su -c "pm enable com.google.android.gsf/.update.SystemUpdateActivity"
su -c "pm enable com.google.android.gsf/.update.SystemUpdatePanoActivity"
su -c "pm enable com.google.android.gsf/.update.SystemUpdateService"
su -c "pm enable com.google.android.gsf/.update.SystemUpdateService$Receiver"
su -c "pm enable com.google.android.gsf/.update.SystemUpdateService$SecretCodeReceiver"


mount -o remount,ro -t auto /
mount -t rootfs -o remount,ro rootfs
mount -o remount,ro -t auto /system
mount -o remount,rw /data
mount -o remount,rw /cache
