#!/bin/sh

repeat=0

/etc/init.d/weston stop
while true
do
	killall ETCS_QML
	COUNT=`ps -ef |grep ETCS_QML |grep -v grep | wc -l`
	if [ 0 == ${COUNT} ]; then
		echo V > /dev/watchdog
		break;
	fi
	
	repeat=`expr $repeat + 1`
	if [ $repeat -ge 10 ];then
		echo "opmode time out error!!!"
		exit -1
	fi
	
	sleep 1
done

echo 0 > /sys/class/leds/beep/brightness
echo 255 > /sys/class/backlight/backlight/brightness
echo "OK"