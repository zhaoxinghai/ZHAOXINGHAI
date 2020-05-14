
/*
* Date:     2016-8-4
* Author :  xinghai,zhao
* Content : define the callback function and return result
*/

#ifndef CALLBACK_H
#define CALLBACK_H

#include "predefine.h"
#include "paerror.h"
#include "device.h"

#ifdef _MSC_VER
#define MYSOCK SOCKET
#else
#define MYSOCK int
#endif

enum CONSTATE
{
    CON_FULL_CONNECT,
    CON_PARTLY_CONNECT,
    CON_INTERRUPTED,
    CON_DIS_CONNECT,
    CON_RECONNECT_POSSIBLE,
    CON_PARTLY_DEFECT,   //internal used
    CON_UNKNOW
};

//for the device Status
enum e_StateRetVal
{
    RETVAL_OK = 0,
    RETVAL_ERROR,
    NOT_INSTALLED,
    NOT_INSTALLED_HIGHER,
    NOT_INSTALLED_LOWER
};

enum e_ControlState
{
    CONTROL_NOT_AVAILABLE = 0,
    CONTROL_OK = 1,
    CONTROL_DEFECT = 2,

    CONTROL_MICROPHONE_DEFECT = 3,  //V11,NewMsg,Device Info
    CONTROL_SIGNAL_LOST = 4         //V11,NewMsg,Device Info
};

enum e_AudioState
{
    AUDIO_NOT_AVAILABLE = 0,
    AUDIO_OK,
    AUDIO_LEVEL_HIGH,         //amplification out of allowed range
    AUDIO_LEVEL_LOW,          //amplification out of allowed range
    AUDIO_THD_HIGH,           //total harmonic threshold too high
    AUDIO_NO_SETPOINT,        //

    //V11
    AUDIO_IMPEDANCE_HIGH,     //line impedance out of allowed range
    AUDIO_IMPEDANCE_LOW,      //line impedance out of allowed range
    AUDIO_IMPEDANCE_SHORT,    //line short circuit
    AUDIO_IMPEDANCE_OPEN,     //line open circuit
    AUDIO_EARTH_FAULT,        //line earth fault
};

enum e_BusyState
{
    BUSY_NOT_AVAILABLE,
    BUSY_ACTIVATE,
    BUSY_INACTIVATE
};


//for News message
enum e_DeviceInfoState
{
    DEVICE_INFO_OK,
    DEVICE_INFO_DEFECT,

    DEVICE_INFO_CONTROL_ERROR,      //V11
    DEVICE_INFO_MICROPHONE_DEFECT,  //V11
    DEVICE_INFO_SIGNAL_LOST         //V11
};

enum e_AudioInfoState
{
    AUDIO_INFO_OK,
    AUDIO_INFO_LEVEL_HIGH,
    AUDIO_INFO_LEVEL_LOW,
    AUDIO_INFO_THD_HIGH,
    AUDIO_INFO_NO_SETPOINT,
    AUDIO_INFO_GENERAL,
    AUDIO_INFO_22KHZ_SURVEY
};

enum e_ReplaceState
{
    REPLACE_INACTIVATE,
    REPLACE_ACTIVATE
};

//for NewsMessage
enum e_NewsMsgType
{
    PAMSG_NEWSTYPE_BOOT = 0,
    PAMSG_NEWSTYPE_DEVICE = 1,
    PAMSG_NEWSTYPE_AUDIO = 2,
    PAMSG_NEWSTYPE_REPLACE = 3,
    PAMSG_NEWSTYPE_TEXT = 8
};

enum e_BootMode
{
    COLD_BOOT = 0,
    WARM_BOOT,
    RESET_BOOT
};

enum e_ReadMsgRetVal
{
    READ_MSG_OK = 0,
    READ_MSG_ERROR,
    READ_MSG_EMPTY,
    READ_MSG_TOOHIGH,
};

struct t_Time
{
    int   Sec;	    //0 to 59	second
    int   Min;	    //0 to 59	minute
    int   Hour;	    //0 to 23	hour
};

struct t_DateTime
{
    t_DateTime()
    {
        Sec = 0;
        Min = 0;
        Hour = 0;
        Day = 1;
        Month = 1;
        Year = 1990;
        wDay = 0;
    }
    int   Sec;	    //0 to 59	second
    int   Min;	    //0 to 59	minute
    int   Hour;	    //0 to 23	hour
    int   Day;	    //1 to 31	day
    int   Month;	//1 to 12	month
    int   Year;	    //0 to 9999(for ever)	year
    int   wDay;     //0 to 6    day of the week
};

//////////////////////////////////////////////////////////////////////

