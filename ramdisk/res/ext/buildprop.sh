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
# Launcher and Touch
setprop ro.HOME_APP_ADJ=1
setprop debug.performance.tuning=1
setprop video.accelerate.hw=1

# Battery and Wifi
setprop wifi.supplicant_scan_interval=190
setprop pm.sleep_mode=1
setprop ro.ril.disable.power.collapse=1
setprop ro.mot.eri.losalert.delay=1000
setprop profiler.force_disable_err_rpt=1
setprop profiler.force_disable_ulog=1
setprop ro.config.nocheckin=1

# Fix App
setprop ro.kernel.android.checkjni=0
setprop ro.telephony.call_ring.delay=0

# GPU
setprop debug.sf.hw=1

# Sleep Mode
setprop ro.ril.sensor.sleep.control=1
setprop ro.wifi.hotspotUI=1
setprop ro.tether.denied=false

# Fix Optimizer Ram
setprop ro.config.dha_empty_max=36
setprop ro.config.dha_cached_max=12
setprop ro.config.dha_th_rate=2.3
setprop ro.config.dha_lmk_scale=0.545
setprop ro.config.sdha_apps_bg_max=70
setprop ro.config.sdha_apps_bg_min=8
setprop ro.config.oomminfree_high=7628,9768,11909,14515,16655,20469

sync
