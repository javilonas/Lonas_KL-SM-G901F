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

# Initial
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

# protect init from oom
echo "-1000" > /proc/1/oom_score_adj

# set sysrq to 2 = enable control of console logging level
echo "2" > /proc/sys/kernel/sysrq

# kill radio logcat to sdcard
pkill -f "logcat -b radio -v time"

sleep 0.9s

# Enable Intelli_thermal
chmod -h 0777 /sys/module/intelli_thermal/parameters/enabled
echo "Y" > /sys/module/intelli_thermal/parameters/enabled
chmod -h 0666 /sys/module/intelli_thermal/parameters/enabled

# Enable Intelli_Plug
chmod -h 0777 /sys/module/intelli_plug/parameters/intelli_plug_active
echo "1" > /sys/module/intelli_plug/parameters/intelli_plug_active
chmod -h 0666 /sys/module/intelli_plug/parameters/intelli_plug_active

# Enable Simple GPU algorithm.
chmod -h 0777 /sys/module/simple_gpu_algorithm/parameters/simple_gpu_activate
echo "1" > /sys/module/simple_gpu_algorithm/parameters/simple_gpu_activate
chmod -h 0666 /sys/module/simple_gpu_algorithm/parameters/simple_gpu_activate

sleep 0.5s

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

sleep 0.5s

echo 20 > /sys/module/cpu_boost/parameters/boost_ms
echo 1497600 > /sys/module/cpu_boost/parameters/sync_threshold
echo 0 > /sys/devices/system/cpu/cpufreq/barry_allen/sampling_down_factor
echo 0 > /sys/module/cpu_boost/parameters/input_boost_freq
echo 40 > /sys/module/cpu_boost/parameters/input_boost_ms

chmod 777 /dev/cpuctl/apps/cpu.notify_on_migrate
echo 0 > /dev/cpuctl/apps/cpu.notify_on_migrate
chmod -h 0644 /dev/cpuctl/apps/cpu.notify_on_migrate

chown -h system /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq
chown -h system /sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq
chown -h root.system /sys/devices/system/cpu/cpu1/online
chown -h root.system /sys/devices/system/cpu/cpu2/online
chown -h root.system /sys/devices/system/cpu/cpu3/online
chown -h system.system /sys/devices/system/cpu/cpu0/cpufreq/UV_mV_table
chmod -h 0664 /sys/devices/system/cpu/cpu1/online
chmod -h 0664 /sys/devices/system/cpu/cpu2/online
chmod -h 0664 /sys/devices/system/cpu/cpu3/online
chmod -h 0664 /sys/devices/system/cpu/cpu0/cpufreq/UV_mV_table

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

# Change cpu-boost sysfs permission
chown -h system.system /sys/module/cpu_boost/parameters/sync_threshold
chown -h system.system /sys/module/cpu_boost/parameters/boost_ms
chmod -h 0644 /sys/module/cpu_boost/parameters/sync_threshold
chmod -h 0644 /sys/module/cpu_boost/parameters/boost_ms

# Change bimc-boost sysfs permission
chown -h system.system /sys/module/qcom_cpufreq/parameters/boost_ms
chmod -h 0644 /sys/module/qcom_cpufreq/parameters/boost_ms

# Change PM debug parameters permission
chown -h radio.system /sys/module/qpnp_power_on/parameters/wake_enabled
chown -h radio.system /sys/module/qpnp_power_on/parameters/reset_enabled
chown -h radio.system /sys/module/qpnp_int/parameters/debug_mask
chown -h radio.system /sys/module/lpm_levels/parameters/secdebug
chmod -h 664 /sys/module/qpnp_power_on/parameters/wake_enabled
chmod -h 664 /sys/module/qpnp_power_on/parameters/reset_enabled
chmod -h 664 /sys/module/qpnp_int/parameters/debug_mask
chmod -h 664 /sys/module/lpm_levels/parameters/secdebug

# mdss schedule info debugging
echo 1 > /d/tracing/events/irq/mdss_dsi_cmd_mdp_busy_start/enable
echo 1 > /d/tracing/events/irq/mdss_dsi_cmd_mdp_busy_end/enable
echo 1 > /d/tracing/events/irq/mdss_dsi_isr_start/enable
echo 1 > /d/tracing/events/irq/mdss_dsi_isr_end/enable
echo 1 > /d/tracing/events/irq/mdss_mdp_isr_start/enable
echo 1 > /d/tracing/events/irq/mdss_mdp_isr_end/enable

# Volume down key(connect to PMIC RESIN) wakeup enable/disable
chown -h radio.system /sys/power/volkey_wakeup
chmod -h 664 /sys/power/volkey_wakeup
echo 0 > /sys/power/volkey_wakeup

