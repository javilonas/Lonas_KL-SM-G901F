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

sleep 1

PATH=/sbin:/system/sbin:/system/bin:/system/xbin
export PATH

# Battery and Wifi
setprop wifi.supplicant_scan_interval 497
setprop pm.sleep_mode 1
setprop ro.config.nocheckin 1

# Sleep Mode
setprop ro.ril.sensor.sleep.control 1
setprop ro.wifi.hotspotUI 1
setprop ro.tether.denied false

# off fast Dormancy
setprop ro.semc.enable.fast_dormancy false

sync
