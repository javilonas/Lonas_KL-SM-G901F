#!/system/bin/sh
#

echo "............Closing APPS before init";

killall -q kswapd0
sleep 71
am kill-all
killall -q com.android.MtpApplication
killall -q com.sec.android.inputmethod
killall -q com.sec.android.provider.logsprovider
killall -q com.android.nfc
killall -q com.samsung.android.providers.context
killall -q com.sec.android.app.sysscope
killall -q com.android.server.device.enterprise:remote
killall -q com.android.server.vpn.enterprise:remote
killall -q com.google.android.music:main
killall -q com.google.android.gm
killall -q com.google.process.gapps
killall -q com.google.android.gsf.login
killall -q com.google.android.apps.uploader
killall -q android.process.media
killall -q com.android.email
killall -q com.sec.android.provider.badge
killall -q com.google.android.youtube
killall -q com.android.calendar
killall -q com.android.providers.calendar
killall -q com.android.mms
killall -q com.wsomacp
killall -q com.samsung.map
killall -q org.simalliance.openmobileapi.service:remote
killall -q com.google.android.apps.maps:FriendService
killall -q com.google.android.apps.maps:GoogleLocationService
killall -q eu.chainfire.supersu
killall -q com.google.android.partnersetup
killall -q com.sec.android.app.controlpanel
killall -q com.android.contacts
killall -q com.google.android.googlequicksearchbox
killall -q com.sec.android.app.tmserver
killall -q com.google.android.apps.maps
killall -q com.sec.android.gallery3d
killall -q com.sec.android.app.videoplayer
killall -q com.google.android.apps.maps:LocationFriendService
killall -q com.sec.android.app.fm
killall -q com.android.music
killall -q com.android.browser
killall -q com.osp.app.signin
killall -q com.samsung.bt.avrcp
killall -q com.broadcom.bt.app.system
killall -q com.sec.android.providers.drm
killall -q com.seven.Z7.service
killall -q com.android.MtpApplication
killall -q org.projectvoodoo.louder
killall -q com.google.android.apps.maps:LocationFriendService
killall -q android.process.media
killall -q com.android.email
killall -q com.sec.android.app.twdvfs
killall -q com.android.server.vpn.enterprise:remote
killall -q com.android.contacts
killall -q com.samsung.map
killall -q com.sec.android.app.tmserver
killall -q com.sec.factory
killall -q com.google.android.youtube
killall -q com.fmm.dm
killall -q com.google.android.apps.uploader
killall -q com.sec.android.app.popupuireceiver
killall -q com.google.android.gsf.login
killall -q com.sec.android.gallery3d
killall -q com.sec.android.widgetapp.alarmclock
killall -q org.simalliance.openmobileapi.service:remote
killall -q com.google.process.gapps
killall -q com.google.android.apps.maps:Netwo
killall -q com.google.android.apps.maps:Frien
killall -q com.sec.android.app.samsungapps.una
killall -q com.google.process.gapps
killall -q com.sec.android.widgetapp.apnews
killall -q com.android.email
killall -q com.sec.android.app.controlpanel
killall -q com.cooliris.media
killall -q com.qo.android.am3
killall -q com.qo.android.am3:remote
killall -q com.android.updater
killall -q com.google.android.partnersetup
killall -q com.google.android.gm
killall -q com.android.vending
killall -q com.google.android.voicesearch
killall -q com.google.android.googlequicksearchbox
killall -q com.android.voicedialer
killall -q android.process.media
killall -q com.android.mms
killall -q com.android.calendar
killall -q com.android.providers.calendar
killall -q com.sec.android.Kies
killall -q com.sec.android.app.ve
killall -q com.google.process.gapps
killall -q com.sec.android.providers.downloads
killall -q com.sec.android.daemonapp.accuweather
killall -q com.sec.android.fotaclient
killall -q com.android.deskclock
killall -q com.android.bluetooth
killall -q com.google.android.apps.uploader
killall -q com.google.android.apps.maps:FriendService
killall -q com.google.android.gm
killall -q com.android.im
killall -q android.process.im
killall -q alei.switchpro
renice -20 `pidof com.android.phone`
renice -14 `pidof com.gau.go.launcherex`
renice -14 `pidof com.android.launcher2`
renice -14 `pidof com.teslacoilsw.launcher`
renice -14 `pidof com.anddoes.launcher`
renice -14 `pidof com.sec.android.app.launcher`
renice -14 `pidof com.android.launcher`
renice -14 `pidof com.teslacoilsw.launcher`
renice -14 `pidof com.teslacoilsw.launcher.prime`
renice -14 `pidof org.adw.launcher`
renice -15 `pidof com.android.systemui`
renice -9 `pidof com.android.settings`
renice -3 `pidof android.process.acore`
renice -3 `pidof android.process.media`

echo "............done";
