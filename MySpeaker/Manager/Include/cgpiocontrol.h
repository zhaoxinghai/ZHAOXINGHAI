#ifndef CGPIOCONTROL_H
#define CGPIOCONTROL_H

#include <QObject>
#include <QString>
#include <QThread>

class CGPIOControl : public QThread
{
    Q_OBJECT

public:
    CGPIOControl();
    static CGPIOControl* pGPIOControl;
    static CGPIOControl* GetInstance();
    static void Release();
    void run();

public slots:
    void init(bool enableWatchDog);
    QString getUImageVersion();
    QString getDtbVersion();
    QString getImageFitVersion();
    QString getFilesystemVersion();

    int firstSetBacklight(QString value);
    int setBacklight(int value);
    int getBacklight();

    int openWatchdog();
    int closeWatchdog();
    int feadWatchdog();

    int openBeep();
    int closeBeep();

    bool isScreenProtectorValid();

signals:
    void touchEventSignal();

private:

    int setEnableWatchDog(bool value);
    bool getEnableWatchDog();

    bool m_enableWatchDog;
};

#endif //CGPIOCONTROL_H
