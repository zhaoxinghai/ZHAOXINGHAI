
#include "routelocal.h"
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

CRouteLocal::CRouteLocal(CAnnouncement* pA):CRoute(pA)
{
    m_eType = ROUTE_LOCAL;
    m_nInterruptdom = -1;
    m_bInputConnect = false;
    m_bCreateJob = false;

    m_bInputInterrupt = false;
    m_bDelayActivate = false;
}

CRouteLocal::~CRouteLocal()
{
}

void CRouteLocal::Activate()
{
    if (!m_bCreateJob)
    {
        m_bCreateJob = true;
        if (!CreateAudioJob())
        {
            OnRouteState(CON_DIS_CONNECT);
            return;
        }
    }

    //for physical input
    if (m_Activate.Property.nPort > 0)
    {
        if (!g_SDKServer.IsPortCanUsed(true,
            m_Activate.Property.nPort, m_Activate.Property.nPriority))
        {
            //input is busy now, try to activate when the input restore
            m_bInputInterrupt = true;
            m_bDelayActivate = true;
            LOG_DEBUG("%d Input is busy, port %d",m_RouteResult.nProcess, m_Activate.Property.nPort);
            OnRouteState(CON_INTERRUPTED);
        }
        else
        {
            LOG_DEBUG("%d Input is free, port %d",m_RouteResult.nProcess, m_Activate.Property.nPort);

            //notify destination
            CRoute::Activate();
        }
    }
    else
    {
        //notify destination
        CRoute::Activate();
    }
}

void CRouteLocal::OnActivate(int nNode, t_ActivateRet ret)
{
	LOG_DEBUG("%d OnActivate Node %d,State: %s",m_RouteResult.nProcess,nNode,GetState(ret.e_ConState).c_str());
	OnNodeState(nNode,ret);
}

void CRouteLocal::OnDeActivate(int nNode, t_ActivateRet ret)
{
	LOG_DEBUG("%d OnDeActivate Node %d,State: %s",m_RouteResult.nProcess,nNode,GetState(ret.e_ConState).c_str());

	OnNodeState(nNode,ret);
}

void CRouteLocal::OnInterrupt(int nNode, t_ActivateRet ret)
{
	LOG_DEBUG("%d OnInterrupt Node %d,State: %s",m_RouteResult.nProcess,nNode,GetState(ret.e_ConState).c_str());

    if (!IsInterrupt())
    {
    	ReActivateNode(nNode);
    }
}

void CRouteLocal::OnReActivate(int nNode, t_ActivateRet ret)
{
	LOG_DEBUG("%d OnReActivate Node %d,State: %s",m_RouteResult.nProcess,nNode,GetState(ret.e_ConState).c_str());
	OnNodeState(nNode, ret);
}

void CRouteLocal::OnRouteState(int nNode, t_ActivateRet ret)
{
	LOG_DEBUG("%d OnRouteState Node %d,State: %s",m_RouteResult.nProcess,nNode,GetState(ret.e_ConState).c_str());
	OnNodeState(nNode, ret);
}

void CRouteLocal::OnNodeState(int nNode, t_ActivateRet ret)
{
    //full mode,disconnect
	if (IsFullMode() && ret.e_ConState == CON_DIS_CONNECT)
	{
        LOG_DEBUG("%d Node %d disconnect",m_RouteResult.nProcess,nNode);
        DeActivate();
		OnRouteState(CON_DIS_CONNECT);
		return;
	}

    //input interrupted
    if (m_bInputInterrupt)
    {
        if(ret.e_ConState == CON_FULL_CONNECT
                || ret.e_ConState == CON_PARTLY_CONNECT)
        {
            InterruptNode(nNode);
        }
        OnRouteState(CON_INTERRUPTED);
        return;
    }

    if(IsFullMode())
    {
    	OnActivateFull(nNode,ret);
    }
    else
    {
    	OnActivatePart(nNode,ret);
    }
}

void CRouteLocal::OnActivatePart(int nNode, t_ActivateRet ret)
{
	if(ret.e_ConState == CON_RECONNECT_POSSIBLE)
	{
        if(IsReconnect())
        {
            ReActivateNode(nNode);
        }
        else
        {
            if(m_bJobStarted)
            {
                long long lTm = CCommon::GetSysTimeMicros() - m_lStartedTime;
                if(lTm > 500*1000)
                {
                    DeActivateNode(nNode);
                }
                else
                {
                    ReActivateNode(nNode);
                }
            }
            else
            {
                ReActivateNode(nNode);
            }
        }
	}
	OnGroupState();
}

