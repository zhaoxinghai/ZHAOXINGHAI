#!/bin/bash

Filesystem_dirname=ETCS_filesystem_patch

echo "filesystem is upgrading..."

################## Get current system version #################
if [ -e /etc/sysVersion.ini ]; then
	sys_version=$(awk -F "=" '/ver/ {print $2}' /etc/sysVersion.ini)
fi

if [ -z "$sys_version" ]; then
   #Unable to read and reset system version
   sys_version="1.0.0"
fi

############################################################################
fromat_version(){
	version=$1
	if [[ $version =~ "T" ]]; then
		#This is a test version!
		result=${version//-T/.0}
	elif [[ $version =~ "V" ]]; then
		#This is a verified version!"
		result=${version//-V/.1}
	else
		#This is a release version!"
		result="$version.9999"
	fi
	echo "$result"
}

version_lt(){
	ver_1=$(fromat_version $1)
	IFS='.' read -a arr1 <<< "$ver_1";

	ver_2=$(fromat_version $2)
	IFS='.' read -a arr2 <<< "$ver_2";

	if [ ${arr1[0]} -lt ${arr2[0]} ]; then
		return 0
	elif [ ${arr1[0]} -gt ${arr2[0]} ]; then
		return 1
	fi
	
	if [ ${arr1[1]} -lt ${arr2[1]} ]; then
		return 0
	elif [ ${arr1[1]} -gt ${arr2[1]} ]; then
		return 1
	fi
	
	if [ ${arr1[2]} -lt ${arr2[2]} ]; then
		return 0
	elif [ ${arr1[2]} -gt ${arr2[2]} ]; then
		return 1
	fi
	
	if [ ${arr1[3]} -lt ${arr2[3]} ]; then
		return 0
	fi
	
	return 1
}


####################### 1.0.0 => 1.1.0 ########################
if version_lt $sys_version  "1.1.0"; then
	echo "current version is smaller than 1.1.0"
	cp $HIDE_DIR/Script/$Filesystem_dirname/passwd /etc/ 
	cp $HIDE_DIR/Script/$Filesystem_dirname/shadow /etc/ 
	cp $HIDE_DIR/Script/$Filesystem_dirname/group /etc/ 
	cp $HIDE_DIR/Script/$Filesystem_dirname/gshadow /etc/
	cp $HIDE_DIR/Script/$Filesystem_dirname/hostname /etc/
	cp $HIDE_DIR/Script/$Filesystem_dirname/startup /etc/
	cp $HIDE_DIR/Script/$Filesystem_dirname/profile /etc/
	cp $HIDE_DIR/Script/$Filesystem_dirname/fstab /etc/
	cp $HIDE_DIR/Script/$Filesystem_dirname/sysVersion.ini /etc/
	cp $HIDE_DIR/Script/$Filesystem_dirname/weston /etc/init.d/
	chmod o=rx /etc/init.d/weston
	cp $HIDE_DIR/Script/$Filesystem_dirname/automount.rules /etc/udev/rules.d/
	cp $HIDE_DIR/Script/$Filesystem_dirname/mount.sh /etc/udev/scripts/
	cp $HIDE_DIR/Script/$Filesystem_dirname/admin /home/ -rf
	cp $HIDE_DIR/Script/$Filesystem_dirname/smb.conf /usr/local/samba/lib/
	cp $HIDE_DIR/Script/$Filesystem_dirname/smbusers /usr/local/samba/lib/
	cp $HIDE_DIR/Script/$Filesystem_dirname/smbpasswd /usr/local/samba/lib/

fi

####################### 1.1.0 => 1.2.0 #######################
if version_lt $sys_version  "1.2.0"; then

	echo "current version is smaller than 1.2.0"
	cp $HIDE_DIR/Script/version_1.2/sysVersion.ini /etc/
	
	echo "Disable root user at ssh"
	rm /etc/systemd/system/sockets.target.wants/dropbear.socket
	rm /etc/rc3.d/S10dropbear
	rm /etc/default/dropbear
	
	echo "Disable su cmd"
	cp $HIDE_DIR/Script/version_1.2/su /etc/pam.d/

	echo "Remove dropbear start cmd at startup"
	cp $HIDE_DIR/Script/version_1.2/startup /etc/
	
	
	echo "Upgrade admin default passwd"
	cp $HIDE_DIR/Script/version_1.2/shadow /etc/ 
	chmod 600 /etc/shadow
	
	echo "Upgrade samba"
	if [ ! -d "/usr/local/samba/sbin" ]
	then
		mkdir -p /usr/local/samba/sbin
	fi
	if [ ! -d "/usr/local/samba/etc" ]
	then
		mkdir -p /usr/local/samba/etc
	fi
	cp $HIDE_DIR/Script/version_1.2/smbd /usr/local/samba/sbin
	cp $HIDE_DIR/Script/version_1.2/bin/smbpasswd /usr/local/samba/bin/
	chmod o=rx /usr/local/samba/bin/smbpasswd
	ln -s /usr/local/samba/bin/smbpasswd /usr/bin/smbpasswd
	cp $HIDE_DIR/Script/version_1.2/lib/smbpasswd /usr/local/samba/lib/
	chmod 600 /usr/local/samba/lib/smbpasswd
	cp $HIDE_DIR/Script/version_1.2/smb.conf /usr/local/samba/etc/
	
	rm /usr/local/samba/lib/smbusers
	rm /usr/local/samba/lib/smb.conf
	rm /usr/local/samba/bin/smbd
	rm -R /usr/local/samba/private/*
	rm -R /usr/local/samba/var/*
	

	echo "Save samba admin default passwd for firmware"
	if [ ! -d "/run/media/mmcblk0p1/Config" ]
	then
		mkdir -p /run/media/mmcblk0p1/Config
	fi
	cp $HIDE_DIR/Script/version_1.2/smbpass /run/media/mmcblk0p1/Config
	
	echo "Backup default SSH and samba password"
	if [ ! -d "/usr/local/backup" ]
	then
		mkdir -p /usr/local/backup
	fi
	cp $HIDE_DIR/Script/version_1.2/shadow /usr/local/backup
	chmod 600 /usr/local/backup/shadow
	cp $HIDE_DIR/Script/version_1.2/lib/smbpasswd /usr/local/backup
	chmod 600 /usr/local/backup/smbpasswd
	cp $HIDE_DIR/Script/version_1.2/smbpass /usr/local/backup
	
	echo "Reset conofiguration file"
	rm -f /run/media/mmcblk0p1/Config/config.xml
	rm -f /run/media/mmcblk0p1/Config/setting.xml
	
fi


####################### 1.2.0 => 1.3.1 #######################
if version_lt $sys_version  "1.3.1"; then
	echo "current version is smaller than 1.3.1"
	cp $HIDE_DIR/Script/version_1.3/sysVersion.ini /etc/
	rm -rf /lib/modules
	tar zxvf $HIDE_DIR/Script/version_1.3/modules.tgz -C /lib
fi

cp $HIDE_DIR/Script/version_1.3/startup /etc/

export RebootFlag=1
