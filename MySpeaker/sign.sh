#!/bin/sh

#cmd "./sign.sh" => nomal packet
#cmd "./sign.sh fac" => factory produced test packet


#set your release version here
SET_VERSION="1.4.0-T001"
BASE_VERSION="1.2.0"

echo "\n############################# Packet ############################\n"

is_file_exist() {
	if [ ! -f $1 ];then
		echo "error: miss $1"
		exit 1
	fi
}

is_dir_exist() {
	if [ ! -d $1 ];then
		echo "error: miss $1"
		exit 1
	fi
}

tar_dir() {
	is_dir_exist $2
	tar -rf $1 $2
}

tar_file() {
	is_file_exist $2
	tar -rf $1 $2
}

if [ -f ./out/install.tar ];then
	rm -f ./out/install.tar
fi

tar_dir ./out/install.tar ./Script

tar_dir ./out/install.tar ./Bin/Images
tar_dir ./out/install.tar ./Bin/Fonts
tar_dir ./out/install.tar ./Bin/Presignal

tar_file ./out/install.tar ./Bin/libConfig.so
tar_file ./out/install.tar ./Bin/libProtocol.so
tar_file ./out/install.tar ./Bin/libLogic.so
tar_file ./out/install.tar ./Bin/libpcap.so
tar_file ./out/install.tar ./Bin/libAec.so
tar_file ./out/install.tar ./Bin/libboost_system.so
tar_file ./out/install.tar ./Bin/libsecurity.so
tar_file ./out/install.tar ./Bin/ETCS_QML
tar_file ./out/install.tar ./Bin/Telnet.out

cp ./Bin/Config/factory_config/config.xml ./Bin/Config/config_factory_backup.xml

if [ "$1" = "fac" ]; then
	PACKET_SUFFXI="-Factory"
	
	cp ./Bin/Config/factory_produced_test_config/config.xml ./Bin/Config/config.xml
	cp ./Bin/Config/factory_produced_test_config/config.xml ./Bin/Config/config_backup.xml
else
	PACKET_SUFFXI=""
	cp ./Bin/Config/factory_config/config.xml ./Bin/Config/config.xml
	cp ./Bin/Config/factory_config/config.xml ./Bin/Config/config_backup.xml
fi

tar_file ./out/install.tar ./Bin/Config/config.xml
tar_file ./out/install.tar ./Bin/Config/config_backup.xml
tar_file ./out/install.tar ./Bin/Config/config_factory_backup.xml
tar_file ./out/install.tar ./Bin/Config/langStrings.xml
tar_file ./out/install.tar ./Bin/Config/localConfig_backup.xml
tar_file ./out/install.tar ./Bin/Config/keyboard.xml
tar_file ./out/install.tar ./Bin/Config/fonts.xml
tar_file ./out/install.tar ./Bin/Config/lockImg.png
tar_file ./out/install.tar ./Bin/Config/lockImg_default.png
tar_file ./out/install.tar ./Bin/Config/configuration.ini
tar_file ./out/install.tar ./Bin/Config/dynamicconfig.xml

cp ./Logic/test-script.xml ./Bin/Config/

tar_file ./out/install.tar ./Bin/opmode.sh
tar_file ./out/install.tar ./Bin/environment.sh
tar_file ./out/install.tar ./Bin/dataExport.sh

echo "\n######################### Signing ############################\n"

openssl dgst -sha256 -binary -out ./out/install.sha256 ./out/install.tar

(cat ./Sign/header.dat;cat ./out/install.sha256) > ./out/install.sha256.der

rm ./out/install.sha256

curl -X POST "https://hppki.honeywell.com/signserver/process" --cacert ./Sign/HoneywellProductPKI.pem --key ./Sign/Ethernet_touch_call_station.key --cert ./Sign/Ethernet_touch_call_station.pem -F "workerName=Ethernet_touch_call_station" -F "filerecievefile=@./out/install.sha256.der" -o ./out/sign.txt --fail

rm ./out/install.sha256.der
cd out
if [ -e sign.txt ]; then
	if [ -z "$BASE_VERSION" ]; then
		echo $SET_VERSION > version.txt
		tar -czf ETCS$PACKET_SUFFXI-Full-$SET_VERSION.tar.gz install.tar sign.txt version.txt
	else
		echo "$SET_VERSION\n$BASE_VERSION" > version.txt
		tar -czf ETCS$PACKET_SUFFXI-$SET_VERSION.tar.gz install.tar sign.txt version.txt
	fi
	rm install.tar
	rm sign.txt
	rm version.txt
	echo "\nsgin OK.\n"
else
	echo "\nError: sgin.txt not exist!\n"
fi
