#include <QtGui/QApplication>
#include <QTextCodec>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //--------------------------------------------֧�ֱ��ر���(����)--------------------------------------------
    QTextCodec::setCodecForLocale ( QTextCodec:: codecForLocale());
    QTextCodec::setCodecForCStrings ( QTextCodec:: codecForLocale());
    QTextCodec::setCodecForTr ( QTextCodec:: codecForLocale());

    //��ֹ����������س���
    HANDLE g_hHandle = CreateMutex(NULL,TRUE,TEXT("CheckIp"));
    if(GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(g_hHandle);
        QMessageBox::warning(NULL, "����",
                             "MacProgrammer.exe ������!", "OK");
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
        QMessageBox::warning(NULL, "����",
                             "MacProgrammer.exe ��ʼ��ʧ��!", "OK");
        return 0;
    }
}