struct CResultBase
{
    CResultBase()
    {
        strClassName = "CResultBase";
        nErrorCode = 0;
        strErrorDesc = "OK";
    }
    virtual ~CResultBase()
    {
    }
    std::string   strClassName;
    int           nErrorCode;
    std::string   strErrorDesc;
};

//interface AskVersion's result
struct CVersonResult : public CResultBase
{
    CVersonResult()
    {
        strClassName = "CVersonResult";
        nProtVerAct = 0;
        nPortVerMax = 0;
    }
    virtual ~CVersonResult()
    {
    }
    std::string strVer;
    std::string strDate;
    std::string strTime;
    std::string strCompiler;
    int    nProtVerAct;
    int    nPortVerMax;
};

//remote system is connect or disconnect
struct CConnectResult : public CResultBase
{
    CConnectResult()
    {
        strClassName = "CConnectResult";
        bConnect = false;
        bStandalone = false;
        bTCP = false;
        sock = 0;
        type = DEVICE_IP_SPEAKER_A20;
    }
    virtual ~CConnectResult()
    {
    }
    std::string   strIP;
    e_DEVICE_TYPE type;
    bool bTCP;
    MYSOCK sock;
    bool bConnect;
    bool bStandalone;
};

//remote system error state
struct CSysStateResult : public CResultBase
{
    CSysStateResult()
    {
        strClassName = "CSysStateResult";
        IsError = false;
        bErrorChange = false;
        nNode = 0;
    }
    virtual ~CSysStateResult()
    {
    }
    int         nNode;
    std::string strIP;
    bool   bErrorChange;
    bool   IsError;
};

//announcement's state
struct CActivateResult : public CResultBase
{
    CActivateResult()
    {
        strClassName = "CActivateResult";
        LastSrcIdx = 0;
        SrcIdx = 0;
        RetVal = -1;
        chProcess = 0;
        chRequest = 0;
        e_ConState = CON_UNKNOW;
        bCreateAudioJob = true;
    }
    virtual ~CActivateResult()
    {
    }
    int RetVal;  //0:OK  1:Error
    int chRequest;
    int chProcess;
    CONSTATE e_ConState;
    int SrcIdx;
    int LastSrcIdx;

    bool bCreateAudioJob;  //inter used
};

//local adp play listening
struct CLocalPlayListeningResult : public CResultBase
{
    CLocalPlayListeningResult()
    {
        chRequest = -1;
        strClassName = "CLocalPlayListeningResult";
    }
    virtual ~CLocalPlayListeningResult()
    {
    }
    int chRequest;
};

//local record to adp file
struct CLocalRecordResult : public CResultBase
{
    CLocalRecordResult()
    {
        strClassName = "CLocalRecordResult";
        chRequest = -1;
        chProcess = -1;
    }
    virtual ~CLocalRecordResult()
    {
    }
    std::string    strFilePath;
    int       chRequest;
    int       chProcess;
};

//usage report
struct t_UsageReport
{
    t_UsageReport()
    {
        bInput = false;
        bLocalSource = false;
        nPort = 0;
        nTitle = 0;
        nRtpNode = 0;
        nRtpChannel = 0;
        nPriority = 0;
        nCSNode = 0;
        nCSNumber = 0;
        ConState = 0;
        nSource = 0;
    }
    bool      bInput;
    bool      bLocalSource;
    int       nPort;
    int       nRtpNode;
    int       nRtpChannel;

    int       nSource;   //V10

    int       nTitle;    //V11
    int       ConState;  //V11
    int       nPriority; //V11
    int       nCSNode;   //V11
    int       nCSNumber; //V11
};

//destination usage report
struct CUsageReportResult : public CResultBase
{
    CUsageReportResult()
    {
        strClassName = "CUsageReportResult";
    }
    virtual ~CUsageReportResult()
    {
        vBusyInfo.clear();
    }
    CUsageReportResult& operator = (const CUsageReportResult& c)
    {
        if(this != &c)
        {
            this->nErrorCode = c.nErrorCode;
            this->strErrorDesc = c.strErrorDesc;
            this->strIP = c.strIP;
            this->vBusyInfo.clear();
            this->vBusyInfo.assign(c.vBusyInfo.begin(), c.vBusyInfo.end());
        }
        return (*this);
    }

    std::string strIP;
    std::vector<t_UsageReport> vBusyInfo;
};

//for local announcement, audio file is error
struct CFileErrorResult :public CResultBase
{
    CFileErrorResult()
    {
        strClassName = "CFileErrorResult";
    }
    virtual ~CFileErrorResult()
    {
    }
    std::string  strPath;
};

struct CSourceIndexResult :public CResultBase
{
    CSourceIndexResult()
    {
        strClassName = "CSourceIndexResult";
        chProcess = 0;
        nSourceIndex = 0;
    }
    virtual ~CSourceIndexResult()
    {
    }
    int    chProcess;
    int    nSourceIndex;
};

