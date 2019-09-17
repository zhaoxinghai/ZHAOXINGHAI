#include "MainWindow.h"
#include "ui_MainWindow.h"

#pragma comment(lib, "ws2_32.lib" )

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    UdpSocket = new QUdpSocket(this);
    CtrlPort = D_DeviceCtrlPort;
    ReceivePort = D_ReceivePort;
    CtrlMult = QHostAddress(D_TerminalCtrlMult).toIPv4Address();
    CheckMode = D_ProduceMode;
    InitFlag = true;
    UdpOldData.clear();
    FrameCount = 0;
    SocketMult = SOCKET_ERROR;
    ProgPath = QApplication::applicationDirPath();
    ValidByte = 0;
    LowestValidByte = 0;
    FirstMac = "";
    LastMac = "";
    ui->PushButtonExport->setEnabled(false);
    ui->PushButtonStartup->setEnabled(false);
    MacPCS = 0;
    DeviceCount = 0;
    Timer = new QTimer(this);
    Timer->setSingleShot(true);
    TimerMacIndex = 0;
    NetworkArg.clear();
    ReadWait = 1;
    ResetWait = 5;

    //------------------------------------------------------------------------
    X618ValidByte = 0;
    X618LowestValidByte = 0;
    X618FirstMac = "";
    X618LastMac = "";
    X618MacPCS = 0;
    X618ReadWait = 1;
    X618ResetWait = 5;
    X618UdpSocket = new QUdpSocket(this);
    X618WritePort = 28001;
    X618ReadPort = 29001;
    X618DeviceIp = "192.168.2.200";
    X618Timer = new QTimer(this);
    X618Timer->setSingleShot(true);
    X618TimerMacIndex = 0;
    ui->PushButtonX618Export->setEnabled(false);
    ui->PushButtonX618Startup->setEnabled(false);

    //------------------------------------------------------------------------
    PrintStartup = false;
    PrintPrinter = new QPrinter();
    PrintFont = this->font();
    PrintLeft = 0;
    PrintTop = 0;


    //connect(UdpSocket, SIGNAL(readyRead()), this, SLOT(udpSocketReadyReadSlot()));
    connect(ui->PushButtonPreestablish, SIGNAL(clicked(bool)),
            this, SLOT(pushButtonPreestablishClickedSlot(bool)));
    connect(ui->PushButtonExport, SIGNAL(clicked()),
            this, SLOT(pushButtonExportClickedSlot()));
    connect(ui->PushButtonStartup, SIGNAL(clicked(bool)),
            this, SLOT(pushButtonStartupClickedSlot(bool)));
    connect(Timer, SIGNAL(timeout()), this, SLOT(timerSlot()));

    connect(ui->CheckBoxPrint, SIGNAL(toggled(bool)), this, SLOT(checkBoxPrintSlot(bool)));
    connect(ui->PushButtonPrinter, SIGNAL(clicked()), this, SLOT(pushButtonPrinterSlot()));
    connect(ui->PushButtonFont, SIGNAL(clicked()), this, SLOT(pushButtonFontSlot()));
    connect(ui->LineEditLeft, SIGNAL(textChanged(QString)), this, SLOT(lineEditLeftSlot(QString)));
    connect(ui->LineEditTop, SIGNAL(textChanged(QString)), this, SLOT(lineEditTopSlot(QString)));

    InitFlag = bindUdp() && initConfig();
    WSADATA WsaData;
    if(WSAStartup(MAKEWORD(2, 2), &WsaData) != 0 )//wsastartup成功返回0
    {
        InitFlag = false;
    }
    //addGroup();

    initOtherConfig();
    this->showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::getInitFlag()
{
    return InitFlag;
}

bool MainWindow::bindUdp()
{
    bool Flag = false;
    Flag = UdpSocket->bind(QHostAddress::Any, ReceivePort,
                           QUdpSocket::DontShareAddress | QUdpSocket::ReuseAddressHint);
    return Flag;
}

