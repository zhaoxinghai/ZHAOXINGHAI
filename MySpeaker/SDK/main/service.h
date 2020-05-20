
#ifndef SERVICE_H
#define SERVICE_H

#include "sdkdefine.h"
#include "sdk.h"
#include "sdkconnect.h"
#include "mythread.h"
#include "topsystem.h"
#include "threadrecv.h"
#include "threadsend.h"
#include "threadaccept.h"
#include "threadtimer.h"
#include "threaddevice.h"
#include "threadcapture.h"
#include "threadplay.h"
#include "threaddevice.h"
#include "define.h"
#include "routemanager.h"
#include "threadpool.h"

struct t_Levelmeter
{
    CTSystem  *pSys;
    int        nCounter;
    bool       bCapture;
    int        nPort;
};

class CThreadCapture;
class CThreadPlay;
class CService: public CMyThread
{
public:
    CService();
    ~CService();

    //main thread message run
    virtual void Run();

    //init dom's node
    bool Init(t_Node &curNode,std::vector<t_Node> &vSystem);

    //init IOMAP
    void InitFileMAP(std::vector<t_FILEMAP> &vIOMAP);

    //get file path from channel-title
    bool GetFileMAP(int nChannel,int nTitle,t_FILEMAP &file);

    //begin thread
    void Start();

    //get current node
    int GetNode();

public:
    //push to message loop
    void Push(std::shared_ptr<CMsg> pMsg);

    //exit
    void Exit(bool bWait);
    bool IsExit();

    //main thread timer
    void Every200MiniSecond();
    void EverySecond();
    void EveryMini(t_DateTime &tmNow);

    //get system
    CTSystem* GetD1System(int nNode);
    CTSystem* GetUDPD1SystemByIP(std::string strIP);
    CTSystem* GetTCPD1SystemBySock(MYSOCK sock);

    //callback to client
    void ExcuteCallback(const CResult *pResult);

    //socket callback
    void TCPConnect(CMsg* pMsg);
    void TCPDisConnect(MYSOCK sock);
    void ReceiveTCP(CMsg* pMsg);
    void ReceiveUDP(CMsg* pMsg);
    void ReceiveSecurity(CMsg* pMsg);

    //push socket to recv thread
    void AddRecvSocket(MYSOCK sock);
    void RemoveRecvSocket(MYSOCK sock);

    //recv life signal
    void RecvLifeSignal(CTSystem* pSys,bool bLocalError);

    //send busy state
    void SendBusyState();

    //tcp broadcast
    void TCPBroadcast(CMsgBase* pMsg);

    //System Port(not realy running)
    void AllocatePort(bool bCapture, int nPort,int nPriority,int nRTPChannel);
    void FreePort(bool bCapture, int nPort);
    bool IsInput(int nPort);
    bool IsOutput(int nPort);
    
    bool IsPortBusy(bool bCapture,int nPort);
    bool IsPortCanUsed(bool bCapture,int nPort,int nPriority);
    bool IsPortFree(bool bCapture, int nPort);

    //for usage report
    bool IsInputBusy();
    bool IsOutputBusy();

    //Rtp send signal
    void PushRtpJob();
    void PopRtpJob();

    CThreadDevice* GetThreadDevice(bool bCapture,int nPort);

    //add message to network send thread
    void SendMsg(std::shared_ptr<t_SendMsg> pMsg);

    //get group from one port
    void GetGroupPort(int nPort, std::vector<int> &vGroup);

    //is standalone
    bool IsStandAlone();

    //Usage report
    t_UsageReport* GetSourceUsage(int nRTPNode,int nRTPChannel);
    void SourceUsageUpdate(t_UsageReport &srcUsage);

    //mute function for CCCF
    void MuteSpeaker(int nNode,bool bMute);

    CTSystem*   m_pSelfSystem;
    bool        m_bLocalError;
    bool        m_bBusyNow;
    bool        m_bExit;
    e_Mode      m_eMode;

    //multicast base address
    int         m_nBaseAddress;
    int         m_nBasePort;

    //some switchs
    bool        m_bMonitorLifeSignal;
    bool        m_bMonitorBusyState;
    bool        m_bCCCF;
    bool        m_bEnableSecurity;

protected:
    //loop the message
    std::shared_ptr<CMsg> GetMsg();

    //if we want to receive request,and detect by service,do this
    void SendLifeSignal();
    void CheckLifeSignal(int nSeconds);

    void RunMessage(CMsg *pMsg);

    void LocalGongSignal(int chProcess,t_UsedPriority &used);
    void LocalAudioSignal(int chProcess,t_UsedPriority &used);

    void RemoteLevelmeter(CTSystem* pSys);
    void CheckLevelmeter();

public:
    ThreadPool                                   m_ThreadPool;
    std::vector<std::shared_ptr<CThreadCapture>> m_vThreadCapture;
    std::vector<std::shared_ptr<CThreadPlay>>    m_vThreadPlay;
    std::vector<std::shared_ptr<CThreadRecv>>    m_vTreadRecv;
    std::vector<std::shared_ptr<CTSystem>>       m_vD1System;
   
    //local device port busy?
    std::map<int, t_UsedPriority>      m_PortCapture;
    std::map<int, t_UsedPriority>      m_PortPlay;

    //pre-signal and audio playing?
    std::map<int,t_UsedPriority>       m_GongPlaying;
    std::map<int,t_UsedPriority>       m_AudioPlaying;

    //port group(channel)
    std::vector<std::vector<int>>   m_vPortGroup;

    //IOMAP
    std::vector<t_FILEMAP>     m_vFileMAP;

    //levelmeter for PAServer
    std::map<int,t_Levelmeter> m_mapLevelmeter;

    //route manager
    CRouteManager              m_routeManager;
    float                      m_VolumeScaleSG;  //Signal Generator
    float                      m_VolumeScaleEG;  //End Signal Generator
    float                      m_fMicADC;
    float                      m_fVolumeMic;
    float                      m_fVolumeLineIn;
    float                      m_fVolumeSpeaker;
    float                      m_fVolumeLineOut;

    int                        m_nMaxDebugLength;

protected:
    CThreadSend              m_ThreadSend;
    CThreadTimer             m_ThreadTimer;
    CThreadAccept            m_ThreadAccept;

    //some settings before init
    bool                     m_bEnableAccept;
    int                      m_nTimerCounter;

    //alsa init
    bool                     m_bALSAInitCapture;
    bool                     m_bALSAInitPlay;

    //main message
    std::queue<std::shared_ptr<CMsg>>  m_qMessage;
    bool                     m_bIntercom;
};

extern CService    g_SDKServer;
extern SDKCallback g_CallBack;

#endif

