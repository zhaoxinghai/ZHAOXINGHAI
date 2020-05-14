#include <string.h>
#include <mutex>

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QList>

#ifdef WIN32

#else
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/input.h>
#include <sys/select.h>
#include <poll.h>
#include <fcntl.h>
#include <dirent.h>
//#include <stdio.h>
#include <string.h>
#endif

#include "cudisk.h"
#include "Logic.h"
#include "public.h"

static std::mutex m_mutex;

CUDisk* CUDisk::pUDisk = NULL;

CUDisk* CUDisk::GetInstance()
{
    if (pUDisk == NULL)
    {
        m_mutex.lock();
        if (pUDisk == NULL)
        {
            pUDisk = new CUDisk();
        }
        m_mutex.unlock();
    }
    return pUDisk;
}

void CUDisk::Release()
{
    if (NULL != pUDisk)
    {
        m_mutex.lock();
        if (NULL != pUDisk)
        {
            delete pUDisk;
            pUDisk = NULL;
        }
        m_mutex.unlock();
    }
}

CUDisk::CUDisk()
{

}

void CUDisk::init()
{
    this->start();
}

void CUDisk::run()
{
    //wait for qml initial
    sleep(2);
#ifdef WIN32
    return;
#else

    int status_old = NO_UDISK;
    int status = NO_UDISK;
    while(1)
    {
        sleep(2);
        status = checkUDiskStatus();
        if (status == ERR_UDISK)
        {
            continue;
        }
        if (status != status_old)
        {
            if (status == HAVE_UDISK)
            {
                emit updateUDiskIconSignal(INSERT_STATUS);
                emit insertUDiskSignal();
            }
            else if (status == NO_UDISK)
            {
                if (CUDisk::GetInstance()->isUDiskMounted() == true)
                {
                    CUDisk::GetInstance()->selfUmountUDisk();
                }
                emit updateUDiskIconSignal(REMOVE_STATUS);
                emit removeUDiskSignal();
            }

            status_old = status;
        }
    }

#endif
}

int CUDisk::mountUDisk()
{
#ifdef WIN32
    return 0;
#else
    QString cmd = QString("mount -t vfat -o rw,iocharset=utf8,umask=000 ") + getUDiskDevicePath()  + " " + U_DISK_PATH;
    CLogic::GetInstance()->System(cmd.toStdString().c_str(), SYSTEM_CMD_MOUNT_UDISK);
    return 0;
#endif
}

int CUDisk::umountUDisk()
{
#ifdef WIN32
    return 0;
#else
    PA::LogDebug("%s Line %d : %s\n", __FILE__, __LINE__, "umountUDisk");
    QString cmd = QString("umount ") + U_DISK_PATH;
    CLogic::GetInstance()->System(cmd.toStdString().c_str(), SYSTEM_CMD_UMOUNT_UDISK);
    return 0;
#endif
}

int CUDisk::umountUDisk(int id)
{
#ifdef WIN32
    return 0;
#else
    QString cmd = QString("umount ") + U_DISK_PATH;
    CLogic::GetInstance()->System(cmd.toStdString().c_str(), id);
    return 0;
#endif
}

int CUDisk::selfMountUDisk()
{
#ifdef WIN32
    return 0;
#else
    QString cmd = QString("mount -t vfat -o rw,iocharset=utf8,umask=000 ") + getUDiskDevicePath() + " " + U_DISK_PATH;
    system(cmd.toStdString().c_str());
    return 0;
#endif
}

int CUDisk::selfUmountUDisk()
{
#ifdef WIN32
    return 0;
#else
    PA::LogDebug("%s Line %d : %s\n", __FILE__, __LINE__, "selfUmountUDisk");
    QString cmd = QString("umount ") + U_DISK_PATH;
    system(cmd.toStdString().c_str());

    return 0;
#endif
}

bool CUDisk::isUDiskMounted()
{
#ifdef WIN32
    return true;
#else
    bool ret;
    QFile file(LINUX_MOUNTS_PATH);
    ret = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (ret == true)
    {
        QByteArray allLine = file.readAll();
        QString str(allLine);
        ret = str.contains(UDISK_DEVICE_NAME);
        file.close();
        return ret;
    }

    return false;
#endif
}

int CUDisk::checkUDiskStatus()
{
#ifdef WIN32
    return 0;
#else
    bool ret;
    QFile file(LINUX_PARTITIONS_PATH);
    ret = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (ret == true)
    {
        QByteArray allLine = file.readAll();
        QString str(allLine);
        ret = str.contains(UDISK_DEVICE_NAME);
        if (ret == true)
        {
            file.close();
            return HAVE_UDISK;
        }
        else
        {
            ret = str.contains("sd");
            if (ret == true)
            {
                file.close();
                return HAVE_UDISK;
            }
            else
            {
                file.close();
                return NO_UDISK;
            }
        }
    }

    return ERR_UDISK;
#endif
}

int CUDisk::isUDiskBusy()
{
#ifdef WIN32
    return -1;
#else
    FILE* fp = NULL;
	const int bufSize = 100;
    char buf[bufSize] = {0};
    char command[bufSize] = {0};
	snprintf(command, sizeof(command), "fuser -m /run/media/sda1");
    fp = popen(command,"r");
    if(fp == NULL)
    {
        perror("popen");
        return -1;
    }
    if( (fgets(buf, bufSize, fp)) != NULL )
    {
        if (fp != NULL)
        {
            pclose(fp);
        }
        return 0;
    }
    else
    {
        if (fp != NULL)
        {
            pclose(fp);
        }

        return 1;
    }

#endif

}

bool CUDisk::isUDiskMountPathExist()
{
    QDir dir(U_DISK_PATH);
    return dir.exists();
}

bool CUDisk::createUDiskMountPath()
{
    QDir dir;
    return  dir.mkpath(U_DISK_PATH);
}

void CUDisk::uSleep(int us)
{
#ifdef WIN32
    return ;
#else
    usleep(us);

    return;
#endif
}

QString CUDisk::getUDiskDevicePath()
{
#ifdef WIN32
    return "";
#else
    bool ret;
    QString deviceName;
    DIR *dir = NULL;
    struct dirent *ptr = NULL;

    QRegExp rx("sd[a-g]1");

    dir = opendir(U_DISK_DEVICE_PATH);
    if (dir == NULL)
    {
        return "";
    }

    struct dirent *entry = (struct dirent *)malloc(sizeof(struct dirent));
    struct dirent *result = NULL;
    while (true)
    {
        if (readdir_r(dir, entry, &result) != 0)
        {
            break;
        }
        if (result == NULL)
        {
            break;
        }
        if (result->d_name[0] == '.')
        {
            continue;
        }
        deviceName = QString(result->d_name);
        ret = rx.exactMatch(deviceName);
        if (ret == true)
        {
            free(entry);
            closedir(dir);
            return QString(U_DISK_DEVICE_PATH) + deviceName;
        }
    }

    free(entry);
    closedir(dir);
    return "";
#endif
}