void MainWindow::addGroup()
{
    struct ip_mreq Mreq;
    int Flag = 1;
    struct sockaddr_in Sin;
    memset(&Sin, 0, sizeof(Sin));
    memset(&Mreq,0,sizeof(Mreq));
    Sin.sin_family = AF_INET;
    Sin.sin_addr.s_addr = htonl(INADDR_ANY);
    Sin.sin_port = htons(ReceivePort);
    if((SocketMult = socket(AF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR)
    {
        perror("Socket Error.");
    }
    if(setsockopt(SocketMult,SOL_SOCKET, SO_REUSEADDR,(char *)(&Flag), sizeof(Flag)) == SOCKET_ERROR)
    {
        perror("setsockopt:SO_REUSEADDR");
    }
    if(bind(SocketMult,(struct sockaddr *)&Sin, sizeof(Sin)) == SOCKET_ERROR)
    {
        perror("Bind Error");
    }
    if(setsockopt(SocketMult,IPPROTO_IP, IP_MULTICAST_LOOP, (char *)(&Sin), sizeof(Sin)) == SOCKET_ERROR)
    {
        perror("setsockopt:IP_MULTICAST_LOOP");
    }
    //Mreq.imr_multiaddr.s_addr = inet_addr(D_TerminalCtrlMult);
    Mreq.imr_multiaddr.s_addr = htonl(CtrlMult);
    Mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    //Mreq.imr_interface.s_addr = inet_addr("127.0.0.1");
    if (setsockopt(SocketMult, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)(&Mreq), sizeof(Mreq)) == SOCKET_ERROR)
    {
        perror("setsockopt:IP_ADD_MEMBERSHIP");
    }
    QSocketNotifier *SocketNotifier = new QSocketNotifier(SocketMult,
                                     QSocketNotifier::Read,this);
    QObject::connect(SocketNotifier, SIGNAL(activated(int)),this, SLOT(dataReceived(int)));
}

void MainWindow::udpSocketReadyReadSlot()
{
    while(UdpSocket->hasPendingDatagrams() == true)
    {
        QByteArray Data;
        Data.resize(UdpSocket->pendingDatagramSize());
        QHostAddress Sender;
        quint16 SenderPort;
        UdpSocket->readDatagram(Data.data(), Data.size(), &Sender, &SenderPort);
        parseNetwordDataFun(Data);
    }
}

void MainWindow::parseNetwordDataFun(QByteArray data)
{
    QByteArray Data = data;
    UdpOldData.append(Data);
    Data = UdpOldData;
    UdpOldData.clear();
    for(int i = 0;
        (i < Data.size()) && (Data.size() >= (i + D_FrameMiniSize));
        i++)
    {
        bool Flag = false;
        unsigned short FrameHeader = 0x0000;
        FrameHeader += (unsigned char)Data.at(i) << 8;
        FrameHeader += (unsigned char)Data.at(i + 1);
        if(FrameHeader == D_FrameHeader)
        {
            unsigned short FrameDataSize = (unsigned char)(Data.at(i + D_FrameDataSizeHigh) << 8) +
                                    (unsigned char)(Data.at(i + D_FrameDataSizeLow));
            if((i + D_FrameMiniSize + FrameDataSize) <= Data.size() &&
                    FrameDataSize >=0 &&
                    FrameDataSize <= D_FrameDataMaxSize)
            {
                Flag = true;
                QByteArray Cmd;
                Cmd.clear();
                Cmd = Data.mid(i, D_FrameMiniSize + FrameDataSize);
                i = i + D_FrameMiniSize + FrameDataSize - 1;
                parseCmdFun(Cmd);
                UdpOldData.append(Cmd);
            }
        }
        if(false == Flag)
        {
            UdpOldData.append(Data.at(i));
        }
    }
    UdpOldData = Data.right(Data.length() - UdpOldData.length());
}

E_Error MainWindow::parseCmdFun(QByteArray cmd)
{
    E_Error Flag = Natural;
    //unsigned short FrameHeader    = getFrameHeader(cmd);
    //unsigned char FrameVersion    = getFrameVersion(cmd);
    unsigned char FrameFalg       = getFrameFlag(cmd);
    //unsigned short FrameNumber    = getFrameNumber(cmd);
    //unsigned int FrameSrc         = getFrameSrc(cmd);
    //unsigned int FrameDest        = getFrameDest(cmd);
    unsigned short FrameCmd       = getFrameCmd(cmd);
    unsigned short FrameDataLen   = getFrameDataLen(cmd);
    QByteArray FrameData          = getFrameData(FrameDataLen, cmd);
    unsigned short FrameCrc       = getFrameCrc(cmd);

    if(parseFrameCrc(cmd, FrameFalg, FrameCrc) == false)
    {
        Flag = FrameCrcError;
    }
    if(Natural == Flag)
    {
        switch(FrameCmd)
        {
            case D_QueryNetworkArgAnswer:
            Flag = parseFrameCmdQueryNetworkArgAnswer_1(FrameData);
            break;
        default:
            Flag = FrameCmdError;
            break;
        }
    }
    /*QString FrameSrcStr = hexToString(FrameSrc);
    QString FrameCmdStr = getCmdString(FrameCmd);
    QString FrameDataStr = hexToString(FrameData);
    QString FrameStr = hexToString(cmd);
    QString ResultStr = getErrorString(Flag);
    emit messageNetwork(FrameSrcStr, FrameCmdStr, FrameDataStr, ResultStr);
    if(Flag != Natural)
    {
        QString LogString = QString("[%1] [%2] [%3] [%4]").
                arg(FrameSrcStr).
                arg(FrameCmdStr).
                arg(ResultStr).
                arg(FrameStr);
        Db.Message(Database::LogFile, D_ASServerLog, LogString);
    }*/
    return Flag;
}

unsigned short MainWindow::getFrameHeader(QByteArray cmd)
{
    unsigned short FrameHeader = 0x0000;
    if(cmd.size() > D_FrameHeaderLow)
    {
        FrameHeader += (unsigned char)cmd.at(D_FrameHeaderHigh) << 8;
        FrameHeader += (unsigned char)cmd.at(D_FrameHeaderLow);
    }
    return FrameHeader;
}

unsigned char MainWindow::getFrameVersion(QByteArray cmd)
{
    unsigned char FrameVersion = 0x00;
    if(cmd.size() > D_FrameVerion)
    {
        FrameVersion += (unsigned char)cmd.at(D_FrameVerion);
    }
    return FrameVersion;
}

unsigned char MainWindow::getFrameFlag(QByteArray cmd)
{
    unsigned char FrameFlag = 0x00;
    if(cmd.size() > D_FrameFlag)
    {
        FrameFlag += (unsigned char)cmd.at(D_FrameFlag);
    }
    return FrameFlag;
}

unsigned short MainWindow::getFrameNumber(QByteArray cmd)
{
    unsigned short FrameNumber = 0x0000;
    if(cmd.size() > D_FrameNumberLow)
    {
        FrameNumber += (unsigned char)cmd.at(D_FrameNumberHigh) << 8;
        FrameNumber += (unsigned char)cmd.at(D_FrameNumberLow);
    }
    return FrameNumber;
}

unsigned int MainWindow::getFrameSrc(QByteArray cmd)
{
    unsigned int FrameSrc = 0x00000000;
    if(cmd.size() > D_FrameSrcNumberLow)
    {
        FrameSrc += (unsigned char)cmd.at(D_FrameSrcSystem) << 24;
        FrameSrc += (unsigned char)cmd.at(D_FrameSrcModel) << 16;
        FrameSrc += (unsigned char)cmd.at(D_FrameSrcNumberHigh) << 8;
        FrameSrc += (unsigned char)cmd.at(D_FrameSrcNumberLow);
    }
    return FrameSrc;
}

unsigned int MainWindow::getFrameDest(QByteArray cmd)
{
    unsigned int FrameDest = 0x00000000;
    if(cmd.size() > D_FrameDestNumberLow)
    {
        FrameDest += (unsigned char)cmd.at(D_FrameDestSystem) << 24;
        FrameDest += (unsigned char)cmd.at(D_FrameDestModel) << 16;
        FrameDest += (unsigned char)cmd.at(D_FrameDestNumberHigh) << 8;
        FrameDest += (unsigned char)cmd.at(D_FrameDestNumberLow);
    }
    return FrameDest;
}

unsigned short MainWindow::getFrameCmd(QByteArray cmd)
{
    unsigned short FrameCmd = 0x0000;
    if(cmd.size() > D_FrameCmdLow)
    {
        FrameCmd += (unsigned char)cmd.at(D_FrameCmdHigh) << 8;
        FrameCmd += (unsigned char)cmd.at(D_FrameCmdLow);
    }
    return FrameCmd;
}

unsigned short MainWindow::getFrameDataLen(QByteArray cmd)
{
    unsigned short FrameDataLen = 0x0000;
    if(cmd.size() > D_FrameDataSizeLow)
    {
        FrameDataLen += (unsigned char)cmd.at(D_FrameDataSizeHigh) << 8;
        FrameDataLen += (unsigned char)cmd.at(D_FrameDataSizeLow);
    }
    return FrameDataLen;
}

QByteArray MainWindow::getFrameData(int dataSize, QByteArray cmd)
{
    QByteArray Data;
    Data.clear();
    if(cmd.size() == (dataSize + D_FrameMiniSize))
    {
        Data = cmd.mid(D_FrameDataSizeLow + 1, dataSize);
    }
    return Data;
}

unsigned short MainWindow::getFrameCrc(QByteArray cmd)
{
    unsigned short FrameCrc = 0x0000;
    if(cmd.size() >= D_FrameMiniSize)
    {
        FrameCrc += (unsigned char)cmd.at(cmd.size() - 2) << 8;
        FrameCrc += (unsigned char)cmd.at(cmd.size() - 1);
    }
    return FrameCrc;
}

bool MainWindow::parseFrameCrc(QByteArray cmd, unsigned char frameFlag, unsigned short frameCrc)
{
    bool Flag = true;
    if((frameFlag & D_FrameFlagCheck) == D_FrameFlagCheck)
    {
        Flag = (frameCrc == checkCrc(cmd));
    }
    return Flag;
}

E_Error MainWindow::parseFrameCmdQueryNetworkArgAnswer_1(QByteArray frameData)
{
    E_Error Flag = Natural;
    if(frameData.size() == 41)
    {
        NetworkArg = frameData;
        DeviceCount++;
    }
    else
    {
        Flag = FrameDataError;
    }
    return Flag;
}

unsigned short MainWindow::checkCrc(QByteArray cmd)
{
    QByteArray Data;
    Data.clear();
    for(int i=D_FrameHeaderSize;i<cmd.size()-D_FrameCrcSize;i++)
    {
        Data.append(cmd.at(i));
    }
    return CRC16Count(Data);
}

void MainWindow::dataReceived(int sock)
{
    int SinLen = 0;
    char Message[D_FrameDataMaxSize];
    struct sockaddr_in Sin;
    memset(&Sin, 0, sizeof(Sin));
    memset(Message,0,D_FrameDataMaxSize);
    Sin.sin_family = AF_INET;
    Sin.sin_addr.s_addr = htonl(INADDR_ANY);
    Sin.sin_port = htons(ReceivePort);
    SinLen = sizeof(Sin);
    int Count = D_FrameDataMaxSize;
    QByteArray Data;
    Data.clear();
    while(D_FrameDataMaxSize == Count)
    {
        Count = recvfrom(sock, Message, D_FrameDataMaxSize, 0,(struct sockaddr *)&Sin, &SinLen);
        for(int i=0;i<Count&&i<D_FrameDataMaxSize;i++)
        {
            Data.append(Message[i]);
        }
    }
    parseNetwordDataFun(Data);
}

bool MainWindow::initConfig()
{
    bool Flag = false;
    //QString Path = ProgPath + "/MacProgrammer.ini";
    QString Path = "./MacProgrammer.ini";
    QFileInfo File(Path);
    if(File.exists() && File.isFile())
    {
        QSettings Setting(Path, QSettings::IniFormat);
        ReadWait = Setting.value("T_General/T_ReadWait", 1).toInt();
        ResetWait = Setting.value("T_General/T_ResetWait", 5).toInt();
        ValidByte = Setting.value("T_General/T_ValidByte", 0).toInt();
        LowestValidByte = Setting.value("T_General/T_LowestValidByte", 0).toInt() - 1;
        FirstMac = Setting.value("T_General/T_FirstMac", "").toString() + ":";
        LastMac = Setting.value("T_General/T_LastMac", "").toString() + ":";
        if((ValidByte >= 1 && ValidByte <= 4) &&
                (LowestValidByte >=0 && LowestValidByte <= 5) &&
                (FirstMac.count(":") == 6) &&
                (LastMac.count(":") == 6))
        {
            Flag = true;
        }
        PrintStartup = Setting.value("T_Print/T_Startup", false).toBool();
        int PrinterIndex = Setting.value("T_Print/T_Printer", 0).toInt();
        QString Font = Setting.value("T_Print/T_Font", "").toString();
        PrintLeft = Setting.value("T_Print/T_Left", 0).toInt();
        PrintTop = Setting.value("T_Print/T_Top", 0).toInt();
        int FontIndex = Font.section(",", 0, 0).toInt();
        int FontBold = Font.section(",", 1, 1).toInt();
        int FontItalic = Font.section(",", 2, 2).toInt();
        int FontSize = Font.section(",", 3, 3).toInt();
        int FontStrikeOut = Font.section(",", 4, 4).toInt();
        int FontUnderline = Font.section(",", 5, 5).toInt();

        QList<QPrinterInfo> List = QPrinterInfo::availablePrinters();
        if(PrinterIndex < List.size())
        {
            PrintPrinter->~QPrinter();
            PrintPrinter = new QPrinter(List.at(PrinterIndex));
        }
        QFontDatabase Fonts;
        if(FontIndex < Fonts.families().size())
        {
            PrintFont.setFamily(Fonts.families().at(FontIndex));
            PrintFont.setBold(FontBold);
            PrintFont.setItalic(FontItalic);
            PrintFont.setPointSize(FontSize);
            PrintFont.setStrikeOut(FontStrikeOut);
            PrintFont.setUnderline(FontUnderline);
        }
        showFontInfo(PrintFont);
        ui->LabelPrinter->setText(PrintPrinter->printerName());
        ui->CheckBoxPrint->setChecked(PrintStartup);
        checkBoxPrintSlot(PrintStartup);
        ui->LineEditLeft->setText(QString::number(PrintLeft));
        ui->LineEditTop->setText(QString::number(PrintTop));
    }
    return Flag;
}

void MainWindow::pushButtonPreestablishClickedSlot(bool checked)
{
    if(checked == true)
    {
        MacPCS = ui->SpinBoxMacPCS->value();
        if(checkMacSize() == false)
        {
            MacPCS = 0;
            ui->PushButtonPreestablish->setChecked(false);
            QMessageBox::warning(this, "警告", "Mac地址空间不足!", "OK");
            return;
        }
    }
    else
    {
        MacPCS = 0;
    }
    ui->GroupBoxStep->setTitle(QString("进度 0/%1").arg(MacPCS));
    ui->PushButtonExport->setEnabled(checked);
    ui->PushButtonStartup->setEnabled(checked);
    ui->SpinBoxMacPCS->setEnabled(!checked);
}

void MainWindow::pushButtonExportClickedSlot()
{
    QString FilePath = QFileDialog::getSaveFileName(this, "导出", "./", "文本文档 (*.txt)");
    if(FilePath.isEmpty() == false)
    {
        QFile File(FilePath);
        if(File.open(QIODevice::WriteOnly) == true)
        {
            QStringList List = getValidMacList();
            for(int i=0;i<List.size();i++)
            {
                File.write(List.at(i) + "\n");
            }
            File.close();
        }
        else
        {
            QMessageBox::warning(this, "警告", "导出文档失败!", "OK");
        }
    }
}

bool MainWindow::checkMacSize()
{
    unsigned int ValidFirstMac = 0;
    unsigned int ValidLastMac = 0;
    bool Flag = getValidMac(FirstMac, &ValidFirstMac) && getValidMac(LastMac, &ValidLastMac);
    unsigned int ValidMacCount = ValidFirstMac + MacPCS - 1;
    if(ValidMacCount > (0xFFFFFFFF >> (4 - ValidByte)) || ValidMacCount > ValidLastMac)
    {
        Flag = false;
    }
    return Flag;
}

bool MainWindow::getValidMac(QString mac, unsigned int *validMac)
{
    bool Flag = false;
    *validMac = 0;
    QString ValidMac = mac.section(":", LowestValidByte - ValidByte + 1, LowestValidByte) + ":";
    for(int i=ValidMac.count(":")-1,n=0;i>=0;i--,n++)
    {
        Flag = true;
        bool Ok = false;
        unsigned char Hex = ValidMac.section(":", i, i).toInt(&Ok, 16);
        if(false == Ok)
        {
            Flag = false;
            break;
        }
        *validMac += Hex << (n * 8);
    }
    return Flag;
}

QStringList MainWindow::getValidMacList()
{
    QStringList List;
    List.clear();
    unsigned int ValidFirstMac = 0;
    getValidMac(FirstMac, &ValidFirstMac);
    unsigned int ValidMacCount = ValidFirstMac + MacPCS - 1;
    for(unsigned int i=ValidFirstMac;i<=ValidMacCount;i++)
    {
        QString FirstPart = "";
        if((LowestValidByte - ValidByte) >= 0)
        {
            FirstPart = FirstMac.section(":", 0, LowestValidByte - ValidByte) + ":";
        }
        QString MidPart = "";
        for(int n=ValidByte-1;n>=0;n--)
        {
            MidPart += QString::number(((i >> (n * 8)) & 0x000000FF), 16) + ":";
        }
        QString LastPart = FirstMac.section(":", LowestValidByte + 1);
        QString Mac = QString("%1%2%3").arg(FirstPart).arg(MidPart).arg(LastPart);
        List<<RepairMac(Mac);
    }
    return List;
}

QString MainWindow::RepairMac(QString mac)
{
    QString Mac = "";
    for(int i=0;i<mac.count(":");i++)
    {
        QString HexStr = mac.section(":", i, i).upper();
        if(HexStr.length() == 1)
        {
            HexStr = "0" + HexStr;
        }
        if(HexStr.isEmpty() == false)
        {
            Mac += HexStr + ":";
        }
    }
    Mac = Mac.section(":", 0, 5);
    return Mac;
}

void MainWindow::pushButtonStartupClickedSlot(bool checked)
{
    if(checked == true)
    {
        if(Timer->isActive() == false)
        {
            ui->PlainTextEdit->clear();
            TimerMacIndex = 0;
            Timer->start(1000);
            QStringList List = getValidMacList();
            if(List.size() > 0)
            {
                ui->PlainTextEdit->appendPlainText(QString("正在烧写第%1 PCS,所用Mac为%2.").
                                                   arg(TimerMacIndex + 1).arg(List.at(0)));
            }
        }
    }
    else
    {
        if(Timer->isActive() == true)
        {
            Timer->stop();
        }
    }
}

void MainWindow::timerSlot()
{
    bool Flag = false;
    QStringList List = getValidMacList();
    if(TimerMacIndex >= 0 && TimerMacIndex < List.size())
    {
        QString Mac = List.at(TimerMacIndex);
        int SendResult = 0;
        int Result = queryNetworkArg();
        switch(Result)
        {
        case 0:
            SendResult = sendDeviceNetworkArg(Mac);
            switch(SendResult)
            {
            case 0:
                Flag = true;
                break;
            case 1:
                //TimerMacIndex = List.size() + 10;
                break;
            case 2:
                Flag = true;
                break;
            }
            break;
        case 1:
            //TimerMacIndex = List.size() + 10;
            break;
        case 2:
            Flag = true;
            break;
        }
    }
    if(TimerMacIndex == List.size())
    {
        Flag = false;
    }
    if(Flag == true)
    {
        Timer->start(1000);
        ui->PlainTextEdit->appendPlainText(QString("正在烧写第%1 PCS,所用Mac为%2.").
                                           arg(TimerMacIndex + 1).arg(List.at(TimerMacIndex)));
    }
    else
    {
        ui->PushButtonStartup->setChecked(false);
        //if(TimerMacIndex == List.size())
        {
            updataFirstMac();
            initConfig();
        }
    }
}

int MainWindow::queryNetworkArg(bool send)
{
    int Flag = 0;
    QByteArray Data;
    Data.clear();
    FrameCount = (FrameCount + 1) < 0 ? 0 : FrameCount + 1;
    unsigned char FrameFlag = (0 | D_FrameFlagCommand | D_FrameFlagNotCheckScrDest |
                               (D_FrameCrcEnable == true ? D_FrameFlagCheck : 0x00));
    QByteArray Frame = getFrame(D_FrameVersion_1, FrameFlag, FrameCount,
                                0x00000001, D_QueryNetworkArg, Data);
    if(send == true)
    {
        UdpSocket->writeDatagram(Frame, QHostAddress(CtrlMult), CtrlPort);
    }
    DeviceCount = 0;
    QTime Time;
    Time.restart();
    while(Time.elapsed() < (ReadWait * 1000))
    {
        ;
    }
    udpSocketReadyReadSlot();
    QMessageBox MsgBox;
    MsgBox.setStandardButtons(QMessageBox::Abort | QMessageBox::Retry);
    MsgBox.setWindowTitle("警告");
    MsgBox.setIcon(QMessageBox::Warning);
    MsgBox.setWindowIcon(QIcon(":/Resource/Title.png"));
    switch(DeviceCount)
    {
    case 0:
        MsgBox.setText("未检测到设备在线,请查看.");
        ui->PlainTextEdit->appendPlainText("未检测到设备在线,请查看.");
        break;
    case 1:
        break;
    default:
        MsgBox.setText("设备过多,请确保只有一台设备在线.");
        ui->PlainTextEdit->appendPlainText("设备过多,请确保只有一台设备在线.");
        break;
    }
    if(DeviceCount != 1)
    {
        switch (MsgBox.exec())
        {
        case QMessageBox::Abort:
            Flag = 1;
            break;
        default:
            Flag = 2;
            break;
        }
    }
    return Flag;
}

QByteArray MainWindow::getFrame(unsigned char frameVersion, unsigned char frameFlag,
                                    unsigned short frameNumber, unsigned int frameSrc,
                                    unsigned short frameCmd,
                                    QByteArray frameData)
{
    unsigned int FrameDest = 0x00000001;
    QByteArray Cmd;
    Cmd.clear();
    Cmd.append(frameVersion);
    Cmd.append(frameFlag);
    Cmd.append((frameNumber >> 8) & 0x000000FF);
    Cmd.append(frameNumber & 0x000000FF);
    Cmd.append((frameSrc >> 24) & 0x000000FF);
    Cmd.append((frameSrc >> 16) & 0x000000FF);
    Cmd.append((frameSrc >> 8) & 0x000000FF);
    Cmd.append(frameSrc & 0x000000FF);
    Cmd.append((FrameDest >> 24) & 0x000000FF);
    Cmd.append((FrameDest >> 16) & 0x000000FF);
    Cmd.append((FrameDest >> 8) & 0x000000FF);
    Cmd.append(FrameDest & 0x000000FF);
    Cmd.append((frameCmd >> 8) & 0x000000FF);
    Cmd.append(frameCmd & 0x000000FF);
    Cmd.append((frameData.size() >> 8) & 0x000000FF);
    Cmd.append(frameData.size() & 0x000000FF);
    Cmd.append(frameData);
    unsigned short FrameCrC = CRC16Count(Cmd);
    Cmd.append((FrameCrC >> 8) & 0x000000FF);
    Cmd.append(FrameCrC & 0x000000FF);
    QByteArray Frame;
    Frame.clear();
    Frame.append((D_FrameHeader >> 8) & 0x00FF);
    Frame.append(D_FrameHeader & 0x00FF);
    Frame.append(Cmd);
    return Frame;
}

int MainWindow::sendDeviceNetworkArg(QString mac)
{
    int Flag = 0;
    QByteArray Data;
    Data.clear();
    int Mode = D_ProduceMode;
    Data.append(Mode);
    mac = mac + ":";
    for(int i=0;mac.section(":", i, i).isEmpty()==false;i++)
    {
        bool Ok = false;
        char Hex = mac.section(":", i, i).toInt(&Ok, 16);
        Data.append(Hex);
    }
    Data.append(NetworkArg.mid(7));
    FrameCount = (FrameCount + 1) < 0 ? 0 : FrameCount + 1;
    unsigned char FrameFlag = (0 | D_FrameFlagNotification | D_FrameFlagNotCheckScrDest |
                               (D_FrameCrcEnable == true ? D_FrameFlagCheck : 0x00));
    QByteArray Frame = getFrame(D_FrameVersion_1, FrameFlag, FrameCount,
                                0x00000001, D_NetworkArg, Data);
    UdpSocket->writeDatagram(Frame, QHostAddress(CtrlMult), CtrlPort);
    queryNetworkArg(false);
    QMessageBox MsgBox;
    MsgBox.setWindowIcon(QIcon(":/Resource/Title.png"));
    if(DeviceCount <= 0)
    {
        MsgBox.setStandardButtons(QMessageBox::Abort | QMessageBox::Retry);
        MsgBox.setWindowTitle("警告");
        MsgBox.setText("设备更新Mac失败.");
        ui->PlainTextEdit->appendPlainText("设备更新Mac失败.");
        MsgBox.setIcon(QMessageBox::Warning);
    }
    else
    {
        ui->GroupBoxStep->setTitle(QString("进度 %1/%2").arg(TimerMacIndex + 1).arg(MacPCS));
        QString TypeStr = getDeviceTypeStr();
        QFile File("./" + QDate::currentDate().toString("yyyy") + ".log");
        if(File.open(QIODevice::Append) == true)
        {
            File.write(QString("[%1]     %2     %3\n").
                       arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).
                       arg(TypeStr).arg(mac.section(":", 0, 5)));
            File.close();
        }
        printMac(mac.section(":", 0, 5));

        resetDevice();
        QTime Time;
        Time.restart();
        while(Time.elapsed() < (ResetWait * 1000))
        {
            ;
        }
        MsgBox.setStandardButtons(QMessageBox::Abort | QMessageBox::Ok);
        MsgBox.setWindowTitle("信息");
        MsgBox.setText("设备烧写Mac成功.\n请查设备Mac,再次确认Mac是否一致.\n单击[OK],更新下一台设备。");
        ui->PlainTextEdit->appendPlainText("设备烧写Mac成功.");
        ui->PlainTextEdit->appendPlainText(" ");
        MsgBox.setIcon(QMessageBox::Information);
        TimerMacIndex++;
    }
    switch(MsgBox.exec())
    {
    case QMessageBox::Abort:
        Flag = 1;
        break;
    case QMessageBox::Retry:
        Flag = 2;
        break;
    }
    return Flag;
}

