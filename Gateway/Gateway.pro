TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

win32{
LIBS += -lOle32 -lWinMM -lshell32 -lwsock32
}

unix{
TARGET = test
LIBS += -L./ -lpthread
QMAKE_CXXFLAGS += -g
}

INCLUDEPATH=./g711 \
    ./g722 \
    ./libresample

SOURCES += main.cpp \
    audiodevicewin.cpp \
    audiojob.cpp \
    audioplay.cpp \
    audioreceive.cpp \
    d1adpcmcodec.cpp \
    threaddevice.cpp \
    common.cpp \
    threadpool.cpp \
    audiobuffer.cpp \
    audiofile.cpp \
    cyclebuffer.cpp \
    d1rtpframelayer.cpp \
    mythread.cpp \
    public.cpp \
    mysocket.cpp \
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
    common.h \
    threadpool.h \
    audiobuffer.h \
    audiofile.h \
    cyclebuffer.h \
    d1rtpframelayer.h \
    mythread.h \
    public.h \
    mysocket.h \
    threadcapture.h \
    g711/g711.h \
    g722/g722.h \
    libresample/configtemplate.h \
    libresample/filterkit.h \
    libresample/libresample.h \
    libresample/resample_defs.h \
    libresample/config.h