echo 4 > /sys/module/lpm_levels/enable_low_power/l2
echo 1 > /sys/module/msm_pm/modes/cpu0/power_collapse/suspend_enabled
echo 1 > /sys/module/msm_pm/modes/cpu1/power_collapse/suspend_enabled
echo 1 > /sys/module/msm_pm/modes/cpu2/power_collapse/suspend_enabled
echo 1 > /sys/module/msm_pm/modes/cpu3/power_collapse/suspend_enabled
echo 1 > /sys/module/msm_pm/modes/cpu0/power_collapse/idle_enabled
echo 1 > /sys/module/msm_pm/modes/cpu1/power_collapse/idle_enabled
echo 1 > /sys/module/msm_pm/modes/cpu2/power_collapse/idle_enabled
echo 1 > /sys/module/msm_pm/modes/cpu3/power_collapse/idle_enabled
echo 1 > /sys/module/msm_pm/modes/cpu0/standalone_power_collapse/suspend_enabled
echo 1 > /sys/module/msm_pm/modes/cpu1/standalone_power_collapse/suspend_enabled
echo 1 > /sys/module/msm_pm/modes/cpu2/standalone_power_collapse/suspend_enabled
echo 1 > /sys/module/msm_pm/modes/cpu3/standalone_power_collapse/suspend_enabled
echo 1 > /sys/module/msm_pm/modes/cpu0/standalone_power_collapse/idle_enabled
echo 1 > /sys/module/msm_pm/modes/cpu1/standalone_power_collapse/idle_enabled
echo 1 > /sys/module/msm_pm/modes/cpu2/standalone_power_collapse/idle_enabled
echo 1 > /sys/module/msm_pm/modes/cpu3/standalone_power_collapse/idle_enabled
echo 1 > /sys/module/msm_pm/modes/cpu0/retention/idle_enabled
echo 1 > /sys/module/msm_pm/modes/cpu1/retention/idle_enabled
echo 1 > /sys/module/msm_pm/modes/cpu2/retention/idle_enabled
echo 1 > /sys/module/msm_pm/modes/cpu3/retention/idle_enabled
echo 1 > /sys/devices/system/cpu/cpu1/online
echo 1 > /sys/devices/system/cpu/cpu2/online
echo 1 > /sys/devices/system/cpu/cpu3/online

# GPU permission
chown -h radio.system /sys/class/kgsl/kgsl-3d0/default_pwrlevel
chown -h radio.system /sys/class/kgsl/kgsl-3d0/idle_timer
chmod -h 0664 /sys/class/kgsl/kgsl-3d0/default_pwrlevel
chmod -h 0664 /sys/class/kgsl/kgsl-3d0/idle_timer

# Change cpubw sysfs permission and setting
chown -h radio.system /sys/class/devfreq/0.qcom,cpubw/available_frequencies
chown -h radio.system /sys/class/devfreq/0.qcom,cpubw/available_governors
chown -h radio.system /sys/class/devfreq/0.qcom,cpubw/governor
chown -h radio.system /sys/class/devfreq/0.qcom,cpubw/max_freq
chown -h radio.system /sys/class/devfreq/0.qcom,cpubw/min_freq
chmod -h 0664 /sys/class/devfreq/0.qcom,cpubw/available_frequencies
chmod -h 0664 /sys/class/devfreq/0.qcom,cpubw/available_governors
chmod -h 0664 /sys/class/devfreq/0.qcom,cpubw/governor
chmod -h 0664 /sys/class/devfreq/0.qcom,cpubw/max_freq
chmod -h 0664 /sys/class/devfreq/0.qcom,cpubw/min_freq

#Set default values on boot
echo "240000000" > /sys/class/kgsl/kgsl-3d0/devfreq/min_freq
echo "600000000" > /sys/class/kgsl/kgsl-3d0/max_gpuclk

# -10mv (Ahorro batería ON)
echo "670 680 690 700 710 720 730 740 750 810 820 830 840 850 860 870 880 890 900 910 920 930 940 950 965 980 995 1010 1025 1030 1045 1060" > /sys/devices/system/cpu/cpu0/cpufreq/UV_mV_table

sleep 0.5s

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

# Init KILL
/res/ext/killing.sh > /dev/null 2>&1

# Init SQlite
/res/ext/sqlite.sh > /dev/null 2>&1

# Init Zipalign
/res/ext/zipalign.sh > /dev/null 2>&1

# Init Wifi Sleeper
/res/ext/Wifi_sleeper.sh > /dev/null 2>&1

