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
# libera pagecache cada 3 horas si esta está por debajo de 20360 kbytes
# 

/sbin/busybox renice 19 `pidof libera_ram.sh`
FREE=`free -m | grep -i mem | awk '{print $4}'`

while [ 1 ];
do
		if [ $FREE -lt 20360 ]; then
				sync
				echo "3" > /proc/sys/vm/drop_caches
				sleep 1
				echo "0" > /proc/sys/vm/drop_caches
		fi
sleep 10800
done