void MainWindow::resetDevice()
{
    QByteArray Data;
    Data.clear();
    int Action = D_SystemCtrlActionReset;
    Data.append((Action >> 8) & 0x000000FF);
    Data.append(Action & 0x000000FF);
    FrameCount = (FrameCount + 1) < 0 ? 0 : FrameCount + 1;
    unsigned char FrameFlag = (0 | D_FrameFlagNotification | D_FrameFlagNotCheckScrDest |
                               (D_FrameCrcEnable == true ? D_FrameFlagCheck : 0x00));
    QByteArray Frame = getFrame(D_FrameVersion_1, FrameFlag, FrameCount,
                                0x00000001, D_SystemCtrl, Data);
    UdpSocket->writeDatagram(Frame, QHostAddress(CtrlMult), CtrlPort);
}

void MainWindow::updataFirstMac()
{
    QString Path = "./MacProgrammer.ini";
    QFileInfo File(Path);
    if(File.exists() && File.isFile())
    {
        unsigned int ValidFirstMac = 0;
        getValidMac(FirstMac, &ValidFirstMac);
        //unsigned int ValidMacCount = ValidFirstMac + MacPCS;
        unsigned int ValidMacCount = ValidFirstMac + TimerMacIndex;
        QString FirstPart = "";
        if((LowestValidByte - ValidByte) >= 0)
        {
            FirstPart = FirstMac.section(":", 0, LowestValidByte - ValidByte) + ":";
        }
        QString MidPart = "";
        for(int n=ValidByte-1;n>=0;n--)
        {
            MidPart += QString::number(((ValidMacCount >> (n * 8)) & 0x000000FF), 16) + ":";
        }
        QString LastPart = FirstMac.section(":", LowestValidByte + 1);
        QString Mac = QString("%1%2%3").arg(FirstPart).arg(MidPart).arg(LastPart);
        QSettings Setting(Path, QSettings::IniFormat);
        Setting.setValue("T_General/T_FirstMac", RepairMac(Mac));
    }
}

