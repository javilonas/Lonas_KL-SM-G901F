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

chmod 777 /dev/cpuctl/apps/cpu.notify_on_migrate
echo 0 > /dev/cpuctl/apps/cpu.notify_on_migrate
chmod -h 0644 /dev/cpuctl/apps/cpu.notify_on_migrate

chown -h system /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq
chown -h system /sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq
chown -h root.system /sys/devices/system/cpu/cpu1/online
chown -h root.system /sys/devices/system/cpu/cpu2/online
chown -h root.system /sys/devices/system/cpu/cpu3/online
chmod -h 664 /sys/devices/system/cpu/cpu1/online
chmod -h 664 /sys/devices/system/cpu/cpu2/online
chmod -h 664 /sys/devices/system/cpu/cpu3/online

# Change barry_allen sysfs permission
chown -h system.system /sys/devices/system/cpu/cpufreq/barry_allen/timer_rate
chown -h system.system /sys/devices/system/cpu/cpufreq/barry_allen/timer_slack
chown -h system.system /sys/devices/system/cpu/cpufreq/barry_allen/min_sample_time
chown -h system.system /sys/devices/system/cpu/cpufreq/barry_allen/hispeed_freq
chown -h system.system /sys/devices/system/cpu/cpufreq/barry_allen/target_loads
chown -h system.system /sys/devices/system/cpu/cpufreq/barry_allen/go_hispeed_load
chown -h system.system /sys/devices/system/cpu/cpufreq/barry_allen/above_hispeed_delay
chown -h system.system /sys/devices/system/cpu/cpufreq/barry_allen/boost
chown -h system.system /sys/devices/system/cpu/cpufreq/barry_allen/boostpulse
chown -h system.system /sys/devices/system/cpu/cpufreq/barry_allen/boostpulse_duration
chown -h system.system /sys/devices/system/cpu/cpufreq/barry_allen/io_is_busy
chown -h system.system /sys/devices/system/cpu/cpufreq/barry_allen/sampling_down_factor
chown -h system.system /sys/devices/system/cpu/cpufreq/barry_allen/sync_freq
chown -h system.system /sys/devices/system/cpu/cpufreq/barry_allen/up_threshold_any_cpu_freq
chown -h system.system /sys/devices/system/cpu/cpufreq/barry_allen/up_threshold_any_cpu_load
chmod -h 0644 /sys/devices/system/cpu/cpufreq/barry_allen/timer_rate
chmod -h 0644 /sys/devices/system/cpu/cpufreq/barry_allen/timer_slack
chmod -h 0644 /sys/devices/system/cpu/cpufreq/barry_allen/min_sample_time
chmod -h 0644 /sys/devices/system/cpu/cpufreq/barry_allen/hispeed_freq
chmod -h 0644 /sys/devices/system/cpu/cpufreq/barry_allen/target_loads
chmod -h 0644 /sys/devices/system/cpu/cpufreq/barry_allen/go_hispeed_load
chmod -h 0644 /sys/devices/system/cpu/cpufreq/barry_allen/above_hispeed_delay
chmod -h 0644 /sys/devices/system/cpu/cpufreq/barry_allen/boost
chmod -h 0644 /sys/devices/system/cpu/cpufreq/barry_allen/boostpulse
chmod -h 0644 /sys/devices/system/cpu/cpufreq/barry_allen/boostpulse_duration
chmod -h 0644 /sys/devices/system/cpu/cpufreq/barry_allen/io_is_busy
chmod -h 0644 /sys/devices/system/cpu/cpufreq/barry_allen/sampling_down_factor
chmod -h 0644 /sys/devices/system/cpu/cpufreq/barry_allen/sync_freq
chmod -h 0644 /sys/devices/system/cpu/cpufreq/barry_allen/up_threshold_any_cpu_freq
chmod -h 0644 /sys/devices/system/cpu/cpufreq/barry_allen/up_threshold_any_cpu_load

