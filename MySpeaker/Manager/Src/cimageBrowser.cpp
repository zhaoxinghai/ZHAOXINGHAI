#include <string.h>
#include <mutex>

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>

#include "cimageBrowser.h"
#include "public.h"

static std::mutex m_mutex;

CImageBrowser* CImageBrowser::pImageBrowser = NULL;

CImageBrowser* CImageBrowser::GetInstance()
{
    if (pImageBrowser == NULL)
    {
        m_mutex.lock();
        if (pImageBrowser == NULL)
        {
            pImageBrowser = new CImageBrowser();
        }
        m_mutex.unlock();
    }
    return pImageBrowser;
}

void CImageBrowser::Release()
{
    if (NULL != pImageBrowser)
    {
        m_mutex.lock();
        if (NULL != pImageBrowser)
        {
            delete pImageBrowser;
            pImageBrowser = NULL;
        }
        m_mutex.unlock();
    }
}

CImageBrowser::CImageBrowser()
{

}

void CImageBrowser::init()
{

}

int CImageBrowser::clearImageList()
{
    m_imageList.clear();

    return 0;
}

int CImageBrowser::createImageList()
{
    QStringList filter;
    filter << "*.png" << "*.jpg";

#ifdef WIN32
    QDir m_dir = QDir(getCurrentPath() + IMAGE_DIR);
#else
    QDir m_dir = QDir(QString(U_DISK_PATH) + IMAGE_DIR);
#endif

    m_dir.setNameFilters(filter);
    m_dir.setSorting(QDir::Name);

    QList<QFileInfo> fileInfo = m_dir.entryInfoList(filter);
    for (int i = 0; i < fileInfo.count(); i++)
    {
        IMAGE_ITEM tmp;
        tmp.fileName = fileInfo.at(i).fileName();
        tmp.filePath = fileInfo.at(i).absolutePath();
        tmp.uriFileName = getLocalUriHead() + fileInfo.at(i).absolutePath() + "/" + fileInfo.at(i).fileName();

        m_imageList.push_back(tmp);
    }

    return 0;
}

int CImageBrowser::getImageListCount()
{
    return m_imageList.size();
}

QString CImageBrowser::getImageUriFileName(int pos)
{
    if (getImageListCount() <= 0)
    {
        return "";
    }
    return m_imageList[pos].uriFileName;
}
