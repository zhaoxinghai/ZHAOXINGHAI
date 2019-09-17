#include <QtGui/QApplication>
#include <QTextCodec>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //--------------------------------------------支持本地编码(中文)--------------------------------------------
    QTextCodec::setCodecForLocale ( QTextCodec:: codecForLocale());
    QTextCodec::setCodecForCStrings ( QTextCodec:: codecForLocale());
    QTextCodec::setCodecForTr ( QTextCodec:: codecForLocale());

    //防止启动多个本地程序
    HANDLE g_hHandle = CreateMutex(NULL,TRUE,TEXT("CheckIp"));
    if(GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(g_hHandle);
        QMessageBox::warning(NULL, "警告",
                             "MacProgrammer.exe 运行中!", "OK");
        return 0;
    }

    QString StrPluginsPath = QApplication::applicationDirPath()+"/plugins/";
    a.addLibraryPath(StrPluginsPath);

    MainWindow w;
    w.show();

    if(w.getInitFlag() == true)
    {
        return a.exec();
    }
    else
    {
        QMessageBox::warning(NULL, "警告",
                             "MacProgrammer.exe 初始化失败!", "OK");
        return 0;
    }
}
