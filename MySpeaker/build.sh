#!/bin/sh

#cmd "./build.sh"          => nomal build
#cmd "./build.sh reb"      => normal rebuild
#cmd "./build.sh security" => security rebuild

#set your release version here,VER1 and VER2 is for designer
SET_VERSION="1.4.0-T001"
VER1=1
VER2=3

if [ ! -d "out" ];then
	mkdir out
fi

#need to rebuild?
if [ $# = 1 ]; then
	echo "\n############################# clean ############################\n"
	(cd ConfigManage/libConfig/ArmRelease && make clean)
	(cd Protocol && make clean)
	(cd Logic && make clean)
	(cd ETCS_QML && make clean)
	(cd Telnet && make clean)
	(cd security_network_lib && make clean)
	
	rm -r ./Bin/log
	rm ./Bin/libProtocol.so
	rm ./Bin/libLogic.so
	rm ./Bin/ETCS_QML
	rm ./Bin/Telnet.out
	rm ./Bin/libsecurity.so
	
	#auto general version.h file
	VERSION_FILE="./Protocol/include/version.h"
	BUILD_DATE=$(date +%Y-%m-%d)
	BUILD_TIME=$(date +%H:%M)

	echo "#ifndef VERSION_H" > $VERSION_FILE
	echo "#define VERSION_H" >> $VERSION_FILE
	echo "#define PROJECT_NAME \"Ethernet Touch Call Station\"" >> $VERSION_FILE
	echo "#define COMPILE_TIME \"${BUILD_DATE} ${BUILD_TIME}\"" >> $VERSION_FILE
	echo "#define VERSION \"${SET_VERSION}\"" >> $VERSION_FILE
	echo "#define VERSION1 ${VER1}" >> $VERSION_FILE
	echo "#define VERSION2 ${VER2}" >> $VERSION_FILE
	
	#define SECURITY
	if [ "$1" = "security" ]; then
		echo "#define SECURITY" >> $VERSION_FILE
	fi
	
	echo "#endif" >> $VERSION_FILE
fi

echo "\n############################# libConfig ############################\n"

(cd ConfigManage/libConfig/ArmRelease && make all)
cp ConfigManage/libConfig/ArmRelease/libConfig.so ./Bin

echo "\n############################# security ############################\n"

(cd security_network_lib && qmake security_network_lib.pro)
(cd security_network_lib && make all -j4)
cp security_network_lib/libsecurity.so ./Bin

echo "\n############################# Protocol ############################\n"

(cd Protocol && qmake Protocol.pro)
(cd Protocol && make all -j4)

echo "\n############################# Logic ############################\n"

(cd Logic && qmake Logic.pro)
(cd Logic && make all -j4)

echo "\n############################# Telnet ############################\n"

(cd Telnet && qmake)
(cd Telnet && make all -j4)

echo "\n############################# ETCS_QML ############################\n"

(cd ETCS_QML && qmake)
(cd ETCS_QML && make all -j4)

echo "\nBuild completed, please run the "sign" script to sign.\n"