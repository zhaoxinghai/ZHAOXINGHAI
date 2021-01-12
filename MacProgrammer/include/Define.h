#ifndef DEFINE_H
#define DEFINE_H

#include <QString>
#include <QByteArray>
#include "TranslatorStr.h"

#define D_Version              ": 1.1.9 20180330"  //add by ZL

#define D_NameSize              22
#define D_PointPlaySongSize     64

#define D_ColorWhiteAlpha               "rgba(255, 255, 255, 192)"
#define D_ColorYellowAlpha              "rgba(255, 255, 64, 192)"
#define D_ColorGrayAlpha                "rgba(192, 192, 192, 192)"
#define D_ColorRedAlpha                 "rgba(255, 0, 0, 192)"
#define D_ColorGreenAlpha               "rgba(0, 160, 0, 192)"

#define D_OddColor          "#FFFFDE"
#define D_EvenColor         "#C9D6E5"

#define D_Master            true
#define D_ASServerPath      "../ASServerGUI-build-desktop/debug/ASServerGUI.exe"
#define D_ASClientIni       "./ASClient.ini"
#define D_ASServerGUIIni    "./ASServerGUI.ini"
#define D_ConversionIni     "./Conversion.ini"
#define D_CheckIpIni        "./CheckIp.ini"

#define D_ASClientLog       "./Log/ASClient %1.log"
#define D_ASServerLog       "./Log/ASServer %1.log"
#define D_DeviceErrorLog    "./Log/DeviceError %1.log"
#define D_OperationLog      "./Log/Operation %1.log"
#define D_ASClientDataBase  "./ASClient.db"
#define D_ASServerDataBase  "./ASServer.db"
#define D_SongListDataBase  "./Song.lst"

#define D_WidgetMinWidth    50
#define D_WidgetMinHeight   25
#define D_WidgetWidth       100
#define D_WidgetHeight      120

#define D_MapSize           1024
#define D_AudioMapNumber    1
#define D_ZoneMapNumber     2
#define D_SpacingSymbol     ","

#define D_Confirm           1
#define D_Cancel            0

#define D_ClientCtrlPort    9002
#define D_DeviceCtrlPort    8002
#define D_DataPort          2011
#define D_ClientDebugPort   9007
#define D_ServerDebugPort   8007
#define D_SDKToClientPort   10001
#define D_ClientToSDKPort   10002

#define D_DialogWidth       (800 - 80)
#define D_DialogHeight      (600 - 60)
#define D_DialogWidth800    800
#define D_DialogHeight600   600

#define D_StatusTimeout     10000
#define D_OnlineCount       3

//----------------------------------------------------
#define D_Produce           TranslatorStr::getProduceStr()
#define D_Project           TranslatorStr::getProjectStr()

#define D_Enabled           TranslatorStr::getEnabledStr()
#define D_Disabled          TranslatorStr::getDisabledStr()

#define D_Repeat            TranslatorStr::getRepeatStr()
#define D_Loop              TranslatorStr::getLoopStr()
#define D_Shuffle           TranslatorStr::getShuffleStr()
#define D_General           TranslatorStr::getGeneralStr()

#define D_NotAvailableStr   TranslatorStr::getInvalidStr()
#define D_UnusedStr         TranslatorStr::getUnusedStr()
#define D_BusyStr           TranslatorStr::getBusyStr()
#define D_AlarmStr          TranslatorStr::getAlarmStr()
#define D_DefectiveStr      TranslatorStr::getDefectiveStr()

#define D_Unconnected       TranslatorStr::getUnconnectedStr()
#define D_Connected         TranslatorStr::getConnectedStr()
#define D_Alarmed           TranslatorStr::getAlarmStr()

#define D_Online            TranslatorStr::getOnlineStr()
#define D_Offline           TranslatorStr::getOfflineStr()

#define D_MondayStr         TranslatorStr::getMondayStr()
#define D_TuesdayStr        TranslatorStr::getTuesdayStr()
#define D_WednesdayStr      TranslatorStr::getWednesdayStr()
#define D_ThursdayStr       TranslatorStr::getThursdayStr()
#define D_FridayStr         TranslatorStr::getFridayStr()
#define D_SaturdayStr       TranslatorStr::getSaturdayStr()
#define D_SundayStr         TranslatorStr::getSundayStr()
//----------------------------------------------------

#define D_SpinBoxStyle      "Plastique"

#define D_MapViewLevel      0x80
#define D_ClassicViewLevel  0x40
#define D_TerminalViewLevel 0x20
#define D_ZoneViewLevel     0x10
#define D_CalendarLevel     0x08
#define D_SettingLevel      0x04
#define D_MonitorLevel      0x02

#define D_Monday            1
#define D_Tuesday           2
#define D_Wednesday         3
#define D_Thursday          4
#define D_Friday            5
#define D_Saturday          6
#define D_Sunday            7

