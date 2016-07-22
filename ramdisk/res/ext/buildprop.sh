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

# home app in memory all the time
setprop ro.HOME_APP_ADJ -17

# Net
setprop net.dns1 8.8.8.8
setprop net.dns2 8.8.4.4
setprop net.rmnet0.dns1 8.8.8.8
setprop net.rmnet0.dns2 8.8.4.4

setprop persist.telephony.support.ipv4 1
setprop persist.telephony.support.ipv6 1

sync

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) Build PROP CUSTOM activated.." | tee -a $LOG_FILE
