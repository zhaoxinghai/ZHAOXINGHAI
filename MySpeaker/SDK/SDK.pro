#-------------------------------------------------
#
# Project created by QtCreator 2018-05-16T16:00:49
#
#-------------------------------------------------

QT       -= core gui

CONFIG += c++11 plugin
TARGET = sdk
TEMPLATE = lib
DESTDIR += ../Bin

#
win32{
DEFINES += DLL_EXPORTS
LIBS += -L../Bin
LIBS += -lOle32 -lWinMM -lshell32 -lwsock32
}

unix{
LIBS += -L../Bin -lpthread -lasound
#QMAKE_CXXFLAGS += -g
}

INCLUDEPATH +=  \
    ../3party/include/ \
    ./include \
    ./audio \
    ./common \
    ./main \
    ./message \
    ./libpcap \
    ./third

SOURCES += \
    audio/audiobuffer.cpp \
    audio/audiocapture.cpp \
    audio/audiodevicealsa.cpp \
    audio/audiodevicewin.cpp \
    audio/audiojob.cpp \
    audio/audioplay.cpp \
    audio/audioreceive.cpp \
    audio/d1adpcmcodec.cpp \
    audio/d1rtpframelayer.cpp \
    common/common.cpp \
    common/cyclebuffer.cpp \
    common/log.cpp \
    common/mylog.cpp \
    common/mysocket.cpp \
    common/mythread.cpp \
    main/d1system.cpp \
    main/interfacemsg.cpp \
    main/public.cpp \
    main/route.cpp \
    main/routelocal.cpp \
    main/routemanager.cpp \
    main/routenetwork.cpp \
    main/routeremote.cpp \
    main/service.cpp \
    main/threadaccept.cpp \
    main/threadcapture.cpp \
    main/threaddevice.cpp \
    main/threadplay.cpp \
    main/threadpool.cpp \
    main/threadrecv.cpp \
    main/threadsend.cpp \
    main/threadtimer.cpp \
    main/transport.cpp \
    message/msgbase.cpp \
    message/msgcheckroute.cpp \
    message/msgcommon.cpp \
    message/msgconnection.cpp \
    message/msgconnectrequest.cpp \
    message/msgdevice.cpp \
    message/msgfactory.cpp \
    message/msgsynctime.cpp \
    message/msgsysstatus.cpp \
    message/msgusagereport.cpp \
    audio/audiofile.cpp \
    audio/audiocapture2.cpp \
    main/sdk.cpp

HEADERS += \
    audio/audiobuffer.h \
    audio/audiocapture.h \
    audio/audiodef.h \
    audio/audiodevice.h \
    audio/audiodevicealsa.h \
    audio/audiodevicewin.h \
    audio/audiojob.h \
    audio/audioplay.h \
    audio/audioreceive.h \
    audio/d1adpcmcodec.h \
    audio/d1rtpframelayer.h \
    common/common.h \
    common/cyclebuffer.h \
    common/log.h \
    common/mylog.h \
    common/mysocket.h \
    common/mythread.h \
    include/callback.h \
    include/connection.h \
    include/detectmemoryleak.h \
    include/device.h \
    include/paerror.h \
    include/predefine.h \
    main/d1system.h \
    main/define.h \
    main/interfacemsg.h \
    main/public.h \
    main/route.h \
    main/routemanager.h \
    main/service.h \
    main/threadaccept.h \
    main/threadcapture.h \
    main/threadconnect.h \
    main/threaddevice.h \
    main/threadplay.h \
    main/threadpool.h \
    main/threadrecv.h \
    main/threadsend.h \
    main/threadtimer.h \
    main/transport.h \
    message/msgbase.h \
    message/msgcheckroute.h \
    message/msgcommon.h \
    message/msgcommunication.h \
    message/msgconnection.h \
    message/msgconnectrequest.h \
    message/msgdevice.h \
    message/msgfactory.h \
    message/msgnews.h \
    message/msgsynctime.h \
    message/msgsysstatus.h \
    message/msgusagereport.h \
    audio/audiofile.h \
    include/sdk.h

DISTFILES += \
    audio/fircoeff.dat
