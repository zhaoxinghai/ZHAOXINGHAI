
#include "route.h"
#include "service.h"
#include "routemanager.h"
#include "msgconnection.h"
#include "msgconnectrequest.h"
#include "msgcheckroute.h"
#include "public.h"
#include "common.h"

CRoute::CRoute(CActivate *pA)
{
    m_pData = &CService::GetInstance()->m_routeManager;
    m_pActivate = NULL;
    if (pA->type == ACTIVATE_NORMAL)
    {
        m_pActivate = new CActivate(pA);
    }
    else if (pA->type == ACTIVATE_MICR)
    {
        m_pActivate = new CActivateMicr((CActivateMicr*)pA);
    }
    else if (pA->type == ACTIVATE_PLAY)
    {
        m_pActivate = new CActivatePlay((CActivatePlay*)pA);
    }
    m_eType = ROUTE_NETWORK;
    m_RouteResult.RetVal = 0;
    m_RouteResult.chProcess = pA->chProcess;
    m_RouteResult.e_ConState = CON_UNKNOW;
    m_bJobFinish = false;

    m_nCalltime = 0;
    m_bPause = true;
    m_bRtpOnly = false;
    m_bJobStarted = false;
    m_nPort = -1;
    m_nRtpChannel = 0;
    m_nRtpNode = -1;
    m_lStartedTime = 0;
}

CRoute::~CRoute()
{
    if(m_pActivate)
    {
        delete m_pActivate;
        m_pActivate = NULL;
    }
}

void CRoute::DeActivate()
{
    for (unsigned int i = 0; i < m_vDest.size(); i++)
    {
        if (m_vDest[i].ret.e_ConState == CON_DIS_CONNECT)
        {
            continue;
        }
        CTSystem *pSystem = CService::GetInstance()->GetD1System(m_vDest[i].Sys);
        if (pSystem == NULL || !pSystem->IsConnect())
        {
            continue;
        }

        LOG_DEBUG("%d DeActivate Node %d",m_RouteResult.chProcess,m_vDest[i].Sys);

        CDeActivate de;
        de.chRequest = m_vDest[i].ret.chRequest;
        de.chProcess = m_vDest[i].ret.chProcess;

        m_vDest[i].bRequest = true;
        m_vDest[i].nRequestSeconds = 0;
        m_vDest[i].eRequest = CON_DEACTIVATE;

        CMsgConnection Connect(pSystem);
        Connect.DeActivate(&de);
    }
}

void CRoute::Activate()
{
    for (unsigned int i = 0; i < m_vDest.size(); i++)
    {
        ActivateNode(m_vDest[i].Sys);
    }
}

bool CRoute::UpdateJob(CONSTATE &conState)
{
    if (conState == CON_FULL_CONNECT || conState == CON_PARTLY_CONNECT)
    {
        if(!m_bJobStarted)
        {
            m_bJobStarted = true;
            m_lStartedTime = CCommon::GetSysTimeMicros();
        }
        m_bPause = false;  //for timeout

        CAudioJob* pJob = m_pData->GetAudio(m_RouteResult.chProcess);
        if(pJob == NULL)
        {
            return false;
        }
        if(!pJob->IsRunning()
                && pJob->m_className!="CAudioReceive"
                && m_pData->GetCapturePlayAudio()>=THREAD_POOL_COUNT)
        {
            LOG_DEBUG("%d thread pool is full.",m_RouteResult.chProcess);
            m_pData->StopAudio(m_RouteResult.chProcess);
            return false;
        }
        else
        {
            m_pData->StartAudio(m_RouteResult.chProcess);
        }
    }
    else if (conState == CON_DIS_CONNECT)
    {
        m_pData->StopAudio(m_RouteResult.chProcess);
    }
    else
    {
        m_bPause = true;  //for timeout
        m_pData->PauseAudio(m_RouteResult.chProcess);
    }
    return true;
}

bool CRoute::IsReconnect()
{
    if (IS_RECONNECT(m_pActivate->Flags))
    {
        return true;
    }
    return false;
}