void CRouteLocal::OnActivateFull(int nNode, t_ActivateRet ret)
{
	//one of the DOM is interrupted
	if (m_nInterruptdom != -1)
	{
		if (m_nInterruptdom == nNode)
		{
			//monitor the first interrupt DOM to restore
			if (ret.e_ConState == CON_FULL_CONNECT
			 || ret.e_ConState == CON_PARTLY_CONNECT)
			{
                LOG_DEBUG("%d Node %d restore",m_RouteResult.nProcess,nNode);
				m_nInterruptdom = -1;
                ReActivateExcept(nNode);
			}
			else if(ret.e_ConState == CON_RECONNECT_POSSIBLE)
			{
                LOG_DEBUG("%d Node %d restore",m_RouteResult.nProcess,nNode);
				m_nInterruptdom = -1;
                ReActivateExcept(-1);
                return;
			}
			OnGroupState();
		}
		else
		{
            if(ret.e_ConState == CON_FULL_CONNECT
                || ret.e_ConState == CON_PARTLY_CONNECT)
            {
                InterruptNode(nNode);
            }
			OnRouteState(CON_INTERRUPTED);
		}
		return;
	}

	//full mode,remember first interrupt DOM
	if (m_nInterruptdom == -1 && ret.e_ConState == CON_INTERRUPTED)
	{
        LOG_DEBUG("%d Node %d first interrupt",m_RouteResult.nProcess,nNode);

		m_nInterruptdom = nNode;
        for (unsigned int i = 0; i < m_vDest.size(); i++)
        {
            if(m_vDest[i].nNode != nNode)
            {
                if(m_vDest[i].ret.e_ConState == CON_FULL_CONNECT
                    || m_vDest[i].ret.e_ConState == CON_PARTLY_CONNECT)
                {
                    InterruptNode(m_vDest[i].nNode);
                }
            }
        }
		OnRouteState(CON_INTERRUPTED);
		return;
	}
	OnGroupState();
}

void CRouteLocal::OnGroupState()
{
	//update counter
	int nFullConCount = 0;
	int nPartConCount = 0;
	int nInterruptCount = 0;
	int nReConPossibleCount = 0;
	int nDisCount = 0;

	for (unsigned int i = 0; i < m_vDest.size(); i++)
	{
		if (m_vDest[i].ret.e_ConState == CON_FULL_CONNECT)
		{
			nFullConCount++;
		}
		else if (m_vDest[i].ret.e_ConState == CON_PARTLY_CONNECT)
		{
			nPartConCount++;
		}
		else if (m_vDest[i].ret.e_ConState == CON_INTERRUPTED)
		{
			nInterruptCount++;
		}
		else if(m_vDest[i].ret.e_ConState == CON_RECONNECT_POSSIBLE)
		{
			nReConPossibleCount++;
		}
		else if (m_vDest[i].ret.e_ConState == CON_DIS_CONNECT)
		{
			nDisCount++;
		}
	}

	//state
	if (nFullConCount == (int)m_vDest.size())
	{
		OnRouteState(CON_FULL_CONNECT);
	}
	else if (nPartConCount > 0 || nFullConCount > 0)
	{
		OnRouteState(CON_PARTLY_CONNECT);
	}
    else if(nReConPossibleCount>0 || nInterruptCount > 0)
	{
		OnRouteState(CON_INTERRUPTED);
	}
	else
	{
		OnRouteState(CON_DIS_CONNECT);
	}
}

bool CRouteLocal::CreateAudioJob()
{
    /*
    //Get channel
    m_Activate.Property.nAudioChannal = m_pData->GetChannel();
    if (m_Activate.Property.nAudioChannal == -1)
    {
        LOG_DEBUG("%d No available RTP channel", m_RouteResult.nProcess);
        //m_RouteResult.bCreateAudioJob = false;
        return false;
    }

    m_nRtpChannel = m_Activate.Property.nAudioChannal;

    if (m_Activate.Property.eType == ACTIVATE_PLAY)
    {
        CAudioPlay* Job = new CAudioPlay(LOCAL_ADP2NET);  //checked
    
        Job->SetRepeatCount(m_Activate.Property.nRepeatCount);
        Job->SetChProcess(p->nProcess);

        t_FILEMAP file;
        if(pSer->GetFileMAP(p->nGongChannel, p->nGongTitle,file))
        {
            Job->SetPreSignalFile(file);
        }
        Job->SetAudioPath(p->vIOMapPath);
        float fScale = CCommon::dB2FloatScale((p->Level)/ 10.0f);
        Job->VolumeAjust(fScale);
        Job->VolumeAjustSG(pSer->m_VolumeScaleSG);
        Job->SetRtpNode(pSer->GetNode());
        Job->SetRtpChannel(p->nRtpChannel);
        m_pData->AddAudio(Job);
    }
    else
    {
        CAudioCapture* Job = new CAudioCapture(CAPTURE_NET_ANNOUNCEMENT, m_Activate.Property.nPort);  //checked

        Job->EnableVLAN(pSer->m_bVLANOpen);
        Job->SetChProcess(m_pActivate->nProcess);

        t_FILEMAP file;
        if(pSer->GetFileMAP(m_pActivate-, m_pActivate->nGongTitle,file))
        {
            Job->SetPreSignalFile(file);
        }
        //alarm record
        if(m_Activate.Property.nPriority <= 20 && m_Activate.Property.nPort == 41)
        {
            //Job->SetVARecord(true);
        }

        float fScale = CCommon::dB2FloatScale((m_pActivate->Level) / 10.0f);
        Job->VolumeAjust(fScale);
        Job->VolumeAjustSG(pSer->m_VolumeScaleSG);
        Job->SetRtpNode(pSer->GetNode());
        Job->SetRtpChannel(pActivate->nRtpChannel);
        Job->OpenGongAudio();
        m_pData->AddAudio(Job);
    }*/
    return true;
}

