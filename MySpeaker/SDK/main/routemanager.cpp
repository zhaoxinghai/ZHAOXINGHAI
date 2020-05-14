
#include "routemanager.h"
#include "device.h"
#include "sdk.h"
#include "msgfactory.h"
#include "mylog.h"
#include "public.h"
#include "threadtimer.h"
#include "define.h"
#include "service.h"
#include "common.h"

#define RTP_PLAY_PORT_START   0
#define RTP_PLAY_PORT_END     127

CRouteManager::CRouteManager()
{
    for (int i = RTP_PLAY_PORT_START;i <= RTP_PLAY_PORT_END;i++)
    {
        m_RtpPort[i] = false;
    }
    m_nChannelLast = 0;
    m_bRestoreOutput = false;
    m_bRestoreInput = false;
    m_nRemoteMaxCount = 50;
    m_nPlayingMaxCount = 10;
    m_nRestoreSec = 0;
    m_AmpHandle = NULL;
}

CRouteManager::~CRouteManager()
{
    //announcement
    m_vRoute.clear();

    //audiojob running
    m_vAudioRun.clear();
}

void CRouteManager::AddAudio(CAudioJob* pJob)
{  
    std::unique_lock < std::mutex > lck(m_mutexAudio);

    m_vAudioRun.push_back(pJob);
}

void CRouteManager::PauseAudio(int chProcess)
{
    CAudioJob* pJob = GetAudio(chProcess);
    if(pJob == NULL)
    {
        return;
    }
    pJob->Pause();
}

void CRouteManager::StartAudio(int chProcess)
{
    CAudioJob* pJob = GetAudio(chProcess);
    if(pJob == NULL)
    {
        return;
    }
    pJob->Start();
    
    if (!pJob->IsRunning())
    {
        pJob->SetRunning(true);
        CService::GetInstance()->m_ThreadPool.push(pJob);
    }
}

void CRouteManager::StopAudio(int chProcess)
{
    CAudioJob* pJob = GetAudio(chProcess);
    if(pJob == NULL)
    {
        return;
    }

    pJob->Stop();
    if(!pJob->IsRunning())
    {
        AudioJobFinish(pJob);
    }
}

int CRouteManager::StopRecord(int nRecordID)
{
    for(unsigned int i = 0;i<m_vAudioRun.size();i++)
    {
        CAudioJob* pJob = m_vAudioRun[i];
        if(pJob->GetRecordID() == nRecordID)
        {
            pJob->Stop();
            if(!pJob->IsRunning())
            {
                AudioJobFinish(pJob);
            }
            return pJob->GetChProcess();
        }
    }
    return 0;
}

void CRouteManager::VolumeAjustAudio(int chRequest, float fVolumeScale)
{
    CAudioJob* pJob = GetAudio(chRequest);
    if(pJob == NULL)
    {
        return;
    }
    pJob->VolumeAjust(fVolumeScale);
}

int CRouteManager::GetCapturePlayAudio()
{
    std::unique_lock < std::mutex > lck(m_mutexAudio);

    int count = 0;
    for(unsigned int i = 0;i<m_vAudioRun.size();i++)
    {
        CAudioJob* pJob = m_vAudioRun[i];
        if(pJob->IsRunning())
        {
            if(pJob->m_className=="CAudioCapture"
                || pJob->m_className=="CAudioCapture2"
                || pJob->m_className=="CAudioPlay"
                || pJob->m_className=="CAudioPlay2")
            {
                count++;
            }
        }
    }
    return count;
}

CAudioJob* CRouteManager::GetAudio(int chProcess)
{
    for(unsigned int i = 0;i<m_vAudioRun.size();i++)
    {
        CAudioJob* pJob = m_vAudioRun[i];
        if(pJob->GetChProcess() == chProcess)
        {
            return pJob;
        }
    }
    return NULL;
}

void CRouteManager::VolumeAjustAudioSG(float fVolumeScale)
{
    for(unsigned int i = 0;i<m_vAudioRun.size();i++)
    {
        CAudioJob* pJob = m_vAudioRun[i];
        pJob->VolumeAjustSG(fVolumeScale);
    }
}

