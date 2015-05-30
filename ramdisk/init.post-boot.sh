#!/system/bin/sh

PATH=/sbin:/system/sbin:/system/bin:/system/xbin
export PATH

# Inicio
mount -o remount,rw -t auto /system
mount -t rootfs -o remount,rw rootfs

if [ ! -f /system/xbin/busybox ]; then
rm -rf /system/xbin/busybox
fi

if [ ! -f /system/bin/busybox ]; then
rm -rf /system/bin/busybox
fi

cp -f /sbin/busybox /system/xbin/busybox

/system/xbin/busybox --install -s /system/xbin

ln -s /system/xbin/busybox /system/bin/busybox

sync

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

# Perfil consumo Medio
echo "0" > /sys/devices/system/cpu/cpufreq/barry_allen/ba_locked
echo "20000" > /sys/devices/system/cpu/cpufreq/barry_allen/above_hispeed_delay
echo "0" > /sys/devices/system/cpu/cpufreq/barry_allen/boost
echo "" > /sys/devices/system/cpu/cpufreq/barry_allen/boostpulse
echo "80000" > /sys/devices/system/cpu/cpufreq/barry_allen/boostpulse_duration
echo "90" > /sys/devices/system/cpu/cpufreq/barry_allen/go_hispeed_load
echo "1728000" > /sys/devices/system/cpu/cpufreq/barry_allen/hispeed_freq
echo "1" > /sys/devices/system/cpu/cpufreq/barry_allen/io_is_busy
echo "60000" > /sys/devices/system/cpu/cpufreq/barry_allen/min_sample_time
echo "100000" > /sys/devices/system/cpu/cpufreq/barry_allen/sampling_down_factor
echo "1036800" > /sys/devices/system/cpu/cpufreq/barry_allen/sync_freq
echo "80" > /sys/devices/system/cpu/cpufreq/barry_allen/target_loads
echo "30000" > /sys/devices/system/cpu/cpufreq/barry_allen/timer_rate
echo "80000" > /sys/devices/system/cpu/cpufreq/barry_allen/timer_slack
echo "1267200" > /sys/devices/system/cpu/cpufreq/barry_allen/up_threshold_any_cpu_freq
echo "50" > /sys/devices/system/cpu/cpufreq/barry_allen/up_threshold_any_cpu_load
echo "1" > /sys/devices/system/cpu/cpufreq/barry_allen/ba_locked
 
sleep 0.5s

sync

#Supersu
/system/xbin/daemonsu --auto-daemon &

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
echo "3642" > /proc/sys/vm/min_free_kbytes

# Zswap compresión y gestión (Javilonas)
echo "50" > /sys/module/zswap/parameters/max_pool_percent
echo "80" > /sys/module/zswap/parameters/max_compression_ratio

# Máximo ahorro batería (Javilonas)
echo "3000" > /proc/sys/vm/dirty_writeback_centisecs

sync

sleep 0.2s

chmod 777 /sys/module/lowmemorykiller/parameters/minfree
echo "12288,15360,18432,21504,24576,30720" > /sys/module/lowmemorykiller/parameters/minfree
chmod 0644 /sys/module/lowmemorykiller/parameters/minfree

chmod 777 /sys/module/lowmemorykiller/parameters/adj
echo "0,58,117,235,529,1000" > /sys/module/lowmemorykiller/parameters/adj
chmod 0644 /sys/module/lowmemorykiller/parameters/adj

sync

sleep 0.2s

stop thermal-engine
/system/xbin/busybox run-parts /system/etc/init.d
start thermal-engine

sync

sleep 0.2s

# Now wait for the rom to finish booting up
# (by checking for any android process)
while ! pgrep android.process.acore ; do
  sleep 2
done

mount -t rootfs -o remount,ro rootfs
mount -o remount,ro -t auto /system

