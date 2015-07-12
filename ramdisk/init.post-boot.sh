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

# Profile battery extreme
echo "0" > /sys/devices/system/cpu/cpufreq/barry_allen/ba_locked 
echo "20000 1400000:40000 1700000:20000" > /sys/devices/system/cpu/cpufreq/barry_allen/above_hispeed_delay 
echo "0" > /sys/devices/system/cpu/cpufreq/barry_allen/boost 
echo "" > /sys/devices/system/cpu/cpufreq/barry_allen/boostpulse 
echo "80000" > /sys/devices/system/cpu/cpufreq/barry_allen/boostpulse_duration 
echo "100" > /sys/devices/system/cpu/cpufreq/barry_allen/go_hispeed_load 
echo "300000" > /sys/devices/system/cpu/cpufreq/barry_allen/hispeed_freq 
echo "1" > /sys/devices/system/cpu/cpufreq/barry_allen/io_is_busy 
echo "5000" > /sys/devices/system/cpu/cpufreq/barry_allen/min_sample_time 
echo "100000" > /sys/devices/system/cpu/cpufreq/barry_allen/sampling_down_factor 
echo "1036800" > /sys/devices/system/cpu/cpufreq/barry_allen/sync_freq 
echo "85 900000:90 1200000:70" > /sys/devices/system/cpu/cpufreq/barry_allen/target_loads 
echo "100000" > /sys/devices/system/cpu/cpufreq/barry_allen/timer_rate 
echo "20000" > /sys/devices/system/cpu/cpufreq/barry_allen/timer_slack 
echo "1190400" > /sys/devices/system/cpu/cpufreq/barry_allen/up_threshold_any_cpu_freq 
echo "50" > /sys/devices/system/cpu/cpufreq/barry_allen/up_threshold_any_cpu_load 
echo "1" > /sys/devices/system/cpu/cpufreq/barry_allen/ba_locked 
 
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

chmod 0666 /sys/class/misc/rem_sound/rem_sound
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

stop thermal-engine
/system/xbin/busybox run-parts /system/etc/init.d
start thermal-engine

sync

sleep 0.2s

busy=/sbin/busybox;

# Iniciar Liberar Memoria
/res/ext/libera_ram.sh &
$busy renice 19 `pidof libera_ram.sh`

sleep 0.3s

/res/ext/smoothsystem.sh &
$busy renice 19 `pidof smoothsystem.sh`

sleep 0.3s

/res/ext/killing.sh &

sleep 0.3s

sync

# lmk tweaks for fewer empty background processes
minfree=7628,9768,11909,14515,16655,20469;
lmk=/sys/module/lowmemorykiller/parameters/minfree;
minboot=`cat $lmk`;
while sleep 1; do
  if [ `cat $lmk` != $minboot ]; then
    [ `cat $lmk` != $minfree ] && echo $minfree > $lmk || exit;
  fi;
done &

# wait for systemui and increase its priority
while sleep 1; do
  if [ `$busy pidof com.android.systemui` ]; then
    systemui=`$busy pidof com.android.systemui`;
    $busy renice -18 $systemui;
    $busy echo -17 > /proc/$systemui/oom_adj;
    $busy chmod 100 /proc/$systemui/oom_adj;
    exit;
  fi;
done &

# lmk whitelist for common launchers and increase launcher priority
list="com.android.launcher com.android.launcher2 com.sec.android.app.launcher com.google.android.googlequicksearchbox org.adw.launcher org.adwfreak.launcher net.alamoapps.launcher com.anddoes.launcher com.android.lmt com.chrislacy.actionlauncher.pro com.cyanogenmod.trebuchet com.gau.go.launcherex com.gtp.nextlauncher com.miui.mihome2 com.mobint.hololauncher com.mobint.hololauncher.hd com.qihoo360.launcher com.teslacoilsw.launcher com.teslacoilsw.launcher.prime com.tsf.shell org.zeam";
while sleep 60; do
  for class in $list; do
    if [ `$busy pgrep $class | head -n 1` ]; then
      launcher=`$busy pgrep $class`;
      $busy echo -17 > /proc/$launcher/oom_adj;
      $busy chmod 100 /proc/$launcher/oom_adj;
      $busy renice -18 $launcher;
    fi;
  done;
  exit;
done &

# Fix para problemas Con aplicaciones
$busy setprop ro.kernel.android.checkjni 0
$busy setprop ro.HOME_APP_ADJ -17

# Now wait for the rom to finish booting up
# (by checking for any android process)
while ! pgrep android.process.acore ; do
  sleep 2
done

# kill radio logcat to sdcard
$busy pkill -f "logcat -b radio -v time";

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

sleep 10

# Enable Dynamic FSync
echo "1" > /sys/kernel/dyn_fsync/Dyn_fsync_active

# Enable KSM
echo "1" > /sys/kernel/mm/ksm/run

# Enable Intelli_Plug
echo "1" > /sys/module/intelli_plug/parameters/intelli_plug_active

# Free Up More Ram For Apps
echo "200" > /proc/sys/vm/vfs_cache_pressure

# Enable Simple GPU algorithm.
echo "1" > /sys/module/simple_gpu_algorithm/parameters/simple_gpu_activate

sleep 10

# -50mv (Ahorro batería ON)
echo "575 630 640 650 660 670 680 690 700 710 770 780 790 800 810 820 830 840 850 860 870 880 890 900 910 925 940 955 970 985 990 1005 1020" > /sys/devices/system/cpu/cpu0/cpufreq/UV_mV_table


mount -t rootfs -o remount,ro rootfs
mount -o remount,ro -t auto /system
mount -o remount,ro -t auto /
