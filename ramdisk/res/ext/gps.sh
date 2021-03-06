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

LOG_FILE=/data/gps.log

PATH=/sbin:/system/sbin:/system/bin:/system/xbin
export PATH

if [ -e $LOG_FILE ]; then
	rm $LOG_FILE
fi

if [ ! -f $LOG_FILE ]; then
	touch $LOG_FILE
fi

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) Activating gps.." | tee -a $LOG_FILE

# Google play services wakelock fix
sleep 5
su -c pm disable com.google.android.gms/.ads.settings.AdsSettingsActivity
su -c pm disable com.google.android.gms/com.google.android.location.places.ui.aliaseditor.AliasEditorActivity
su -c pm disable com.google.android.gms/com.google.android.location.places.ui.aliaseditor.AliasEditorMapActivity
su -c pm disable com.google.android.gms/com.google.android.location.settings.ActivityRecognitionPermissionActivity
su -c pm disable com.google.android.gms/com.google.android.location.settings.GoogleLocationSettingsActivity
su -c pm disable com.google.android.gms/com.google.android.location.settings.LocationHistorySettingsActivity
su -c pm disable com.google.android.gms/com.google.android.location.settings.LocationSettingsCheckerActivity
su -c pm disable com.google.android.gms/.usagereporting.settings.UsageReportingActivity
su -c pm disable com.google.android.gms/.googlehelp.helpactivities.SystemAppTrampolineActivity
su -c pm disable com.google.android.gms/.feedback.AnnotateScreenshotActivity
su -c pm disable com.google.android.gms/.ads.adinfo.AdvertisingInfoContentProvider
su -c pm disable com.google.android.gms/com.google.android.location.reporting.service.ReportingContentProvider
su -c pm disable com.google.android.gms/com.google.android.location.internal.LocationContentProvider
su -c pm enable com.google.android.gms/.common.stats.net.contentprovider.NetworkUsageContentProvider
su -c pm disable com.google.android.gms/.ads.config.GServicesChangedReceiver
su -c pm disable com.google.android.gms/com.google.android.contextmanager.systemstate.SystemStateReceiver
su -c pm disable com.google.android.gms/.ads.jams.SystemEventReceiver
su -c pm disable com.google.android.gms/.ads.config.FlagsReceiver
su -c pm disable com.google.android.gms/.ads.social.DoritosReceiver
su -c pm disable com.google.android.gms/.analytics.AnalyticsReceiver
su -c pm disable com.google.android.gms/.analytics.internal.GServicesChangedReceiver
su -c pm disable com.google.android.gms/.common.analytics.CoreAnalyticsReceiver
su -c pm enable com.google.android.gms/.common.stats.GmsCoreStatsServiceLauncher
su -c pm disable com.google.android.gms/com.google.android.location.internal.AnalyticsSamplerReceiver
su -c pm disable com.google.android.gms/.checkin.CheckinService$ActiveReceiver
su -c pm disable com.google.android.gms/.checkin.CheckinService$ClockworkFallbackReceiver
su -c pm disable com.google.android.gms/.checkin.CheckinService$ImposeReceiver
su -c pm disable com.google.android.gms/.checkin.CheckinService$SecretCodeReceiver
su -c pm disable com.google.android.gms/.checkin.CheckinService$TriggerReceiver
su -c pm disable com.google.android.gms/.checkin.EventLogService$Receiver
su -c pm disable com.google.android.gms/com.google.android.location.reporting.service.ExternalChangeReceiver
su -c pm disable com.google.android.gms/com.google.android.location.reporting.service.GcmRegistrationReceiver
su -c pm disable com.google.android.gms/com.google.android.location.copresence.GcmRegistrationReceiver
su -c pm disable com.google.android.gms/com.google.android.location.copresence.GservicesBroadcastReceiver
su -c pm disable com.google.android.gms/com.google.android.location.internal.LocationProviderEnabler
su -c pm disable com.google.android.gms/com.google.android.location.internal.NlpNetworkProviderSettingsUpdateReceiver
su -c pm disable com.google.android.gms/com.google.android.location.network.ConfirmAlertActivity$LocationModeChangingReceiver
su -c pm disable com.google.android.gms/com.google.android.location.places.ImplicitSignalsReceiver
su -c pm disable com.google.android.gms/com.google.android.libraries.social.mediamonitor.MediaMonitor
su -c pm disable com.google.android.gms/.location.copresence.GcmBroadcastReceiver
su -c pm disable com.google.android.gms/.location.reporting.service.GcmBroadcastReceiver
su -c pm disable com.google.android.gms/.social.location.GservicesBroadcastReceiver
su -c pm disable com.google.android.gms/.update.SystemUpdateService$Receiver
su -c pm disable com.google.android.gms/.update.SystemUpdateService$OtaPolicyReceiver
su -c pm disable com.google.android.gms/.update.SystemUpdateService$SecretCodeReceiver
su -c pm disable com.google.android.gms/.update.SystemUpdateService$ActiveReceiver
su -c pm disable com.google.android.gms/com.google.android.contextmanager.service.ContextManagerService
su -c pm enable com.google.android.gms/.ads.AdRequestBrokerService
su -c pm disable com.google.android.gms/.ads.GservicesValueBrokerService
su -c pm disable com.google.android.gms/.ads.identifier.service.AdvertisingIdNotificationService
su -c pm enable com.google.android.gms/.ads.identifier.service.AdvertisingIdService
su -c pm disable com.google.android.gms/.ads.jams.NegotiationService
su -c pm disable com.google.android.gms/.ads.pan.PanService
su -c pm disable com.google.android.gms/.ads.social.GcmSchedulerWakeupService
su -c pm disable com.google.android.gms/.analytics.AnalyticsService
su -c pm disable com.google.android.gms/.analytics.internal.PlayLogReportingService
su -c pm disable com.google.android.gms/.analytics.service.AnalyticsService
su -c pm disable com.google.android.gms/.analytics.service.PlayLogMonitorIntervalService
su -c pm disable com.google.android.gms/.analytics.service.RefreshEnabledStateService
su -c pm disable com.google.android.gms/.auth.be.proximity.authorization.userpresence.UserPresenceService
su -c pm disable com.google.android.gms/.common.analytics.CoreAnalyticsIntentService
su -c pm enable com.google.android.gms/.common.stats.GmsCoreStatsService
su -c pm disable com.google.android.gms/.backup.BackupStatsService
su -c pm disable com.google.android.gms/.deviceconnection.service.DeviceConnectionAsyncService
su -c pm disable com.google.android.gms/.deviceconnection.service.DeviceConnectionServiceBroker
su -c pm disable com.google.android.gms/.wallet.service.analytics.AnalyticsIntentService
su -c pm enable com.google.android.gms/.checkin.CheckinService
su -c pm enable com.google.android.gms/.checkin.EventLogService
su -c pm disable com.google.android.gms/com.google.android.location.internal.AnalyticsUploadIntentService
su -c pm disable com.google.android.gms/com.google.android.location.reporting.service.DeleteHistoryService
su -c pm disable com.google.android.gms/com.google.android.location.reporting.service.DispatchingService
su -c pm disable com.google.android.gms/com.google.android.location.reporting.service.InternalPreferenceServiceDoNotUse
su -c pm disable com.google.android.gms/com.google.android.location.reporting.service.LocationHistoryInjectorService
su -c pm disable com.google.android.gms/com.google.android.location.reporting.service.ReportingAndroidService
su -c pm disable com.google.android.gms/com.google.android.location.reporting.service.ReportingSyncService
su -c pm disable com.google.android.gms/com.google.android.location.activity.HardwareArProviderService
su -c pm disable com.google.android.gms/com.google.android.location.fused.FusedLocationService
su -c pm disable com.google.android.gms/com.google.android.location.fused.service.FusedProviderService
su -c pm disable com.google.android.gms/com.google.android.location.geocode.GeocodeService
su -c pm disable com.google.android.gms/com.google.android.location.geofencer.service.GeofenceProviderService
su -c pm enable com.google.android.gms/com.google.android.location.internal.GoogleLocationManagerService
su -c pm disable com.google.android.gms/com.google.android.location.places.PlaylogService
su -c pm enable com.google.android.gms/com.google.android.location.places.service.GeoDataService
su -c pm enable com.google.android.gms/com.google.android.location.places.service.PlaceDetectionService
su -c pm disable com.google.android.gms/com.google.android.libraries.social.mediamonitor.MediaMonitorIntentService
su -c pm disable com.google.android.gms/.config.ConfigService
su -c pm enable com.google.android.gms/.stats.PlatformStatsCollectorService
su -c pm enable com.google.android.gms/.usagereporting.service.UsageReportingService
su -c pm enable com.google.android.gms/.update.SystemUpdateActivity
su -c pm enable com.google.android.gms/.update.SystemUpdateService
su -c pm enable com.google.android.gms/.update.SystemUpdateService$ActiveReceiver
su -c pm enable com.google.android.gms/.update.SystemUpdateService$Receiver
su -c pm enable com.google.android.gms/.update.SystemUpdateService$SecretCodeReceiver
su -c pm enable com.google.android.gsf/.update.SystemUpdateActivity
su -c pm enable com.google.android.gsf/.update.SystemUpdatePanoActivity
su -c pm enable com.google.android.gsf/.update.SystemUpdateService
su -c pm enable com.google.android.gsf/.update.SystemUpdateService$Receiver
su -c pm enable com.google.android.gsf/.update.SystemUpdateService$SecretCodeReceiver
su -c pm enable com.google.android.gms/com.google.android.location.network.ConfirmAlertActivity
su -c pm enable com.google.android.gms/com.google.android.location.network.LocationProviderChangeReceiver
su -c pm enable com.google.android.gms/com.google.android.location.internal.server.GoogleLocationService
su -c pm enable com.google.android.gms/com.google.android.location.internal.PendingIntentCallbackService
su -c pm enable com.google.android.gms/com.google.android.location.network.NetworkLocationService
su -c pm enable com.google.android.gms/com.google.android.location.util.PreferenceService
su -c pm disable com.google.android.gms/.fitness.settings.FitnessSettingsActivity
su -c pm disable com.google.android.gms/.fitness.sync.FitnessContentProvider
su -c pm disable com.google.android.gms/.fitness.disconnect.FitDisconnectReceiver
su -c pm disable com.google.android.gms/.fitness.sensors.sample.CollectSensorReceiver
su -c pm disable com.google.android.gms/.fitness.service.FitnessInitReceiver
su -c pm disable com.google.android.gms/.fitness.wearables.WearableSyncServiceReceiver
su -c pm disable com.google.android.gms/.fitness.disconnect.FitCleanupService
su -c pm disable com.google.android.gms/.fitness.sensors.activity.ActivityRecognitionService
su -c pm disable com.google.android.gms/.fitness.sensors.floorchange.FloorChangeRecognitionService
su -c pm disable com.google.android.gms/.fitness.sensors.sample.CollectSensorService
su -c pm disable com.google.android.gms/.fitness.service.DebugIntentService
su -c pm disable com.google.android.gms/.fitness.service.ble.FitBleBroker
su -c pm disable com.google.android.gms/.fitness.service.config.FitConfigBroker
su -c pm disable com.google.android.gms/.fitness.service.history.FitHistoryBroker
su -c pm disable com.google.android.gms/.fitness.service.internal.FitInternalBroker
su -c pm disable com.google.android.gms/.fitness.service.proxy.FitProxyBroker
su -c pm disable com.google.android.gms/.fitness.service.recording.FitRecordingBroker
su -c pm disable com.google.android.gms/.fitness.service.sensors.FitSensorsBroker
su -c pm disable com.google.android.gms/.fitness.service.sessions.FitSessionsBroker
su -c pm disable com.google.android.gms/.fitness.store.maintenance.StoreMaintenanceService
su -c pm disable com.google.android.gms/.fitness.sync.FitnessSyncAdapterService
su -c pm disable com.google.android.gms/.fitness.wearables.WearableSyncService
su -c pm disable com.google.android.gms/.car.BroadcastRedirectActivity
su -c pm disable com.google.android.gms/.car.CarErrorDisplayActivity
su -c pm disable com.google.android.gms/.car.CarHomeActivity
su -c pm disable com.google.android.gms/.car.CarHomeActivity1
su -c pm disable com.google.android.gms/.car.CarHomeActivity2
su -c pm disable com.google.android.gms/.car.CarServiceSettingsActivity
su -c pm disable com.google.android.gms/.car.FirstActivity
su -c pm disable com.google.android.gms/.car.SetupActivity
su -c pm disable com.google.android.gms/.car.CarIntentService
su -c pm enable com.google.android.gms/.car.CarService
su -c pm disable com.google.android.gms/.car.InCallServiceImpl
su -c pm disable com.google.android.gms/.car.VoiceActionService
su -c pm disable com.google.android.gms/.car.diagnostics.CrashReporterService
su -c pm disable com.google.android.gms/.feedback.FeedbackActivity
su -c pm disable com.google.android.gms/.feedback.IntentListenerFeedbackActivity
su -c pm disable com.google.android.gms/.feedback.PreviewActivity
su -c pm disable com.google.android.gms/.feedback.PreviewScreenshotActivity
su -c pm disable com.google.android.gms/.feedback.ShowTextActivity
su -c pm disable com.google.android.gms/.feedback.SuggestionsActivity
su -c pm disable com.google.android.gms/.feedback.FeedbackConnectivityReceiver
su -c pm disable com.google.android.gms/.feedback.FeedbackAsyncService
su -c pm enable com.google.android.gms/.feedback.FeedbackService
su -c pm disable com.google.android.gms/.feedback.LegacyBugReportService
su -c pm disable com.google.android.gms/.feedback.SendService
su -c pm disable com.google.android.gms/.googlehelp.helpactivities.ClickToCallActivity
su -c pm disable com.google.android.gms/.googlehelp.helpactivities.EmailActivity
su -c pm disable com.google.android.gms/.googlehelp.helpactivities.HelpActivity
su -c pm disable com.google.android.gms/.googlehelp.helpactivities.OpenHangoutActivity
su -c pm disable com.google.android.gms/.googlehelp.helpactivities.OpenHelpActivity
su -c pm disable com.google.android.gms/.googlehelp.helpactivities.RealtimeSupportClassifierActivity
su -c pm disable com.google.android.gms/.googlehelp.webview.GoogleHelpWebViewActivity
su -c pm disable com.google.android.gms/.googlehelp.GcmBroadcastReceiver
su -c pm disable com.google.android.gms/.googlehelp.metrics.ConnectivityBroadcastReceiver
su -c pm disable com.google.android.gms/.googlehelp.metrics.MetricsReportService
su -c pm disable com.google.android.gms/.googlehelp.metrics.ReportBatchedMetricsService
su -c pm disable com.google.android.gms/.googlehelp.service.ChatStatusUpdateService
su -c pm disable com.google.android.gms/.googlehelp.service.ClearHelpHistoryIntentService
su -c pm disable com.google.android.gms/.googlehelp.service.GoogleHelpService
su -c pm disable com.google.android.gms/.googlehelp.service.VideoCallStatusUpdateService
su -c pm disable com.google.android.gms/.kids.account.AccountRemovalConfirmActivity
su -c pm disable com.google.android.gms/.kids.account.AccountSetupActivity
su -c pm disable com.google.android.gms/.kids.account.ShowAppsActivity
su -c pm disable com.google.android.gms/.kids.account.activities.RegisterProfileOwnerActivity
su -c pm disable com.google.android.gms/.kids.creation.activities.FamilyCreationActivity
su -c pm disable com.google.android.gms/.kids.chimera.KidsDataProviderProxy
su -c pm disable com.google.android.gms/.kids.account.receiver.ProfileOwnerReceiver
su -c pm disable com.google.android.gms/.kids.chimera.AccountChangeReceiverProxy
su -c pm disable com.google.android.gms/.kids.chimera.AccountSetupCompletedReceiverProxy
su -c pm disable com.google.android.gms/.kids.chimera.DeviceTimeAndDateChangeReceiverProxy
su -c pm disable com.google.android.gms/.kids.chimera.InternalEventReceiverLmpProxy
su -c pm disable com.google.android.gms/.kids.chimera.InternalEventReceiverProxy
su -c pm disable com.google.android.gms/.kids.chimera.LocationModeChangedReceiverProxy
su -c pm disable com.google.android.gms/.kids.chimera.PackageChangedReceiverProxy
su -c pm disable com.google.android.gms/.kids.chimera.SystemEventReceiverProxy
su -c pm disable com.google.android.gms/.kids.chimera.LongRunningServiceProxy
su -c pm disable com.google.android.gms/.kids.common.sync.ManualSyncReceiver
su -c pm disable com.google.android.gms/.kids.GcmReceiverService
su -c pm disable com.google.android.gms/.kids.chimera.AccountSetupServiceProxy
su -c pm disable com.google.android.gms/.kids.chimera.KidsApiServiceProxy
su -c pm disable com.google.android.gms/.kids.chimera.KidsDataSyncServiceProxy
su -c pm disable com.google.android.gms/.kids.chimera.KidsServiceProxy
su -c pm disable com.google.android.gms/.kids.chimera.SlowOperationServiceProxy
su -c pm disable com.google.android.gms/.kids.chimera.TimeoutsSystemAlertServiceProxy
su -c pm disable com.google.android.gms/.kids.device.RingService
su -c pm disable com.google.android.gms/.nearby.messages.settings.NearbyMessagesAppOptInActivity
su -c pm disable com.google.android.gms/.nearby.settings.NearbyAccessActivity
su -c pm disable com.google.android.gms/.nearby.settings.NearbySettingsActivity
su -c pm disable com.google.android.gms/.nearby.messages.NearbyMessagesBroadcastReceiver
su -c pm disable com.google.android.gms/.nearby.settings.NearbyAppUninstallReceiver
su -c pm disable com.google.android.gms/.nearby.bootstrap.service.NearbyBootstrapService
su -c pm enable com.google.android.gms/.nearby.connection.service.NearbyConnectionsAndroidService
su -c pm enable com.google.android.gms/.nearby.connection.service.NearbyConnectionsAsyncService
su -c pm disable com.google.android.gms/.nearby.messages.service.NearbyMessagesService
su -c pm disable com.google.android.gms/.nearby.sharing.service.NearbySharingService
su -c pm disable com.google.android.gms/.phenotype.receiver.PhenotypeBroadcastReceiver
su -c pm disable com.google.android.gms/.phenotype.service.PhenotypeCommitService
su -c pm disable com.google.android.gms/.phenotype.service.PhenotypeIntentService
su -c pm enable com.google.android.gms/.phenotype.service.PhenotypeService
su -c pm disable com.google.android.gms/.phenotype.service.sync.PhenotypeConfigurator
su -c pm disable com.google.android.gms/.phenotype.service.util.PhenotypeDebugService
su -c pm disable com.google.android.gms/.pseudonymous.service.PseudonymousIdIntentService
su -c pm disable com.google.android.gms/.pseudonymous.service.PseudonymousIdService
su -c pm disable com.google.android.gms/.photos.autobackup.ui.AutoBackupSettingsActivity
su -c pm disable com.google.android.gms/.photos.autobackup.ui.AutoBackupSettingsRedirectActivity
su -c pm disable com.google.android.gms/.photos.autobackup.ui.LocalFoldersBackupSettings
su -c pm disable com.google.android.gms/.photos.autobackup.ui.promo.AutoBackupPromoActivity
su -c pm disable com.google.android.gms/.photos.InitializePhotosIntentReceiver
su -c pm disable com.google.android.gms/.photos.autobackup.AutoBackupWorkService
su -c pm enable com.google.android.gms/.photos.autobackup.service.AutoBackupService
su -c pm enable com.google.android.gms/.playlog.service.MonitorAlarmReceiver
su -c pm enable com.google.android.gms/.playlog.service.WallClockChangedReceiver
su -c pm enable com.google.android.gms/.playlog.service.MonitorService
su -c pm enable com.google.android.gms/.playlog.service.PlayLogBrokerService
su -c pm enable com.google.android.gms/.playlog.service.PlayLogIntentService
su -c pm enable com.google.android.gms/.playlog.uploader.RequestUploadService
su -c pm enable com.google.android.gms/.playlog.uploader.UploaderService
su -c pm disable com.google.android.gms/.plus.activity.AccountSignUpActivity
su -c pm disable com.google.android.gms/.plus.apps.ListAppsActivity
su -c pm disable com.google.android.gms/.plus.apps.ManageAppActivity
su -c pm disable com.google.android.gms/.plus.apps.ManageDeviceActivity
su -c pm disable com.google.android.gms/.plus.apps.ManageMomentActivity
su -c pm disable com.google.android.gms/.plus.audience.AclSelectionActivity
su -c pm disable com.google.android.gms/.plus.audience.AudienceSearchActivity
su -c pm disable com.google.android.gms/.plus.audience.CircleCreationActivity
su -c pm disable com.google.android.gms/.plus.audience.CircleSelectionActivity
su -c pm disable com.google.android.gms/.plus.audience.FaclSelectionActivity
su -c pm disable com.google.android.gms/.plus.audience.UpdateActionOnlyActivity
su -c pm disable com.google.android.gms/.plus.audience.UpdateCirclesActivity
su -c pm disable com.google.android.gms/.plus.circles.AddToCircleConsentActivity
su -c pm disable com.google.android.gms/.plus.oob.PlusActivity
su -c pm disable com.google.android.gms/.plus.oob.UpgradeAccountActivity
su -c pm disable com.google.android.gms/.plus.oob.UpgradeAccountInfoActivity
su -c pm disable com.google.android.gms/.plus.plusone.PlusOneActivity
su -c pm disable com.google.android.gms/.plus.sharebox.AddToCircleActivity
su -c pm disable com.google.android.gms/.plus.sharebox.ReplyBoxActivity
su -c pm disable com.google.android.gms/.plus.sharebox.ShareBoxActivity
su -c pm disable com.google.android.gms/.plus.provider.PlusProvider
su -c pm disable com.google.android.gms/.plus.service.DefaultIntentService
su -c pm disable com.google.android.gms/.plus.service.ImageIntentService
su -c pm disable com.google.android.gms/.plus.service.OfflineActionSyncAdapterService
su -c pm disable com.google.android.gms/.plus.service.PlusService
su -c pm disable com.google.android.gms/.wifi.gatherer2.receiver.WifiStateChangeReceiver
su -c pm disable com.google.android.gms/.wifi.gatherer2.receiver.GoogleAccountChangeReceiver
su -c pm disable com.google.android.gms/.wifi.gatherer2.service.GcmReceiverService
su -c pm disable com.google.android.gms/.wifi.gatherer2.service.WifiUpdateRetryTaskService
su -c pm disable com.google.android.gms/.wifi.gatherer2.service.KeyManagerServce
su -c pm disable com.google.android.gms/.ads.measurement.GmpConversionTrackingBrokerService
su -c pm disable com.google.android.gms/.measurement.service.MeasurementBrokerService
su -c pm disable com.google.android.gms/.perfprofile.uploader.PerfProfileCollectorService
su -c pm disable com.google.android.gms/.perfprofile.uploader.RequestPerfProfileCollectionService
su -c pm disable com.google.android.gsf/.update.SystemUpdateActivity
su -c pm disable com.google.android.gsf/.update.SystemUpdatePanoActivity
su -c pm disable com.google.android.gsf/com.google.android.gsf.checkin.CheckinService\$Receiver
su -c pm disable com.google.android.gsf/com.google.android.gsf.checkin.CheckinService\$SecretCodeReceiver
su -c pm disable com.google.android.gsf/com.google.android.gsf.checkin.CheckinService\$TriggerReceiver
su -c pm disable com.google.android.gsf/.checkin.EventLogService$Receiver
su -c pm disable com.google.android.gsf/.update.SystemUpdateService$Receiver
su -c pm disable com.google.android.gsf/.update.SystemUpdateService$SecretCodeReceiver
su -c pm disable com.google.android.gsf/.checkin.CheckinService
su -c pm disable com.google.android.gsf/.checkin.EventLogService
su -c pm disable com.google.android.gsf/.update.SystemUpdateService

echo "" | tee -a $LOG_FILE
echo "$( date +"%m-%d-%Y %H:%M:%S" ) gps activated.." | tee -a $LOG_FILE
