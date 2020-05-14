
#include "service.h"
#include "routemanager.h"
#include "interfacemsg.h"
#include "device.h"
#include "sdk.h"
#include "msgfactory.h"
#include "mylog.h"
#include "threadtimer.h"
#include "define.h"
#include "threadplay.h"
#include "threadcapture.h"
#include "audiobuffer.h"
#include "public.h"

#ifndef _MSC_VER
#include "audiodevicealsa.h"
#include "cpsw_vlan.h"
#endif

extern PACallback g_Callback;
CService *CService::inst =  new CService();                        //checked
CService::CService()
{
    m_bExit = false;
    m_bEnableAccept = true;
    m_bIntercom = false;
    m_pSelfSystem = NULL;
    m_nTimerCounter = 0;
    m_bLocalError = false;
    m_bBusyNow = false;
    m_VolumeScaleSG = 1.0f;

    m_fVolumeMic = 0;
    m_fVolumeLineIn = 0;
    m_fVolumeSpeaker = 0;
    m_fVolumeLineOut = 0;
    m_fMicADC = 1.0f;
    m_nMaxDebugLength = 200;

    m_bALSAInitCapture = false;
    m_bALSAInitPlay = false;
    m_eMode = MODE_NORMAL;

    m_bMonitorLifeSignal = true;
    m_bMonitorBusyState = true;
    m_bCCCF = false;
    m_bEnableSecurity = true;

    m_nBaseAddress = 224;
}

CService::~CService()
{
    //release all object
    m_vTreadRecv.clear();
    m_vD1System.clear();

#ifdef _MSC_VER
    CoUninitialize();
#endif

    //release instance
    CAudioData::release();
    CSound::Release();
    CMyLog::Release();
}

CService* CService::GetInstance()
{
    return inst;
}

void CService::Release()
{
    delete inst;
    inst = NULL;
}

bool CService::Init(t_Node &curNode, std::vector<t_Node> &vSystem)
{
#ifdef _MSC_VER
    CoInitializeEx(NULL, COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY);
#else
#endif

    //local system
    std::shared_ptr<CTSystem> pSystem = std::make_shared<CTSystem>(curNode);
    m_pSelfSystem = pSystem.get();
    m_vD1System.push_back(pSystem);

    m_pSelfSystem->m_Node.strIP = curNode.strIP;
    m_pSelfSystem->SetConfigSystem();
    m_pSelfSystem->m_IsTCP = false;
    m_pSelfSystem->SetSelfSystem();

    for(unsigned int i = 0;i<vSystem.size();i++)
    {
        t_Node &node = vSystem[i];
        if (node.nNode == curNode.nNode)
        {
            continue;
        }

        std::shared_ptr<CTSystem> pSys = std::make_shared<CTSystem>(node);
        pSys->m_bIsConfig = true;
        pSys->m_IsTCP = false;
        pSys->m_bIsSelfSystem = false;
        m_vD1System.push_back(pSys);
    }
    return true;
}

int CService::GetNode()
{
    return m_pSelfSystem->m_Node.nNode;
}

void CService::InitFileMAP(std::vector<t_FILEMAP> &vIOMAP)
{
    m_vFileMAP.clear();
	for(unsigned int i = 0;i<vIOMAP.size();i++)
	{
        m_vFileMAP.push_back(vIOMAP[i]);
	}
}

bool CService::GetFileMAP(int nChannel, int nTitle,t_FILEMAP &file)
{
    /*
    for (unsigned int i = 0; i<m_vFileMAP.size(); i++)
    {
        if (m_vFileMAP[i].nChannel == nChannel
            && m_vFileMAP[i].nTitle == nTitle)
        {
            file = m_vFileMAP[i];
            return true;
        }
    }
    */
    return false;
}