void CRouteManager::Init()
{
    m_AmpHandle = CCommon::OpenGPIO("/sys/class/leds/amp_control/brightness",false);
    if(m_AmpHandle)
    {
        std::string str = CCommon::StrFormat("%d",255);
        CCommon::WriteGPIO2(m_AmpHandle,str.c_str());
    }
}

void CRouteManager::AllocatePort(int chRequest, bool bCapture, int nPort, int nPriority,int nRTPChannel)
{
    if(m_AmpHandle && !bCapture && nPort==41)
    {
        std::string str = CCommon::StrFormat("%d",0);
        CCommon::WriteGPIO2(m_AmpHandle,str.c_str());
    }

    CService::GetInstance()->AllocatePort(bCapture, nPort, nPriority, nRTPChannel);

    CAudioJob* pJob = GetAudio(chRequest);
    if(pJob == NULL)
    {
        return;
    }
    pJob->AllocatePort(bCapture, nPort);
}

void CRouteManager::FreePort(int chRequest, bool bCapture, int nPort)
{
    if(m_AmpHandle && !bCapture && nPort==41)
    {
        std::string str = CCommon::StrFormat("%d",255);
        CCommon::WriteGPIO2(m_AmpHandle,str.c_str());
    }

    CService::GetInstance()->FreePort(bCapture, nPort);

    CAudioJob* pJob = GetAudio(chRequest);
    if(pJob == NULL)
    {
        return;
    }
    pJob->FreePort(bCapture, nPort);
}

void CRouteManager::RemoveAudio(int chProcess)
{
    std::unique_lock < std::mutex > lck(m_mutexAudio);

    auto iter = m_vAudioRun.begin();
    while (iter != m_vAudioRun.end())
    {
        CAudioJob* pJob = (*iter);
        if (pJob->GetChProcess() == chProcess)
        {
            m_vAudioRun.erase(iter);
            delete pJob;
            pJob = NULL;
            return;
        }
        iter++;
    }
}

CRoute* CRouteManager::GetRouteByProcess(int chProcess)
{
    auto iter = m_vRoute.begin();
    while (iter != m_vRoute.end())
    {
        CRoute* pRoute = (*iter);
        if (pRoute->m_RouteResult.chProcess == chProcess)
        {
            return pRoute;
        }
        iter++;
    }
    return NULL;
}

CRoute* CRouteManager::GetRouteByProcessRequest(int chProcess,int chRequest)
{
    auto iter = m_vRoute.begin();
    while (iter != m_vRoute.end())
    {
        CRoute* pRoute = (*iter);
        if (pRoute->m_RouteResult.chProcess == chProcess
                && pRoute->m_RouteResult.chRequest == chRequest)
        {
            return pRoute;
        }
        iter++;
    }
    return NULL;
}

CRoute* CRouteManager::GetRouteByRequest(int chRequest)
{
    auto iter = m_vRoute.begin();
    while (iter != m_vRoute.end())
    {
        CRoute* pRoute = (*iter);
        for (unsigned int i = 0; i < pRoute->m_vDest.size(); i++)
        {
            if (pRoute->m_vDest[i].ret.chRequest == chRequest)
            {
                return pRoute;
            }
        }
        iter++;
    }
    return NULL;
}

bool CRouteManager::IsRequestExist(int nNode,int chRequest)
{
    for(unsigned int i = 0;i<m_vRoute.size();i++)
    {
        CRoute* pRoute =  m_vRoute[i];
        if(pRoute->GetType()==ROUTE_REMOTE)
        {
            CRouteRemote* p = (CRouteRemote*)pRoute;
            if(p->m_pSystem->m_Node.nNode==nNode
                    && p->m_RouteResult.chRequest == chRequest)
            {
                return true;
            }
        }
    }
    return false;
}

