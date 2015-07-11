#!/sbin/busybox sh
#

echo "............Closing APPS before init";

/sbin/busybox killall -q kswapd0
sleep 71
am kill-all
/sbin/busybox killall -q com.android.MtpApplication
/sbin/busybox killall -q com.sec.android.inputmethod
/sbin/busybox killall -q com.sec.android.provider.logsprovider
/sbin/busybox killall -q com.android.nfc
/sbin/busybox killall -q com.samsung.android.providers.context
/sbin/busybox killall -q com.sec.android.app.sysscope
/sbin/busybox killall -q com.android.server.device.enterprise:remote
/sbin/busybox killall -q com.android.server.vpn.enterprise:remote
/sbin/busybox killall -q com.google.android.music:main
/sbin/busybox killall -q com.google.android.gm
/sbin/busybox killall -q com.google.process.gapps
/sbin/busybox killall -q com.google.android.gsf.login
/sbin/busybox killall -q com.google.android.apps.uploader
/sbin/busybox killall -q android.process.media
/sbin/busybox killall -q com.android.email
/sbin/busybox killall -q com.sec.android.provider.badge
/sbin/busybox killall -q com.google.android.youtube
/sbin/busybox killall -q com.android.calendar
/sbin/busybox killall -q com.android.providers.calendar
/sbin/busybox killall -q com.android.mms
/sbin/busybox killall -q com.wsomacp
/sbin/busybox killall -q com.samsung.map
/sbin/busybox killall -q org.simalliance.openmobileapi.service:remote
/sbin/busybox killall -q com.google.android.apps.maps:FriendService
/sbin/busybox killall -q com.google.android.apps.maps:GoogleLocationService
/sbin/busybox killall -q eu.chainfire.supersu
/sbin/busybox killall -q com.google.android.partnersetup
/sbin/busybox killall -q com.sec.android.app.controlpanel
/sbin/busybox killall -q com.android.contacts
/sbin/busybox killall -q com.google.android.googlequicksearchbox
/sbin/busybox killall -q com.sec.android.app.tmserver
/sbin/busybox killall -q com.google.android.apps.maps
/sbin/busybox killall -q com.sec.android.gallery3d
/sbin/busybox killall -q com.sec.android.app.videoplayer
/sbin/busybox killall -q com.google.android.apps.maps:LocationFriendService
/sbin/busybox killall -q com.sec.android.app.fm
/sbin/busybox killall -q com.android.music
/sbin/busybox killall -q com.android.browser
/sbin/busybox killall -q com.osp.app.signin
/sbin/busybox killall -q com.samsung.bt.avrcp
/sbin/busybox killall -q com.broadcom.bt.app.system
/sbin/busybox killall -q com.sec.android.providers.drm
/sbin/busybox killall -q com.seven.Z7.service
/sbin/busybox killall -q com.android.MtpApplication
/sbin/busybox killall -q org.projectvoodoo.louder
/sbin/busybox killall -q com.google.android.apps.maps:LocationFriendService
/sbin/busybox killall -q android.process.media
/sbin/busybox killall -q com.android.email
/sbin/busybox killall -q com.sec.android.app.twdvfs
/sbin/busybox killall -q com.android.server.vpn.enterprise:remote
/sbin/busybox killall -q com.android.contacts
/sbin/busybox killall -q com.samsung.map
/sbin/busybox killall -q com.sec.android.app.tmserver
/sbin/busybox killall -q com.sec.factory
/sbin/busybox killall -q com.google.android.youtube
/sbin/busybox killall -q com.fmm.dm
/sbin/busybox killall -q com.google.android.apps.uploader
/sbin/busybox killall -q com.sec.android.app.popupuireceiver
/sbin/busybox killall -q com.google.android.gsf.login
/sbin/busybox killall -q com.sec.android.gallery3d
/sbin/busybox killall -q com.sec.android.widgetapp.alarmclock
/sbin/busybox killall -q org.simalliance.openmobileapi.service:remote
/sbin/busybox killall -q com.google.process.gapps
/sbin/busybox killall -q com.google.android.apps.maps:Netwo
/sbin/busybox killall -q com.google.android.apps.maps:Frien
/sbin/busybox killall -q com.sec.android.app.samsungapps.una
/sbin/busybox killall -q com.google.process.gapps
/sbin/busybox killall -q com.sec.android.widgetapp.apnews
/sbin/busybox killall -q com.android.email
/sbin/busybox killall -q com.sec.android.app.controlpanel
/sbin/busybox killall -q com.cooliris.media
/sbin/busybox killall -q com.qo.android.am3
/sbin/busybox killall -q com.qo.android.am3:remote
/sbin/busybox killall -q com.android.updater
/sbin/busybox killall -q com.google.android.partnersetup
/sbin/busybox killall -q com.google.android.gm
/sbin/busybox killall -q com.android.vending
/sbin/busybox killall -q com.google.android.voicesearch
/sbin/busybox killall -q com.google.android.googlequicksearchbox
/sbin/busybox killall -q com.android.voicedialer
/sbin/busybox killall -q android.process.media
/sbin/busybox killall -q com.android.mms
/sbin/busybox killall -q com.android.calendar
/sbin/busybox killall -q com.android.providers.calendar
/sbin/busybox killall -q com.sec.android.Kies
/sbin/busybox killall -q com.sec.android.app.ve
/sbin/busybox killall -q com.google.process.gapps
/sbin/busybox killall -q com.sec.android.providers.downloads
/sbin/busybox killall -q com.sec.android.daemonapp.accuweather
/sbin/busybox killall -q com.sec.android.fotaclient
/sbin/busybox killall -q com.android.deskclock
/sbin/busybox killall -q com.android.bluetooth
/sbin/busybox killall -q com.google.android.apps.uploader
/sbin/busybox killall -q com.google.android.apps.maps:FriendService
/sbin/busybox killall -q com.google.android.gm
/sbin/busybox killall -q com.android.im
/sbin/busybox killall -q android.process.im
/sbin/busybox killall -q alei.switchpro
/sbin/busybox renice -20 `pidof com.android.phone`
/sbin/busybox renice -14 `pidof com.gau.go.launcherex`
/sbin/busybox renice -14 `pidof com.android.launcher2`
/sbin/busybox renice -14 `pidof com.teslacoilsw.launcher`
/sbin/busybox renice -14 `pidof com.anddoes.launcher`
/sbin/busybox renice -14 `pidof com.sec.android.app.launcher`
/sbin/busybox renice -14 `pidof com.android.launcher`
/sbin/busybox renice -14 `pidof com.teslacoilsw.launcher`
/sbin/busybox renice -14 `pidof com.teslacoilsw.launcher.prime`
/sbin/busybox renice -14 `pidof org.adw.launcher`
/sbin/busybox renice -15 `pidof com.android.systemui`
/sbin/busybox renice -9 `pidof com.android.settings`
/sbin/busybox renice -3 `pidof android.process.acore`
/sbin/busybox renice -3 `pidof android.process.media`

echo "............done";
