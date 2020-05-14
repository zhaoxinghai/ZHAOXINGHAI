
#include "route.h"
#include "service.h"
#include "routemanager.h"
#include "msgconnection.h"
#include "msgconnectrequest.h"
#include "msgcheckroute.h"
#include "public.h"
#include "audioplay.h"
#include "audiocapture.h"
#include "audioreceive.h"
#include "common.h"

CRouteRemote::CRouteRemote(int nNode,CActivate* pA):CRouteLocal(pA)
{
    m_eType = ROUTE_REMOTE;
    m_pSystem = CService::GetInstance()->GetD1System(nNode);
    m_nCheckTimes = 0;
    m_nTimerCount10 = 0;
    m_RouteResult.chRequest = pA->chRequest;
    m_eRespone = CON_ON_ROUTESTATE;
    m_bBeInterrupt = false;
}

CRouteRemote::~CRouteRemote()
{
}

void CRouteRemote::BeActivate()
{
	if (IsRtpActivate())
	{
        LOG_DEBUG("%d RTP BeActivate request %d,node(%d)",
                  m_RouteResult.chProcess,m_RouteResult.chRequest,m_pSystem->m_Node.nNode);

        m_eRespone = CON_ON_ACTIVATE;
        m_bRtpOnly = true;
		BeActivateRtp();
	}
	else
	{
        LOG_DEBUG("%d Remote BeActivate request %d,node(%d)",
                  m_RouteResult.chProcess,m_RouteResult.chRequest,m_pSystem->m_Node.nNode);

        m_eRespone = CON_ON_ACTIVATE;
        m_bRtpOnly = false;
		BeActivateLocal();
	}
}

void CRouteRemote::BeDeActivate()
{
    m_eRespone = CON_ON_DEACTIVATE;
    if(m_bRtpOnly)
    {
        LOG_DEBUG("%d RTP BeDeActivate, request %d",m_RouteResult.chProcess,m_RouteResult.chRequest);
    }
    else
    {
        LOG_DEBUG("%d Remote BeDeActivate, request %d",m_RouteResult.chProcess,m_RouteResult.chRequest);
        CRoute::DeActivate();
    }
}

void CRouteRemote::BeInterrupt()
{
    if(m_bRtpOnly)
    {
        LOG_DEBUG("%d RTP BeInterrupt, request %d",m_RouteResult.chProcess,m_RouteResult.chRequest);
    }
    else
    {
        LOG_DEBUG("%d Remote BeInterrupt, request %d",m_RouteResult.chProcess,m_RouteResult.chRequest);
    }
    m_bBeInterrupt = true;
    m_eRespone = CON_ON_INTERRUPT;
    bool bFree = ReleaseOutput();
    OnRouteState(CON_INTERRUPTED);

    if(bFree)
    {
        CService::GetInstance()->SendBusyState();
    }
}

void CRouteRemote::BeReActivate(t_AudioSrc &/*src*/)
{
    m_bBeInterrupt = false;
    if(m_bRtpOnly)
    {
        LOG_DEBUG("%d RTP BeReActivate, request %d",m_RouteResult.chProcess,m_RouteResult.chRequest);
        m_eRespone = CON_ON_REACTIVATE;

        if(m_RouteResult.e_ConState == CON_INTERRUPTED
            || m_RouteResult.e_ConState == CON_RECONNECT_POSSIBLE)
        {
            ActivateOutput();
        }
        else
        {
            CMsgConnectRequest request(m_pSystem);
            request.RouteStatus(m_RouteResult, m_eRespone);
        }
    }
    else
    {
        LOG_DEBUG("%d Remote BeReActivate, request %d",m_RouteResult.chProcess,m_RouteResult.chRequest);
    }
}

void CRouteRemote::BeRouteStatus()
{
    if(m_bRtpOnly)
    {
        LOG_DEBUG("%d RTP BeRouteState, request %d",m_RouteResult.chProcess,m_RouteResult.chRequest);
    }
    else
    {
        LOG_DEBUG("%d Remote BeRouteState, request %d",m_RouteResult.chProcess,m_RouteResult.chRequest);
    }
    m_eRespone = CON_ON_ROUTESTATE;
    CMsgConnectRequest request(m_pSystem);
    request.RouteStatus(m_RouteResult, CON_ON_ROUTESTATE);
}