void MainWindow::initOtherConfig()
{
    connect(ui->TabWidgetDevice, SIGNAL(currentChanged(int)),
            this, SLOT(tabWidgetDeviceCurrentChangedSlot(int)));
    connect(ui->ComboBoxX618, SIGNAL(activated(int)),
            this, SLOT(comboBoxX618ActivatedSlot(int)));
    connect(ui->PushButtonX618Preestablish, SIGNAL(clicked(bool)),
            this, SLOT(pushButtonX618PreestablishClickedSlot(bool)));
    connect(ui->PushButtonX618Export, SIGNAL(clicked()),
            this, SLOT(pushButtonX618ExportClickedSlot()));
    connect(ui->PushButtonX618Startup, SIGNAL(clicked(bool)),
            this, SLOT(pushButtonX618StartupClickedSlot(bool)));
    connect(X618Timer, SIGNAL(timeout()), this, SLOT(x618TimerSlot()));

    QString Path = "./MacProgrammer.ini";
    QFileInfo File(Path);
    if(File.exists() && File.isFile())
    {
        QSettings Setting(Path, QSettings::IniFormat);
        int DeviceType = Setting.value("T_General/T_DeviceType", 0).toInt();
        selectDevice(DeviceType);
    }
}

void MainWindow::selectDevice(int type)
{
    switch(type)
    {
    case D_NCX:
    case D_NCXTerminal:
    case D_NCXMic:
        ui->TabWidgetDevice->setCurrentIndex(0);
        break;
    case D_X618DCS:
        ui->TabWidgetDevice->setCurrentIndex(1);
        ui->ComboBoxX618->setCurrentIndex(0);
        break;
    case D_X618NPM:
        ui->TabWidgetDevice->setCurrentIndex(1);
        ui->ComboBoxX618->setCurrentIndex(1);
        break;
    }
}

