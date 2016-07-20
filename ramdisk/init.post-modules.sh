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

PATH=/sbin:/system/sbin:/system/bin:/system/xbin
export PATH

# Inicio
mount -o remount,rw -t auto /system/lib/modules/
mount -o remount,rw -t auto /system/etc/wifi/
mount -o remount,rw -t auto /system/etc/firmware/wlan/qca_cld/
mount -t rootfs -o remount,rw rootfs

cp -f -R /res/modules/*.ko /system/lib/modules/ > /dev/null 2>&1
sync
cp -f -R /res/etc/firmware/wlan/qca_cld/WCNSS_cfg.dat /system/etc/firmware/wlan/qca_cld/ > /dev/null 2>&1
cp -f -R /res/etc/firmware/wlan/qca_cld/*.ini /system/etc/firmware/wlan/qca_cld/ > /dev/null 2>&1
cp -f -R /res/etc/firmware/wlan/qca_cld/*.bin /system/etc/firmware/wlan/qca_cld/ > /dev/null 2>&1
sync
cp -f -R /res/etc/wifi/*.conf /system/etc/wifi/ > /dev/null 2>&1
cp -f -R /res/etc/wifi/*.ini /system/etc/wifi/ > /dev/null 2>&1
cp -f -R /res/etc/wifi/*.bin /system/etc/wifi/ > /dev/null 2>&1
sync
sed -i s/560/480/ /system/build.prop > /dev/null 2>&1

mount -t rootfs -o remount,ro rootfs
mount -o remount,rw -t auto /system/etc/firmware/wlan/qca_cld/
mount -o remount,rw -t auto /system/etc/wifi/
mount -o remount,ro -t auto /system/lib/modules/

