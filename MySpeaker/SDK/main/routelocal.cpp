
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

CRouteLocal::CRouteLocal(CActivate* pA):CRoute(pA)
{
    m_eType = ROUTE_LOCAL;
    m_nInterruptdom = -1;
    m_bInputConnect = false;
    m_bCreateJob = false;

    m_bInputInterrupt = false;
    m_bDelayActivate = false;

	//some audio destinations
	for (unsigned int i = 0; i < m_pActivate->vDest.size(); i++)
	{
        t_AudioDestRoute dest(m_pActivate->vDest[i]);
		m_vDest.push_back(dest);
	}

    //micropone live
    if (m_pActivate->type == ACTIVATE_MICR)
    {
        CActivateMicr* pActivate = (CActivateMicr*)m_pActivate;
        m_nPort = pActivate->nPort;
    }
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
    if (m_nPort != -1)
    {
        if (!CService::GetInstance()->IsPortCanUsed(true,
            m_nPort, m_pActivate->Priority))
        {
            //input is busy now, try to activate when the input restore
            m_bInputInterrupt = true;
            m_bDelayActivate = true;
            LOG_DEBUG("%d Input is busy, port %d",m_RouteResult.chProcess,m_nPort);
            OnRouteState(CON_INTERRUPTED);
        }
        else
        {
            LOG_DEBUG("%d Input is free, port %d",m_RouteResult.chProcess,m_nPort);

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

void CRouteLocal::OnActivate(int nNode, CActivateResult &ret)
{
	LOG_DEBUG("%d OnActivate Node %d,State: %s",m_RouteResult.chProcess,nNode,GetState(ret.e_ConState).c_str());
	OnNodeState(nNode,ret);
}

void CRouteLocal::OnDeActivate(int nNode,CActivateResult &ret)
{
	LOG_DEBUG("%d OnDeActivate Node %d,State: %s",m_RouteResult.chProcess,nNode,GetState(ret.e_ConState).c_str());

	OnNodeState(nNode,ret);
}

void CRouteLocal::OnInterrupt(int nNode, CActivateResult &ret)
{
	LOG_DEBUG("%d OnInterrupt Node %d,State: %s",m_RouteResult.chProcess,nNode,GetState(ret.e_ConState).c_str());

    if (!IsInterrupt())
    {
    	ReActivateNode(nNode);
    }
}

void CRouteLocal::OnReActivate(int nNode, CActivateResult &ret)
{
	LOG_DEBUG("%d OnReActivate Node %d,State: %s",m_RouteResult.chProcess,nNode,GetState(ret.e_ConState).c_str());
	OnNodeState(nNode, ret);
}

void CRouteLocal::OnRouteState(int nNode, CActivateResult &ret)
{
	LOG_DEBUG("%d OnRouteState Node %d,State: %s",m_RouteResult.chProcess,nNode,GetState(ret.e_ConState).c_str());
	OnNodeState(nNode, ret);
}

void CRouteLocal::OnNodeState(int nNode, CActivateResult &ret)
{
    //full mode,disconnect
	if (IsFullMode() && ret.e_ConState == CON_DIS_CONNECT)
	{
        LOG_DEBUG("%d Node %d disconnect",m_RouteResult.chProcess,nNode);
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

void CRouteLocal::OnActivatePart(int nNode, CActivateResult &ret)
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

void CRouteLocal::OnActivateFull(int nNode, CActivateResult &ret)
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
                LOG_DEBUG("%d Node %d restore",m_RouteResult.chProcess,nNode);
				m_nInterruptdom = -1;
                ReActivateExcept(nNode);
			}
			else if(ret.e_ConState == CON_RECONNECT_POSSIBLE)
			{
                LOG_DEBUG("%d Node %d restore",m_RouteResult.chProcess,nNode);
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
        LOG_DEBUG("%d Node %d first interrupt",m_RouteResult.chProcess,nNode);

		m_nInterruptdom = nNode;
        for (unsigned int i = 0; i < m_vDest.size(); i++)
        {
            if(m_vDest[i].Sys != nNode)
            {
                if(m_vDest[i].ret.e_ConState == CON_FULL_CONNECT
                    || m_vDest[i].ret.e_ConState == CON_PARTLY_CONNECT)
                {
                    InterruptNode(m_vDest[i].Sys);
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
    CActivateMicr* pActivate = (CActivateMicr*)m_pActivate;

    //Get channel
    pActivate->nRtpChannel = m_pData->GetChannel();
    if (pActivate->nRtpChannel == -1)
    {
        LOG_DEBUG("%d No available RTP channel", m_RouteResult.chProcess);
        m_RouteResult.bCreateAudioJob = false;
        return false;
    }
    CService* pSer = CService::GetInstance();
    m_nRtpChannel = pActivate->nRtpChannel;

    if (m_pActivate->type == ACTIVATE_PLAY)
    {
        CActivatePlay* p = (CActivatePlay*)pActivate;
        CAudioPlay* Job = new CAudioPlay(LOCAL_ADP2NET);  //checked

        Job->EnableVLAN(pSer->m_bVLANOpen);
        Job->SwitchVLANNet(pSer->m_VLANSock,pSer->m_strVLANNet);

        Job->SetRepeatCount(p->nRepeatCount);
        Job->SetChProcess(p->chProcess);
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
        CAudioCapture* Job = new CAudioCapture(CAPTURE_NET_ANNOUNCEMENT, pActivate->nPort);  //checked

        Job->EnableVLAN(pSer->m_bVLANOpen);
        Job->SwitchVLANNet(pSer->m_VLANSock,pSer->m_strVLANNet);

        Job->SetChProcess(m_pActivate->chProcess);
        t_FILEMAP file;
        if(pSer->GetFileMAP(m_pActivate-, m_pActivate->nGongTitle,file))
        {
            Job->SetPreSignalFile(file);
        }
        if(pSer->m_bCCCF)
        {
            if(pActivate->Priority <= 20 && pActivate->nPort == 41)
            {
                Job->SetVARecord(true);
            }
        }

        float fScale = CCommon::dB2FloatScale((m_pActivate->Level) / 10.0f);
        Job->VolumeAjust(fScale);
        Job->VolumeAjustSG(pSer->m_VolumeScaleSG);
        Job->SetRtpNode(pSer->GetNode());
        Job->SetRtpChannel(pActivate->nRtpChannel);
        Job->OpenGongAudio();
        m_pData->AddAudio(Job);
    }
    */
    return true;
}

bool CRouteLocal::ResetInput(CONSTATE conState)
{
    if (m_nPort == -1)
    {
        return false;
    }

	if (conState == CON_FULL_CONNECT || conState == CON_PARTLY_CONNECT)
	{
		if (!m_bInputConnect)
		{
			//interrupt conflict input port
			m_pData->InterruptInput(this);

			m_bInputConnect = true;
            CActivateMicr* p = (CActivateMicr*)m_pActivate;
			m_pData->AllocatePort(m_RouteResult.chProcess,
                true, m_nPort, m_pActivate->Priority,p->nRtpChannel);
            return true;
		}
	}
	else
	{
		if (m_bInputConnect)
		{
			m_bInputConnect = false;
			m_pData->FreePort(m_RouteResult.chProcess, true, m_nPort);

            auto pMsg = std::make_shared<CMsg>();
            pMsg->type = MSG_RESTORE_INPUT;
            CService::GetInstance()->Push(pMsg);
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
        LOG_DEBUG("%d Local RouteState: %s",m_RouteResult.chProcess,
            GetState(m_RouteResult.e_ConState).c_str());

        CService::GetInstance()->ExcuteCallback(&m_RouteResult);
    }
    if (ResetInput(conState))
    {
        CService::GetInstance()->SendBusyState();
    }
}

void CRouteLocal::InterruptInput()
{
    if (m_nPort==-1)
    {
        return;
    }

    m_bInputInterrupt = true;
    LOG_DEBUG("%d Interrupt Input",m_RouteResult.chProcess);

    if (IsReconnect())
    {
        for (unsigned int i = 0; i < m_vDest.size(); i++)
        {
            if(m_vDest[i].ret.e_ConState == CON_FULL_CONNECT
                || m_vDest[i].ret.e_ConState == CON_PARTLY_CONNECT)
            {
                InterruptNode(m_vDest[i].Sys);
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
    if(m_nPort == -1 || m_bInputInterrupt == false)
    {
        return false;
    }
    if (!CService::GetInstance()->IsPortCanUsed(true,
        m_nPort, m_pActivate->Priority))
    {
        return false;
    }

    LOG_DEBUG("%d Restore Input,port %d",m_RouteResult.chProcess,m_nPort);

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
    	if(m_vDest[i].Sys != nNode)
    	{
    		ReActivateNode(m_vDest[i].Sys);
    	}
    }
}

void CRouteLocal::EverySecond()
{
    CRoute::EverySecond();
    
    if (IsDisConnect())
        return;

    //check route
    for (unsigned int i = 0; i < m_vDest.size(); i++)
    {
        if (!m_vDest[i].bActivate)
        {
            continue;
        }

        m_vDest[i].nNoCheckSeconds++;
        if (m_vDest[i].nNoCheckSeconds >= 30)
        {
            m_vDest[i].nNoCheckSeconds = 0;
            m_vDest[i].bActivate = false;
            CActivateResult ret = m_vDest[i].ret;
            ret.e_ConState = CON_DIS_CONNECT;
            OnRouteState(m_vDest[i].Sys, ret);
            LOG_DEBUG("%d no check for 30 seconds", m_RouteResult.chProcess);
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
