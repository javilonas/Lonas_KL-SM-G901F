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
# debug_level off
# 

LOG_FILE=/data/debug_level.log

PATH=/sbin:/system/sbin:/system/bin:/system/xbin
export PATH

if [ -e $LOG_FILE ]; then
	rm $LOG_FILE
fi

if [ ! -f $LOG_FILE ]; then
	touch $LOG_FILE
fi

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) Activating debug_level.." | tee -a $LOG_FILE

# Debug level
if [ -e /sys/module/lowmemorykiller/parameters/debug_level ]; then
	chmod -h 0777 /sys/module/lowmemorykiller/parameters/debug_level
	echo "0" > /sys/module/lowmemorykiller/parameters/debug_level
	chmod -h 0666 /sys/module/lowmemorykiller/parameters/debug_level
fi

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) debug_level activated.." | tee -a $LOG_FILE
