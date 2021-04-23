QT -= core
QT -= gui

CONFIG += c++11

TARGET = Test
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

win32: QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings
win32: QMAKE_CFLAGS_RELEASE -= -Zc:strictStrings
win32: QMAKE_CFLAGS -= -Zc:strictStrings
win32: QMAKE_CXXFLAGS -= -Zc:strictStrings

SOURCES += main.cpp \
    mysharemem.cpp \
    memcomtent.cpp

HEADERS += \
    mysharemem.h \
    memcontent.h