bool CRoute::IsReconnectRequest()
{
	if (IS_RECONNECT_REQUEST(m_pActivate->Flags))
	{
		return true;
	}
	return false;
}

bool CRoute::IsPartlyMode()
{
    if (IS_PARTIAL_CONNECTION(m_pActivate->Flags))
    {
        return true;
    }
    return false;
}

bool CRoute::IsFullMode()
{
    return !IsPartlyMode();
}

void CRoute::EverySecond()
{
    if (IsDisConnect())
        return;

    //check timeout
	if (m_pActivate->TimeOut > 0 )
	{
		m_nCalltime++;
		if(m_nCalltime >= m_pActivate->TimeOut)
		{
			//2, do disconnect (timeout)
			OnRouteState(CON_DIS_CONNECT);
			return;
		}
	}

    //try activate,reactivate and interrupt
    for (unsigned int i = 0; i < m_vDest.size(); i++)
    {
        t_AudioDestRoute &dst = m_vDest[i];
        if (dst.bRequest)
        {
            dst.nRequestSeconds++;
            if (dst.nRequestSeconds >= 5)
            {
                //activate again
                if (!dst.bActivate)
                {
                    ActivateNode(dst.Sys);
                }
                else if (dst.eRequest == CON_REACTIVATE)
                {
                    if(dst.ret.e_ConState==CON_FULL_CONNECT
                            || dst.ret.e_ConState==CON_PARTLY_CONNECT)
                    {
                        dst.bRequest = false;
                    }
                    else
                    {
                        ReActivateNode(dst.Sys);
                    }
                }
                else if(dst.eRequest == CON_INTERRUPT)
                {
                    if(dst.ret.e_ConState==CON_INTERRUPTED)
                    {
                        dst.bRequest = false;
                    }
                    else
                    {
                        InterruptNode(dst.Sys);
                    }
                }
                else
                {

                }
            }
        }
    }
}

bool CRoute::IsDisConnect()
{
    if (m_RouteResult.e_ConState == CON_DIS_CONNECT)
        return true;

    return false;
}

bool CRoute::IsRtpOnly()
{
    return m_bRtpOnly;
}

int CRoute::GetRtpNode()
{
    return m_nRtpNode;
}

int CRoute::GetRtpChannel()
{
    return m_nRtpChannel;
}

e_ROUTE_TYPE CRoute::GetType()
{
    return m_eType;
}

void CRoute::BeCheckRoute(unsigned short chRequest)
{
    for (unsigned int i = 0; i < m_vDest.size(); i++)
    {
        if (m_vDest[i].ret.chRequest == chRequest)
        {
            m_vDest[i].nNoCheckSeconds = 0;
            break;
        }
    }
}

t_AudioDestRoute* CRoute::GetDestination(int nNode)
{
	for (unsigned int i = 0; i < m_vDest.size(); i++)
	{
		if (m_vDest[i].Sys == nNode)
		{
			return &m_vDest[i];
		}
	}
	return NULL;
}

void CRoute::UpdateResult(int nNode, CActivateResult &ret, e_RESPONE response)
{
    e_REQUEST request = CON_ACTIVATE;
    switch (response)
    {
    case CON_ON_ACTIVATE:
        request = CON_ACTIVATE;
        break;
    case CON_ON_DEACTIVATE:
        request = CON_DEACTIVATE;
        break;
    case CON_ON_REACTIVATE:
        request = CON_REACTIVATE;
        break;
    case CON_ON_INTERRUPT:
        request = CON_INTERRUPT;
        break;
    case CON_ON_ROUTESTATE:
        request = CON_ROUTESTATE;
        break;
    case CON_ON_CHECKROUTE:
        request = CON_CHECKROUTE;
        break;
    }
    
	for (unsigned int i = 0; i < m_vDest.size(); i++)
	{
        if (m_vDest[i].Sys == nNode
                && m_vDest[i].ret.chRequest == ret.chRequest)
		{
            m_vDest[i].ret.chProcess = ret.chProcess;
            m_vDest[i].ret.RetVal = ret.RetVal;
            m_vDest[i].ret.e_ConState = ret.e_ConState;
            m_vDest[i].ret.SrcIdx = ret.SrcIdx;
            m_vDest[i].ret.LastSrcIdx = ret.LastSrcIdx;
			m_vDest[i].bActivate = true;

            //stop request
            if (nNode == m_vDest[i].Sys
                && m_vDest[i].bRequest
                && m_vDest[i].eRequest == request)
            {
                m_vDest[i].bRequest = false; 
            }
            break;
		}
	}
}