void MainWindow::tabWidgetDeviceCurrentChangedSlot(int index)
{
    index = index;
    int DeviceType = getDeviceType();
    updataDeviceType(DeviceType);
}

void MainWindow::comboBoxX618ActivatedSlot(int index)
{
    index = index;
    int DeviceType = getDeviceType();
    updataDeviceType(DeviceType);
}

void MainWindow::updataDeviceType(int type)
{
    QString Path = "./MacProgrammer.ini";
    QFileInfo File(Path);
    if(File.exists() && File.isFile())
    {
        QSettings Setting(Path, QSettings::IniFormat);
        Setting.setValue("T_General/T_DeviceType", type);
    }
}

int MainWindow::getDeviceType()
{
    int DeviceType = D_NCX;
    switch(ui->TabWidgetDevice->currentIndex())
    {
    case 1:
        switch(ui->ComboBoxX618->currentIndex())
        {
        case 1:
            DeviceType = D_X618NPM;
            break;
        default:
            DeviceType = D_X618DCS;
            break;
        }
        break;
    default:
        break;
    }
    return DeviceType;
}

QString MainWindow::getDeviceTypeStr()
{
    QString DeviceType = "NCX";
    switch(ui->TabWidgetDevice->currentIndex())
    {
    case 1:
        switch(ui->ComboBoxX618->currentIndex())
        {
        case 1:
            DeviceType = "X618NPM";
            break;
        default:
            DeviceType = "X618DCS";
            break;
        }
        break;
    default:
        break;
    }
    return DeviceType;
}

void MainWindow::pushButtonX618PreestablishClickedSlot(bool checked)
{
    if(checked == true)
    {
        initX618Arg();
        X618MacPCS = ui->SpinBoxX618MacPCS->value();
        if(checkX618MacSize() == false)
        {
            X618MacPCS = 0;
            ui->PushButtonX618Preestablish->setChecked(false);
            QMessageBox::warning(this, "警告", "Mac地址空间不足!", "OK");
            return;
        }
    }
    else
    {
        X618MacPCS = 0;
    }
    ui->GroupBoxX618Step->setTitle(QString("进度 0/%1").arg(X618MacPCS));
    ui->PushButtonX618Export->setEnabled(checked);
    ui->PushButtonX618Startup->setEnabled(checked);
    ui->SpinBoxX618MacPCS->setEnabled(!checked);
}

void MainWindow::pushButtonX618ExportClickedSlot()
{
    QString FilePath = QFileDialog::getSaveFileName(this, "导出", "./", "文本文档 (*.txt)");
    if(FilePath.isEmpty() == false)
    {
        QFile File(FilePath);
        if(File.open(QIODevice::WriteOnly) == true)
        {
            QStringList List = getX618ValidMacList();
            for(int i=0;i<List.size();i++)
            {
                File.write(List.at(i) + "\n");
            }
            File.close();
        }
        else
        {
            QMessageBox::warning(this, "警告", "导出文档失败!", "OK");
        }
    }
}

void MainWindow::pushButtonX618StartupClickedSlot(bool checked)
{
    if(checked == true)
    {
        if(X618Timer->isActive() == false)
        {
            ui->PlainTextX618Edit->clear();
            X618TimerMacIndex = 0;
            X618Timer->start(1000);
            QStringList List = getX618ValidMacList();
            if(List.size() > 0)
            {
                ui->PlainTextX618Edit->appendPlainText(QString("正在烧写第%1 PCS,所用Mac为%2.").
                                                   arg(TimerMacIndex + 1).arg(List.at(0)));
            }
        }
    }
    else
    {
        if(X618Timer->isActive() == true)
        {
            X618Timer->stop();
        }
    }
}

void MainWindow::initX618Arg()
{
    QString Path = "./MacProgrammer.ini";
    QFileInfo File(Path);
    if(File.exists() && File.isFile())
    {
        QSettings Setting(Path, QSettings::IniFormat);
        X618ReadWait = Setting.value("T_X618/T_ReadWait", 1).toInt();
        X618ResetWait = Setting.value("T_X618/T_ResetWait", 5).toInt();
        X618ValidByte = Setting.value("T_X618/T_ValidByte", 0).toInt();
        X618LowestValidByte = Setting.value("T_X618/T_LowestValidByte", 0).toInt() - 1;
        X618FirstMac = Setting.value("T_X618/T_FirstMac", "").toString() + ":";
        X618LastMac = Setting.value("T_X618/T_LastMac", "").toString() + ":";

        QString KeyStr = "T_X618DCS";
        switch(getDeviceType())
        {
        case D_X618NPM:
            KeyStr = "T_X618NPM";
            break;
        default:
            break;
        }
        X618WritePort = Setting.value(KeyStr + "/T_WritePort", 28001).toInt();
        X618ReadPort = Setting.value(KeyStr + "/T_ReadPort", 29001).toInt();
        X618DeviceIp = Setting.value(KeyStr + "/T_DeviceIP", "192.168.2.200").toString();
        if(bindX618Udp() == false)
        {
            QMessageBox::warning(this, "警告", "预设置失败.", "OK");
        }
    }
}

bool MainWindow::checkX618MacSize()
{
    unsigned int ValidFirstMac = 0;
    unsigned int ValidLastMac = 0;
    bool Flag = getX618ValidMac(X618FirstMac, &ValidFirstMac) && getX618ValidMac(X618LastMac, &ValidLastMac);
    unsigned int ValidMacCount = ValidFirstMac + X618MacPCS - 1;
    if(ValidMacCount > (0xFFFFFFFF >> (4 - X618ValidByte)) || ValidMacCount > ValidLastMac)
    {
        Flag = false;
    }
    return Flag;
}

bool MainWindow::getX618ValidMac(QString mac, unsigned int *validMac)
{
    bool Flag = false;
    *validMac = 0;
    QString ValidMac = mac.section(":", X618LowestValidByte - X618ValidByte + 1, X618LowestValidByte) + ":";
    for(int i=ValidMac.count(":")-1,n=0;i>=0;i--,n++)
    {
        Flag = true;
        bool Ok = false;
        unsigned char Hex = ValidMac.section(":", i, i).toInt(&Ok, 16);
        if(false == Ok)
        {
            Flag = false;
            break;
        }
        *validMac += Hex << (n * 8);
    }
    return Flag;
}

QStringList MainWindow::getX618ValidMacList()
{
    QStringList List;
    List.clear();
    unsigned int ValidFirstMac = 0;
    getValidMac(X618FirstMac, &ValidFirstMac);
    unsigned int ValidMacCount = ValidFirstMac + X618MacPCS - 1;
    for(unsigned int i=ValidFirstMac;i<=ValidMacCount;i++)
    {
        QString FirstPart = "";
        if((X618LowestValidByte - X618ValidByte) >= 0)
        {
            FirstPart = X618FirstMac.section(":", 0, X618LowestValidByte - X618ValidByte) + ":";
        }
        QString MidPart = "";
        for(int n=X618ValidByte-1;n>=0;n--)
        {
            MidPart += QString::number(((i >> (n * 8)) & 0x000000FF), 16) + ":";
        }
        QString LastPart = X618FirstMac.section(":", X618LowestValidByte + 1);
        QString Mac = QString("%1%2%3").arg(FirstPart).arg(MidPart).arg(LastPart);
        List<<RepairMac(Mac);
    }
    return List;
}

