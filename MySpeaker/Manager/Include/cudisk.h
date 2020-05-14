#ifndef CUDISK_H
#define CUDISK_H

#include <QObject>
#include <QString>
#include <QThread>

class CUDisk : public QThread
{
    Q_OBJECT

public:
    CUDisk();
    static CUDisk* pUDisk;
    static CUDisk* GetInstance();
    static void Release();
    void run();

public slots:
    void init();
    int mountUDisk();
    int umountUDisk();
    int umountUDisk(int id);
    int selfMountUDisk();
    int selfUmountUDisk();
    bool isUDiskMounted();
    int checkUDiskStatus();
    bool isUDiskMountPathExist();
    bool createUDiskMountPath();
    int isUDiskBusy();
    void uSleep(int us);

signals:
    void updateUDiskIconSignal(int status);
    void insertUDiskSignal();
    void removeUDiskSignal();

private:

    QString getUDiskDevicePath();
};

#endif