void CService::Exit(bool bWait)
{
    m_bExit = true;

    //stop audio thread
    m_routeManager.StopAllAudio();
    m_ThreadPool.stop();
    CCommon::Trace("%s","ThreadPool::Exit");

    //stop timer
    m_ThreadTimer.EndThread(bWait);
    CCommon::Trace("%s","ThreadTimer::Exit");

    //stop all audio thread
    for (unsigned int i = 0; i < m_vThreadCapture.size(); i++)
    {
        m_vThreadCapture[i]->EndThread(bWait);
    }
    CCommon::Trace("%s","ThreadCapture::Exit");

    for (unsigned int i = 0; i < m_vThreadPlay.size(); i++)
    {
        m_vThreadPlay[i]->EndThread(bWait);
    }
    CCommon::Trace("%s","ThreadPlay::Exit");

    //stop all network
    for (unsigned int i = 0; i < m_vTreadRecv.size(); i++)
    {
        m_vTreadRecv[i]->EndThread(bWait);
    }
    CCommon::Trace("%s","ThreadRecv::Exit");

    m_ThreadSend.EndThread(bWait);
    CCommon::Trace("%s","ThreadSend::Exit");

    m_ThreadAccept.EndThread(bWait);
    CCommon::Trace("%s","ThreadAccept::Exit");

    EndThread(bWait);
    CCommon::Trace("%s","ThreadService::Exit");
}

bool CService::IsExit()
{
    return m_bExit;
}

std::shared_ptr<CMsg> CService::GetMsg()
{
    std::unique_lock < std::mutex > lck(m_mutex);

    if (m_qMessage.empty() && m_bActive)
    {
        m_cv.wait(lck);
    }

    std::shared_ptr<CMsg> pMsg;
    if (!m_qMessage.empty())
    {
        pMsg = m_qMessage.front();
        m_qMessage.pop();
    }
    return pMsg;
}

void CService::Run()
{
    CCommon::SetThreadName("CThreadService");

    while (m_bActive)
    {
        std::shared_ptr<CMsg> msg = GetMsg();

        CMsg* pMsg = msg.get();

        if(pMsg != NULL && !m_bExit)
        {
            RunMessage(pMsg);
        }
    }

    //clean queue
    while (!m_qMessage.empty())
    {
        auto msg = m_qMessage.front();
        m_qMessage.pop();

        CMsg* pMsg = msg.get();
        if(pMsg != NULL)
        {
            RunMessage(pMsg);
        }
    }
}

void CService::Start()
{
    BeginThread();

    for (unsigned int i = 0; i < m_vThreadCapture.size(); i++)
    {
        m_vThreadCapture[i]->BeginThread(THREAD_HIGHEST);
    }
    for (unsigned int i = 0; i < m_vThreadPlay.size(); i++)
    {
        m_vThreadPlay[i]->BeginThread(THREAD_HIGHEST);
    }
    m_ThreadTimer.BeginThread(THREAD_BELOW_NORMAL);

    //AudioJob threadpool(2 is for receive audio)
    m_ThreadPool.start(THREAD_POOL_COUNT+2);


    //normal network connect
    if (m_bEnableAccept)
    {
        m_ThreadAccept.BeginThread(THREAD_NORMAL);
    }

    //use UDP
    if(m_bMonitorBusyState)
    {
        std::string strAddress = CCommon::StrFormat("%d.255.255.255",m_nBaseAddress);
        auto pRecv = std::make_shared<CThreadRecv>(false);
        pRecv->EnableMulticast();
        pRecv->SetMulticastAddress(strAddress);
        m_vTreadRecv.push_back(pRecv);
    }
    if(m_bMonitorLifeSignal)
    {
        std::string strAddress = CCommon::StrFormat("%d.255.255.254",m_nBaseAddress);
        auto pRecv = std::make_shared<CThreadRecv>(false);
        pRecv->EnableMulticast();
        pRecv->SetMulticastAddress(strAddress);
        m_vTreadRecv.push_back(pRecv);
    }

    for (unsigned int i = 0; i < m_vTreadRecv.size(); i++)
    {
        m_vTreadRecv[i]->BeginThread(THREAD_NORMAL);
    }

    m_ThreadSend.BeginThread(THREAD_NORMAL);
}

