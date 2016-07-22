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
# Tweaks for batery (intelli thermal)
#

LOG_FILE=/data/intelli_tweaks.log

PATH=/sbin:/system/sbin:/system/bin:/system/xbin
export PATH

if [ -e $LOG_FILE ]; then
	rm $LOG_FILE
fi

if [ ! -f $LOG_FILE ]; then
	touch $LOG_FILE
fi

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) Activating intelli_tweaks.." | tee -a $LOG_FILE

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
echo "1" > /sys/module/intelli_plug/parameters/nr_run_profile_sel

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) intelli_tweaks activated.." | tee -a $LOG_FILE
