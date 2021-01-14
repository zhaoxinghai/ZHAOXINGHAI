#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTreeWidgetItem>
#include <QTimer>
#include <QFile>
#include <QFileDialog>
#include <QPrintDialog>
#include <QFont>
#include <QFontDialog>
#include <QFontDatabase>
#include <QPrinterInfo>
#include <QPrinter>
#include <QPainter>
#include <QtSql\QtSql>
#include <QSettings>
#include <ws2tcpip.h>
#include "./include/ProtocolFrame.h"
#include "GNPThread.h"

#define D_ProduceMode       0x00
#define D_ReceivePort       8001

#define D_QueryX618NetworkArg       0x0119
#define D_QueryX618NetworkArgAnswer 0x8119
#define D_X618NetworkArg            0x0121
#define D_X618NetworkArgAnswer      0x8121

#define D_NCX                       0x00
#define D_NCXTerminal               0x01
#define D_NCXMic                    0x02

#define D_X618                      0x10
#define D_X618DCS                   0x11
#define D_X618NPM                   0x12

#define D_D1_ETCS                   0x20
#define D_D1_INC                    0x21

#define D_X618SpecialID             0x000011CA

namespace Ui {
    class MainWindow;
}
class CGNPThread;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool getInitFlag();
    virtual void timerEvent( QTimerEvent *event);
private slots:
    void udpSocketReadyReadSlot();
    void dataReceived(int sockfd);
    void pushButtonPreestablishClickedSlot(bool checked);
    void pushButtonExportClickedSlot();
    void pushButtonStartupClickedSlot(bool checked);
    void timerSlot();

    //------------------------------------------------------------------------
    void tabWidgetDeviceCurrentChangedSlot(int index);
    void comboBoxX618ActivatedSlot(int index);
    void pushButtonX618PreestablishClickedSlot(bool checked);
    void pushButtonX618ExportClickedSlot();
    void pushButtonX618StartupClickedSlot(bool checked);
    void x618TimerSlot();

    //------------------------------------------------------------------------
    void checkBoxPrintSlot(bool checked);
    void pushButtonPrinterSlot();
    void pushButtonFontSlot();
    void lineEditLeftSlot(QString text);
    void lineEditTopSlot(QString text);
private:
    Ui::MainWindow *ui;
    bool InitFlag;
    QUdpSocket *UdpSocket;
    int CtrlPort;
    int ReceivePort;
    unsigned int CtrlMult;
    char CheckMode;
    QByteArray UdpOldData;
    unsigned short FrameCount;
    int SocketMult;
    QString ProgPath;
    QString iniPath;
    int ValidByte;
    int LowestValidByte;
    QString FirstMac;
    QString LastMac;
    int MacPCS;
    int DeviceCount;
    QTimer *Timer;
    int TimerMacIndex;
    QByteArray NetworkArg;
    int ReadWait;
    int ResetWait;

    //------------------------------------------------------------------------
    int X618ValidByte;
    int X618LowestValidByte;
    QString X618FirstMac;
    QString X618LastMac;
    int X618MacPCS;
    int X618ReadWait;
    int X618ResetWait;
    int X618WritePort;
    int X618ReadPort;
    QString X618DeviceIp;
    QUdpSocket *X618UdpSocket;
    QTimer *X618Timer;
    int X618TimerMacIndex;

    //------------------------------------------------------------------------
    bool PrintStartup;
    QPrinter *PrintPrinter;
    QFont PrintFont;
    int PrintLeft;
    int PrintTop;

    bool bindUdp();
    void addGroup();
    void parseNetwordDataFun(QByteArray data);
    E_Error parseCmdFun(QByteArray cmd);
    unsigned short getFrameHeader(QByteArray cmd);
    unsigned char getFrameVersion(QByteArray cmd);
    unsigned char getFrameFlag(QByteArray cmd);
    unsigned short getFrameNumber(QByteArray cmd);
    unsigned int getFrameSrc(QByteArray cmd);
    unsigned int getFrameDest(QByteArray cmd);
    unsigned short getFrameCmd(QByteArray cmd);
    unsigned short getFrameDataLen(QByteArray cmd);
    QByteArray getFrameData(int dataSize, QByteArray cmd);
    unsigned short getFrameCrc(QByteArray cmd);
    bool parseFrameCrc(QByteArray cmd, unsigned char frameFlag, unsigned short frameCrc);
    E_Error parseFrameCmdQueryNetworkArgAnswer_1(QByteArray frameData);
    unsigned short checkCrc(QByteArray cmd);
    bool initConfig();

    static bool checkMacSize(int iMacPCS,QString sFirstMac, QString sLastMac,
                             int iValidByte,int iLowestValidByte);

    static bool getValidMac(QString mac, unsigned int *validMac,
                            int iValidByte,int iLowestValidByte);

    static QStringList getValidMacList(QString sFirstMac,int iPcs,
                                int iValidByte,int iLowestValidByte);

    static QString RepairMac(QString mac);

    int queryNetworkArg(bool send = true);
    QByteArray getFrame(unsigned char frameVersion, unsigned char frameFlag,
                        unsigned short frameNumber, unsigned int frameSrc,
                        unsigned short frameCmd,
                        QByteArray frameData);
    int sendDeviceNetworkArg(QString mac);
    void resetDevice();
    void updataFirstMac();

    //------------------------------------------------------------------------
    void initOtherConfig();
    void selectDevice(int type);
    void updataDeviceType(int type);
    int getDeviceType();
    QString getDeviceTypeStr();
    void initX618Arg();
    QStringList getX618ValidMacList();
    QString X618RepairMac(QString mac);
    bool bindX618Udp();
    int getDeviceSizeForNetData(QByteArray data);
    QByteArray queryX618NetworkArg();
    unsigned int getDeviceIpForNetData(QByteArray data);
    unsigned int getDeviceMaskForNetData(QByteArray data);
    unsigned int getDeviceGatewayForNetData(QByteArray data);
    int sendX618NetworkArg(unsigned int ip, unsigned int mask,
                           unsigned int gateway, QString mac);
    QList<QByteArray> getCmds(QByteArray data);
    void updataX618FirstMac();
    QByteArray getFrameLittle(unsigned char frameVersion, unsigned char frameFlag,
                              unsigned short frameNumber, unsigned int frameSrc,
                              unsigned short frameCmd,
                              QByteArray frameData);
    unsigned short bigToLittle(unsigned short data);
    unsigned short littleToBig(unsigned short data);
    void showFontInfo(QFont font);
    void printMac(QString mac);

    //for D1
private slots:
    void pushButtonD1PreestablishClickedSlot(bool checked);
    void pushButtonD1ExportClickedSlot();
    void pushButtonD1StartupClickedSlot(bool checked);
    void pushComboD1(int index);

private:
    void initD1Config();
    QStringList getD1ValidMacList();
    bool checkD1MacSize();
    void updataD1FirstMac();

    unsigned int getMacCount(QString macFirst, QString macLast);

    //flash mac address
    void D1SetMac();
    static DWORD WINAPI ThreadProcD1(void* arg);
    void Run();

public:


private:
    CGNPThread  *m_GNPThread;

    QStringList  D1MacList;
    int          D1ValidByte;
    int          D1LowestValidByte;
    QString      D1FirstMac;
    QString      D1LastMac;
    int          D1MacPCS;

    int          D1MacIndex;
    int          D1TimerID;
};

#endif // MAINWINDOW_H
