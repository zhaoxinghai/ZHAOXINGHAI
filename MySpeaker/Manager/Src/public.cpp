#include <QCoreApplication>
#include "public.h"

QString getCurrentPath()
{
    QString tmp;
    tmp = QCoreApplication::applicationDirPath();
    QString runPath = tmp + "/";
    return runPath;
}

QString getLocalUriHead()
{
#ifdef WIN32
    return "file:///";
#else
    return "file://";
#endif
}
