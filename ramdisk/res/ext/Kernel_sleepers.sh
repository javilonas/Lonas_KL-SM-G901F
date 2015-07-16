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

LOG_FILE=/data/Kernel_sleepers.log

if [ -e $LOG_FILE ]; then
	rm $LOG_FILE
fi

if [ ! -f $LOG_FILE ]; then
	touch $LOG_FILE
fi

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) Applying kernel sleepers.." | tee -a $LOG_FILE

# Kernel sleepers
mount -t debugfs -o rw none /sys/kernel/debug
if [ -e "/sys/kernel/debug/sched_features" ]; then
	echo "NO_AFFINE_WAKEUPS" > /sys/kernel/debug/sched_features
fi
if [ -e "/sys/kernel/debug/sched_features" ]; then
	echo "NO_ARCH_POWER" > /sys/kernel/debug/sched_features
fi
if [ -e "/sys/kernel/debug/sched_features" ]; then
	echo "NO_CACHE_HOT_BUDDY" > /sys/kernel/debug/sched_features
fi
if [ -e "/sys/kernel/debug/sched_features" ]; then
	echo "NO_DOUBLE_TICK" > /sys/kernel/debug/sched_features
fi
if [ -e "/sys/kernel/debug/sched_features" ]; then
	echo "NO_FORCE_SD_OVERLAP" > /sys/kernel/debug/sched_features
fi
if [ -e "/sys/kernel/debug/sched_features" ]; then
	echo "NO_GENTLE_FAIR_SLEEPERS" > /sys/kernel/debug/sched_features
fi
if [ -e "/sys/kernel/debug/sched_features" ]; then
	echo "NO_HRTICK" > /sys/kernel/debug/sched_features
fi
if [ -e "/sys/kernel/debug/sched_features" ]; then
	echo "NO_LAST_BUDDY" > /sys/kernel/debug/sched_features
fi
if [ -e "/sys/kernel/debug/sched_features" ]; then
	echo "NO_LB_BIAS" > /sys/kernel/debug/sched_features
fi
if [ -e "/sys/kernel/debug/sched_features" ]; then
	echo "NO_LB_MIN" > /sys/kernel/debug/sched_features
fi
if [ -e "/sys/kernel/debug/sched_features" ]; then
	echo "NO_NEW_FAIR_SLEEPERS" > /sys/kernel/debug/sched_features
fi
if [ -e "/sys/kernel/debug/sched_features" ]; then
	echo "NO_NEXT_BUDDY" > /sys/kernel/debug/sched_features
fi
if [ -e "/sys/kernel/debug/sched_features" ]; then
	echo "NO_NONTASK_POWER" > /sys/kernel/debug/sched_features
fi
if [ -e "/sys/kernel/debug/sched_features" ]; then
	echo "NO_NORMALIZED_SLEEPERS" > /sys/kernel/debug/sched_features
fi
if [ -e "/sys/kernel/debug/sched_features" ]; then
	echo "NO_OWNER_SPIN" > /sys/kernel/debug/sched_features
fi
if [ -e "/sys/kernel/debug/sched_features" ]; then
	echo "NO_RT_RUNTIME_SHARE" > /sys/kernel/debug/sched_features
fi
if [ -e "/sys/kernel/debug/sched_features" ]; then
	echo "NO_START_DEBIT" > /sys/kernel/debug/sched_features
fi
if [ -e "/sys/kernel/debug/sched_features" ]; then
	echo "NO_TTWU_QUEUE" > /sys/kernel/debug/sched_features
fi

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) Kernel sleepers applied" | tee -a $LOG_FILE