bool CRouteManager::IsRequestFull()
{
    int count = 0;
    for(unsigned int i = 0;i<m_vRoute.size();i++)
    {
        if(m_vRoute[i]->GetType()==ROUTE_REMOTE)
        {
            count++;
        }
    }
    if(count >= m_nRemoteMaxCount)
    {
        LOG_DEBUG("%s","remote request is full.");
        return true;
    }
    else
    {
        return false;
    }
}

void CRouteManager::EverySecond()
{
    bool bDeactivate = false;
    for (unsigned int i = 0; i < m_vRoute.size();i++)
    {
        CRoute* pRoute = m_vRoute[i];
        if (!pRoute->IsDisConnect())
        {
            pRoute->EverySecond();
        }

        //remove the disconnect route
        if (pRoute->IsDisConnect())
        {
            bDeactivate = true;
        }
    }
    if (bDeactivate)
    {
        CleanRoute();
    }

    m_nRestoreSec++;
    if(m_nRestoreSec >= 2)
    {
        if(m_bRestoreOutput)
        {
            RestoreOutput();
        }
        if(m_bRestoreInput)
        {
            RestoreInput();
        }
        m_nRestoreSec = 0;
    }
}

void CRouteManager::CleanRoute()
{
    std::unique_lock<std::mutex> lock(m_mutexRoute);

    auto iter = m_vRoute.begin();
    while (iter != m_vRoute.end())
    {
        auto pRoute = (*iter);
        if (pRoute->m_RouteResult.e_ConState==CON_DIS_CONNECT)
        {
            iter = m_vRoute.erase(iter);
            delete pRoute;
            pRoute = NULL;
            break;
        }
        else
        {
            iter++;
        }
    }
}

void CRouteManager::AddRoute(CRoute* pRoute)
{
    std::unique_lock<std::mutex> lock(m_mutexRoute);

    auto iter = m_vRoute.begin();
   
    bool bInsert = false;
    while (iter != m_vRoute.end())
    {
        auto item = (*iter);
        int nPriority = item->m_pActivate->Priority;
        if (pRoute->m_pActivate->Priority >= nPriority)
        {
            m_vRoute.insert(iter, pRoute);
            bInsert = true;
            break;
        }
        iter++;
    }

    //push back
    if (!bInsert)
    {
        m_vRoute.push_back(pRoute);
    }
}

int CRouteManager::GetChannel()
{
    for (int i = (m_nChannelLast+1);i <= RTP_PLAY_PORT_END;i++)
    {
        if (m_RtpPort[i] == false)
        {
            m_RtpPort[i] = true;
            m_nChannelLast = i;
            return i;
        }
    }
    for (int i = 0;i <= m_nChannelLast;i++)
    {
        if (m_RtpPort[i] == false)
        {
            m_RtpPort[i] = true;
            m_nChannelLast = i;
            return i;
        }
    }
    return -1;
}

void CRouteManager::RemoveChannel(int nChannel)
{
    m_RtpPort[nChannel] = false;
}

void CRouteManager::InterruptInput(CRouteLocal* pNewRoute)
{
    for (unsigned int i = 0; i < m_vRoute.size(); i++)
    {
        CRoute* pRoute = m_vRoute[i];

        //self,can't interrupt any route
        if (pRoute->m_RouteResult.chProcess == pNewRoute->m_RouteResult.chProcess)
            break;

        //network don't need to interrupt
        if (pRoute->GetType() == ROUTE_NETWORK)
            continue;

        CRouteLocal* p = (CRouteLocal*)pRoute;
        if (pNewRoute->m_nPort == p->m_nPort )
        {
            p->InterruptInput();
        }
    }
}

void CRouteManager::InterruptInput(int nPort)
{
    for (unsigned int i = 0; i < m_vRoute.size(); i++)
    {
        CRoute* pRoute = m_vRoute[i];

        //network don't need to interrupt
        if (pRoute->GetType() == ROUTE_NETWORK)
            continue;

        CRouteLocal* p = (CRouteLocal*)pRoute;
        if (nPort == p->m_nPort)
        {
            p->InterruptInput();
        }
    }
}

