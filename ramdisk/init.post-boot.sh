#!/system/bin/sh
#
# Copyright (c) 2015 Javier Sayago <admin@lonasdigital.com>
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
mount -o remount,rw -t auto /system
mount -t rootfs -o remount,rw rootfs

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

# KNOX Off
/res/ext/eliminar_knox.sh

/res/ext/killing.sh

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
/res/ext/sqlite.sh

# Iniciar Zipalign
/res/ext/zipalign.sh

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

chmod 0777 /sys/class/misc/rem_sound/rem_sound
echo "1" > /sys/class/misc/rem_sound/rem_sound

chmod 0666 /sys/class/misc/rem_sound/headphone_volume
chmod 0666 /sys/class/misc/rem_sound/speaker_volume
chmod 0666 /sys/class/misc/rem_sound/mic_level_general
chmod 0666 /sys/class/misc/rem_sound/locked_attribute
chmod 0666 /sys/class/misc/rem_sound/debug
chmod 0666 /sys/class/misc/rem_sound/register_dump
chmod 0666 /sys/class/misc/rem_sound/version

sync

sleep 0.2s

# IPv6 privacy tweak
echo "2" > /proc/sys/net/ipv6/conf/all/use_tempaddr

# TCP tweaks
echo "1" > /proc/sys/net/ipv4/tcp_low_latency
echo "0" > /proc/sys/net/ipv4/tcp_timestamps
echo "1" > /proc/sys/net/ipv4/tcp_tw_reuse
echo "1" > /proc/sys/net/ipv4/tcp_sack
echo "1" > /proc/sys/net/ipv4/tcp_dsack
echo "1" > /proc/sys/net/ipv4/tcp_tw_recycle
echo "1" > /proc/sys/net/ipv4/tcp_window_scaling
echo "1" > /proc/sys/net/ipv4/tcp_moderate_rcvbuf
echo "1" > /proc/sys/net/ipv4/route/flush
echo "2" > /proc/sys/net/ipv4/tcp_syn_retries
echo "2" > /proc/sys/net/ipv4/tcp_synack_retries
echo "5" > /proc/sys/net/ipv4/tcp_keepalive_probes
echo "10" > /proc/sys/net/ipv4/tcp_keepalive_intvl
echo "10" > /proc/sys/net/ipv4/tcp_fin_timeout
echo "2" > /proc/sys/net/ipv4/tcp_ecn

echo "50" > /sys/module/zswap/parameters/max_pool_percent

sleep 0.5s

sync

LOOP=`ls -d /sys/block/loop*`
RAM=`ls -d /sys/block/ram*`
MMC=`ls -d /sys/block/mmc*`
ZSWA=`ls -d /sys/block/vnswap*`
for j in $LOOP $RAM $MMC $ZSWA
do 
echo "0" > $j/queue/rotational
echo "2048" > $j/queue/read_ahead_kb; 

done

echo "2048" > /sys/devices/virtual/bdi/179:0/read_ahead_kb;


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

# Enable KSM
chmod 0777 /sys/kernel/mm/ksm/run
echo "1" > /sys/kernel/mm/ksm/run
chmod 0664 /sys/kernel/mm/ksm/run

# Enable Intelli_Plug
chmod 0777 /sys/module/intelli_plug/parameters/intelli_plug_active
echo "1" > /sys/module/intelli_plug/parameters/intelli_plug_active
chmod 0664 /sys/module/intelli_plug/parameters/intelli_plug_active

# Enable Simple GPU algorithm.
chmod 0777 /sys/module/simple_gpu_algorithm/parameters/simple_gpu_activate
echo "1" > /sys/module/simple_gpu_algorithm/parameters/simple_gpu_activate
chmod 0664 /sys/module/simple_gpu_algorithm/parameters/simple_gpu_activate

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


mount -t rootfs -o remount,ro rootfs
mount -o remount,ro -t auto /system
mount -o remount,ro -t auto /
