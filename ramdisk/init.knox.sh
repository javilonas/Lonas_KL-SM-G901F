#!/system/bin/sh

# Disable knox
	pm disable com.sec.enterprise.knox.cloudmdm.smdms
	pm disable com.sec.knox.bridge
	pm disable com.sec.enterprise.knox.attestation
	pm disable com.sec.knox.knoxsetupwizardclient
	pm disable com.samsung.knox.rcp.components	
	pm disable com.samsung.android.securitylogagent