# Permissions for Audio
chown system.system /sys/devices/fe1af000.slim/es705-codec-gen0/keyword_grammar_path
chown system.system /sys/devices/fe1af000.slim/es705-codec-gen0/keyword_net_path
chown system.system /sys/devices/fe1af000.slim/es704-codec-gen0/keyword_grammar_path
chown system.system /sys/devices/fe1af000.slim/es704-codec-gen0/keyword_net_path

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

sleep 0.3s

sync

# init.d
chmod 755 /system/etc/init.d/ -R
if [ ! -d /system/etc/init.d ]; then
mkdir -p /system/etc/init.d/;
chown -R root.root /system/etc/init.d;
chmod 777 /system/etc/init.d/;
else
/sbin/busybox run-parts /system/etc/init.d
fi;

sync

#Set default values on boot
echo "200000000" > /sys/class/kgsl/kgsl-3d0/devfreq/min_freq
echo "600000000" > /sys/class/kgsl/kgsl-3d0/max_gpuclk

sync

# Iniciar SQlite
/res/ext/sqlite.sh

# Iniciar Zipalign
/res/ext/zipalign.sh

# Iniciar Wifi Sleeper
/res/ext/Wifi_sleeper.sh

# Iniciar Kernel Sleepers
/res/ext/Kernel_sleepers.sh

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

# RemSound
chown system.system /sys/class/misc/rem_sound/rem_sound
chmod -h 0666 /sys/class/misc/rem_sound/rem_sound
echo "1" > /sys/class/misc/rem_sound/rem_sound

chown system.system /sys/class/misc/rem_sound/headphone_volume
chmod -h 0666 /sys/class/misc/rem_sound/headphone_volume

chown system.system /sys/class/misc/rem_sound/speaker_volume
chmod -h 0666 /sys/class/misc/rem_sound/speaker_volume

chown system.system /sys/class/misc/rem_sound/mic_level_general
chmod -h 0666 /sys/class/misc/rem_sound/mic_level_general

chown system.system /sys/class/misc/rem_sound/locked_attribute
chmod -h 0666 /sys/class/misc/rem_sound/locked_attribute

chown system.system /sys/class/misc/rem_sound/debug
chmod -h 0666 /sys/class/misc/rem_sound/debug

chown system.system /sys/class/misc/rem_sound/register_dump
chmod -h 0666 /sys/class/misc/rem_sound/register_dump

chown system.system /sys/class/misc/rem_sound/version
chmod -h 0666 /sys/class/misc/rem_sound/version

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

# Enable Dynamic FSync
chmod 0777 /sys/kernel/dyn_fsync/Dyn_fsync_active
echo "1" > /sys/kernel/dyn_fsync/Dyn_fsync_active
chmod 0664 /sys/kernel/dyn_fsync/Dyn_fsync_active

# Enable KSM and optimice Tweaks
chmod 0777 /sys/kernel/mm/ksm/*
echo "1" > /sys/kernel/mm/ksm/run
echo "512" > /sys/kernel/mm/ksm/pages_to_scan
echo "1000" > /sys/kernel/mm/ksm/sleep_millisecs
chmod 0664 /sys/kernel/mm/ksm/*

# Enable Intelli_Plug
chmod 0777 /sys/module/intelli_plug/parameters/intelli_plug_active
echo "1" > /sys/module/intelli_plug/parameters/intelli_plug_active
chmod 0664 /sys/module/intelli_plug/parameters/intelli_plug_active

# Enable zen_decision.
chmod 0777 /sys/kernel/zen_decision/enabled
chmod "1" /sys/kernel/zen_decision/enabled
echo "500" > /sys/kernel/zen_decision/wake_wait_time
chmod 0664 /sys/kernel/zen_decision/enabled

# Enable Simple GPU algorithm.
chmod 0777 /sys/module/simple_gpu_algorithm/parameters/simple_gpu_activate
echo "1" > /sys/module/simple_gpu_algorithm/parameters/simple_gpu_activate
chmod 0664 /sys/module/simple_gpu_algorithm/parameters/simple_gpu_activate

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
