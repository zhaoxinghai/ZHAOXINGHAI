#-------------------------------------------------
#
# Project created by QtCreator 2012-03-26T16:07:42
#
#-------------------------------------------------

QT       += core gui

TARGET = MacProgrammer
TEMPLATE = app

SOURCES += main.cpp\
        MainWindow.cpp \
    ../AS-Server/ProtocolFrame.cpp \
    ../NCX-I02X/TranslatorStr.cpp

HEADERS  += MainWindow.h \
    ../Include/ProtocolFrame.h \
    ../Include/TranslatorStr.h

FORMS    += MainWindow.ui

QT += qt3support \
        qt3support \
        network \
        sql

OTHER_FILES += \
    myapp.rc

RC_FILE = myapp.rc

RESOURCES += \
    Png.qrc

#CONFIG +=console release