void MainWindow::x618TimerSlot()
{
    QMessageBox MsgBox;
    MsgBox.setStandardButtons(QMessageBox::Abort | QMessageBox::Retry);
    MsgBox.setWindowTitle("警告");
    MsgBox.setIcon(QMessageBox::Warning);
    MsgBox.setWindowIcon(QIcon(":/Resource/Title.png"));

    QByteArray Data;
    Data.clear();
    int DeviceSize = 1;
    if(getDeviceType() != D_X618NPM)
    {
        Data = queryX618NetworkArg();
        DeviceSize = getDeviceSizeForNetData(Data);
    }
    QStringList List = getX618ValidMacList();
    switch(DeviceSize)
    {
    case 0:
        MsgBox.setText("未检测到设备在线,请查看.");
        ui->PlainTextX618Edit->appendPlainText("未检测到设备在线,请查看.");
        break;
    case 1:
        if(X618TimerMacIndex >= 0 && X618TimerMacIndex < List.size())
        {
            unsigned int Ip = getDeviceIpForNetData(Data);
            unsigned int Mask = getDeviceMaskForNetData(Data);
            unsigned int Gateway = getDeviceGatewayForNetData(Data);
            QString Mac = List.at(X618TimerMacIndex);
            int Result = sendX618NetworkArg(Ip, Mask, Gateway, Mac);
            switch(Result)
            {
            case 0:
            {
                X618TimerMacIndex++;
                ui->GroupBoxX618Step->setTitle(QString("进度 %1/%2").arg(X618TimerMacIndex).arg(X618MacPCS));
                QFile File("./" + QDate::currentDate().toString("yyyy") + ".log");
                QString TypeStr = getDeviceTypeStr();
                if(File.open(QIODevice::Append) == true)
                {
                    File.write(QString("[%1]     %2     %3\n").
                               arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).
                               arg(TypeStr).
                               arg(Mac.section(":", 0, 5)));
                    File.close();
                }
                printMac(Mac.section(":", 0, 5));
                MsgBox.setStandardButtons(QMessageBox::Abort | QMessageBox::Ok);
                MsgBox.setWindowTitle("信息");
                MsgBox.setText("设备烧写Mac成功.\n请查设备Mac,再次确认Mac是否一致.\n单击[OK],更新下一台设备。");
                ui->PlainTextX618Edit->appendPlainText("设备烧写Mac成功.");
                ui->PlainTextX618Edit->appendPlainText(" ");
                MsgBox.setIcon(QMessageBox::Information);
                break;
            }
            case 1:
                MsgBox.setText("存储网络参数错误,请查看.");
                ui->PlainTextX618Edit->appendPlainText("存储网络参数错误,请查看.");
                break;
            case 3:
                MsgBox.setText("设置参数出错,请查看.");
                ui->PlainTextX618Edit->appendPlainText("设置参数出错,请查看.");
                break;
            case 7:
                MsgBox.setText("IP错误,请查看.");
                ui->PlainTextX618Edit->appendPlainText("IP错误,请查看.");
                break;
            case 8:
                MsgBox.setText("子网掩码错误,请查看.");
                ui->PlainTextX618Edit->appendPlainText("子网掩码错误,请查看.");
                break;
            case 9:
                MsgBox.setText("网关错误,请查看.");
                ui->PlainTextX618Edit->appendPlainText("网关错误,请查看.");
                break;
            case 17:
                MsgBox.setText("设置MAC地址错误,请查看.");
                ui->PlainTextX618Edit->appendPlainText("设置MAC地址错误,请查看.");
                break;
            default:
                MsgBox.setText("通信异常,请查看.");
                ui->PlainTextX618Edit->appendPlainText("通信异常,请查看.");
                break;
            }
        }
        break;
    default:
        MsgBox.setText("设备过多,请确保只有一台设备在线.");
        ui->PlainTextX618Edit->appendPlainText("设备过多,请确保只有一台设备在线.");
        break;
    }


    bool Flag = false;
    switch (MsgBox.exec())
    {
    case QMessageBox::Abort:
        break;
    case QMessageBox::Retry:
        Flag = true;
        break;
    default:
        Flag = !(X618TimerMacIndex >= List.size());
        break;
    }
    if(Flag == true)
    {
        X618Timer->start(1000);
        ui->PlainTextX618Edit->appendPlainText(QString("正在烧写第%1 PCS,所用Mac为%2.").
                                               arg(X618TimerMacIndex + 1).arg(List.at(X618TimerMacIndex)));
    }
    else
    {
        ui->PushButtonX618Startup->setChecked(false);
        ui->PushButtonX618Preestablish->setChecked(false);
        pushButtonX618PreestablishClickedSlot(false);
        {
            updataX618FirstMac();
        }
    }
}

int MainWindow::getDeviceSizeForNetData(QByteArray data)
{
    int DeviceSize = 0;
    QByteArray Data = data;
    for(int i = 0;
        (i < Data.size()) && (Data.size() >= (i + D_FrameMiniSize));
        i++)
    {
        unsigned short FrameHeader = 0x0000;
        FrameHeader += (unsigned char)Data.at(i);
        FrameHeader += (unsigned char)Data.at(i + 1) << 8;
        if(FrameHeader == D_FrameHeader)
        {
            unsigned short FrameDataSize = (unsigned char)(Data.at(i + D_FrameDataSizeHigh)) +
                                    (unsigned char)(Data.at(i + D_FrameDataSizeLow) << 8);
            if((i + D_FrameMiniSize + FrameDataSize) <= Data.size() &&
                    FrameDataSize >=0 &&
                    FrameDataSize <= D_FrameDataMaxSize)
            {
                QByteArray Cmd;
                Cmd.clear();
                Cmd = Data.mid(i, D_FrameMiniSize + FrameDataSize);
                i = i + D_FrameMiniSize + FrameDataSize - 1;
                if(littleToBig(getFrameCmd(Cmd)) == D_QueryX618NetworkArgAnswer)
                {
                    DeviceSize++;
                }
            }
        }
    }
    return DeviceSize;
}

QByteArray MainWindow::queryX618NetworkArg()
{
    QByteArray Data;
    Data.clear();
    FrameCount = (FrameCount + 1) < 0 ? 0 : FrameCount + 1;
    unsigned char FrameFlag = (0 | D_FrameFlagCommand |
                               (D_FrameCrcEnable == true ? D_FrameFlagCheck : 0x00));
    QByteArray Frame = getFrameLittle(D_FrameVersion_1, FrameFlag, FrameCount,
                                      D_X618SpecialID, D_QueryX618NetworkArg, Data);
    X618UdpSocket->writeDatagram(Frame, QHostAddress(X618DeviceIp), X618WritePort);
    QTime Time;
    Time.restart();
    while(Time.elapsed() < (X618ReadWait * 1000))
    {
        ;
    }
    Data.clear();
    while(X618UdpSocket->hasPendingDatagrams() == true)
    {
        Data.resize(X618UdpSocket->pendingDatagramSize());
        QHostAddress Sender;
        quint16 SenderPort;
        X618UdpSocket->readDatagram(Data.data(), Data.size(), &Sender, &SenderPort);
    }
    return Data;
}

bool MainWindow::bindX618Udp()
{
    X618UdpSocket->close();
    bool Flag = false;
    Flag = X618UdpSocket->bind(QHostAddress::Any, X618ReadPort,
                               QUdpSocket::DontShareAddress | QUdpSocket::ReuseAddressHint);
    return Flag;
}

