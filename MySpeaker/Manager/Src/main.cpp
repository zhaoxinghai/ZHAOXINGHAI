#include <QApplication>
#include <QQmlApplicationEngine>
#include <qqmlcontext.h>
#include <QQuickView>
#include <QDebug>
#include <QDesktopWidget>
#include <QTextCodec>
#include <QSharedMemory>
#include <QtCore>
#include <QSortFilterProxyModel>

#include "cqmllogic.h"
#include "cqmlpage.h"
#include "cqmlusermanage.h"
#include "ListModel.h"
#include "cqmlzonemodel.h"
#include "cqmlsourcemodel.h"
#include "cqmlsourcemanage.h"
#include "langstr.h"
#include "cgpiocontrol.h"
#include "clocalconfig.h"
#include "cschedulemanage.h"
#include "crecord.h"
#include "chistory.h"
#include "ckeyboard.h"
#include "czonemanage.h"
#include "cudisk.h"
#include "cimageBrowser.h"
#include "cfontSetting.h"

#define BUFSZ 150

int main(int argc, char *argv[])
{

#ifdef WIN32
    HANDLE hMutex = ::CreateMutex(NULL, TRUE, L"ETCS_RUNNING_MUTEX");
    if (hMutex == NULL || GetLastError() == ERROR_INVALID_HANDLE || GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return FALSE;
    }
#else

    FILE* fp = NULL;
    int count;
    char buf[BUFSZ] = {0};
    char command[BUFSZ] = {0};
    snprintf(command, sizeof(command), "ps -ef | grep ETCS_QML | grep -v grep | wc -l" );
    fp = popen(command,"r");
    if(fp == NULL)
    {
        perror("popen");
    }
    else
    {
        if( (fgets(buf, BUFSZ, fp)) != NULL )
        {
            count = atoi(buf);
            if(count > 1)
            {
                printf("process: total is %d\n",count);
                pclose(fp);
                return -1;
            }
        }
    }

    if (fp != NULL)
    {
        pclose(fp);
    }

#endif

    QApplication app(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QString tmpArg = QString(argv[1]);
    if (tmpArg == "-nowd")
    {
        CGPIOControl::GetInstance()->init(false);
    }
    else
    {
        CGPIOControl::GetInstance()->init(true);
    }
    CLogic::GetInstance()->InitArg(argc, argv);

    CLocalConfig::GetInstance()->init();
    LangStr::GetInstance()->init();
    CRecord::GetInstance()->init();
    CQmlSourceManage::GetInstance()->init();

    CQmlLogic::GetInstance()->init();

    CZoneManage::GetInstance()->init();
    CQmlPage::GetInstance()->init();
    CQmlUserManage::GetInstance()->init();
    CScheduleManage::GetInstance()->init();

    CHistory::GetInstance()->init();
    CKeyboard::GetInstance()->init();
    CKeyboard::GetInstance()->initKeyMap(CLocalConfig::GetInstance()->readConfig("LocalConfig", "Language"));
    CFontSetting::GetInstance()->init();

    QSortFilterProxyModel proxy;
    proxy.setSourceModel(CQmlLogic::GetInstance()->getPredefineModel());
    proxy.setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy.setFilterRole(2);

    QSortFilterProxyModel zoneProxy;
    zoneProxy.setSourceModel(CZoneManage::GetInstance()->getZoneModel());
    zoneProxy.setFilterCaseSensitivity(Qt::CaseInsensitive);
    zoneProxy.setFilterRole(2);

    QSortFilterProxyModel zoneVAProxy;
    zoneVAProxy.setSourceModel(CZoneManage::GetInstance()->getZoneModelVA());
    zoneVAProxy.setFilterCaseSensitivity(Qt::CaseInsensitive);
    zoneVAProxy.setFilterRole(2);

    /*
    if (CQmlUserManage::GetInstance()->checkConfigFileOK() == -1)
    {
        CQmlUserManage::GetInstance()->clearUserListModel();
    }
    */

    QQuickView viewer;
    QQmlContext *ctxt = viewer.rootContext();

    ctxt->setContextProperty("logicObject", CQmlLogic::GetInstance());
    ctxt->setContextProperty("zoneManageObject", CZoneManage::GetInstance());
    ctxt->setContextProperty("zoneProxyModel", &zoneProxy);
    ctxt->setContextProperty("zoneProxyModelVA", &zoneVAProxy);
    ctxt->setContextProperty("activateModel", CQmlLogic::GetInstance()->getActivateModel());
    ctxt->setContextProperty("faultModel", CQmlLogic::GetInstance()->getFaultModel());
    ctxt->setContextProperty("predefineModel", &proxy);
    ctxt->setContextProperty("usbSourceModel", CQmlSourceManage::GetInstance()->getUsbSourceModel());
    ctxt->setContextProperty("sourceManageObject", CQmlSourceManage::GetInstance());
    ctxt->setContextProperty("userManageObject", CQmlUserManage::GetInstance());
    ctxt->setContextProperty("pageObject", CQmlPage::GetInstance());
    ctxt->setContextProperty("localConfigObject", CLocalConfig::GetInstance());
    ctxt->setContextProperty("gpioObject", CGPIOControl::GetInstance());
    ctxt->setContextProperty("langStrObject", LangStr::GetInstance());
    ctxt->setContextProperty("scheduleManage", CScheduleManage::GetInstance());
    ctxt->setContextProperty("recordObject", CRecord::GetInstance());
    ctxt->setContextProperty("historyObject", CHistory::GetInstance());
    ctxt->setContextProperty("keyboardObject", CKeyboard::GetInstance());
    ctxt->setContextProperty("udiskObject", CUDisk::GetInstance());
    ctxt->setContextProperty("imageBrowserObject", CImageBrowser::GetInstance());
    ctxt->setContextProperty("fontSettingObject", CFontSetting::GetInstance());

    viewer.setSource(QUrl(QStringLiteral("qrc:/Logic/main.qml")));

    //QObject::connect(viewer.engine(), SIGNAL(quit()), qApp, SLOT(quit()));

    QDesktopWidget *desktop =  QApplication::desktop();
    QRect screen = desktop->screenGeometry();

    if ((screen.width() == 800) && (screen.height() == 480))
    {
        viewer.setMinimumSize(QSize(800, 480));
        viewer.setMaximumSize(QSize(800, 480));
        viewer.setFlags(Qt::FramelessWindowHint);
        viewer.showFullScreen();
    }
    else
    {
        int titleHeight = 80;
        int minHeight = 690;
        int realScreenHeight = screen.height() - titleHeight;

        if (realScreenHeight >= 800)
        {
            viewer.setMinimumSize(QSize(480, 800));
            viewer.setMaximumSize(QSize(480, 800));
        }
        else if ((realScreenHeight >= minHeight) && (realScreenHeight < 800))
        {
            viewer.setMinimumSize(QSize(480, realScreenHeight));
            viewer.setMaximumSize(QSize(480, realScreenHeight));
        }
        else if (realScreenHeight < minHeight)
        {
            viewer.setMinimumSize(QSize(480, minHeight));
            viewer.setMaximumSize(QSize(480, minHeight));
        }
       // viewer.setFlags(Qt::FramelessWindowHint);
        viewer.show();
    }

	return app.exec();
}

