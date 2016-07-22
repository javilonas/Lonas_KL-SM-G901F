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
# Kernel panic setup
#

LOG_FILE=/data/panic.log

PATH=/sbin:/system/sbin:/system/bin:/system/xbin
export PATH

if [ -e $LOG_FILE ]; then
	rm $LOG_FILE
fi

if [ ! -f $LOG_FILE ]; then
	touch $LOG_FILE
fi

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) Activating panic.." | tee -a $LOG_FILE

if [ -e /proc/sys/kernel/panic_on_oops ]; then
	echo "0" > /proc/sys/kernel/panic_on_oops
fi
if [ -e /proc/sys/kernel/panic ]; then
	echo "0" > /proc/sys/kernel/panic
fi
if [ -e /proc/sys/vm/panic_on_oom ]; then
	echo "0" > /proc/sys/vm/panic_on_oom
fi

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) panic activated.." | tee -a $LOG_FILE
