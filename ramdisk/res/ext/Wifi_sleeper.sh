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

LOG_FILE=/data/Wifi_sleeper.log

if [ -e $LOG_FILE ]; then
	rm $LOG_FILE
fi

if [ ! -f $LOG_FILE ]; then
	touch $LOG_FILE
fi

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) Activating wifi sleeper.." | tee -a $LOG_FILE

# wifi_idle_wait value = 15 seconds
sqlite=/system/xbin/sqlite3
wifi_idle_wait=15000

RETURN_VALUE=$($sqlite /data/data/com.android.providers.settings/databases/settings.db "select value from secure where name='wifi_idle_ms'")

if [ $RETURN_VALUE='' ]; then
   $sqlite /data/data/com.android.providers.settings/databases/settings.db "insert into secure (name, value) values ('wifi_idle_ms', $wifi_idle_wait )"
else
   $sqlite /data/data/com.android.providers.settings/databases/settings.db "update secure set value=$wifi_idle_wait where name='wifi_idle_ms'"
fi

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) Wifi sleeper activated.." | tee -a $LOG_FILE

