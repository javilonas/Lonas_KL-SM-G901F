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

# Disable knox
	pm disable com.sec.enterprise.knox.cloudmdm.smdms
	pm disable com.sec.knox.bridge
	pm disable com.sec.enterprise.knox.attestation
	pm disable com.sec.knox.knoxsetupwizardclient
	pm disable com.samsung.knox.rcp.components	
	pm disable com.samsung.android.securitylogagent

