#!/system/bin/sh
#
# Script Zipalign by javilonas
#

LOG_FILE=/data/zipalign.log;
ZIPALIGNDB=/data/zipalign.db;

if [ -e $LOG_FILE ]; then
	rm $LOG_FILE;
fi;

if [ ! -f $ZIPALIGNDB ]; then
	touch $ZIPALIGNDB;
fi;

echo "Starting FV Automatic ZipAlign $( date +"%m-%d-%Y %H:%M:%S" )" | tee -a $LOG_FILE;

for DIR in /system/app /data/app; do
	cd $DIR;
	for APK in *.apk; do
		if [ $APK -ot $ZIPALIGNDB ] && [ $(grep "$DIR/$APK" $ZIPALIGNDB|wc -l) -gt 0 ]; then
			echo "Already checked: $DIR/$APK" | tee -a $LOG_FILE;
		else
			ZIPCHECK=`/sbin/zipalign -c -v 4 $APK | grep FAILED | wc -l`;
			if [ $ZIPCHECK == "1" ]; then
				echo "Now aligning: $DIR/$APK" | tee -a $LOG_FILE;
				/sbin/zipalign -v -f 4 $APK /sdcard/download/$APK;
				mount -o rw,remount /system;
				cp -f -p /sdcard/download/$APK $APK;
				grep "$DIR/$APK" $ZIPALIGNDB > /dev/null || echo $DIR/$APK >> $ZIPALIGNDB;
			else
				echo "Already aligned: $DIR/$APK" | tee -a $LOG_FILE;
				grep "$DIR/$APK" $ZIPALIGNDB > /dev/null || echo $DIR/$APK >> $ZIPALIGNDB;
			fi;
		fi;
	done;
done;

mount -o ro,remount /system;
touch $ZIPALIGNDB;
echo "Automatic ZipAlign finished at $( date +"%m-%d-%Y %H:%M:%S" )" | tee -a $LOG_FILE;