unsigned int MainWindow::getDeviceIpForNetData(QByteArray data)
{
    unsigned int Ip = 0;
    QByteArray Data = data;
    for(int i = 0;
        (i < Data.size()) && (Data.size() >= (i + D_FrameMiniSize));
        i++)
    {
        unsigned short FrameHeader = 0x0000;
        FrameHeader += (unsigned char)Data.at(i);
        FrameHeader += (unsigned char)Data.at(i + 1) << 8;
        if(FrameHeader == D_FrameHeader)
        {
            unsigned short FrameDataSize = (unsigned char)(Data.at(i + D_FrameDataSizeHigh)) +
                                    (unsigned char)(Data.at(i + D_FrameDataSizeLow) << 8);
            if((i + D_FrameMiniSize + FrameDataSize) <= Data.size() &&
                    FrameDataSize >=0 &&
                    FrameDataSize <= D_FrameDataMaxSize)
            {
                QByteArray Cmd;
                Cmd.clear();
                Cmd = Data.mid(i, D_FrameMiniSize + FrameDataSize);
                i = i + D_FrameMiniSize + FrameDataSize - 1;
                if(littleToBig(getFrameCmd(Cmd)) == D_QueryX618NetworkArgAnswer)
                {
                    unsigned short FrameDataLen = littleToBig(getFrameDataLen(Cmd));
                    QByteArray FrameData = getFrameData(FrameDataLen, Cmd);
                    if(FrameData.size() >= 22)
                    {
                        Ip = (unsigned char)FrameData.at(7) << 24;
                        Ip += (unsigned char)FrameData.at(6) << 16;
                        Ip += (unsigned char)FrameData.at(5) << 8;
                        Ip += (unsigned char)FrameData.at(4);
                    }
                    break;
                }
            }
        }
    }
    return Ip;
}

unsigned int MainWindow::getDeviceMaskForNetData(QByteArray data)
{
    unsigned int Mask = 0;
    QByteArray Data = data;
    for(int i = 0;
        (i < Data.size()) && (Data.size() >= (i + D_FrameMiniSize));
        i++)
    {
        unsigned short FrameHeader = 0x0000;
        FrameHeader += (unsigned char)Data.at(i);
        FrameHeader += (unsigned char)Data.at(i + 1) << 8;
        if(FrameHeader == D_FrameHeader)
        {
            unsigned short FrameDataSize = (unsigned char)(Data.at(i + D_FrameDataSizeHigh)) +
                                    (unsigned char)(Data.at(i + D_FrameDataSizeLow) << 8);
            if((i + D_FrameMiniSize + FrameDataSize) <= Data.size() &&
                    FrameDataSize >=0 &&
                    FrameDataSize <= D_FrameDataMaxSize)
            {
                QByteArray Cmd;
                Cmd.clear();
                Cmd = Data.mid(i, D_FrameMiniSize + FrameDataSize);
                i = i + D_FrameMiniSize + FrameDataSize - 1;
                if(littleToBig(getFrameCmd(Cmd)) == D_QueryX618NetworkArgAnswer)
                {
                    unsigned short FrameDataLen = littleToBig(getFrameDataLen(Cmd));
                    QByteArray FrameData = getFrameData(FrameDataLen, Cmd);
                    if(FrameData.size() >= 22)
                    {
                        Mask = (unsigned char)FrameData.at(11) << 24;
                        Mask += (unsigned char)FrameData.at(10) << 16;
                        Mask += (unsigned char)FrameData.at(9) << 8;
                        Mask += (unsigned char)FrameData.at(8);
                    }
                    break;
                }
            }
        }
    }
    return Mask;
}

unsigned int MainWindow::getDeviceGatewayForNetData(QByteArray data)
{
    unsigned int Gateway = 0;
    QByteArray Data = data;
    for(int i = 0;
        (i < Data.size()) && (Data.size() >= (i + D_FrameMiniSize));
        i++)
    {
        unsigned short FrameHeader = 0x0000;
        FrameHeader += (unsigned char)Data.at(i);
        FrameHeader += (unsigned char)Data.at(i + 1) << 8;
        if(FrameHeader == D_FrameHeader)
        {
            unsigned short FrameDataSize = (unsigned char)(Data.at(i + D_FrameDataSizeHigh)) +
                                    (unsigned char)(Data.at(i + D_FrameDataSizeLow) << 8);
            if((i + D_FrameMiniSize + FrameDataSize) <= Data.size() &&
                    FrameDataSize >=0 &&
                    FrameDataSize <= D_FrameDataMaxSize)
            {
                QByteArray Cmd;
                Cmd.clear();
                Cmd = Data.mid(i, D_FrameMiniSize + FrameDataSize);
                i = i + D_FrameMiniSize + FrameDataSize - 1;
                if(littleToBig(getFrameCmd(Cmd)) == D_QueryX618NetworkArgAnswer)
                {
                    unsigned short FrameDataLen = littleToBig(getFrameDataLen(Cmd));
                    QByteArray FrameData = getFrameData(FrameDataLen, Cmd);
                    if(FrameData.size() >= 22)
                    {
                        Gateway = (unsigned char)FrameData.at(15) << 24;
                        Gateway += (unsigned char)FrameData.at(14) << 16;
                        Gateway += (unsigned char)FrameData.at(13) << 8;
                        Gateway += (unsigned char)FrameData.at(12);
                    }
                    break;
                }
            }
        }
    }
    return Gateway;
}

int MainWindow::sendX618NetworkArg(unsigned int ip, unsigned int mask,
                                   unsigned int gateway, QString mac)
{
    int Flag = 2;
    unsigned int NetworkType = 0;
    QByteArray Data;
    Data.clear();
    Data.append(NetworkType & 0x000000FF);
    Data.append((NetworkType >> 8) & 0x000000FF);
    Data.append((NetworkType >> 16) & 0x000000FF);
    Data.append((NetworkType >> 24) & 0x000000FF);
    Data.append(ip & 0x000000FF);
    Data.append((ip >> 8) & 0x000000FF);
    Data.append((ip >> 16) & 0x000000FF);
    Data.append((ip >> 24) & 0x000000FF);
    Data.append(mask & 0x000000FF);
    Data.append((mask >> 8) & 0x000000FF);
    Data.append((mask >> 16) & 0x000000FF);
    Data.append((mask >> 24) & 0x000000FF);
    Data.append(gateway & 0x000000FF);
    Data.append((gateway >> 8) & 0x000000FF);
    Data.append((gateway >> 16) & 0x000000FF);
    Data.append((gateway >> 24) & 0x000000FF);
    mac = mac + ":";
    for(int i=0;i<6;i++)
    {
        bool Ok = false;
        char Hex = mac.section(":", i, i).toInt(&Ok, 16);
        Data.append(Hex);
    }
    FrameCount = (FrameCount + 1) < 0 ? 0 : FrameCount + 1;
    unsigned char FrameFlag = (0 | D_FrameFlagCommand |
                               (D_FrameCrcEnable == true ? D_FrameFlagCheck : 0x00));
    QByteArray Frame = getFrameLittle(D_FrameVersion_1, FrameFlag, FrameCount,
                                D_X618SpecialID, D_X618NetworkArg, Data);
    X618UdpSocket->writeDatagram(Frame, QHostAddress(X618DeviceIp), X618WritePort);

    QTime Time;
    Time.restart();
    while(Time.elapsed() < (X618ResetWait * 1000))
    {
        ;
    }
    Data.clear();
    while(X618UdpSocket->hasPendingDatagrams() == true)
    {
        Data.resize(X618UdpSocket->pendingDatagramSize());
        QHostAddress Sender;
        quint16 SenderPort;
        X618UdpSocket->readDatagram(Data.data(), Data.size(), &Sender, &SenderPort);
    }
    QList<QByteArray> List = getCmds(Data);
    for(int i=0;i<List.size();i++)
    {
        QByteArray Cmd = List.at(i);
        if(littleToBig(getFrameCmd(Cmd)) == D_X618NetworkArgAnswer)
        {
            unsigned short FrameDataLen = littleToBig(getFrameDataLen(Cmd));
            QByteArray FrameData = getFrameData(FrameDataLen, Cmd);
            if(FrameData.size() >= 2)
            {
                Flag = (unsigned char)FrameData.at(0);
                Flag += (unsigned char)FrameData.at(1) << 8;
            }
            break;
        }
    }
    return Flag;
}

QList<QByteArray> MainWindow::getCmds(QByteArray data)
{
    QList<QByteArray> List;
    List.clear();
    QByteArray Data = data;
    for(int i = 0;
        (i < Data.size()) && (Data.size() >= (i + D_FrameMiniSize));
        i++)
    {
        unsigned short FrameHeader = 0x0000;
        FrameHeader += (unsigned char)Data.at(i);
        FrameHeader += (unsigned char)Data.at(i + 1) << 8;
        if(FrameHeader == D_FrameHeader)
        {
            unsigned short FrameDataSize = (unsigned char)(Data.at(i + D_FrameDataSizeHigh)) +
                                    (unsigned char)(Data.at(i + D_FrameDataSizeLow) << 8);
            if((i + D_FrameMiniSize + FrameDataSize) <= Data.size() &&
                    FrameDataSize >=0 &&
                    FrameDataSize <= D_FrameDataMaxSize)
            {
                QByteArray Cmd;
                Cmd.clear();
                Cmd = Data.mid(i, D_FrameMiniSize + FrameDataSize);
                i = i + D_FrameMiniSize + FrameDataSize - 1;
                List.append(Cmd);
            }
        }
    }
    return List;
}

