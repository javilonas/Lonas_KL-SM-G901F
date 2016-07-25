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

LOG_FILE=/data/buildprop.log

sleep 1

PATH=/sbin:/system/sbin:/system/bin:/system/xbin
export PATH

if [ -e $LOG_FILE ]; then
	rm $LOG_FILE
fi

if [ ! -f $LOG_FILE ]; then
	touch $LOG_FILE
fi

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) Activating Build PROP CUSTOM.." | tee -a $LOG_FILE

# Scrolling cache
setprop persist.sys.scrollingcache 3
setprop ro.max.fling_velocity 12000
setprop ro.min.fling_velocity 8000
setprop windowsmgr.max_events_per_sec 90
setprop touch.deviceType touchScreen
setprop touch.orientationAware 1
setprop touch.size.calibration diameter
setprop touch.size.scale 32.0368
setprop touch.size.bias -5.1253
setprop touch.size.isSummed 0
setprop touch.pressure.calibration amplitude
setprop touch.pressure.scale 0.0125
setprop touch.orientation.calibration none

# home app in memory all the time
setprop ro.HOME_APP_ADJ -17

# Net
setprop net.dns1 8.8.8.8
setprop net.dns2 8.8.4.4
setprop net.rmnet0.dns1 8.8.8.8
setprop net.rmnet0.dns2 8.8.4.4

setprop persist.telephony.support.ipv4 1
setprop persist.telephony.support.ipv6 1

# Flag_Tuner
setprop ENFORCE_PROCESS_LIMIT false
setprop MAX_SERVICE_INACTIVITY false
setprop MIN_HIDDEN_APPS false
setprop MAX_HIDDEN_APPS false
setprop CONTENT_APP_IDLE_OFFSET false
setprop EMPTY_APP_IDLE_OFFSET false
setprop MAX_ACTIVITIES false
setprop ACTIVITY_INACTIVITY_RESET_TIME false
setprop MAX_RECENT_TASKS false
setprop MIN_RECENT_TASKS false
setprop APP_SWITCH_DELAY_TIME false
setprop MAX_PROCESSES false
setprop PROC_START_TIMEOUT false
setprop CPU_MIN_CHECK_DURATION false
setprop GC_TIMEOUT false
setprop SERVICE_TIMEOUT false
setprop MIN_CRASH_INTERVAL false

# Misc_Improvements
setprop ro.config.htc.nocheckin 1
setprop ro.config.nocheckin 1
setprop ro.kernel.android.checkjni 0
setprop ro.kernel.checkjni 0
setprop profiler.force_disable_ulog 1
setprop profiler.force_disable_err_rpt 1
setprop ro.com.google.networklocation 0
setprop ro.telephony.call_ring.delay 0

sync

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) Build PROP CUSTOM activated.." | tee -a $LOG_FILE
