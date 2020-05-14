#!/bin/sh

SD_PATH=/run/media/mmcblk0p1

LOGS_DIR=Logs/
CONFIG_DIR=Config/
IMAGES_DIR=Images/
FONTS_DIR=Fonts/
AUDIO_DATABASE_XML=Audio/audio-database.xml

TMP_PWD=$PWD
cd $SD_PATH

tar cvf $1 $LOGS_DIR $CONFIG_DIR $IMAGES_DIR $FONTS_DIR $AUDIO_DATABASE_XML
cp $1 $2
rm $1

cd $TMP_PWD