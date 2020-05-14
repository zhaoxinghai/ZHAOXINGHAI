TEMPLATE = app

QT += xml widgets qml quick

win32{
MOC_DIR += Moc
OBJECTS_DIR += Obj
INCLUDEPATH +=  Include \
                Keyboard \
                ..\Logic\include  \
                ..\Logic\common \
                ..\Protocol\include \
                ..\ConfigManage\libConfig\source \
                Third \
				
LIBS += -L..\Bin  -lLogic -lProtocol -llibConfig
DESTDIR += ..\Bin
DEFINES += win32
}

unix{
MOC_DIR += ./Moc
OBJECTS_DIR += ./Obj
INCLUDEPATH +=  Include \
                Keyboard \
                ../Logic/include \
                ../Logic/common \
                ../Protocol/include \
                ../ConfigManage/libConfig/source \
                Third \

LIBS += -L../Bin/ -lboost_system -lLogic -lProtocol -lConfig -lAec -lpcap -lsecurity
DESTDIR += ../Bin
DESTDIR += ../Bin
DEFINES += unix
QMAKE_CXXFLAGS += -g
}

#DEFINES += _DEBUG_

SOURCES += Src/cgpiocontrol.cpp \
    Src/clocalconfig.cpp \
    Src/cqmlactivatemodel.cpp \
    Src/cqmlfaultmodel.cpp \
    Src/cqmllogic.cpp \
    Src/cqmlpage.cpp \
    Src/cqmlpredefinemodel.cpp \
    Src/cqmlsourcemanage.cpp \
    Src/cqmlsourcemodel.cpp \
    Src/cqmlusermanage.cpp \
    Src/langstr.cpp \
    Src/ListModel.cpp \
    Src/main.cpp \
    Src/public.cpp \
    Src/cschedulemanage.cpp \
    Src/crecord.cpp \
    Src/chistory.cpp \
    Keyboard/ckeyboard.cpp \
    Src/czonemanage.cpp \
    Third/tinyxml2.cpp \
    Src/cudisk.cpp \
    Src/cimageBrowser.cpp \
    Src/cfontSetting.cpp \
    Src/zoneModel.cpp
RESOURCES += \
    qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += Include/cgpiocontrol.h \
    Include/clocalconfig.h \
    Include/cqmlactivatemodel.h \
    Include/cqmlfaultmodel.h \
    Include/cqmllogic.h \
    Include/cqmlpage.h \
    Include/cqmlpredefinemodel.h \
    Include/cqmlsourcemanage.h \
    Include/cqmlsourcemodel.h \
    Include/cqmlusermanage.h \
    Include/langstr.h \
    Include/ListItem.h \
    Include/ListModel.h \
    Include/public.h \
    Include/resource.h \
    Include/cschedulemanage.h \
    Include/crecord.h \
    Include/chistory.h \
    Keyboard/ckeyboard.h \
    Keyboard/conf.h \
    Include/czonemanage.h \
    Third/tinyxml2.h \
    Include/cudisk.h \
    Include/cimageBrowser.h \
    Include/cfontSetting.h \
    Include/zoneModel.h \
    Include/constant.h \
    Third/picosha2.h