# Init Kernel Sleepers
/res/ext/Kernel_sleepers.sh > /dev/null 2>&1

# Init Default tweaks buildprop
/res/ext/buildprop.sh > /dev/null 2>&1

# Init Gps
/res/ext/gps.sh > /dev/null 2>&1

# Init Liberar RAM
/res/ext/libera_ram.sh > /dev/null 2>&1

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
echo "512 512000 256 2048" > /proc/sys/kernel/sem
echo "268535656" > /proc/sys/kernel/shmmax
echo "2048" > /proc/sys/kernel/msgmni
echo "65836" > /proc/sys/kernel/msgmax
echo "524488" > /proc/sys/fs/file-max
echo "33200" > /proc/sys/fs/inotify/max_queued_events
echo "584" > /proc/sys/fs/inotify/max_user_instances
echo "10696" > /proc/sys/fs/inotify/max_user_watches
echo "0" > /proc/sys/kernel/randomize_va_space

# Tweaks Memory
chown root system /sys/module/lowmemorykiller/parameters/adj
chmod -h 0666 /sys/module/lowmemorykiller/parameters/adj
chown root system /sys/module/lowmemorykiller/parameters/minfree
chmod -h 0666 /sys/module/lowmemorykiller/parameters/minfree

chown root system /proc/sys/kernel/random/read_wakeup_threshold
chmod -h 0777 /proc/sys/kernel/random/read_wakeup_threshold
echo "256" > /proc/sys/kernel/random/read_wakeup_threshold
chmod -h 0666 /proc/sys/kernel/random/read_wakeup_threshold

chown root system /proc/sys/kernel/random/write_wakeup_threshold
chmod -h 0777 /proc/sys/kernel/random/write_wakeup_threshold
echo "384" > /proc/sys/kernel/random/write_wakeup_threshold
chmod -h 0666 /proc/sys/kernel/random/write_wakeup_threshold

chown root system /proc/sys/vm/vfs_cache_pressure
chmod -h 0777 /proc/sys/vm/vfs_cache_pressure
echo "10" /proc/sys/vm/vfs_cache_pressure
chmod -h 0666 /proc/sys/vm/vfs_cache_pressure

chown root system /proc/sys/vm/min_free_kbytes
chmod -h 0777 /proc/sys/vm/min_free_kbytes
echo "8192" /proc/sys/vm/min_free_kbytes
chmod -h 0666 /proc/sys/vm/min_free_kbytes

chown root system /proc/sys/vm/min_free_order_shif
chmod -h 0777 /proc/sys/vm/min_free_order_shif
echo "2" > /proc/sys/vm/min_free_order_shif
chmod -h 0666 /proc/sys/vm/min_free_order_shif

chown root system /proc/sys/vm/overcommit_ratio
chmod -h 0777 /proc/sys/vm/overcommit_ratio
echo "50" > /proc/sys/vm/overcommit_ratio
chmod -h 0666 /proc/sys/vm/overcommit_ratio

chown root system /proc/sys/vm/dirty_expire_centisecs
chmod -h 0777 /proc/sys/vm/dirty_expire_centisecs
echo "1000" /proc/sys/vm/dirty_expire_centisecs
chmod -h 0666 /proc/sys/vm/dirty_expire_centisecs

chown root system /proc/sys/vm/dirty_writeback_centisecs
chmod -h 0777 /proc/sys/vm/dirty_writeback_centisecs
echo "2000" /proc/sys/vm/dirty_writeback_centisecs
chmod -h 0666 /proc/sys/vm/dirty_writeback_centisecs

chown root system /proc/sys/vm/dirty_background_ratio
chmod -h 0777 /proc/sys/vm/dirty_background_ratio
echo "80" /proc/sys/vm/dirty_background_ratio
chmod -h 0666 /proc/sys/vm/dirty_background_ratio

chown root system /proc/sys/vm/dirty_ratio
chmod -h 0777 /proc/sys/vm/dirty_ratio
echo "90" > /proc/sys/vm/dirty_ratio
chmod -h 0666 /proc/sys/vm/dirty_ratio

sleep 0.5s

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
#echo "256" > /sys/block/mmcblk1/queue/read_ahead_kb
#echo "512" > /sys/block/mmcblk0/queue/read_ahead_kb
#echo "256" > /sys/block/mmcblk0rpmb/queue/read_ahead_kb

sleep 0.3s

# Debug level
if [ -e /sys/module/lowmemorykiller/parameters/debug_level ]; then
	chmod -h 0777 /sys/module/lowmemorykiller/parameters/debug_level
	echo "0" > /sys/module/lowmemorykiller/parameters/debug_level
	chmod -h 0666 /sys/module/lowmemorykiller/parameters/debug_level