bool CRouteRemote::IsRtpActivate()
{
	t_AudioSrc &src = m_pActivate->vSrc[0];

    if (src.Type == AS_TYPE_AN)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int CRouteRemote::GetPhysicalPort()
{
    auto it = m_pActivate->vSrc.begin();
    while(it != m_pActivate->vSrc.end())
    {
        t_AudioSrc &src = (*it);
        int num = src.Num + 1;
        if (CService::GetInstance()->IsInput(num))
        {
            m_pActivate->vSrc.erase(it);
            return num;
        }
        it++;
    }
    return -1;
}

bool CRouteRemote::IsOutputExist(std::vector<int> &vOutput)
{
	for(unsigned int i = 0;i<vOutput.size();i++)
	{
		if(!CService::GetInstance()->IsOutput(vOutput[i]))
		{
			return false;
		}
	}
	return true;
}

void CRouteRemote::BeActivateLocal()
{
    SET_RECONNECT_REQUEST(m_pActivate->Flags);

	int nPort = GetPhysicalPort();

	if (nPort != -1)
	{
		m_nPort = nPort;
		BeActivatePhysical();
	}
	else
	{
		m_nPort = -1;
		BeActivateVirtual();
	}
}

void CRouteRemote::BeActivateRtp()
{
    //only one destination
    if (m_vDest.size() == 1 && m_vDest[0].Sys == CService::GetInstance()->GetNode())
    {
        //check the port
        if (m_vDest[0].vOutputPort.size() == 0
            || !IsOutputExist(m_vDest[0].vOutputPort)
            || !CreateAudioJob())
        {
            LOG_ERROR("RTP BeActivate:%s", "Output Port Error");
            CMsgConnectRequest request(m_pSystem);
            m_RouteResult.e_ConState = CON_DIS_CONNECT;
            request.RouteStatus(m_RouteResult, m_eRespone);
            return;
        }
        GetOutput(m_vDest[0].vOutputPort);
        m_vDest.clear();
        ActivateOutput();
    }
    else
    {
        LOG_ERROR("RTP BeActivate:%s", "Only support one destination");
        CMsgConnectRequest request(m_pSystem);
        m_RouteResult.e_ConState = CON_DIS_CONNECT;
        request.RouteStatus(m_RouteResult, m_eRespone);
    }
}

void CRouteRemote::GetOutput(std::vector<int> &vOutput)
{
    for (unsigned int i = 0; i < vOutput.size(); i++)
    {
        t_Output out;
        out.nPort = vOutput[i];
        out.bConnect = false;
        m_vOutput.push_back(out);
    }
}

bool CRouteRemote::CreateAudioJob()
{
    if (m_bRtpOnly)
    {
        CService* pSer = CService::GetInstance();
        CAudioReceive *pJob = new CAudioReceive();  //checked

        pJob->SetChProcess(m_RouteResult.chProcess);

        pJob->SetRtpNode(m_pActivate->vSrc[0].Num);
        pJob->SetRtpChannel(m_pActivate->vSrc[0].Title);

        m_nRtpNode = pJob->GetRtpNode();
        m_nRtpChannel = pJob->GetRtpChannel();
        m_pData->AddAudio(pJob);
        return true;
    }
    else
    {
        return CRouteLocal::CreateAudioJob();
    }
}

void CRouteRemote::OnRouteState(CONSTATE conState)
{
    bool bFree = false;
    if(m_RouteResult.e_ConState != conState)
    {
        m_RouteResult.e_ConState = conState;
        if(!CRoute::UpdateJob(conState))
        {
            DeActivate();
            OnRouteState(CON_DIS_CONNECT);
            return;
        }
        if(m_bRtpOnly)
        {
            LOG_DEBUG("%d RTP OnRouteState: %s",m_RouteResult.chProcess,
                  GetState(m_RouteResult.e_ConState).c_str());

            if (m_RouteResult.e_ConState == CON_DIS_CONNECT)
            {
                bFree = ReleaseOutput();
            }
        }
        else
        {
            LOG_DEBUG("%d Remote OnRouteState: %s",m_RouteResult.chProcess,
                      GetState(m_RouteResult.e_ConState).c_str());
        }

        //respond result to remote
        CMsgConnectRequest request(m_pSystem);
        request.RouteStatus(m_RouteResult, m_eRespone);
    }
    else
    {
        if(m_bRtpOnly)
        {
            //fro RTP job, always send the route state
            CMsgConnectRequest request(m_pSystem);
            request.RouteStatus(m_RouteResult, m_eRespone);
        }
    }

    if (ResetInput(conState) || bFree)
    {
        CService::GetInstance()->SendBusyState();
    }
}

void CRouteRemote::EverySecond()
{
    CRouteLocal::EverySecond();

    if (IsDisConnect())
        return;

    //check the remote client every 10 seconds
    m_nTimerCount10++;
    if (m_nTimerCount10 >= 10)
    {
        m_nTimerCount10 = 0;
        m_nCheckTimes++;
        if (m_nCheckTimes < 3)
        {
            //submid:0x09
            CMsgCheckRoute check(m_pSystem);
            check.Check(m_RouteResult.chRequest, m_RouteResult.chProcess);
        }
        else
        {
            LOG_DEBUG("%d no check feedback for 30 seconds", m_RouteResult.chProcess);
            DeActivate();
            OnRouteState(CON_DIS_CONNECT);
        }
    }
}

void CRouteRemote::OnCheckRoute()
{
    m_nCheckTimes = 0;
}

void CRouteRemote::InterruptOutput(std::vector<int> &vConflictPort)
{
    if(!m_bJobStarted)
    {
        //when the job not start, don't need to interrupt.
        return;
    }

    LOG_DEBUG("%d RTP Interrupt Output",m_RouteResult.chProcess);

    if (IsPartlyMode())
    {
        InterruptOutputPartly(vConflictPort);
    }
    else
    {
        InterruptOutputFull();
    }
}

void CRouteRemote::InterruptOutputPartly(std::vector<int> &vConflictPort)
{
    bool bFree = false;
    unsigned int nConnectCount = 0;
    for (unsigned int i = 0; i < m_vOutput.size(); i++)
    {
        if (!m_vOutput[i].bConnect)
        {
            continue;
        }
        //free the conflict port
        if(CPublic::IsPortExist(vConflictPort,m_vOutput[i].nPort))
        {
            m_vOutput[i].bConnect = false;
            m_vOutput[i].bInterrupt = true;
            m_pData->FreePort(m_RouteResult.chProcess,
                false, m_vOutput[i].nPort);
            bFree = true;
        }
        if (m_vOutput[i].bConnect)
        {
            nConnectCount++;
        }
    }
   
    if (nConnectCount == m_vOutput.size())
    {
        OnRouteState(CON_FULL_CONNECT);
    }
    else if (nConnectCount>0)
    {
        OnRouteState(CON_PARTLY_CONNECT);
    }
    else
    {
        //0 connect
        if (IsReconnect())
        {
            OnRouteState(CON_INTERRUPTED);
        }
        else
        {
            //3, do disconnect (interrupt output)
            OnRouteState(CON_DIS_CONNECT);
        }
    }

    if (bFree)
    {
        auto pMsg = std::make_shared<CMsg>();
        pMsg->type = MSG_RESTORE_OUTPUT;
        CService::GetInstance()->Push(pMsg);
    }
}

void CRouteRemote::InterruptOutputFull()
{
    ReleaseOutput();
    if (IsReconnect())
    {
        OnRouteState(CON_INTERRUPTED);
    }
    else
    {
        //3, do disconnect (interrupt output)
        OnRouteState(CON_DIS_CONNECT);
    }
}

void CRouteRemote::ActivateOutput()
{
    if (IsPartlyMode())
    {
        if (!IsPartNewOutputCanUsed())
        {
            OnOutputGroupState();
            return;
        }
    }
    else
    {
        if (!IsAllOutputCanUsed())
        {
            OnRouteState(CON_INTERRUPTED);
            return;
        }
    }
    TryToConnect();
}

void CRouteRemote::TryToConnect()
{
    if (IsOutputConflict())
    {
        m_pData->InterruptOutput(this);
    }

    //allocate free ports
    bool bAllocate = false;
    for (unsigned int i = 0; i < m_vOutput.size(); i++)
    {
        t_Output &output = m_vOutput[i];
        if(output.bConnect == false)
        {
            if (CService::GetInstance()->IsPortFree(false,output.nPort))
            {
                bAllocate = true;
                output.bConnect = true;
                m_pData->AllocatePort(m_RouteResult.chProcess,
                    false, output.nPort, m_pActivate->Priority,m_nRtpChannel);
            }
        }
    }

    OnOutputGroupState();

    if(bAllocate)
    {
        CService::GetInstance()->SendBusyState();
    }
}

void CRouteRemote::OnOutputGroupState()
{
    unsigned int nConnectCount = 0;
    for (unsigned int i = 0; i < m_vOutput.size(); i++)
    {
        t_Output &output = m_vOutput[i];
        if(output.bConnect)
        {
            nConnectCount++;
            continue;
        }
    }

    if (nConnectCount == m_vOutput.size())
    {
        OnRouteState(CON_FULL_CONNECT);
    }
    else if (nConnectCount > 0)
    {
        OnRouteState(CON_PARTLY_CONNECT);
    }
    else
    {
        OnRouteState(CON_INTERRUPTED);
    }
}

bool CRouteRemote::IsNeed2Restore()
{
    for (unsigned int i = 0; i < m_vOutput.size(); i++)
    {
        t_Output &output = m_vOutput[i];
        if (!output.bConnect)
        {
            return true;
        }
    }
    return false;
}

bool CRouteRemote::RestoreOutput()
{
    if(m_bBeInterrupt)
    {
        //don't need to restore if the route be Interrupted.
        return false;
    }
    if (!IsNeed2Restore())
    {
        return false;
    }

    m_eRespone = CON_ON_ROUTESTATE;

    //full connect
    if (IsPartlyMode())
    {
        return RestoreOutputPartly();
    }
    else
    {
        return RestoreOutputFull();
    } 
}

bool CRouteRemote::RestoreOutputFull()
{
    if (!IsAllOutputCanUsed())
    {
        return false;
    }
    if (IsReconnectRequest())
    {
        if(m_RouteResult.e_ConState == CON_RECONNECT_POSSIBLE)
        {
            return false;
        }
        else
        {
            LOG_DEBUG("%d RTP RestoreOutput(Full)",m_RouteResult.chProcess);

            m_RouteResult.e_ConState = CON_RECONNECT_POSSIBLE;
            CMsgConnectRequest request(m_pSystem);
            request.RouteStatus(m_RouteResult, CON_ON_ROUTESTATE);
            return true;
        }
    }
    else
    {
        TryToConnect();
        return true;
    }
}

bool CRouteRemote::RestoreOutputPartly()
{
    if (!IsPartNewOutputCanUsed())
    {
        return false;
    }

    if(m_bJobStarted)
    {
        if(!IsReconnect())
        {
            //the job started, but no reconnect
            return false;
        }
    }

    if(IsReconnectRequest())
    {
        if(m_RouteResult.e_ConState == CON_RECONNECT_POSSIBLE)
        {
            return false;
        }
        else
        {
            LOG_DEBUG("%d RTP RestoreOutput(Part)",m_RouteResult.chProcess);
            m_RouteResult.e_ConState = CON_RECONNECT_POSSIBLE;
            CMsgConnectRequest request(m_pSystem);
            request.RouteStatus(m_RouteResult, CON_ON_ROUTESTATE);
            return true;
        }
    }
    else
    {
        TryToConnect();
        return true;
    }
}

bool CRouteRemote::IsAllOutputCanUsed()
{
    for (unsigned int i = 0; i < m_vOutput.size(); i++)
    {
        t_Output &output = m_vOutput[i];
        if (!CService::GetInstance()->IsPortCanUsed(false,
            output.nPort,m_pActivate->Priority))
        {
            return false;
        }
    }
    return true;
}

bool CRouteRemote::IsPartNewOutputCanUsed()
{
    for (unsigned int i = 0; i < m_vOutput.size(); i++)
    {
        t_Output &output = m_vOutput[i];
        if(output.bConnect == false)
        {
            if (CService::GetInstance()->IsPortCanUsed(false,
                output.nPort, m_pActivate->Priority))
            {
                return true;
            }
        }
    }
    return false;
}

bool CRouteRemote::IsOutputConflict()
{
    for (unsigned int i = 0; i < m_vOutput.size(); i++)
    {
        t_Output &output = m_vOutput[i];
        if(output.bConnect == false)
        {
            if (!CService::GetInstance()->IsPortFree(false,output.nPort))
            {
                return true;
            }
        }
    }
    return false;
}

void CRouteRemote::BeActivatePhysical()
{
    /*
    CActivateMicr *pMicro = new CActivateMicr();
    CopyParameter(pMicro);
    SET_RECONNECT_REQUEST(pMicro->Flags);

    //set Gong
    if (m_pActivate->vSrc.size()>0)
    {
        pMicro->nGongChannel = m_pActivate->vSrc[0].Num + 1;
        pMicro->nGongTitle = m_pActivate->vSrc[0].Title + 1;
        m_RouteResult.LastSrcIdx = 1;
    }
    else
    {
        m_RouteResult.LastSrcIdx = 0;
    }

    if(m_pActivate)
    {
        delete m_pActivate;
        m_pActivate = NULL;
    }
    m_pActivate = pMicro;

	CRouteLocal::Activate();
    */
}

void CRouteRemote::BeActivateVirtual()
{
    CActivatePlay* pPlay = new CActivatePlay();
    CopyParameter(pPlay);
    SET_RECONNECT_REQUEST(pPlay->Flags);

    GetIOMAP(pPlay->vIOMapPath);
    
    if(pPlay->vIOMapPath.size()==0 || !CPublic::IsAllAdpFormat(pPlay->vIOMapPath))
	{
        if(pPlay->vIOMapPath.size()==0)
        {
            LOG_ERROR("Remote BeActivate(Virtual):%s", "No AudioPath Error");
        }
        else
        {
            LOG_ERROR("Remote BeActivate(Virtual):%s", "AudioPath Format Error");
        }
        CMsgConnectRequest request(m_pSystem);
        m_RouteResult.e_ConState = CON_DIS_CONNECT;
        request.RouteStatus(m_RouteResult, m_eRespone);
		return;
	}
    m_RouteResult.LastSrcIdx = pPlay->vIOMapPath.size() - 1;

    if(m_pActivate)
    {
        delete m_pActivate;
        m_pActivate = NULL;
    }
    m_pActivate = pPlay;

	CRouteLocal::Activate();
}

void CRouteRemote::CopyParameter(CActivateMicr* pMic)
{
	pMic->chProcess = m_RouteResult.chProcess;
	pMic->Priority = m_pActivate->Priority;
	pMic->TimeOut = m_pActivate->TimeOut;
    pMic->Level = m_pActivate->Level;
	pMic->Flags = m_pActivate->Flags;
    pMic->nRtpNode = CService::GetInstance()->GetNode();
    pMic->nPort = m_nPort;

    int len = m_pActivate->vSrc.size();
    pMic->nRepeatCount = m_pActivate->vSrc[len-1].Loop;

	//dest
	for (unsigned int i = 0; i < m_pActivate->vDest.size(); i++)
	{
		pMic->vDest.push_back(m_pActivate->vDest[i]);
	}
}

void CRouteRemote::GetIOMAP(std::vector<t_FILEMAP> &vPath)
{
	vPath.clear();

	for (unsigned int i = 0; i<m_pActivate->vSrc.size(); i++)
	{
		int num = m_pActivate->vSrc[i].Num + 1;
		int title = m_pActivate->vSrc[i].Title+1;

        t_FILEMAP file;
        if(CService::GetInstance()->GetFileMAP(num,title,file))
		{
            vPath.push_back(file);
		}
        else
        {
#ifdef _MSC_VER
            std::string strPath = CCommon::GetModulePath() + "Presignal\\nofile.adp";
#else
            std::string strPath = CCommon::GetModulePath() + "Presignal/nofile.adp";
#endif
            file.vPath.push_back(strPath);
            vPath.push_back(file);
        }
	}
}

bool CRouteRemote::ReleaseOutput()
{
    bool bFree = false;
    for (unsigned int i = 0; i < m_vOutput.size(); i++)
    {
        if (m_vOutput[i].bConnect)
        {
            bFree = true;
            m_vOutput[i].bConnect = false;
            m_pData->FreePort(m_RouteResult.chProcess,
                false, m_vOutput[i].nPort);
        }
    }

    if (bFree)
    {
        auto pMsg = std::make_shared<CMsg>();
        pMsg->type = MSG_RESTORE_OUTPUT;
        CService::GetInstance()->Push(pMsg);
    }
    return bFree;
}

void CRouteRemote::CurrentPlayIndex(int nIndex)
{
    m_RouteResult.SrcIdx = nIndex;
    if (m_pSystem)
    {
        CMsgConnectRequest request(m_pSystem);
        request.RouteStatus(m_RouteResult, CON_ON_ROUTESTATE);
    }
}
