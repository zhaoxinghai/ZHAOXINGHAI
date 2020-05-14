#!/bin/sh

logPath=/run/media/mmcblk0p1/Logs/debug

if [ ! -d $logPath ];then
	logPath=/usr/local/log/
	if [ ! -d $logPath ];then
		mkdir -p $logPath
	fi
fi

updateLogDate=$(date +"%Y.%m.%d")
updateLogName="system-${updateLogDate}.txt"
fullLogFile=$logPath/$updateLogName

chmod 777 $HIDE_DIR/Script/imageFit.sh
chmod 777 $HIDE_DIR/Script/fs.sh
chmod 777 $HIDE_DIR/Script/app.sh

exec 2>> $fullLogFile
exec 1>> $fullLogFile

updateStartTime=$(date +%Y-%m-%d\ %H:%M:%S)
echo "$updateStartTime--------Start update system"

source $HIDE_DIR/Script/imageFit.sh $fullLogFile
source $HIDE_DIR/Script/fs.sh $fullLogFile
source $HIDE_DIR/Script/app.sh $fullLogFile

updateEndTime=$(date +%Y-%m-%d\ %H:%M:%S)
echo "$updateEndTime----------End update system"
sync

exec 2>> /dev/null
exec 1>> /dev/null

export FinishFlag=1