void MainWindow::updataX618FirstMac()
{
    QString Path = "./MacProgrammer.ini";
    QFileInfo File(Path);
    if(File.exists() && File.isFile())
    {
        unsigned int ValidFirstMac = 0;
        getX618ValidMac(X618FirstMac, &ValidFirstMac);
        //unsigned int ValidMacCount = ValidFirstMac + MacPCS;
        unsigned int ValidMacCount = ValidFirstMac + X618TimerMacIndex;
        QString FirstPart = "";
        if((X618LowestValidByte - X618ValidByte) >= 0)
        {
            FirstPart = X618FirstMac.section(":", 0, X618LowestValidByte - X618ValidByte) + ":";
        }
        QString MidPart = "";
        for(int n=X618ValidByte-1;n>=0;n--)
        {
            MidPart += QString::number(((ValidMacCount >> (n * 8)) & 0x000000FF), 16) + ":";
        }
        QString LastPart = X618FirstMac.section(":", X618LowestValidByte + 1);
        QString Mac = QString("%1%2%3").arg(FirstPart).arg(MidPart).arg(LastPart);
        QSettings Setting(Path, QSettings::IniFormat);
        Setting.setValue("T_X618/T_FirstMac", RepairMac(Mac));
    }
}

QByteArray MainWindow::getFrameLittle(unsigned char frameVersion, unsigned char frameFlag,
                          unsigned short frameNumber, unsigned int frameSrc,
                          unsigned short frameCmd,
                          QByteArray frameData)
{
    unsigned int FrameDest = D_X618SpecialID;
    QByteArray Cmd;
    Cmd.clear();
    Cmd.append(frameVersion);
    Cmd.append(frameFlag);
    Cmd.append(frameNumber & 0x000000FF);
    Cmd.append((frameNumber >> 8) & 0x000000FF);
    Cmd.append(frameSrc & 0x000000FF);
    Cmd.append((frameSrc >> 8) & 0x000000FF);
    Cmd.append((frameSrc >> 16) & 0x000000FF);
    Cmd.append((frameSrc >> 24) & 0x000000FF);
    Cmd.append(FrameDest & 0x000000FF);
    Cmd.append((FrameDest >> 8) & 0x000000FF);
    Cmd.append((FrameDest >> 16) & 0x000000FF);
    Cmd.append((FrameDest >> 24) & 0x000000FF);
    Cmd.append(frameCmd & 0x000000FF);
    Cmd.append((frameCmd >> 8) & 0x000000FF);
    Cmd.append(frameData.size() & 0x000000FF);
    Cmd.append((frameData.size() >> 8) & 0x000000FF);
    Cmd.append(frameData);
    unsigned short FrameCrC = CRC16Count(Cmd);
    if(getDeviceType() == D_X618NPM)
    {
        char Hex = 0x00;
        for(int i=0;i<(1024-frameData.size());i++)
        {
            Cmd.append(Hex);
        }
    }
    Cmd.append(FrameCrC & 0x000000FF);
    Cmd.append((FrameCrC >> 8) & 0x000000FF);
    QByteArray Frame;
    Frame.clear();
    Frame.append(D_FrameHeader & 0x00FF);
    Frame.append((D_FrameHeader >> 8) & 0x00FF);
    Frame.append(Cmd);
    return Frame;
}

unsigned short MainWindow::bigToLittle(unsigned short data)
{
    unsigned short Data = 0x0000;
    Data = (data >> 8) & 0x00FF;
    Data += (data << 8) & 0xFF00;
    return Data;
}

unsigned short MainWindow::littleToBig(unsigned short data)
{
    unsigned short Data = 0x0000;
    Data = (data >> 8) & 0x00FF;
    Data += (data << 8) & 0xFF00;
    return Data;
}

void MainWindow::checkBoxPrintSlot(bool checked)
{
    ui->WidgetPrint->setHidden(!checked);
    PrintStartup = checked;

    QString Path = "./MacProgrammer.ini";
    QFileInfo File(Path);
    if(File.exists() && File.isFile())
    {
        QSettings Setting(Path, QSettings::IniFormat);
        Setting.setValue("T_Print/T_Startup", PrintStartup);
    }
}

void MainWindow::pushButtonPrinterSlot()
{
    QPrintDialog Dlg(PrintPrinter, this);
    Dlg.setWindowTitle("选择打印机");
    if(Dlg.exec() != QDialog::Accepted)
    {
        return;
    }
    QList<QPrinterInfo> List = QPrinterInfo::availablePrinters();
    for(int i=0;i<List.size();i++)
    {
        if(QPrinter(List.at(i)).printerName() == PrintPrinter->printerName())
        {
            QString Path = "./MacProgrammer.ini";
            QFileInfo File(Path);
            if(File.exists() && File.isFile())
            {
                QSettings Setting(Path, QSettings::IniFormat);
                Setting.setValue("T_Print/T_Printer", i);
            }
            ui->LabelPrinter->setText(PrintPrinter->printerName());
        }
    }
}

void MainWindow::pushButtonFontSlot()
{
    bool Ok = false;
    QFont Font = QFontDialog::getFont(&Ok, PrintFont, this);
    if(Ok == true)
    {
        PrintFont = Font;
        QString Path = "./MacProgrammer.ini";
        QFileInfo File(Path);
        if(File.exists() && File.isFile())
        {
            QFontDatabase Fonts;
            for(int i=0;i<Fonts.families().size();i++)
            {
                if(PrintFont.family() == Fonts.families().at(i))
                {
                    QSettings Setting(Path, QSettings::IniFormat);
                    Setting.setValue("T_Print/T_Font", QString("%1,%2,%3,%4,%5,%6").
                                     arg(i).
                                     arg(PrintFont.bold()).
                                     arg(PrintFont.italic()).
                                     arg(PrintFont.pointSize()).
                                     arg(PrintFont.strikeOut()).
                                     arg(PrintFont.underline()));
                }
            }
        }
        showFontInfo(PrintFont);
    }
}

void MainWindow::showFontInfo(QFont font)
{
    ui->LabelFont->setText("");
    ui->LabelFont->setText(ui->LabelFont->text() + QString("%1(%2号)").
                           arg(font.family()).
                           arg(font.pointSize()));
    if(font.bold() == true)
    {
        ui->LabelFont->setText(ui->LabelFont->text() + " " + "粗");
    }
    if(font.italic() == true)
    {
        ui->LabelFont->setText(ui->LabelFont->text() + " " + "斜");
    }
    if(font.strikeOut() == true)
    {
        ui->LabelFont->setText(ui->LabelFont->text() + " " + "删除线");
    }
    if(font.underline() == true)
    {
        ui->LabelFont->setText(ui->LabelFont->text() + " " + "下划线");
    }
}

void MainWindow::lineEditLeftSlot(QString text)
{
    PrintLeft = text.toInt();
    QString Path = "./MacProgrammer.ini";
    QFileInfo File(Path);
    if(File.exists() && File.isFile())
    {
        QSettings Setting(Path, QSettings::IniFormat);
        Setting.setValue("T_Print/T_Left", PrintLeft);
    }
}

void MainWindow::lineEditTopSlot(QString text)
{
    PrintTop = text.toInt();
    QString Path = "./MacProgrammer.ini";
    QFileInfo File(Path);
    if(File.exists() && File.isFile())
    {
        QSettings Setting(Path, QSettings::IniFormat);
        Setting.setValue("T_Print/T_Top", PrintTop);
    }
}

void MainWindow::printMac(QString mac)
{
    mac = "MAC:" + mac.remove(":");
    if(PrintStartup == true && PrintPrinter != NULL)
    {
        QPainter Painter;
        if(Painter.begin(PrintPrinter) == true)
        {
            Painter.setFont(PrintFont);
            Painter.drawText(PrintLeft, PrintTop + Painter.fontMetrics().height(), mac);
        }
    }
}
