#!/system/bin/sh
#
# Deleted Knox NOTE3 and S5+ by Javilonas 26/04/2015

mount -o remount,rw,nosuid,nodev /cache;
mount -o remount,rw,nosuid,nodev /data;
mount -o remount,rw /;

# cleaning
rm -rf /system/etc/secure_storage/com.sec.knox.seandroid 2> /dev/null;
rm -rf /system/etc/secure_storage/com.sec.knox.store 2> /dev/null;
rm -rf /system/container 2> /dev/null;
rm -rf /system/containers 2> /dev/null;
rm -rf /system/preloadedsso 2> /dev/null;

rm -rf /system/app/Bridge.apk 2> /dev/null;
rm -rf /system/app/Bridge.odex 2> /dev/null;
rm -rf /system/app/Bridge/* 2> /dev/null;
rm -rf /system/app/Bridge 2> /dev/null;

rm -rf /system/priv-app/Bridge.apk 2> /dev/null;
rm -rf /system/priv-app/Bridge.odex 2> /dev/null;
rm -rf /system/priv-app/Bridge/* 2> /dev/null;
rm -rf /system/priv-app/Bridge 2> /dev/null;

rm -rf /system/app/ContainerAgent.apk 2> /dev/null;
rm -rf /system/app/ContainerAgent.odex 2> /dev/null;
rm -rf /system/app/ContainerAgent/* 2> /dev/null;
rm -rf /system/app/ContainerAgent 2> /dev/null;

rm -rf /system/priv-app/ContainerAgent.apk 2> /dev/null;
rm -rf /system/priv-app/ContainerAgent.odex 2> /dev/null;
rm -rf /system/priv-app/ContainerAgent/* 2> /dev/null;
rm -rf /system/priv-app/ContainerAgent 2> /dev/null;

rm -rf /system/app/ContainerEventsRelayManager.apk 2> /dev/null;
rm -rf /system/app/ContainerEventsRelayManager.odex 2> /dev/null;
rm -rf /system/app/ContainerEventsRelayManager/* 2> /dev/null;
rm -rf /system/app/ContainerEventsRelayManager 2> /dev/null;

rm -rf /system/priv-app/ContainerEventsRelayManager.apk 2> /dev/null;
rm -rf /system/priv-app/ContainerEventsRelayManager.odex 2> /dev/null;
rm -rf /system/priv-app/ContainerEventsRelayManager/* 2> /dev/null;
rm -rf /system/priv-app/ContainerEventsRelayManager 2> /dev/null;

rm -rf /system/app/KLMSAgent.apk 2> /dev/null;
rm -rf /system/app/KLMSAgent.odex 2> /dev/null;
rm -rf /system/app/KLMSAgent/* 2> /dev/null;
rm -rf /system/app/KLMSAgent 2> /dev/null;

rm -rf /system/priv-app/KLMSAgent.apk 2> /dev/null;
rm -rf /system/priv-app/KLMSAgent.odex 2> /dev/null;
rm -rf /system/priv-app/KLMSAgent/* 2> /dev/null;
rm -rf /system/priv-app/KLMSAgent 2> /dev/null;

rm -rf /system/priv-app/KNOXAgent.apk 2> /dev/null;
rm -rf /system/priv-app/KNOXAgent.odex 2> /dev/null;
rm -rf /system/priv-app/KNOXAgent/* 2> /dev/null;
rm -rf /system/priv-app/KNOXAgent 2> /dev/null;

rm -rf /system/app/KNOXAgent.apk 2> /dev/null;
rm -rf /system/app/KNOXAgent.odex 2> /dev/null;
rm -rf /system/app/KNOXAgent/* 2> /dev/null;
rm -rf /system/app/KNOXAgent 2> /dev/null;

rm -rf /system/app/KnoxAttestationAgent.apk 2> /dev/null;
rm -rf /system/app/KnoxAttestationAgent.odex 2> /dev/null;
rm -rf /system/app/KnoxAttestationAgent/* 2> /dev/null;
rm -rf /system/app/KnoxAttestationAgent 2> /dev/null;

rm -rf /system/priv-app/KnoxAttestationAgent.apk 2> /dev/null;
rm -rf /system/priv-app/KnoxAttestationAgent.odex 2> /dev/null;
rm -rf /system/priv-app/KnoxAttestationAgent/* 2> /dev/null;
rm -rf /system/priv-app/KnoxAttestationAgent 2> /dev/null;

rm -rf /system/app/KnoxMigrationAgent.apk 2> /dev/null;
rm -rf /system/app/KnoxMigrationAgent.odex 2> /dev/null;
rm -rf /system/app/KnoxMigrationAgent/* 2> /dev/null;
rm -rf /system/app/KnoxMigrationAgent 2> /dev/null;

rm -rf /system/priv-app/KnoxMigrationAgent.apk 2> /dev/null;
rm -rf /system/priv-app/KnoxMigrationAgent.odex 2> /dev/null;
rm -rf /system/priv-app/KnoxMigrationAgent/* 2> /dev/null;
rm -rf /system/priv-app/KnoxMigrationAgent 2> /dev/null;

rm -rf /system/app/KnoxSetupWizardClient.apk 2> /dev/null;
rm -rf /system/app/KnoxSetupWizardClient.odex 2> /dev/null;
rm -rf /system/app/KnoxSetupWizardClient/* 2> /dev/null;
rm -rf /system/app/KnoxSetupWizardClient 2> /dev/null;

rm -rf /system/priv-app/KnoxSetupWizardClient.apk 2> /dev/null;
rm -rf /system/priv-app/KnoxSetupWizardClient.odex 2> /dev/null;
rm -rf /system/priv-app/KnoxSetupWizardClient/* 2> /dev/null;
rm -rf /system/priv-app/KnoxSetupWizardClient 2> /dev/null;

rm -rf /system/app/KnoxSetupWizardStub.apk 2> /dev/null;
rm -rf /system/app/KnoxSetupWizardStub.odex 2> /dev/null;
rm -rf /system/app/KnoxSetupWizardStub/* 2> /dev/null;
rm -rf /system/app/KnoxSetupWizardStub 2> /dev/null;

rm -rf /system/priv-app/KnoxSetupWizardStub.apk 2> /dev/null;
rm -rf /system/priv-app/KnoxSetupWizardStub.odex 2> /dev/null;
rm -rf /system/priv-app/KnoxSetupWizardStub/* 2> /dev/null;
rm -rf /system/priv-app/KnoxSetupWizardStub 2> /dev/null;

rm -rf /system/app/KNOXStore.apk 2> /dev/null;
rm -rf /system/app/KNOXStore.odex 2> /dev/null;
rm -rf /system/app/KNOXStore/* 2> /dev/null;
rm -rf /system/app/KNOXStore 2> /dev/null;

rm -rf /system/priv-app/KNOXStore.apk 2> /dev/null;
rm -rf /system/priv-app/KNOXStore.odex 2> /dev/null;
rm -rf /system/priv-app/KNOXStore/* 2> /dev/null;
rm -rf /system/priv-app/KNOXStore 2> /dev/null;

rm -rf /system/app/KNOXStub.apk 2> /dev/null;
rm -rf /system/app/KNOXStub.odex 2> /dev/null;
rm -rf /system/app/KNOXStub/* 2> /dev/null;
rm -rf /system/app/KNOXStub 2> /dev/null;

rm -rf /system/priv-app/KNOXStub.apk 2> /dev/null;
rm -rf /system/priv-app/KNOXStub.odex 2> /dev/null;
rm -rf /system/priv-app/KNOXStub/* 2> /dev/null;
rm -rf /system/priv-app/KNOXStub 2> /dev/null;

rm -rf /system/app/KnoxVpnServices.apk 2> /dev/null;
rm -rf /system/app/KnoxVpnServices.odex 2> /dev/null;
rm -rf /system/app/KnoxVpnServices/* 2> /dev/null;
rm -rf /system/app/KnoxVpnServices 2> /dev/null;

rm -rf /system/priv-app/KnoxVpnServices.apk 2> /dev/null;
rm -rf /system/priv-app/KnoxVpnServices.odex 2> /dev/null;
rm -rf /system/priv-app/KnoxVpnServices/* 2> /dev/null;
rm -rf /system/priv-app/KnoxVpnServices 2> /dev/null;

rm -rf /system/app/RCPComponents.apk 2> /dev/null;
rm -rf /system/app/RCPComponents.odex 2> /dev/null;
rm -rf /system/app/RCPComponents/* 2> /dev/null;
rm -rf /system/app/RCPComponents 2> /dev/null;

rm -rf /system/priv-app/RCPComponents.apk 2> /dev/null;
rm -rf /system/priv-app/RCPComponents.odex 2> /dev/null;
rm -rf /system/priv-app/RCPComponents/* 2> /dev/null;
rm -rf /system/priv-app/RCPComponents 2> /dev/null;

rm -rf /system/app/SPDClient.apk 2> /dev/null;
rm -rf /system/app/SPDClient.odex 2> /dev/null;
rm -rf /system/app/SPDClient/* 2> /dev/null;
rm -rf /system/app/SPDClient 2> /dev/null;

rm -rf /system/priv-app/SPDClient.apk 2> /dev/null;
rm -rf /system/priv-app/SPDClient.odex 2> /dev/null;
rm -rf /system/priv-app/SPDClient/* 2> /dev/null;
rm -rf /system/priv-app/SPDClient 2> /dev/null;

rm -rf /system/app/SwitchKnoxI.apk 2> /dev/null;
rm -rf /system/app/SwitchKnoxI.odex 2> /dev/null;
rm -rf /system/app/SwitchKnoxI/* 2> /dev/null;
rm -rf /system/app/SwitchKnoxI 2> /dev/null;

rm -rf /system/app/SwitchKnoxII.apk 2> /dev/null;
rm -rf /system/app/SwitchKnoxII.odex 2> /dev/null;
rm -rf /system/app/SwitchKnoxII/* 2> /dev/null;
rm -rf /system/app/SwitchKnoxII 2> /dev/null;

rm -rf /system/priv-app/SwitchKnoxI.apk 2> /dev/null;
rm -rf /system/priv-app/SwitchKnoxI.odex 2> /dev/null;
rm -rf /system/priv-app/SwitchKnoxI/* 2> /dev/null;
rm -rf /system/priv-app/SwitchKnoxI 2> /dev/null;

rm -rf /system/priv-app/SwitchKnoxII.apk 2> /dev/null;
rm -rf /system/priv-app/SwitchKnoxII.odex 2> /dev/null;
rm -rf /system/priv-app/SwitchKnoxII/* 2> /dev/null;
rm -rf /system/priv-app/SwitchKnoxII 2> /dev/null;

rm -rf /system/app/UniversalMDMClient.apk 2> /dev/null;
rm -rf /system/app/UniversalMDMClient.odex 2> /dev/null;
rm -rf /system/app/UniversalMDMClient/* 2> /dev/null;
rm -rf /system/app/UniversalMDMClient 2> /dev/null;

rm -rf /system/priv-app/UniversalMDMClient.apk 2> /dev/null;
rm -rf /system/priv-app/UniversalMDMClient.odex 2> /dev/null;
rm -rf /system/priv-app/UniversalMDMClient/* 2> /dev/null;
rm -rf /system/priv-app/UniversalMDMClient 2> /dev/null;

rm -rf /system/app/KnoxBBCProvider.apk 2> /dev/null;
rm -rf /system/app/KnoxBBCProvider.odex 2> /dev/null;
rm -rf /system/app/KnoxBBCProvider/* 2> /dev/null;
rm -rf /system/app/KnoxBBCProvider 2> /dev/null;

rm -rf /system/priv-app/KnoxBBCProvider.apk 2> /dev/null;
rm -rf /system/priv-app/KnoxBBCProvider.odex 2> /dev/null;
rm -rf /system/priv-app/KnoxBBCProvider/* 2> /dev/null;
rm -rf /system/priv-app/KnoxBBCProvider 2> /dev/null;

rm -rf /system/app/KnoxKeyguard.apk 2> /dev/null;
rm -rf /system/app/KnoxKeyguard.odex 2> /dev/null;
rm -rf /system/app/KnoxKeyguard/* 2> /dev/null;
rm -rf /system/app/KnoxKeyguard 2> /dev/null;

rm -rf /system/priv-app/KnoxKeyguard.apk 2> /dev/null;
rm -rf /system/priv-app/KnoxKeyguard.odex 2> /dev/null;
rm -rf /system/priv-app/KnoxKeyguard/* 2> /dev/null;
rm -rf /system/priv-app/KnoxKeyguard 2> /dev/null;
                                               
rm -rf /system/app/KnoxShortcuts.apk 2> /dev/null;
rm -rf /system/app/KnoxShortcuts.odex 2> /dev/null;
rm -rf /system/app/KnoxShortcuts/* 2> /dev/null;
rm -rf /system/app/KnoxShortcuts 2> /dev/null;

rm -rf /system/priv-app/KnoxShortcuts.apk 2> /dev/null;
rm -rf /system/priv-app/KnoxShortcuts.odex 2> /dev/null;
rm -rf /system/priv-app/KnoxShortcuts/* 2> /dev/null;
rm -rf /system/priv-app/KnoxShortcuts 2> /dev/null;

rm -rf /system/app/KnoxSwitcher.apk 2> /dev/null;
rm -rf /system/app/KnoxSwitcher.odex 2> /dev/null;
rm -rf /system/app/KnoxSwitcher/* 2> /dev/null;
rm -rf /system/app/KnoxSwitcher 2> /dev/null;

rm -rf /system/priv-app/KnoxSwitcher.apk 2> /dev/null;
rm -rf /system/priv-app/KnoxSwitcher.odex 2> /dev/null;
rm -rf /system/priv-app/KnoxSwitcher/* 2> /dev/null;
rm -rf /system/priv-app/KnoxSwitcher 2> /dev/null;
             
rm -rf /system/app/KnoxTrustAgent.apk 2> /dev/null;
rm -rf /system/app/KnoxTrustAgent.odex 2> /dev/null;
rm -rf /system/app/KnoxTrustAgent/* 2> /dev/null;
rm -rf /system/app/KnoxTrustAgent 2> /dev/null;

rm -rf /system/priv-app/KnoxTrustAgent.apk 2> /dev/null;
rm -rf /system/priv-app/KnoxTrustAgent.odex 2> /dev/null;
rm -rf /system/priv-app/KnoxTrustAgent/* 2> /dev/null;
rm -rf /system/priv-app/KnoxTrustAgent 2> /dev/null;
                                      
rm -rf /system/app/KnoxVerifier.apk 2> /dev/null;
rm -rf /system/app/KnoxVerifier.odex 2> /dev/null;
rm -rf /system/app/KnoxVerifier/* 2> /dev/null;
rm -rf /system/app/KnoxVerifier 2> /dev/null;

rm -rf /system/priv-app/KnoxVerifier.apk 2> /dev/null;
rm -rf /system/priv-app/KnoxVerifier.odex 2> /dev/null;
rm -rf /system/priv-app/KnoxVerifier/* 2> /dev/null;
rm -rf /system/priv-app/KnoxVerifier 2> /dev/null;

sync;