void CRouteManager::InterruptOutput(CRouteRemote* pNewRoute)
{
    for (unsigned int i = 0; i < m_vRoute.size(); i++)
    {
        CRoute* pRoute = m_vRoute[i];

        //self,can't interrupt any route
        if (pRoute->m_RouteResult.chProcess == pNewRoute->m_RouteResult.chProcess)
            break;

        if (pRoute->GetType() != ROUTE_REMOTE)
            continue;

        if (!pRoute->IsRtpOnly())
            continue;

        if (pNewRoute->m_vOutput.size() > 0)
        {
            InterruptOutput((CRouteRemote*)pRoute, pNewRoute);
        }
    }
}

void CRouteManager::InterruptOutput(CRouteRemote* p, CRouteRemote* pNewRoute)
{
    std::vector<int> vConflictPort;
    GetConflictPort(p->m_vOutput, pNewRoute->m_vOutput, vConflictPort);
    if(vConflictPort.size()==0)
    {
        return;
    }
    p->InterruptOutput(vConflictPort);
}

void CRouteManager::InterruptAllOutput(int nPort)
{
    std::vector<t_Output>  tmpOutput;
    t_Output out;
    out.nPort = nPort;
    tmpOutput.push_back(out);

    for (unsigned int i = 0; i < m_vRoute.size(); i++)
    {
        CRoute* pRoute = m_vRoute[i];

        if (pRoute->GetType() != ROUTE_REMOTE)
            continue;

        if (!pRoute->IsRtpOnly())
            continue;

        CRouteRemote* p = (CRouteRemote*)pRoute;
        std::vector<int> vConflictPort;
        GetConflictPort(p->m_vOutput, tmpOutput, vConflictPort);
        if(vConflictPort.size() > 0)
        {
            p->InterruptOutput(vConflictPort);
        }
    }
}

void CRouteManager::RestoreOutput()
{
    m_nRestoreSec = 0;
    m_bRestoreOutput = false;

    if (m_vRoute.size() == 0)
        return;

    for (int i = (int)m_vRoute.size()-1; i >=0; i--)
    {
        if (m_vRoute[i]->GetType() == ROUTE_REMOTE 
            && m_vRoute[i]->IsRtpOnly())
        {
            CRouteRemote* p = (CRouteRemote*)m_vRoute[i];
            if (!p->IsDisConnect() && p->RestoreOutput())
            {
                m_bRestoreOutput = true;
                break;
            }
        }
    }
}

void CRouteManager::RestoreInput()
{
    m_nRestoreSec = 0;
    m_bRestoreInput = false;

    if (m_vRoute.size() == 0)
        return;

    for (int i = (int)m_vRoute.size()-1; i >= 0; i--)
    {
        if (m_vRoute[i]->GetType() == ROUTE_NETWORK)
            continue;

        CRouteLocal* p = (CRouteLocal*)m_vRoute[i];
        if (!p->IsDisConnect() && p->RestoreInput())
        {
            m_bRestoreInput = true;
            break;
        }
    }
}

void CRouteManager::SetDevice(bool bCapture, int nPort, std::string strDeviceName)
{
    t_AudioDevice d;
    d.nPort = nPort;
    d.bCapture = bCapture;
    d.strDeviceName = strDeviceName;
    m_vAudioDevice.push_back(d);
}

std::string CRouteManager::GetDevice(bool bCapture, int nPort)
{
    std::string str = "";
    for (unsigned int i = 0; i < m_vAudioDevice.size(); i++)
    {
        if (m_vAudioDevice[i].bCapture == bCapture && m_vAudioDevice[i].nPort == nPort)
        {
            str = m_vAudioDevice[i].strDeviceName;
            break;
        }
    }
    return str;
}

