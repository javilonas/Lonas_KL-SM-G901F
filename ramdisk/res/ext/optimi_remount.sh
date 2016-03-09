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
# script optimi_remount.sh - by Javilonas
#

# Remontar todas las particiones con noatime
for k in $(mount | grep relatime | cut -d " " -f3)
do
sync
mount -o remount,noatime,nodiratime,noauto_da_alloc,barrier=0 $k
done

# Remontar y optimizar particiones con EXT4
for k in $(mount | grep ext4 | cut -d " " -f3)
do
sync
mount -o remount,commit=15 $k
done