#define D_UserLogon         true
#define D_HighVersion       true

#define D_SongTimesMax      10
#define D_SongSizeMax       1000

#define D_AlarmPoint        0x01
#define D_NeighborPoint     0x02

#define D_TerminalCtrlMult  "234.0.0.96"

#define D_TerminalMaxSize       999
//#define D_TerminalMaxSize       8
//#define D_TerminalMaxSize       3
//#define D_TerminalMaxSize       5
//#define D_TestRun             "2019-01-01"

//#define D_DogStartup          false   //lyf 20180115
#define D_DogStartup          true

//#define D_Logo                ":/Resource/Logo.PNG"       //TK-Audio
//#define D_NetAddress          "http://tk-audio.cn/"
//#define D_Logo                ":/Resource/TKOKO.PNG"      //TKOKO
//#define D_NetAddress          "http://www.t-koko.com/"
#define D_Logo                  ""       //TK-Audio
#define D_NetAddress            ""
#define D_ChannelBackupMap

#define D_MicMaxSize            999
#define D_TerminalMaxChannel    2
#define D_ExAlarmDeviceMaxSize  100
#define D_RepeaterMaxSize       999
#define D_SendRepeaterSizeMax   20

#define D_LanguageChinese       0
#define D_LanguageEnglish       1
#define D_LanguageTaiwan        2

#define D_SendEmptyPacketTime   2
#define D_SendPacketSize        640
#define D_SendPacketTimeout     40  //(640 / (128000 / 8)) = 40)

#define D_DefaultUser           "Default"

#define D_OfflineFlag           0x00
#define D_OnlineFlag            0x01
#define D_MonitorFlag           0x02
#define D_TalkFlag              0x03
#define D_NotMonitorFlag        0x04

#define D_DefectiveKeyAMP       "AMP"
#define D_DefectiveKeyMP3       "MP3"
#define D_DefectiveKeyPCM       "PCM"
#define D_DefectiveKeySDC       "SDC"
#define D_DefectiveKeyIOC       "IOC"
#define D_DefectiveKeyVOL       "VOL"
#define D_DefectiveKeyRTC       "RTC"
#define D_DefectiveKeyEEP       "EEP"
#define D_DefectiveKeyFSH       "FSH"
#define D_DefectiveKeySRM       "SRM"

/*enum E_TaskType
{
    E_All = 0,
    E_Day,
    E_Week
};*/

#define D_RemotePhotoBMP        0x0001
#define D_RemotePhotoJPG        0x0002
#define D_RemotePhotoBMPRGB     2

enum E_MicRemoteKey
{
    E_MicInvalid = 0x0000,
    E_MicOne,
    E_MicTwo,
    E_MicThree,
    E_MicFour,
    E_MicFive,
    E_MicSix,
    E_MicSeven,
    E_MicEight,
    E_MicNine,
    E_MicConfirm,
    E_MicZero,
    E_MicCancel,
    E_MicMonitor,
    E_MicTalk,
    E_MicSetting,
    E_MicUp,
    E_MicDown,
    E_MicVolPlus,
    E_MicVolSubtract,
    E_MicBroadcast
};

enum E_AmpRemoteKey
{
    E_AmpInvalid = 0x0000,
    E_AmpBackLight,
    E_AmpSetting,
    E_AmpMute,
    E_AmpOne,
    E_AmpTwo,
    E_AmpThree,
    E_AmpFour,
    E_AmpFive,
    E_AmpSix,
    E_AmpSeven,
    E_AmpEight,
    E_AmpNine,
    E_AmpChannel,
    E_AmpZero,
    E_AmpLocal,
    E_AmpSelectSong,
    E_AmpCall,
    E_AmpUSB,
    E_AmpMode,
    E_AmpExtend,
    E_AmpCancel,
    E_AmpUp,
    E_AmpLeft,
    E_AmpConfirm,
    E_AmpRight,
    E_AmpDown,
    E_AmpPrevious,
    E_AmpPlayOrPause,
    E_AmpVolPlus,
    E_AmpNext,
    E_AmpStop,
    E_AmpVolSubtract
};

