#!/system/bin/sh
#
# SQlite Speed Boost - by Javilonas
#

sleep 1
for i in \
`find /data -iname "*.db"`; 
do \
	/sbin/sqlite3 $i 'VACUUM;';
	/sbin/sqlite3 $i 'REINDEX;';
done;
if [ -d "/data/data" ]; then
	for i in \
	`find /data/data -iname "*.db"`; 
	do \
		/sbin/sqlite3 $i 'VACUUM;';
		/sbin/sqlite3 $i 'REINDEX;';
	done;
fi;
for i in \
`find /sdcard -iname "*.db"`; 
do \
	/sbin/sqlite3 $i 'VACUUM;';
	/sbin/sqlite3 $i 'REINDEX;';
done;
