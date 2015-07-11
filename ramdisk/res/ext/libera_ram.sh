#!/sbin/busybox sh
#
# libera pagecache y swap cada hora si esta está por debajo de 1843200 kbytes (1.800 MB)
# 

/sbin/busybox renice 19 `pidof libera_ram.sh`
DROP_ONE="1"
DROP_TWO="2"
DROP_THREE="3"
FREE=`free -m | grep -i mem | awk '{print $4}'`  

while [ 1 ];
do
        if [ $FREE -lt 1843200 ]; then
                /sbin/busybox sync
                /sbin/busybox sleep 3
		echo $DROP_ONE > /proc/sys/vm/drop_caches
		echo $DROP_TWO > /proc/sys/vm/drop_caches
		echo $DROP_THREE > /proc/sys/vm/drop_caches
		/sbin/busybox sync; echo $DROP_THREE > /proc/sys/vm/drop_caches
                /sbin/busybox sync
		/sbin/busybox swapoff /dev/block/vnswap0 > /dev/null 2>&1
		/sbin/busybox swapon /dev/block/vnswap0 > /dev/null 2>&1
                /sbin/busybox sync
        fi
sleep 3600
done