void CService::RunMessage(CMsg* pMsg)
{
    switch (pMsg->type)
    {
    case MSG_NORMAL_ANNOUNCEMENT:
    case MSG_LOCAL_PLAY_ANNOUNCEMENT:
    case MSG_LOCAL_MIC_ANNOUNCEMENT:
    case MSG_STOP_ANNOUNCEMENT:
    case MSG_LOCAL_RECORD:
    case MSG_STOP_LOCAL_RECORD:
    case MSG_SYNC_DATATIME:
    case MSG_VOLUME_AJUST:
    case MSG_DEVICE_VOLUME_AJUST:
    case MSG_TRANSPORT_LINE:
    case MSG_GET_DEVICE_STATE:
    case MSG_VOLUME_AJUST_PORT:
    case MSG_TEXT_OUT:
    {
        CInterface::RunMessage(pMsg);
        break;
    }
    case MSG_SOCK_CONNECT:
    {
        TCPConnect(pMsg);
        break;
    }
    case MSG_SOCK_DISCONNECT:
    {
        TCPDisConnect(pMsg->socket);
        break;
    }
    case MSG_SOCK_TCP_RECV:
    {
        ReceiveTCP(pMsg);
        break;
    }
    case MSG_SOCK_UDP_RECV:
    {
        ReceiveUDP(pMsg);
        break;
    }
    case MSG_AUDIO_FINISH:
    {
        CAudioJob* p = (CAudioJob*)pMsg->pointer;
        CService::GetInstance()->m_routeManager.AudioJobFinish(p);
        pMsg->pointer = NULL;
        break;
    }
    case MSG_AUDIO_FINISH_PAUSE:
    {
        CAudioJob* p = (CAudioJob*)pMsg->pointer;
        CService::GetInstance()->m_routeManager.AudioJobFinishPause(p);
        pMsg->pointer = NULL;
        break;
    }
    case MSG_PLAY_INDEX:
    {
        m_routeManager.CurrentPlayIndex(pMsg->nInt1,pMsg->nInt2);
        CSourceIndexResult ret;
        ret.chProcess = pMsg->nInt1;
        ret.nSourceIndex = pMsg->nInt2;
        ExcuteCallback(&ret);
        break;
    }
    case MSG_EVERY_200_MINISECOND:
    {
        Every200MiniSecond();

        static int sec = 0;
        sec += 200;
        if(sec >= 1000)
        {
            sec = 0;
            EverySecond();
        }
        break;
    }
    case MSG_SEND_USAGE_REPORT:
    {
        SendBusyState();
        break;
    }
    case MSG_GONG_SIGNAL:
    {
        LocalGongSignal(pMsg->nInt1,pMsg->usedpriority);
        break;
    }
    case MSG_AUDIO_SIGNAL:
    {
        LocalAudioSignal(pMsg->nInt1,pMsg->usedpriority);
        break;
    }
    case MSG_RESTORE_INPUT:
    {
        CService::GetInstance()->m_routeManager.RestoreInput();
        break;
    }
    case MSG_RESTORE_OUTPUT:
    {
        CService::GetInstance()->m_routeManager.RestoreOutput();
        break;
    }
    case MSG_ALSA_INIT_FINISH:
    {
#ifndef _MSC_VER
        if(pMsg->nInt1 == 1)
        {
            m_bALSAInitCapture = true;
        }
        else
        {
            m_bALSAInitPlay = true;
        }
        if(m_bALSAInitCapture && m_bALSAInitPlay)
        {
            CSound::GetInstance()->InitVolume();
        }
#endif
        break;
    }
    case MSG_AUDIO_CONTROL:
    {
        CTSystem* pSys = CService::GetInstance()->GetD1System(pMsg->nInt1);
        if(pSys != NULL)
        {
            CMsgDevice msg(pSys);
            msg.AudioControl(pMsg->device,pMsg->nInt2);
        }
        break;
    }
    case MSG_SEND_THREAD_RUN:
    {
        SendLifeSignal();
        break;
    }
    case MSG_VOL_REPLY:
    {
        CTSystem* pSys = NULL;
        if(pMsg->nInt1==1)  //this is TCP
        {
            pSys = GetTCPD1SystemBySock(pMsg->socket);
        }
        else
        {
            pSys = GetD1System(pMsg->nInt2);
        }
        if(pSys != NULL && pSys->IsConnect())
        {
            CMsgDevice msg(pSys);
            msg.ReplyAudioControl(pMsg->volreply.device,
                                  pMsg->volreply.fVolume,
                                  pMsg->volreply.fMin,
                                  pMsg->volreply.fMax);
        }
        break;
    }
    case MSG_LAMP_TEST:
    {
        if(m_bCCCF)
        {
            const unsigned short lampTest = 502;
            CMsgCommon msg(m_pSelfSystem);
            msg.SpecialFunction(lampTest,pMsg->nInt1==1);
        }
        break;
    }
    default:
    {
        LOG_ERROR("Service RunMessage Error:%d",pMsg->type);
    }
    }
}

