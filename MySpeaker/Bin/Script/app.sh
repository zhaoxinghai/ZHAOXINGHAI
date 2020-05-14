#!/bin/sh

if [ -d $HIDE_DIR/Bin ]; then
	echo "app is upgrading..."

	#create Images
	if [ ! -d "/run/media/mmcblk0p1/Images" ]
	then
		mkdir -p /run/media/mmcblk0p1/Images
	fi
	
	#create Fonts
	if [ ! -d "/run/media/mmcblk0p1/Fonts" ]
	then
		mkdir -p /run/media/mmcblk0p1/Fonts
	fi

	#create Config
	if [ ! -d "/run/media/mmcblk0p1/Config" ]
	then
		mkdir -p /run/media/mmcblk0p1/Config
	fi
	
	#remove files
	rm -rf /home/root/Bin
	rm -rf /run/media/mmcblk0p1/Fonts/*
	rm -rf /run/media/mmcblk0p1/Images/*
	
	#update files
	cp -f $HIDE_DIR/Bin/Telnet.out /usr/bin/
	cp -rf $HIDE_DIR/Bin/Fonts/* /run/media/mmcblk0p1/Fonts/
	cp -rf $HIDE_DIR/Bin/Images/* /run/media/mmcblk0p1/Images/

	cp -f $HIDE_DIR/Bin/Config/fonts.xml /run/media/mmcblk0p1/Config/
	cp -f $HIDE_DIR/Bin/Config/keyboard.xml /run/media/mmcblk0p1/Config/
	cp -f $HIDE_DIR/Bin/Config/langStrings.xml /run/media/mmcblk0p1/Config/
	cp -f $HIDE_DIR/Bin/Config/localConfig_backup.xml /run/media/mmcblk0p1/Config/
	cp -f $HIDE_DIR/Bin/Config/configuration.ini /run/media/mmcblk0p1/Config/
	cp -f $HIDE_DIR/Bin/Config/lockImg_default.png /run/media/mmcblk0p1/Config/lockImg_default.png
	cp -f $HIDE_DIR/Bin/Config/test-script.xml /run/media/mmcblk0p1/Config/test-script.xml
	
	#keep files
	if [ ! -f "/run/media/mmcblk0p1/Config/config.xml" ];then
		cp -f $HIDE_DIR/Bin/Config/config.xml /run/media/mmcblk0p1/Config/
		cp -f $HIDE_DIR/Bin/Config/config_backup.xml /run/media/mmcblk0p1/Config/
	fi
	
	#Backup configuration for factory reset 
	cp -f $HIDE_DIR/Bin/Config/config_factory_backup.xml /usr/local/backup/config.xml
	
	if [ ! -f "/run/media/mmcblk0p1/Config/lockImg.png" ];then
		cp -f $HIDE_DIR/Bin/Config/lockImg.png /run/media/mmcblk0p1/Config/
	fi
	
	if [ ! -f "/run/media/mmcblk0p1/Config/smbpass" ];then
		cp $HIDE_DIR/Script/version_1.2/smbpass /run/media/mmcblk0p1/Config
	fi
	
	
	#repace bin folder
	rm -rf $HIDE_DIR/Bin/Images
	rm -rf $HIDE_DIR/Bin/Config
	rm -rf $HIDE_DIR/Bin/Fonts
	mv $HIDE_DIR/Bin /home/root/
	ln /home/root/Bin/libboost_system.so /home/root/Bin/libboost_system.so.1.68.0
	
    sync
fi
