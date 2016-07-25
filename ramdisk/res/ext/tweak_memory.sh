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
# Tweaks Memory
#

LOG_FILE=/data/tweak_memory.log

PATH=/sbin:/system/sbin:/system/bin:/system/xbin
export PATH

if [ -e $LOG_FILE ]; then
	rm $LOG_FILE
fi

if [ ! -f $LOG_FILE ]; then
	touch $LOG_FILE
fi

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) Activating tweak_memory.." | tee -a $LOG_FILE

# Tweaks Memory
chown root system /sys/module/lowmemorykiller/parameters/adj
chmod -h 0666 /sys/module/lowmemorykiller/parameters/adj
chown root system /sys/module/lowmemorykiller/parameters/minfree
chmod -h 0666 /sys/module/lowmemorykiller/parameters/minfree

# enlarger seeder
chown root system /proc/sys/kernel/random/read_wakeup_threshold
chmod -h 0777 /proc/sys/kernel/random/read_wakeup_threshold
echo "256" > /proc/sys/kernel/random/read_wakeup_threshold
chmod -h 0666 /proc/sys/kernel/random/read_wakeup_threshold

chown root system /proc/sys/kernel/random/write_wakeup_threshold
chmod -h 0777 /proc/sys/kernel/random/write_wakeup_threshold
echo "1376" > /proc/sys/kernel/random/write_wakeup_threshold
chmod -h 0666 /proc/sys/kernel/random/write_wakeup_threshold

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

busybox=/sbin/busybox
RAM=$((`$busybox awk '/MemTotal/{print $2}' /proc/meminfo`/1024))
filemax=$(($RAM/4*256))
mem=`$busybox free | $busybox grep Mem | $busybox awk '{print $2}'`
totmem=`($busybox echo - | $busybox awk -v A=$mem '{print A*1024;}')`
max=`($busybox echo - | $busybox awk -v A=$totmem '{print A*75/100;}')`
$busybox sysctl -w kernel.shmmni=4096
page_size=`$busybox cat /proc/sys/kernel/shmmni`
all=`($busybox echo - | $busybox awk -v A=$totmem -v B=$page_size '{print A*80/100/B}')`

#Kernel tweak
$busybox sysctl -w kernel.sem="350 32768 64 256"
$busybox sysctl -w kernel.shmmax=$max
$busybox sysctl -w kernel.shmall=$all

#FS tweak
$busybox sysctl -w fs.nr_open=524288
$busybox sysctl -w fs.file-max=$filemax
$busybox sysctl -w fs.lease-break-time=5
$busybox sysctl -w fs.inotify.max_queued_events=32000
$busybox sysctl -w fs.inotify.max_user_instances=256
$busybox sysctl -w fs.inotify.max_user_watches=10240

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) tweak_memory activated.." | tee -a $LOG_FILE