bool CRouteLocal::ResetInput(CONSTATE conState)
{
    if (m_Activate.Property.nPort <= 0)
    {
        return false;
    }

	if (conState == CON_FULL_CONNECT || conState == CON_PARTLY_CONNECT)
	{
		if (!m_bInputConnect)
		{
			//interrupt conflict input port
            g_SDKServer.m_routeManager.InterruptInput(this);

			m_bInputConnect = true;
      
            g_SDKServer.m_routeManager.AllocatePort(m_RouteResult.nProcess,
                true, m_Activate.Property.nPort, m_Activate.Property.nPriority, m_Activate.Property.nAudioChannal);
            return true;
		}
	}
	else
	{
		if (m_bInputConnect)
		{
			m_bInputConnect = false;
            g_SDKServer.m_routeManager.FreePort(m_RouteResult.nProcess, true, m_Activate.Property.nPort);

            auto pMsg = std::make_shared<CMsg>();
            pMsg->type = MSG_RESTORE_INPUT;
            g_SDKServer.Push(pMsg);
            return true;
		}
	}
    return false;
}

void CRouteLocal::OnRouteState(CONSTATE conState)
{  
    if(m_RouteResult.e_ConState != conState)
    {
        m_RouteResult.e_ConState = conState;
        if(!CRoute::UpdateJob(conState))
        {
            DeActivate();
            OnRouteState(CON_DIS_CONNECT);
            return;
        }
        LOG_DEBUG("%d Local RouteState: %s",m_RouteResult.nProcess,
            GetState(m_RouteResult.e_ConState).c_str());

        //callback
        CAnnouncementResult ret;
        ret.nErrorCode = m_RouteResult.nErrorCode;
        ret.nPlayIndex = m_RouteResult.nPlayIndex;
        ret.nRepeatIndex = m_RouteResult.nRepeatIndex;
        ret.nRequest = m_RouteResult.nRequest;
        ret.nProcess = m_RouteResult.nProcess;

        if (m_RouteResult.e_ConState == CON_FULL_CONNECT)
            ret.eCallState = CALL_FULL_CONNECT;
        else if (m_RouteResult.e_ConState == CON_PARTLY_CONNECT)
            ret.eCallState = CALL_PARTLY_CONNECT;
        else if (m_RouteResult.e_ConState == CON_INTERRUPTED)
            ret.eCallState = CALL_INTERRUPTED;
        else if (m_RouteResult.e_ConState == CON_RECONNECT_POSSIBLE)
            ret.eCallState = CALL_INTERRUPTED;
        else if (m_RouteResult.e_ConState == CON_DIS_CONNECT)
            ret.eCallState = CALL_DIS_CONNECT;
        else
            ret.eCallState = CALL_DIS_CONNECT;

        g_SDKServer.ExcuteCallback(&ret);
    }
    if (ResetInput(conState))
    {
        g_SDKServer.SendBusyState();
    }
}

void CRouteLocal::InterruptInput()
{
    if (m_Activate.Property.nPort <= 0)
    {
        return;
    }

    m_bInputInterrupt = true;
    LOG_DEBUG("%d Interrupt Input",m_RouteResult.nProcess);

    if (IsReconnect())
    {
        for (unsigned int i = 0; i < m_vDest.size(); i++)
        {
            if(m_vDest[i].ret.e_ConState == CON_FULL_CONNECT
                || m_vDest[i].ret.e_ConState == CON_PARTLY_CONNECT)
            {
                InterruptNode(m_vDest[i].nNode);
            }
        }
        OnRouteState(CON_INTERRUPTED);
    }
    else
    {
        if(m_bJobStarted)
        {
            //when the job started, disconnect.
            DeActivate();
            OnRouteState(CON_DIS_CONNECT);
        }
    }
}