//a repeat announcement, current repeat count
struct CCurrentRepeat :public CResultBase
{
    CCurrentRepeat()
    {
        strClassName = "CCurrentRepeat";
        chProcess = 0;
        nCurrentRepeat = 0;
    }
    virtual ~CCurrentRepeat()
    {
    }
    int    chProcess;
    int    nCurrentRepeat;
};

//transportline from remote network system
struct CTransportLineResult : public CResultBase
{
    CTransportLineResult()
    {
        strClassName = "CTransportLineResult";
        bAction = false;
        nNode = 0;
    }
    virtual ~CTransportLineResult()
    {
        vTL.clear();
    }
    CTransportLineResult& operator = (const CTransportLineResult &c)
    {
        if(this != &c)
        {
            this->bAction = c.bAction;
            this->nNode = c.nNode;
            this->strErrorDesc = c.strErrorDesc;
            this->vTL.clear();
            this->vTL.assign(c.vTL.begin(), c.vTL.end());
        }
        return (*this);
    }

    bool          bAction;
    int           nNode;
    std::vector<t_TL>  vTL;
};

//your are a time client int the network,update your datetime
struct CDataTimeResult : public CResultBase
{
	CDataTimeResult()
    {
        strClassName = "CDataTimeResult";
    }
    virtual ~CDataTimeResult()
    {
    }
    t_DateTime  datatime;
};

//interface TelnetCmd's result
struct CTelnetResult : public CResultBase
{
    CTelnetResult()
    {
        strClassName = "CTelnetResult";
        nNode = 0;
        nRequest = 0;
    }
    virtual ~CTelnetResult()
    {
    }
    int     nNode;
    int     nRequest;
    std::string  strCmd;
    std::string  strResult;
};

//is play pre-signal now?
struct CPresignalResult : public CResultBase
{
	CPresignalResult()
	{
		strClassName = "CPresignalResult";
		bPlaying = true;
		chProcess = 0;
	}
    virtual ~CPresignalResult()
    {

    }

	int    chProcess;
	bool   bPlaying;
};

enum e_DE_STATE
{
    DECODE_WAIT,
    DECODE_DOING,
    DECODE_DONE
};

struct CDecodeAudioResult : public CResultBase
{
    CDecodeAudioResult()
    {
        strClassName = "CDecodeAudioResult"; 
        eState = DECODE_WAIT;
        bRet = false;
    }
    virtual ~CDecodeAudioResult()
    {

    }
    e_DE_STATE   eState;
    std::string  strSrc;
    std::string  strCache;
    bool         bRet;
};

//is microphone activate(for job)
struct CMicResult : public CResultBase
{
	CMicResult()
	{
		strClassName = "CMicResult";
		bMicOpen = true;
		chProcess = 0;
        nPort = 0;
	}
    virtual ~CMicResult()
    {

    }

	int    chProcess;
    int    nPort;
	bool   bMicOpen;
};

struct CTextInResult : public CResultBase
{
    CTextInResult()
    {
        strClassName = "CTextInResult";
        nNode = 0;
        tcpSocket = -1;
    }
    virtual ~CTextInResult()
    {

    }
    int         nNode;
    MYSOCK      tcpSocket;
    std::string strCmd;
};

struct CAudioControlResult : public CResultBase
{
    CAudioControlResult()
    {
        strClassName = "CAudioControlResult";
        IsTCP = false;
    }
    virtual ~CAudioControlResult()
    {
    }
    bool        IsTCP;
    int         nNode;
    MYSOCK      tcpSocket;
    t_VDevice   device;
    float       fVolume;
};

struct CAudioReplyResult : public CResultBase
{
    CAudioReplyResult()
    {
        strClassName = "CAudioReplyResult";
        fVolume = 0;
        fVolMin = 0;
        fVolMax = 0;
    }
    virtual ~CAudioReplyResult()
    {
    }
    int         nNode;
    t_VDevice   device;
    float       fVolume;
    float       fVolMin;   //for 11 only;
    float       fVolMax;   //for 11 only;
};

struct CLampTestResult : public CResultBase
{
    CLampTestResult()
    {
        strClassName = "CLampTestResult";
        bActivate = true;
        nNode = 0;
    }
    virtual ~CLampTestResult()
    {
    }
    int  nNode;
    bool bActivate;
};

struct CMuteSpeakerResult : public CResultBase
{
    CMuteSpeakerResult()
    {
        strClassName = "CMuteSpeakerResult";
        bActivate = true;
        nNode = 0;
    }
    virtual ~CMuteSpeakerResult()
    {
    }
    int  nNode;
    bool bActivate;
};

#endif  //CALLBACK_H