void CRouteManager::AudioJobFinish(CAudioJob * pJob)
{
    pJob->SetRunning(false);
    int chProcess = pJob->GetChProcess();
    bool bRestoreInput = false;
    bool bRestoreOutput = false;

    if (pJob->m_className == "CAudioCapture")
    {
        CAudioCapture* p = (CAudioCapture*)pJob;

        //local record
        if (p->GetType() == CAPTURE_RECORD)
        {
            CLocalRecordResult ret;
            ret.chRequest = p->GetRecordID();
            ret.chProcess = p->GetChProcess();
            ret.strFilePath = p->GetRecordPath();
            ret.nErrorCode = p->GetError();
            CService::GetInstance()->ExcuteCallback(&ret);

            //need to release the port
            FreePort(ret.chProcess,true, p->GetPort());
            bRestoreInput = true;
        }
    }
    else if(pJob->m_className == "CAudioPlay")
    {
        CAudioPlay* p = (CAudioPlay*)pJob;
        if (p->GetType() == LOCAL_ADP2LISTEN)
        {
            CActivateResult ret;
            ret.chProcess = p->GetChProcess();
            ret.e_ConState = CON_DIS_CONNECT;
            CService::GetInstance()->ExcuteCallback(&ret);

            //need to release the listening port
            std::vector<int> vPort;
            p->GetListeningPort(vPort);
            for(unsigned int i = 0;i<vPort.size();i++)
            {
                FreePort(p->GetChProcess(),false, vPort[i]);
            }
            bRestoreOutput = true;
        }
    }
    RemoveAudio(chProcess);

    //deactivate this route
    CRoute *pRoute = GetRouteByProcess(chProcess);
    if (pRoute != NULL && !pRoute->IsDisConnect())
    {
        pRoute->m_bJobFinish = true;
        if (pRoute->GetType() == ROUTE_REMOTE)
        {
            CRouteRemote* p = (CRouteRemote*)pRoute;
            p->BeDeActivate();
            p->OnRouteState(CON_DIS_CONNECT);
        }
        else
        {
            pRoute->DeActivate();
            pRoute->OnRouteState(CON_DIS_CONNECT);
        }
        CleanRoute();
    }

    //restore
    if(bRestoreInput)
    {
        RestoreInput();
    }
    if(bRestoreOutput)
    {
        RestoreOutput();
    }
}

void CRouteManager::AudioJobFinishPause(CAudioJob * pJob)
{
    pJob->SetRunning(false);
    if (pJob->IsNeed2Start())
    {
        if(pJob->m_className!="CAudioReceive"
                && GetCapturePlayAudio()>=THREAD_POOL_COUNT)
        {
            LOG_DEBUG("%s","Audio thread Pool full");
            pJob->m_error = ERROR_THREAD_POOL_FULL;
            AudioJobFinish(pJob);
        }
        else
        {
            pJob->SetRunning(true);
            CService::GetInstance()->m_ThreadPool.push(pJob);
        }
    }
}

void CRouteManager::GetConflictPort(
    std::vector<t_Output> &vSrc, 
    std::vector<t_Output> &vDest, 
    std::vector<int> &vConflict  )
{
    //busy 256 zones
    unsigned char szBusyOut[32];
    memset(szBusyOut, 0, sizeof(szBusyOut));
    for (unsigned int i = 0; i < vSrc.size(); i++)
    {
        if (!vSrc[i].bConnect)
        {
            continue;
        }
        std::vector<int> vGroup;
        CService::GetInstance()->GetGroupPort(vSrc[i].nPort,vGroup);
        CMsgBase::ChangeVectorto32Byte(szBusyOut, vGroup);
    }

    //new 256 zones
    unsigned char szNetOut[32];
    memset(szNetOut, 0, sizeof(szNetOut));
    std::vector<int> vNewOut;
    for (unsigned int i = 0; i < vDest.size(); i++)
    {
        vNewOut.push_back(vDest[i].nPort);
    }
    CMsgBase::ChangeVectorto32Byte(szNetOut, vNewOut);

    //get conflict port
    unsigned char szConflictOut[32];
    memset(szConflictOut, 0, sizeof(szConflictOut));
    for (unsigned int i = 0; i < 32; i++)
    {
        szConflictOut[i] = szBusyOut[i] & szNetOut[i];
    }
    CMsgBase::Change32BytetoVector(szConflictOut, vConflict);
}

