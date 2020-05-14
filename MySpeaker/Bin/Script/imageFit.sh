#!/bin/sh

if [ -e $HIDE_DIR/Script/image.fit ]; then
	echo "image.fit is upgrading..."

	flash_erase /dev/mtd8 0 0
	nandwrite -p /dev/mtd8 $HIDE_DIR/Script/image.fit
	export RebootFlag=1
fi
