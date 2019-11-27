#!/bin/sh
SDK_PATH="/home/rong/ti-processor-sdk-linux-am57xx-evm-03.03.00.04/linux-devkit"
if [ -z "$ZSH_NAME" ] && [ "x$0" = "x./environment-setup" ]; then
    echo "Error: This script needs to be sourced. Please run as \". ./environment-setup\""
    exit 1
else
    if [ -n "$BASH_SOURCE" ]; then
        SDK_PATH="`dirname $BASH_SOURCE`"
    fi
    SDK_PATH=`readlink -f "$SDK_PATH"`
    export SDK_PATH
fi
export SDK_SYS=x86_64-arago-linux
export REAL_MULTIMACH_TARGET_SYS=armv7ahf-neon-linux-gnueabi
export TOOLCHAIN_SYS=arm-linux-gnueabihf
export TOOLCHAIN_PREFIX=$TOOLCHAIN_SYS-
export SDK_PATH_NATIVE=$SDK_PATH/sysroots/$SDK_SYS
export SDK_PATH_TARGET=$SDK_PATH/sysroots/$REAL_MULTIMACH_TARGET_SYS
export PATH=$SDK_PATH_NATIVE/usr/bin:$PATH
export CPATH=$SDK_PATH_TARGET/usr/include:$CPATH
export PKG_CONFIG_SYSROOT_DIR=$SDK_PATH_TARGET
export PKG_CONFIG_PATH=$SDK_PATH_TARGET/usr/lib/pkgconfig
export PKG_CONFIG_ALLOW_SYSTEM_LIBS=1
export CONFIG_SITE=$SDK_PATH/site-config-$REAL_MULTIMACH_TARGET_SYS
export CC=${TOOLCHAIN_PREFIX}gcc
export CXX=${TOOLCHAIN_PREFIX}g++
export GDB=${TOOLCHAIN_PREFIX}gdb
export CPP="${TOOLCHAIN_PREFIX}gcc -E"
export NM=${TOOLCHAIN_PREFIX}nm
export AS=${TOOLCHAIN_PREFIX}as
export AR=${TOOLCHAIN_PREFIX}ar
export RANLIB=${TOOLCHAIN_PREFIX}ranlib
export OBJCOPY=${TOOLCHAIN_PREFIX}objcopy
export OBJDUMP=${TOOLCHAIN_PREFIX}objdump
export STRIP=${TOOLCHAIN_PREFIX}strip
export CONFIGURE_FLAGS="--target=arm-linux-gnueabi --host=arm-linux-gnueabi --build=x86_64-linux --with-libtool-sysroot=$SDK_PATH_TARGET"
export CPPFLAGS=" -march=armv7-a -marm -mfpu=neon  -mfloat-abi=hard --sysroot=$SDK_PATH_TARGET"
export CFLAGS="$CPPFLAGS"
export CXXFLAGS="$CPPFLAGS"
export LDFLAGS=" --sysroot=$SDK_PATH_TARGET"
export OECORE_NATIVE_SYSROOT=$SDK_PATH_NATIVE
export OECORE_TARGET_SYSROOT=$SDK_PATH_TARGET
export OECORE_ACLOCAL_OPTS="-I $SDK_PATH_NATIVE/usr/share/aclocal"
export OECORE_DISTRO_VERSION="2016.12"
export OECORE_SDK_VERSION="2016.12"
export PS1="\[\e[32;1m\][linux-devkit]\[\e[0m\]:\w> "
export PATH=$SDK_PATH_NATIVE/usr/bin/qt5:$PATH
export OE_QMAKE_CFLAGS="$CFLAGS"
export OE_QMAKE_CXXFLAGS="$CXXFLAGS"
export OE_QMAKE_LDFLAGS="$LDFLAGS"
export OE_QMAKE_CC=$CC
export OE_QMAKE_CXX=$CXX
export OE_QMAKE_LINK=$CXX
export OE_QMAKE_AR=$AR
export OE_QMAKE_LIBDIR_QT=$SDK_PATH_TARGET/usr/lib
export OE_QMAKE_INCDIR_QT=$SDK_PATH_TARGET/usr/include/qt5
export OE_QMAKE_HOST_BINDIR_QT=$SDK_PATH_NATIVE/usr/bin/qt5/
export OE_QMAKE_MOC=$SDK_PATH_NATIVE/usr/bin/qt5/moc
export OE_QMAKE_UIC=$SDK_PATH_NATIVE/usr/bin/qt5/uic
export OE_QMAKE_UIC3=$SDK_PATH_NATIVE/usr/bin/qt5/uic3
export OE_QMAKE_RCC=$SDK_PATH_NATIVE/usr/bin/qt5/rcc
export OE_QMAKE_QDBUSCPP2XML=$SDK_PATH_NATIVE/usr/bin/qt5/qdbuscpp2xml
export OE_QMAKE_QDBUSXML2CPP=$SDK_PATH_NATIVE/usr/bin/qt5/qdbusxml2cpp
export OE_QMAKE_QT_CONFIG=$SDK_PATH_TARGET/usr/lib/qt5/mkspecs/qconfig.pri
export OE_QMAKE_STRIP="echo"
export QMAKESPEC=$SDK_PATH_TARGET/usr/lib/qt5/mkspecs/linux-oe-g++
export QMAKE_DEFAULT_LIBDIRS=${QT_QMAKE_LIBDIR_QT}
export QMAKE_DEFAULT_INCDIRS=${QT_QMAKE_INCDIR_QT}

rm Test

(qmake Test.pro && make)
cp Test /home/rong/NFS/