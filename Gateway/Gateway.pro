TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

win32{
LIBS += -lOle32 -lWinMM -lshell32 -lwsock32
}

unix{
TARGET = test
LIBS += -L./ -L./3party/lib -lpthread -lopus
QMAKE_CXXFLAGS += -g
}

INCLUDEPATH=./common \
    ./3party/include \
    ./g711 \
    ./g722 \
    ./libresample

SOURCES += main.cpp \
    audiodevicewin.cpp \
    audiojob.cpp \
    audioplay.cpp \
    audioreceive.cpp \
    d1adpcmcodec.cpp \
    threaddevice.cpp \
    common/common.cpp \
    common/threadpool.cpp \
    audiobuffer.cpp \
    audiofile.cpp \
    common/cyclebuffer.cpp \
    d1rtpframelayer.cpp \
    common/mythread.cpp \
    public.cpp \
    common/mysocket.cpp \
    threadcapture.cpp \
    g711/g711.cpp \
    g722/g722_decode.c \
    g722/g722_encode.c \
    libresample/filterkit.c \
    libresample/resample.c \
    libresample/resamplesubs.c

HEADERS += \
    audiodevicewin.h \
    audiojob.h \
    audioplay.h \
    audioreceive.h \
    d1adpcmcodec.h \
    threaddevice.h \
    common/common.h \
    common/threadpool.h \
    audiobuffer.h \
    audiofile.h \
    common/cyclebuffer.h \
    d1rtpframelayer.h \
    common/mythread.h \
    public.h \
    common/mysocket.h \
    threadcapture.h \
    g711/g711.h \
    g722/g722.h \
    libresample/configtemplate.h \
    libresample/filterkit.h \
    libresample/libresample.h \
    libresample/resample_defs.h \
    libresample/config.h