std::string CRoute::GetState(CONSTATE conState)
{
	if (conState == CON_FULL_CONNECT)
    {
        return "FULLCON";
    }
	else if (conState == CON_PARTLY_CONNECT)
    {
        return "PART DEFECT";
    }
	else if (conState == CON_INTERRUPTED)
    {
        return "BREAK";
    }
	else if (conState == CON_DIS_CONNECT)
    {
        return "DISCON";
    }
	else if(conState == CON_RECONNECT_POSSIBLE)
    {
        return "RE_POSSIBLE";
    }
	else
    {
        return "CON_UNKNOW";
    }

}

std::string CRoute::GetPresignal(int nPre)
{
    if(nPre == 1)
    {
        return "SG 001.01";
    }
    else if(nPre == 2)
    {
        return "SG 001.02";
    }
    else if(nPre == 3)
    {
        return "SG 001.03";
    }
    else
    {
        return "";
    }
}

void CRoute::GetSource(std::string &strPre,std::string &strSrc,bool bStart)
{
    /*
    if(m_pActivate->nGongTitle == 1)
    {
        strPre = "SG 001.01";
    }
    else if(m_pActivate->nGongTitle == 2)
    {
        strPre = "SG 001.02";
    }
    else if(m_pActivate->nGongTitle == 3)
    {
        strPre = "SG 001.03";
    }
    if(m_eType == ROUTE_REMOTE)
    {
        if(m_nPort == 41)
        {
            strSrc = "PR 41.1";
        }
        else if(m_nPort == 42)
        {
            strSrc = "PR 41.2";
        }
        else
        {
            strSrc = CCommon::StrFormat("PR %d.0",m_nRtpChannel);
        }
    }
    else if(m_eType == ROUTE_LOCAL)
    {
        if(m_nPort == 41)
        {
            strSrc = "PR 41.1";
        }
        else if(m_nPort == 42)
        {
            strSrc = "PR 41.2";
        }
        else
        {
            strSrc = CCommon::StrFormat("PR %d.0",m_nRtpChannel);
        }
    }
    else
    {
        strPre = "";
        strSrc = "";
    }
    if(strSrc.length()>0 && !bStart)
    {
        strSrc = CCommon::StrFormat("[%s]",strSrc.c_str());
    }
    */
}

void CRoute::GetDest(std::string szDest[])
{
    if(IsRtpOnly())
    {
        GetDestRtp(szDest);
    }
    else
    {
        GetDestNet(szDest);
    }
}

void CRoute::GetDestRtp(std::string szDest[])
{
    CRouteRemote* p = (CRouteRemote*)this;
    int index = 0;
    for(unsigned int i = 0;i<p->m_vOutput.size();i++)
    {
        t_Output &output = p->m_vOutput[i];
        if(index >= 5)
            break;

        if(output.nPort == 41)
        {
            if(output.bConnect)
            {
                szDest[index++] = "lo PA 041.01";
            }
            else
            {
                szDest[index++] = "[lo PA 041.01]";
            }
        }
        else if(output.nPort == 42)
        {
            if(output.bConnect)
            {
                szDest[index++] = "lo PA 042.01";
            }
            else
            {
                szDest[index++] = "[lo PA 042.01]";
            }
        }
    }
}