void CService::Push(std::shared_ptr<CMsg> pMsg)
{
    std::unique_lock < std::mutex > lck(m_mutex);

    if(m_bExit || m_eMode == MODE_CONFIG)
    {
        return;
    }

    m_qMessage.push(pMsg);
    m_cv.notify_one();
}

bool CService::IsAudioJobOverRun(int &nRunningCount)
{
    nRunningCount = m_routeManager.GetCapturePlayAudio();
    if(nRunningCount >= m_routeManager.m_nPlayingMaxCount)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CService::RemoteLevelmeter(CTSystem* pSys)
{
    auto it = m_mapLevelmeter.begin();
    while(it != m_mapLevelmeter.end())
    {
        t_Levelmeter &level = (*it).second;
        if(level.pSys == pSys)
        {
            it = m_mapLevelmeter.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void CService::CheckLevelmeter()
{
    auto it = m_mapLevelmeter.begin();
    while(it != m_mapLevelmeter.end())
    {
        t_Levelmeter &level = (*it).second;
        level.nCounter++;
        if(level.nCounter > 9)
        {
            it = m_mapLevelmeter.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void CService::Every200MiniSecond()
{
    if (m_bExit || m_eMode == MODE_CONFIG)
    {
        return;
    }

    auto it = m_mapLevelmeter.begin();
    while(it != m_mapLevelmeter.end())
    {
        t_Levelmeter &level = (*it).second;
        bool bCapture = (level.de.type == AS_TYPE_PR) ? true:false;
        float fValue = PA::GetLevelmeter(bCapture,level.de.number+1);

        CMsgDevice msg(level.pSys);
        msg.Levelmeter((*it).first,fValue);

        it++;
    }
}

void CService::EverySecond()
{
    if (m_bExit || m_eMode == MODE_CONFIG)
    {
        return;
    }

    //life signal
    if (m_nTimerCounter % 10 == 0)
    {
        SendLifeSignal();

        if(m_bMonitorLifeSignal)
        {
            CheckLifeSignal(10);
        }
    }

    //busy state
    if (m_nTimerCounter % 15 == 0)
    {
        if(m_bBusyNow)
        {
            SendBusyState();
        }
    }

    //check route
    CService::GetInstance()->m_routeManager.EverySecond();

    //check levelmeter
    CheckLevelmeter();
}

void CService::EveryMini(t_DateTime &tmNow)
{
    if(tmNow.Hour ==0 && tmNow.Min == 0)
    {
        std::string strDir = CMyLog::GetInstance()->m_LogDebug.GetDir();

        //delete 20MB
        unsigned int nMaxlen = 1024*1024*m_nMaxDebugLength;
        CCommon::CleanCache(strDir,nMaxlen,1024*1024*20);
    }
}

void CService::TCPConnect(CMsg* pMsg)
{
    std::string strIP(pMsg->szIP);
    LOG_DEBUG("TCP Connect:%s socket:%d",strIP.c_str(),pMsg->socket);

    t_Node node;
    node.strIP = strIP;
    std::shared_ptr<CTSystem> pSys = std::make_shared<CTSystem>(node);
    m_vD1System.push_back(pSys);

    pSys->m_IsTCP = true;
    pSys->SetSocket(pMsg->socket);
    pSys->SetConnect();

    //add to receive tread
    AddRecvSocket(pMsg->socket);
}

void CService::TCPDisConnect(MYSOCK sock)
{
    auto iter = m_vD1System.begin();
    while (iter != m_vD1System.end())
    {
        CTSystem* pSys = (*iter).get();
        if (pSys->m_IsTCP && pSys->m_sock == sock)
        {
            LOG_DEBUG("TCP DisConnect:%s socket:%d\n",pSys->m_Node.strIP.c_str(),sock);

            RemoteLevelmeter(pSys);

            pSys->SetDisConnnect();
            iter = m_vD1System.erase(iter);
            break;
        }
        iter++;
    }
}

void CService::ReceiveTCP(CMsg * pMsg)
{
    MYSOCK sock = pMsg->socket;
    int len = pMsg->nInt1;

    CTSystem* pSystem = GetTCPD1SystemBySock(sock);
    if (pSystem != NULL)
    {
        pSystem->m_pTransport->Receive(pMsg->szbuffer, len);
    }
}

void CService::ReceiveUDP(CMsg * pMsg)
{
    int len = pMsg->nInt1;
    std::string strIP(pMsg->szIP);

    CTSystem* pSystem = GetUDPD1SystemByIP(strIP);
    if (pSystem != NULL)
    {
        pSystem->SetTCP(false);
        pSystem->m_pTransport->Receive(pMsg->szbuffer, len);
    }
    else
    {
        if(strIP == "127.0.0.1" && m_pSelfSystem != NULL)
        {
            m_pSelfSystem->m_pTransport->Receive(pMsg->szbuffer,len);
        }
    }
}

void CService::ReceiveSecurity(CMsg* pMsg)
{
    int len = pMsg->nInt1;
    std::string strIP(pMsg->szIP);
    for (unsigned int i = 0;i < m_vD1System.size();i++)
    {
        CTSystem* pSys = m_vD1System[i].get();
        if (pSys->m_Node.strIP == strIP)
        {
            pSys->m_pTransport->Receive(pMsg->szbuffer, len);
        }
    }
}

void CService::AddRecvSocket(MYSOCK sock)
{
    for (unsigned int i = 0;i < m_vTreadRecv.size();i++)
    {
        CThreadRecv* pThread = m_vTreadRecv[i].get();
        if (pThread->IsTCP())
        {
            if (pThread->GetSocketSize() < 64)
            {
                pThread->AddScoket(sock);
                return;
            }
        }
    }

    auto pThread = std::make_shared<CThreadRecv>(true);
    pThread->BeginThread(THREAD_NORMAL);
    m_vTreadRecv.push_back(pThread);

    pThread->AddScoket(sock);
}

void CService::RemoveRecvSocket(MYSOCK sock)
{
    for (unsigned int i = 0;i < m_vTreadRecv.size();i++)
    {
        CThreadRecv* pThread = m_vTreadRecv[i].get();
        pThread->RemoveSocket(sock);
    }
}

void CService::CheckLifeSignal(int nSeconds)
{
    for (unsigned int i = 0; i < m_vD1System.size(); i++)
    {
        if(!m_vD1System[i]->m_bIsConfig)
            continue;

        if (m_vD1System[i]->IsInit() && !m_vD1System[i]->IsConnect())
            continue;

        //30 seconds no sysstatus
        m_vD1System[i]->m_nSysStatusSeconds += nSeconds;
        if (m_vD1System[i]->m_nSysStatusSeconds >= 30)
        {
            m_vD1System[i]->SetDisConnnect();
        }
    }
}

void CService::SendBusyState()
{
    if(m_pSelfSystem != NULL)
    {
        CMsgUsageReport msg(m_pSelfSystem);
        //msg.SendLocalSourceUsageReport11();
        //msg.SendRemoteSourceUsageReport11();
    }
}

void CService::SendLifeSignal()
{
    if(m_pSelfSystem != NULL)
    {
        CMsgLifeSignal msg(m_pSelfSystem);
        msg.Broadcast();
    }
}

void CService::RecvLifeSignal(CTSystem* pSys,bool bLocalError)
{
    if(m_bMonitorLifeSignal)
    {
        CSysStateResult ret;
        ret.nNode = pSys->m_Node.nNode;
        ret.bErrorChange = false;
        ret.IsError = bLocalError;
        ExcuteCallback(&ret);
    }

    pSys->m_nSysStatusSeconds = 0;

    if (!pSys->IsConnect())
    {
        pSys->SetConnect();
    }
}

CTSystem* CService::GetD1System(int nNode)
{
    for (unsigned int i = 0;i < m_vD1System.size();i++)
    {
        CTSystem* pSys = m_vD1System[i].get();
        if (pSys->m_Node.nNode == nNode)
        {
            return pSys;
        }
    }
    return NULL;
}

CTSystem * CService::GetUDPD1SystemByIP(std::string strIP)
{
    for (unsigned int i = 0;i < m_vD1System.size();i++)
    {
        CTSystem* pSys = m_vD1System[i].get();
        if (!pSys->m_IsTCP && pSys->m_Node.strIP == strIP)
        {
            return pSys;
        }
    }
    return NULL;
}

CTSystem * CService::GetTCPD1SystemBySock(MYSOCK sock)
{
    CTSystem *pSystem = NULL;
    for (unsigned int i = 0;i < m_vD1System.size();i++)
    {
        if (m_vD1System[i]->m_IsTCP
                && m_vD1System[i]->m_sock == sock)
        {
            pSystem = m_vD1System[i].get();
            break;
        }
    }
    return pSystem;
}

void CService::ExcuteCallback(const CResultBase *pResult)
{
    if(!m_bExit)
    {
        g_Callback(pResult);
    }
}

void CService::TCPBroadcast(CMsgBase* pMsg)
{
    /*
    for (unsigned int i = 0; i < m_vD1System.size(); i++)
    {
        CTSystem* pSys = m_vD1System[i].get();
        if (pSys->m_bIsSelfSystem)
            continue;

        if (!pSys->IsTCP())
            continue;

        if (!pSys->IsConnect())
            continue;

        std::shared_ptr<t_SendMsg> Msg = std::make_shared<t_SendMsg>();
        Msg->bTCP = true;
        Msg->sock = pSys->m_sock;
        strncpy(Msg->szIP, pSys->m_Node.strIP.c_str(),sizeof(Msg->szIP)-1);
        pSys->m_pTransport->GeneralMsgBuffer(pMsg, Msg->szData, Msg->len);

        if(m_bEnableSecurity)
        {
            std::string msg(Msg->szData);
            m_SecurityNetwork.send_data(pSys->m_Node.strIP,msg);
        }
        else
        {
            m_ThreadSend.SendMsg(Msg);
        }
    }
    */
}

void CService::AllocatePort(bool bCapture, int nPort, int nPriority,int nRTPChannel)
{
    if (bCapture)
    {
        if(m_PortCapture[nPort].bUsed)
        {
            LOG_ERROR("CService::AllocatePort Capture:%d",nPort);
        }
        m_PortCapture[nPort].bUsed = true;
        m_PortCapture[nPort].nPriority = nPriority;
        m_PortCapture[nPort].nRtpChannel = nRTPChannel;

        if(m_bCCCF && nPort == 41)
        {
            MuteSpeaker(GetNode(),true);
            const unsigned short speakerMute = 500;
            CMsgCommon msg(m_pSelfSystem);
            msg.SpecialFunction(speakerMute,true);
        }
    }
    else
    {
        if(m_PortPlay[nPort].bUsed)
        {
            LOG_ERROR("CService::AllocatePort Play:%d",nPort);
        }
        m_PortPlay[nPort].bUsed = true;
        m_PortPlay[nPort].nPriority = nPriority;
        m_PortPlay[nPort].nRtpChannel = nRTPChannel;
    }
    CThreadDevice* pDevice = GetThreadDevice(bCapture, nPort);
    pDevice->OccupyPort(nPort);
}

void CService::FreePort(bool bCapture, int nPort)
{
    if (bCapture)
    {
        if(m_PortCapture[nPort].bUsed == false)
        {
            LOG_ERROR("CService::FreePort Capture:%d",nPort);
        }
        m_PortCapture[nPort].bUsed = false;
        m_PortCapture[nPort].nPriority = 255;

        if(m_bCCCF && nPort == 41)
        {
            MuteSpeaker(GetNode(),false);
            const unsigned short speakerMute = 500;
            CMsgCommon msg(m_pSelfSystem);
            msg.SpecialFunction(speakerMute,false);
        }
    }
    else
    {
        if(m_PortPlay[nPort].bUsed == false)
        {
            LOG_ERROR("CService::FreePort Play:%d",nPort);
        }
        m_PortPlay[nPort].bUsed = false;
        m_PortPlay[nPort].nPriority = 255;
    }

    CThreadDevice* pDevice = GetThreadDevice(bCapture, nPort);
    pDevice->FreePort(nPort);
}

bool CService::IsInput(int nPort)
{
    auto iter = m_PortCapture.find(nPort);
    if (iter != m_PortCapture.end())
        return true;

    return false;
}

bool CService::IsOutput(int nPort)
{
    auto iter = m_PortPlay.find(nPort);
    if (iter != m_PortPlay.end())
        return true;

    return false;
}

bool CService::IsInputBusy()
{
    auto iter = m_PortCapture.begin();
    while (iter != m_PortCapture.end())
    {
        if (iter->second.bUsed)
            return true;

        iter++;
    }
    return false;
}

bool CService::IsOutputBusy()
{
    auto iter = m_PortPlay.begin();
    while (iter != m_PortPlay.end())
    {
        if (iter->second.bUsed)
            return true;

        iter++;
    }
    return false;
}

bool CService::IsPortFree(bool bCapture, int nPort)
{
    t_UsedPriority *p = NULL;
    if (bCapture)
    {
        p = &m_PortCapture[nPort];
    }
    else
    {
        p = &m_PortPlay[nPort];
    }
    return !p->bUsed;
}

bool CService::IsPortCanUsed(bool bCapture, int nPort, int nPriority)
{
    t_UsedPriority *p = NULL;
    if (bCapture)
    {
        p = &m_PortCapture[nPort];
    }
    else
    {
        p = &m_PortPlay[nPort];
    }
    if (!p->bUsed)
    {
        return true;
    }
    if (nPriority < p->nPriority)
    {
        return true;
    }
    return false;
}

bool CService::IsPortBusy(bool bCapture, int nPort)
{
    bool bBusy = false;
    if (bCapture)
    {
        bBusy = m_PortCapture[nPort].bUsed;
    }
    else
    {
        bBusy = m_PortPlay[nPort].bUsed;
    }
    return bBusy;
}

void CService::PushRtpJob()
{
    CThreadCapture* p = (CThreadCapture*)m_vThreadCapture[0].get();
    p->PushRtpJob();

    auto pMsg = std::make_shared<CMsg>();
    pMsg->type = MSG_SEND_USAGE_REPORT;
    CService::GetInstance()->Push(pMsg);
}

void CService::PopRtpJob()
{
    CThreadCapture* p = (CThreadCapture*)m_vThreadCapture[0].get();
    p->PopRtpJob();

    auto pMsg = std::make_shared<CMsg>();
    pMsg->type = MSG_SEND_USAGE_REPORT;
    CService::GetInstance()->Push(pMsg);
}

CThreadDevice* CService::GetThreadDevice(bool bCapture, int nPort)
{
    if (bCapture)
    {
        for (unsigned int i = 0; i < m_vThreadCapture.size(); i++)
        {
            if (m_vThreadCapture[i]->IsPortExist(nPort))
            {
                return m_vThreadCapture[i].get();
            }
        }
    }
    else
    {
        for (unsigned int i = 0; i < m_vThreadPlay.size(); i++)
        {
            if (m_vThreadPlay[i]->IsPortExist(nPort))
            {
                return m_vThreadPlay[i].get();
            }
        }
    }
    return NULL;
}

void CService::SendMsg(std::shared_ptr<t_SendMsg> pMsg)
{
    m_ThreadSend.SendMsg(pMsg);
}

void CService::GetGroupPort(int nPort, std::vector<int> &vGroup)
{
    for (unsigned int i = 0; i < m_vPortGroup.size(); i++)
    {
        std::vector<int> &vPort = m_vPortGroup[i];
        for (unsigned int j = 0; j < vPort.size(); j++)
        {
            if (vPort[j] == nPort)
            {
                vGroup = vPort;
                return;
            }
        }
    }
}

bool CService::IsStandAlone()
{
    for (unsigned int i = 0; i < m_vD1System.size(); i++)
    {
        if (m_vD1System[i]->m_Node.nNode == GetNode())
            continue;

        if (!m_vD1System[i]->m_bIsConfig)
            continue;

        if (m_vD1System[i]->IsConnect())
        {
            return false;
        }
    }
    return true;
}

void CService::LocalGongSignal(int chProcess,t_UsedPriority &used)
{
    CPresignalResult ret;
    ret.chProcess = chProcess;
    ret.bPlaying = used.bUsed;
    ExcuteCallback(&ret);

    if(used.bUsed)
    {
        m_GongPlaying[chProcess] = used;
    }
    else
    {
        m_GongPlaying.erase(chProcess);
    }
    SendBusyState();
}

void CService::LocalAudioSignal(int chProcess,t_UsedPriority &used)
{
    if(used.bUsed)
    {
        m_AudioPlaying[chProcess] = used;
    }
    else
    {
        m_AudioPlaying.erase(chProcess);
    }
    SendBusyState();
}

t_UsageReport* CService::GetSourceUsage(int nRTPNode,int nRTPChannel)
{
    CTSystem* pSys = GetD1System(nRTPNode);
    if(pSys == NULL)
    {
        return NULL;
    }

    return pSys->GetSourceUsage(nRTPChannel);
}

void CService::SourceUsageUpdate(t_UsageReport &srcUsage)
{
    /*
    for (unsigned int i = 0;i < m_vD1System.size();i++)
    {
        CTSystem* pTmp = m_vD1System[i].get();
        if(pTmp->m_bIsConfig
                && pTmp->GetNode() != srcUsage.nRtpNode)
        {
            pTmp->SourceUsageUpdate(srcUsage);
        }
    }
    */
}

bool CService::IsGongPlaying(int nRtpChannel)
{
    auto iter = m_GongPlaying.begin();
    for(;iter != m_GongPlaying.end();iter++)
    {
        t_UsedPriority &use = iter->second;
        if(use.nRtpChannel == nRtpChannel)
        {
            return true;
        }
    }
    return false;
}

void CService::MuteSpeaker(int nNode,bool bMute)
{
    m_vThreadPlay[0]->m_bMute = bMute;

    CMuteSpeakerResult ret;
    ret.nNode = nNode;
    ret.bActivate = bMute;
    ExcuteCallback(&ret);
}
