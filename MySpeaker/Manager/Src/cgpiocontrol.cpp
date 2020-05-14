#include <string.h>
#include <mutex>
#include <QGuiApplication>
#include <QScreen>

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
#endif

#include <QDebug>
#include <QSettings>
#include "cgpiocontrol.h"
#include "Logic.h"
#include "public.h"

int g_fd;

static std::mutex m_mutex;

CGPIOControl* CGPIOControl::pGPIOControl = NULL;

CGPIOControl* CGPIOControl::GetInstance()
{
    if (pGPIOControl == NULL)
    {
        m_mutex.lock();
        if (pGPIOControl == NULL)
        {
            pGPIOControl = new CGPIOControl();
        }
        m_mutex.unlock();
    }
    return pGPIOControl;
}

void CGPIOControl::Release()
{
    if (NULL != pGPIOControl)
    {
        m_mutex.lock();
        if (NULL != pGPIOControl)
        {
            delete pGPIOControl;
            pGPIOControl = NULL;
        }
        m_mutex.unlock();
    }
}

CGPIOControl::CGPIOControl()
{

}

void CGPIOControl::init(bool enableWatchDog)
{
    setEnableWatchDog(enableWatchDog);
    this->start();
}

QString CGPIOControl::getUImageVersion()
{
#ifdef WIN32
    return "";
#else
    return "";
#if 0
    return "";
    char info[100] = {0};
    char ver[100] = {0};
    int ret;
    char *pos = NULL;

    int fd = open("/dev/version", O_RDONLY);
    if (fd < 0)
    {
            return "";
    }

    ret = read(fd, info, 100);
    if (ret < 0)
    {
            perror("read error");
            close(fd);
            return "";
    }

    pos = info;
    while(*pos != 32)
    {
        pos++;
    }

    memcpy(ver, info, pos-info);

    close(fd);
    return QString(ver);
#endif
#endif
}

QString CGPIOControl::getDtbVersion()
{
#ifdef WIN32
    return "";
#else
    return "";
#if 0
    char info[100] = {0};
    char ver[100] = {0};
    int ret;
    char *pos = NULL;
    char *pos1 = NULL;

    int fd = open("/dev/version", O_RDONLY);
    if (fd < 0)
    {
            return "";
    }

    ret = read(fd, info, 100);
    if (ret < 0)
    {
            perror("read error");
            close(fd);
            return "";
    }

    pos = info;
    while(*pos != '\0')
    {
        if (*pos == 32)
        {
            pos1 = pos;
        }
        pos++;
    }

    memcpy(ver, pos1, pos-pos1);

    close(fd);
    return QString(ver);
#endif
#endif
}

QString CGPIOControl::getImageFitVersion()
{
#ifdef WIN32
    return "";
#else
    char ver[100] = {0};
    int ret;

    int fd = open("/dev/version", O_RDONLY);
    if (fd < 0)
    {
            return "";
    }

    ret = read(fd, ver, 99);
    if (ret < 0)
    {
            perror("read error");
            close(fd);
            return "";
    }

    close(fd);

    return QString(ver);
#endif
}

QString CGPIOControl::getFilesystemVersion()
{
    QString tmp;
    QSettings settings(SYSTEM_VERSION_FILE, QSettings::IniFormat);
    tmp = settings.value(QString("version/ver")).toString();
    return tmp;
}

int CGPIOControl::firstSetBacklight(QString value)
{
#ifdef WIN32
    return 0;
#else
    QString cmd;
    cmd = "echo " + value + " > " + BACKLIGHT_PATH;
    system(cmd.toStdString().c_str());

    return 0;
#endif
}

int CGPIOControl::setBacklight(int value)
{
     CLogic::GetInstance()->setBacklight(value);
     return 0;
}

int CGPIOControl::getBacklight()
{
    return 0;
}

int CGPIOControl::openWatchdog()
{
#ifdef WIN32
    return 0;
#else

    if (getEnableWatchDog() == false)
    {
        return -1;
    }

    g_fd = open(WATCHDOG_PATH, O_WRONLY);
    if (g_fd < 0)
    {
        perror("watchdog open error");
        return -1;
    }

    return 0;
#endif
}

int CGPIOControl::closeWatchdog()
{
#ifdef WIN32
    return 0;
#else
    unsigned char ch = 'V';
    write(g_fd, &ch, 1);
    close(g_fd);

    return 0;
#endif
}

int CGPIOControl::feadWatchdog()
{
#ifdef WIN32
    return 0;
#else
    unsigned char food = 0;
    write(g_fd, &food, 1);

    return 0;
#endif
}

int CGPIOControl::openBeep()
{
    CLogic::GetInstance()->BeepTest(true);
    return 0;
}

int CGPIOControl::closeBeep()
{
    CLogic::GetInstance()->BeepTest(false);
    return 0;
}

bool CGPIOControl::isScreenProtectorValid()
{
#ifdef WIN32
    return false;
#else
    return true;
#endif
}

/*
bool CGPIOControl::getPicture()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    return screen->grabWindow(0).save("123.jpg");
}
*/

void CGPIOControl::run()
{
#ifdef WIN32
    return;
#else
    fd_set reads;
    int fd;
    int rd;
    int ret;
    unsigned int i;
    struct input_event ev[64];

    fd = open(TOUCH_SCREEN_DEVICE_FILE, O_RDONLY);
    if (fd  < 0)
    {
        printf("open touchscreen file error\n");
        return;
    }

    FD_ZERO(&reads);
    FD_SET(fd, &reads);

    while (1)
    {
        ret = select(fd+1, &reads, NULL, NULL, NULL);
        if (ret == -1)
        {

        }
        else if (ret == 0)
        {

        }
        else
        {
            if (FD_ISSET(fd, &reads))
            {
                rd = read(fd, ev, sizeof(struct input_event) * 64);
                if (rd < (int) sizeof(struct input_event))
                {
                    printf("evtest: error reading\n");
                }

                for (i = 0; i < rd / sizeof(struct input_event); i++)
                {
                    if ((ev[i].type == 1) || (ev[i].type == 3))
                    {
                        emit touchEventSignal();
                        break;
                    }
                }
            }
        }
    }
#endif
}

int CGPIOControl::setEnableWatchDog(bool value)
{
    m_enableWatchDog = value;

    return 0;
}

bool CGPIOControl::getEnableWatchDog()
{
    return m_enableWatchDog;
}