enum E_DefectiveKey
{
    E_DefectiveKeyAMP = 0,
    E_DefectiveKeyMP3,
    E_DefectiveKeyPCM,
    E_DefectiveKeySDC,
    E_DefectiveKeyIOC,
    E_DefectiveKeyVOL,
    E_DefectiveKeyRTC,
    E_DefectiveKeyEEP,
    E_DefectiveKeyFSH,
    E_DefectiveKeySRM
};
#define D_DefectiveKeyTotal       ((int)(E_DefectiveKey::E_DefectiveKeySRM) + 1)
enum E_DefectiveState
{
    E_Normal = 0,
    E_Broken
};
enum E_InfoNotificationObjectType
{
    E_ObjectZone = 1,
    E_ObjectDevice
};
enum E_InfoNotificationType
{
    E_PriorityLow = 1,
    E_DeviceNotAvailable
};
enum E_PAState
{
    E_NotAvailable = 0,
    E_Unused,
    E_Mute,
    E_Busy,
    E_Alarm,
    E_Defective
};
enum E_Program
{
    E_ProgramUnknown = 0,
    E_NCXI02S
};
enum E_Widget
{
    E_WidgetUnknown = 0,
    E_Zone,
    E_Audio,
    E_Map
};
enum E_EventType
{
    E_WarningType = 1001,
    E_InformationType
};
enum E_PlayerMode
{
    E_GeneralPlayerMode = 0,
    E_GatherPlayerMode
};
enum E_PlayMode
{
    E_GeneralPlayMode = 0,
    E_RepeatPlayMode,
    E_LoopPlayMode,
    E_ShufflePlayMode,
    E_OncePlayMode
};

enum E_Error
{
    Natural = 0,
    FrameHeaderError,
    FrameVersionError,
    FrameFlagError,
    FrameSrcSubSystemError,
    FrameSrcModelError,
    FrameSrcNumberError,
    FrameDestSubSystemError,
    FrameDestModelError,
    FrameDestNumberError,
    FrameCmdError,
    FrameLenError,
    FrameCrcError,
    DeviceNumberError,
    DeviceModelError,
    DeviceSubSystemError,
    AS1201NULL,
    AS1201UdpNULL,
    IpError,
    AddInAudioModeError,
    MonitorActionError,
    NameError,
    VolumeTypeError,
    VolumeError,
    VolumeModuleError,
    MulticastIDError,
    MulticastTotalError,
    PointPlayModeError,
    PointPlayActionError,
    ASServerUdpNULL,
    BindASServerUdpError,
    ASClientIniError,
    AS1200WSNULL,
    AS1200WSUdpNULL,
    FileNotExist,
    LatticeActionError,
    SystemCtrlActionError,
    CmdUnused,
    FrameDataError,
    AnswerError,
    AudioNULL,
    Not128kbps,
    LockError,
    AS1203NULL,
    AS1203UdpNULL
};

class SongInfo
{
public:
    SongInfo()
    {
        Path = "";
        Times = 0;
    }
    ~SongInfo()
    {
        ;
    }
    void setPath(QString path)
    {
        Path = path;
    }
    void setTimes(int times)
    {
        Times = times;
    }
    QString getPath()
    {
        return Path;
    }
    int getTimes()
    {
        return Times;
    }

private:
    QString Path;
    int Times;
};

class SongLattice
{
public:
    SongLattice(QString name, int id, bool isDir, char type)
    {
        Name = QString(name.toAscii().left(D_NameSize));
        ID = id;
        DirFlag = isDir;
        Type = type;
    }
    QString getName()
    {
        return Name;
    }
    int getID()
    {
        return ID;
    }
    bool isDir()
    {
        return DirFlag;
    }
    char getType()
    {
        return Type;
    }
    void setName(QString name)
    {
        Name = QString(name.toAscii().left(D_NameSize));
    }
private:
    QString Name;
    int ID;
    bool DirFlag;
    char Type;
};

class ServerInfo
{
public:
    ServerInfo(QString ip, int ctrlPort, int dataPort)
    {
        Ip = ip;
        CtrlPort = ctrlPort;
        DataPort = dataPort;
    }
    QString getIp()
    {
        return Ip;
    }
    int getCtrlPort()
    {
        return CtrlPort;
    }
    int getDataPort()
    {
        return DataPort;
    }
private:
    QString Ip;
    int CtrlPort;
    int DataPort;
};

class AudioLattice
{
public:
    AudioLattice(QString name, unsigned int ip, char mode, int priority)
    {
        Name = QString(name.toAscii().left(D_NameSize));
        Ip = ip;
        Mode = mode;
        Priority = priority;
    }
    QString getName()
    {
        return Name;
    }
    unsigned int getIp()
    {
        return Ip;
    }
    char getMode()
    {
        return Mode;
    }
    int getPriority()
    {
        return Priority;
    }
    void setName(QString name)
    {
        Name = QString(name.toAscii().left(D_NameSize));
    }
    void setIp(unsigned int ip)
    {
        Ip = ip;
    }
    void setMode(char mode)
    {
        Mode = mode;
    }
    void setPriority(int priority)
    {
        Priority = priority;
    }
private:
    QString Name;
    int Ip;
    bool Mode;
    int Priority;
};

//#pragma pack(push,1) //来对齐方式设置压栈，并设新的对齐方式设置为一个字节对齐

//struct song_info_s
//{
//    unsigned short index;
//    unsigned char name[D_NameSize];
//};

//#pragma pack(pop)//恢复对齐状态

#endif // DEFINE_H