fi

# enlarger seeder
chown root system /proc/sys/kernel/random/read_wakeup_threshold
chmod -h 0777 /proc/sys/kernel/random/read_wakeup_threshold
echo "256" > /proc/sys/kernel/random/read_wakeup_threshold
chmod -h 0666 /proc/sys/kernel/random/read_wakeup_threshold

# Carga Rápida
chown system.system /sys/kernel/fast_charge/force_fast_charge
chmod -h 0777 /sys/kernel/fast_charge/force_fast_charge
echo "1" > /sys/kernel/fast_charge/force_fast_charge
chmod -h 0666 /sys/kernel/fast_charge/force_fast_charge

# Enable Dynamic FSync
chmod 0777 /sys/kernel/dyn_fsync/Dyn_fsync_active
echo "1" > /sys/kernel/dyn_fsync/Dyn_fsync_active
chmod 0664 /sys/kernel/dyn_fsync/Dyn_fsync_active

# Enable KSM and optimice Tweaks
chmod -h 0777 /sys/kernel/mm/ksm/*
chown system.system /sys/kernel/mm/ksm/run
echo "1" > /sys/kernel/mm/ksm/run
chown system.system /sys/kernel/mm/ksm/pages_to_scan
echo "512" > /sys/kernel/mm/ksm/pages_to_scan
chown system.system /sys/kernel/mm/ksm/sleep_millisecs
echo "1000" > /sys/kernel/mm/ksm/sleep_millisecs
chmod -h 0666 /sys/kernel/mm/ksm/*

# Tweaks for batery (intelli thermal)
chmod -h 0777 /sys/module/intelli_thermal/parameters/limit_temp_degC
echo "67" > /sys/module/intelli_thermal/parameters/limit_temp_degC
chmod -h 0666 /sys/module/intelli_thermal/parameters/limit_temp_degC

chmod -h 0777 /sys/module/intelli_thermal/parameters/core_limit_temp_degC
echo "79" > /sys/module/intelli_thermal/parameters/core_limit_temp_degC
chmod -h 0666 /sys/module/intelli_thermal/parameters/core_limit_temp_degC

chmod -h 0777 /sys/module/intelli_plug/parameters/cpu_nr_run_threshold
echo "400" > /sys/module/intelli_plug/parameters/cpu_nr_run_threshold
chmod -h 0666 /sys/module/intelli_plug/parameters/cpu_nr_run_threshold

chmod -h 0777 /sys/module/intelli_plug/parameters/nr_run_hysteresis
echo "12" > /sys/module/intelli_plug/parameters/nr_run_hysteresis
chmod -h 0666 /sys/module/intelli_plug/parameters/nr_run_hysteresis

echo "883200" > /sys/module/intelli_plug/parameters/screen_off_max
echo "0" > /sys/module/intelli_plug/parameters/touch_boost_active
echo "2" > /sys/module/intelli_plug/parameters/nr_run_profile_sel

# Activate simple GPU alogarithm
echo "4" > /sys/module/simple_gpu_algorithm/parameters/simple_laziness
echo "6000" > /sys/module/simple_gpu_algorithm/parameters/simple_ramp_threshold

echo "50" > /sys/module/zswap/parameters/max_pool_percent

sleep 0.5s

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

# Now wait for the rom to finish booting up
# (by checking for any android process)
while ! pgrep android.process.acore ; do
	sleep 2
done

# kernel custom test
if [ -e /data/lonastest.log ]; then
	rm /data/lonastest.log
fi

echo  Kernel script is working !!! >> /data/lonastest.log
echo "excecuted on $(date +"%d-%m-%Y %r" )" >> /data/lonastest.log
echo  Done ! >> /data/lonastest.log

sleep 0.5s

$busy /sbin/fstrim -v /system >> /data/lonastest.log
$busy /sbin/fstrim -v /cache >> /data/lonastest.log
$busy /sbin/fstrim -v /data >> /data/lonastest.log

sync

stop thermal-engine
# init.d
chmod 755 /system/etc/init.d/ -R
if [ ! -d /system/etc/init.d ]; then
mkdir -p /system/etc/init.d/;
chown -R root.root /system/etc/init.d;
chmod 777 /system/etc/init.d/;
else
/sbin/busybox run-parts /system/etc/init.d
fi;

start thermal-engine

sleep 0.2s

sync

# Force Disable Mpdecision
stop mpdecision

mount -o remount,ro -t auto /
mount -t rootfs -o remount,ro rootfs
mount -o remount,ro -t auto /system
mount -o remount,rw /data
mount -o remount,rw /cache
