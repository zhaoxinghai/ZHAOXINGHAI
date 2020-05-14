QT       -= core gui
CONFIG += c++11

TARGET = Speaker
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
DESTDIR += ../Bin
LIBS += -L../Bin -lsdk

win32{
LIBS += -lOle32 -lWinMM -lshell32 -lwsock32
}

unix{
LIBS += -lasound
}

INCLUDEPATH +=  ../SDK/include \
    ../SDK/common

SOURCES += main.cpp \
    ../SDK/common/common.cpp

HEADERS += \
    ../SDK/include/sdk.h