bool CRouteLocal::RestoreInput()
{
    if (m_bInputConnect)
    {
        return false;
    }
    if(m_Activate.Property.nPort <= 0 || m_bInputInterrupt == false)
    {
        return false;
    }
    if (!g_SDKServer.IsPortCanUsed(true,
        m_Activate.Property.nPort, m_Activate.Property.nPriority))
    {
        return false;
    }

    LOG_DEBUG("%d Restore Input,port %d",m_RouteResult.nProcess, m_Activate.Property.nPort);

    m_bInputInterrupt = false;

    if(m_bDelayActivate)
    {
        m_bDelayActivate = false;
        CRoute::Activate();
    }
    else
    {
        ReActivateExcept(-1);
    }
    return true;
}

void CRouteLocal::ReActivateExcept(int nNode)
{
    for (unsigned int i = 0; i < m_vDest.size(); i++)
    {
    	if(m_vDest[i].nNode != nNode)
    	{
    		ReActivateNode(m_vDest[i].nNode);
    	}
    }
}

void CRouteLocal::EverySecond()
{
    CRoute::EverySecond();
    
    if (IsDisConnect())
    {
        //time out, disconnect
        return;
    }

    //try activate,reactivate and interrupt
    for (unsigned int i = 0; i < m_vDest.size(); i++)
    {
        t_DestinationRoute& dst = m_vDest[i];
        if (!dst.bRequest)
        {
            continue; //no request, continue
        }
        dst.nRequestSeconds++;
        if (dst.nRequestSeconds < 5)
        {
            continue;
        }
        if (!dst.bActivate)
        {
            ActivateNode(dst.nNode);
        }
        else if (dst.eRequest == CON_REACTIVATE)
        {
            if (dst.ret.e_ConState == CON_FULL_CONNECT
                || dst.ret.e_ConState == CON_PARTLY_CONNECT)
            {
                dst.bRequest = false;
            }
            else
            {
                ReActivateNode(dst.nNode);
            }
        }
        else if (dst.eRequest == CON_INTERRUPT)
        {
            if (dst.ret.e_ConState == CON_INTERRUPTED)
            {
                dst.bRequest = false;
            }
            else
            {
                InterruptNode(dst.nNode);
            }
        }
        else
        {
        }
    }
}

bool CRouteLocal::IsInterrupt()
{
    if (m_bInputInterrupt)
	{
        return true;
	}
	if(m_nInterruptdom != -1)
	{
		return true;
	}
	return false;
}

void CRouteLocal::InterruptNode(int nNode)
{
    t_DestinationRoute* pDest = GetDestination(nNode);
    if (pDest == NULL || !pDest->bActivate || pDest->bDeActivate)
    {
        return;
    }
    pDest->bRequest = true;
    pDest->nRequestSeconds = 0;
    pDest->eRequest = CON_INTERRUPT;

    CTSystem* pSystem = g_SDKServer.GetD1System(nNode);
    if (pSystem == NULL || !pSystem->IsConnect())
    {
        return;
    }

    CMsgConnection Connect(pSystem);
    Connect.Interrupt(pDest->ret.nRequest, pDest->ret.nProcess);

    LOG_DEBUG("%d Interrupt Node %d", m_RouteResult.nProcess, nNode);
}

void CRouteLocal::DeActivateNode(int nNode)
{
    t_DestinationRoute* pDest = GetDestination(nNode);
    if (pDest == NULL || !pDest->bActivate || pDest->bDeActivate)
    {
        return;
    }
    pDest->bDeActivate = true;
    if (pDest->ret.e_ConState == CON_DIS_CONNECT)
    {
        return;
    }
    pDest->ret.e_ConState = CON_DIS_CONNECT;
    pDest->bRequest = true;
    pDest->nRequestSeconds = 0;
    pDest->eRequest = CON_DEACTIVATE;

    CTSystem* pSystem = g_SDKServer.GetD1System(nNode);
    if (pSystem == NULL)
    {
        return;
    }

    CMsgConnection Connect(pSystem);
    Connect.DeActivate(pDest->ret.nRequest, pDest->ret.nProcess);

    LOG_DEBUG("%d DeActivate Node %d", m_RouteResult.nProcess, nNode);
}

void CRouteLocal::ReActivateNode(int nNode)
{
    t_DestinationRoute* pDest = GetDestination(nNode);
    if (pDest == NULL || !pDest->bActivate || pDest->bDeActivate)
    {
        return;
    }
    pDest->bRequest = true;
    pDest->nRequestSeconds = 0;
    pDest->eRequest = CON_REACTIVATE;

    CTSystem* pSystem = g_SDKServer.GetD1System(nNode);
    if (pSystem == NULL || !pSystem->IsConnect())
    {
        return;
    }

    CMsgConnection Connect(pSystem);
    Connect.ReActivate(pDest->ret.nRequest, pDest->ret.nProcess);

    LOG_DEBUG("%d ReActivate Node %d", m_RouteResult.nProcess, nNode);
}