void CRoute::GetDestNet(std::string szDest[])
{
    for(unsigned int i =0;i<m_vDest.size();i++)
    {
        if(i >= 5)
        {
            break;
        }
        std::string str;
        t_AudioDestRoute &dst = m_vDest[i];

        if(dst.bActivate)
        {
            str = CCommon::StrFormat("%d %s",dst.Sys,
                    CRoute::GetState(dst.ret.e_ConState).c_str());
        }
        else
        {
            str = CCommon::StrFormat("%d ---",dst.Sys);
        }
        szDest[i] = CCommon::StrFormat("%-17s",str.c_str());
    }
}

void CRoute::ActivateNode(int nNode)
{
    t_AudioDestRoute* pDest = GetDestination(nNode);
    if(pDest == NULL || pDest->bDeActivate || pDest->bActivate)
    {
        return;
    }
    pDest->bRequest = true;
    pDest->nRequestSeconds = 0;
    pDest->eRequest = CON_ACTIVATE;

    if(pDest->vOutputPort.size()==0)
    {
		LOG_DEBUG("%s","ActivateNode vOutputPort is empty");
        return;
    }
    CTSystem *pSystem = CService::GetInstance()->GetD1System(nNode);
    if (pSystem == NULL || !pSystem->IsConnect())
    {
        return;
    }

    m_pActivate->vDest.clear();
    m_pActivate->vDest.push_back(*pDest);
    pDest->ret.chRequest = CPublic::GetChRequest();
    m_pActivate->chRequest = pDest->ret.chRequest;

    CMsgConnection Connect(pSystem);
    Connect.Activate(m_pActivate);

    LOG_DEBUG("%d Activate Node %d",m_RouteResult.chProcess,nNode);
}

void CRoute::InterruptNode(int nNode)
{
    t_AudioDestRoute* pDest = GetDestination(nNode);
    if (pDest==NULL || !pDest->bActivate || pDest->bDeActivate)
    {
        return;
    }
    pDest->bRequest = true;
    pDest->nRequestSeconds = 0;
    pDest->eRequest = CON_INTERRUPT;

    CTSystem *pSystem = CService::GetInstance()->GetD1System(nNode);
    if (pSystem == NULL || !pSystem->IsConnect())
    {
        return;
    }

    CDeActivate in;
    in.chRequest = pDest->ret.chRequest;
    in.chProcess = pDest->ret.chProcess;
    CMsgConnection Connect(pSystem);
    Connect.Interrupt(&in);

    LOG_DEBUG("%d Interrupt Node %d",m_RouteResult.chProcess,nNode);
}

void CRoute::DeActivateNode(int nNode)
{
    t_AudioDestRoute* pDest = GetDestination(nNode);
    if (pDest==NULL || !pDest->bActivate || pDest->bDeActivate)
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

    CTSystem *pSystem = CService::GetInstance()->GetD1System(nNode);
    if (pSystem == NULL)
    {
        return;
    }

    CDeActivate de;
    de.chRequest = pDest->ret.chRequest;
    de.chProcess = pDest->ret.chProcess;
    CMsgConnection Connect(pSystem);
    Connect.DeActivate(&de);

    LOG_DEBUG("%d DeActivate Node %d",m_RouteResult.chProcess,nNode);
}

void CRoute::ReActivateNode(int nNode)
{
    t_AudioDestRoute* pDest = GetDestination(nNode);
    if(pDest==NULL || !pDest->bActivate || pDest->bDeActivate)
    {
        return;
    }
    pDest->bRequest = true;
    pDest->nRequestSeconds = 0;
    pDest->eRequest = CON_REACTIVATE;

    CTSystem *pSystem = CService::GetInstance()->GetD1System(nNode);
    if (pSystem == NULL || !pSystem->IsConnect())
    {
        return;
    }
    CActivateMicr* pA = (CActivateMicr*)m_pActivate;
    CReActivate re;
    re.chRequest = pDest->ret.chRequest;
    re.chProcess = pDest->ret.chProcess;
    re.nRtpNode = pA->nRtpNode;
    re.nRtpChannel = pA->nRtpChannel;
    CMsgConnection Connect(pSystem);
    Connect.ReActivate(&re);

    LOG_DEBUG("%d ReActivate Node %d",m_RouteResult.chProcess,nNode);
}
