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
# lmk tweaks for fewer empty background processes
# 

LOG_FILE=/data/permission.log

PATH=/sbin:/system/sbin:/system/bin:/system/xbin
export PATH

if [ -e $LOG_FILE ]; then
	rm $LOG_FILE
fi

if [ ! -f $LOG_FILE ]; then
	touch $LOG_FILE
fi

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) Activating permission.." | tee -a $LOG_FILE


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

# Set default values GPU on boot
echo "240000000" > /sys/class/kgsl/kgsl-3d0/devfreq/min_freq
echo "700000000" > /sys/class/kgsl/kgsl-3d0/max_gpuclk

# -10mv (Ahorro batería ON)
#echo "670 680 690 700 710 720 730 740 750 810 820 830 840 850 860 870 880 890 900 910 920 930 940 950 965 980 995 1010 1025 1030 1045 1060" > /sys/devices/system/cpu/cpu0/cpufreq/UV_mV_table

sleep 0.5s

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) permission activated.." | tee -a $LOG_FILE