void CRouteManager::GetLocalUsageRoute(std::vector<CRouteLocal*> &vLocal)
{
    for (unsigned int i = 0; i < m_vRoute.size(); i++)
    {
        CRoute* pRoute = m_vRoute[i];

        if (pRoute->GetType() == ROUTE_NETWORK)
            continue;

        if (pRoute->IsRtpOnly())
            continue;

        CRouteLocal* p = (CRouteLocal*)pRoute;
        if(p->m_RouteResult.e_ConState ==CON_FULL_CONNECT
                || p->m_RouteResult.e_ConState == CON_PARTLY_CONNECT)
        {
            vLocal.push_back(p);
        }
    }
}

void CRouteManager::GetRemoteUsageRoute(std::vector<CRouteRemote*> &vRemote)
{
    for (unsigned int i = 0; i < m_vRoute.size(); i++)
    {
        CRoute* pRoute = m_vRoute[i];
        if (pRoute->IsRtpOnly())
        {
            CRouteRemote* p = (CRouteRemote*)pRoute;
            for(unsigned int k = 0;k<p->m_vOutput.size();k++)
            {
                if (p->m_vOutput[k].bConnect)
                {
                    vRemote.push_back(p);
                    break;
                }
            }
        }
    }
}

void CRouteManager::CurrentPlayIndex(int chProcess, int nIndex)
{
    CRoute* p = GetRouteByProcess(chProcess);
    if (p)
    {
        p->CurrentPlayIndex(nIndex);
    }
}

void CRouteManager::StopAllAudio()
{
    for(unsigned int i = 0;i<m_vAudioRun.size();i++)
    {
        CAudioJob* p = m_vAudioRun[i];
        p->Stop();
    }
}

std::string CRouteManager::Constat()
{
    /*
    std::unique_lock<std::mutex> lock(m_mutexRoute);

    std::string strText = "No.    Trigger         State          Presig-Chan  Source-Chan     Dest-Elem         Dest-Elem         Dest-Elem         Dest-Elem         Dest-Elem\r\n";
    for(unsigned int i = 0;i<m_vRoute.size();i++)
    {
        CRoute* p = m_vRoute[i];

        if(p->m_RouteResult.e_ConState == CON_DIS_CONNECT)
        {
            continue;
        }
        std::string strTri;
        std::string strSrc;
        std::string strPre;
        std::string szDst[5];

        std::string strSt = CCommon::StrFormat("%-14s",CRoute::GetState(p->m_RouteResult.e_ConState).c_str());
        if(p->m_RouteResult.e_ConState == CON_UNKNOW
                || p->m_RouteResult.e_ConState == CON_RECONNECT_POSSIBLE)
        {
            strSt = "BREAK";
        }
        std::string strNO = CCommon::StrFormat("%-6d",p->m_RouteResult.chProcess);

        //source and triggle
        if(p->GetType() == ROUTE_REMOTE)
        {
            CRouteRemote* pRemote = (CRouteRemote*)p;
            strTri = CCommon::StrFormat("SC %d",pRemote->m_pSystem->GetNode());
        }
        else
        {
            strTri = CCommon::StrFormat("CS %d",p->m_pActivate->nTrigger);
        }

        if(p->m_RouteResult.e_ConState == CON_FULL_CONNECT
                || p->m_RouteResult.e_ConState == CON_PARTLY_CONNECT)
        {
            p->GetSource(strPre,strSrc,true);
        }
        else
        {
            p->GetSource(strPre,strSrc,false);
        }
        //destination
        p->GetDest(szDst);

        //format
        strTri = CCommon::StrFormat("%-15s",strTri.c_str());
        strSrc = CCommon::StrFormat("%-15s",strSrc.c_str());
        strPre = CCommon::StrFormat("%-12s",strPre.c_str());

        std::string strItem = CCommon::StrFormat("%s %s %s %s %s %s %s %s %s %s\r\n",
            strNO.c_str(),strTri.c_str(),strSt.c_str(),strPre.c_str(),strSrc.c_str(),
            szDst[0].c_str(),szDst[1].c_str(),szDst[2].c_str(),szDst[3].c_str(),szDst[4].c_str());

        strText += strItem;
    }
    return strText;
    */
    return "";
}

