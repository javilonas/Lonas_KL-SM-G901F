#!/system/bin/sh

PATH=/sbin:/system/sbin:/system/bin:/system/xbin
export PATH

# Inicio
mount -o remount,rw -t auto /system
mount -t rootfs -o remount,rw rootfs

# Soporte LPowerCC
if [ ! -e /data/.lpowercc/lpowercc.xml ]; then
	rm /data/.lpowercc/lpowercc.xml
fi

if [ ! -e /data/.lpowercc/action.cache ]; then
	rm /data/.lpowercc/action.cache
fi

if [ -e /data/.lpowercc/testlp.log ]; then
	rm /data/.lpowercc/testlp.log
fi

if [ ! -d /data/.lpowercc ]; then
	mkdir /data/.lpowercc
	chmod 777 /data/.lpowercc
fi

sleep 0.3s

. /res/lpowercc/lpowercc-helper

read_defaults
read_config

/res/init_uci.sh select default
/res/init_uci.sh apply

apply_config

sleep 0.5s

sync

echo  Init.lpowercc.sh is working !!! >> /data/.lpowercc/testlp.log
echo "Excecuted on $(date +"%d-%m-%Y %r" )" >> /data/.lpowercc/testlp.log
echo  Done ! >> /data/.lpowercc/testlp.log

mount -t rootfs -o remount,ro rootfs
mount -o remount,ro -t auto /system
