
#include "route.h"
#include "service.h"
#include "routemanager.h"
#include "msgconnection.h"
#include "msgconnectrequest.h"
#include "msgcheckroute.h"
#include "public.h"
#include "common.h"
#include "sdkconnect.h"

CRoute::CRoute(CAnnouncement *pA)
{
    m_Activate.Property = pA->Property;

    //some audio destinations
    for (unsigned int i = 0; i < m_Activate.vDest.size(); i++)
    {
        t_DestinationRoute dest(m_Activate.vDest[i]);
        m_vDest.push_back(dest);
    }
    m_eType = ROUTE_NETWORK;
    m_RouteResult.nProcess = m_Activate.nProcess;
    m_RouteResult.e_ConState = CON_UNKNOW;
    m_bJobFinish = false;

    m_nCalltime = 0;
    m_bJobStarted = false;
    m_lStartedTime = 0;
}

CRoute::~CRoute()
{
}

void CRoute::OnActivate(int nNode, t_ActivateRet ret)
{

}

void CRoute::OnDeActivate(int nNode, t_ActivateRet ret)
{
    assert(0);
}

void CRoute::OnInterrupt(int nNode, t_ActivateRet ret)
{
    assert(0);
}

void CRoute::OnReActivate(int nNode, t_ActivateRet ret)
{
    assert(0);
}

void CRoute::DeActivate()
{
    for (unsigned int i = 0; i < m_vDest.size(); i++)
    {
        if (m_vDest[i].ret.e_ConState == CON_DIS_CONNECT)
        {
            continue;
        }
        CTSystem *pSystem = g_SDKServer.GetD1System(m_vDest[i].nNode);
        if (pSystem == NULL)
        {
            LOG_DEBUG("%d DeActivate Node %d", m_RouteResult.nProcess, m_vDest[i].nNode);

            m_vDest[i].bRequest = true;
            m_vDest[i].nRequestSeconds = 0;
            m_vDest[i].eRequest = CON_DEACTIVATE;

            CMsgConnection Connect(pSystem);
            Connect.DeActivate(m_vDest[i].ret.nRequest, m_vDest[i].ret.nProcess);
        }
    }
}

void CRoute::Activate()
{
    for (unsigned int i = 0; i < m_vDest.size(); i++)
    {
        ActivateNode(m_vDest[i].nNode);
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
        //m_bPause = false;  //for timeout

        CAudioJob* pJob = g_SDKServer.m_routeManager.GetAudio(m_RouteResult.nProcess);
        if(pJob == NULL)
        {
            return false;
        }
        if(!pJob->IsRunning()
                && pJob->m_className!="CAudioReceive"
                && g_SDKServer.m_routeManager.GetCapturePlayAudio()>=THREAD_POOL_COUNT)
        {
            LOG_DEBUG("%d thread pool is full.",m_RouteResult.nProcess);
            g_SDKServer.m_routeManager.StopAudio(m_RouteResult.nProcess);
            return false;
        }
        else
        {
            g_SDKServer.m_routeManager.StartAudio(m_RouteResult.nProcess);
        }
    }
    else if (conState == CON_DIS_CONNECT)
    {
        g_SDKServer.m_routeManager.StopAudio(m_RouteResult.nProcess);
    }
    else
    {
        g_SDKServer.m_routeManager.PauseAudio(m_RouteResult.nProcess);
    }
    return true;
}

bool CRoute::IsReconnect()
{
    if (IS_RECONNECT(m_Activate.Property.nFlags))
    {
        return true;
    }
    return false;
}

bool CRoute::IsPartMode()
{
    if (IS_PART_CONNECT(m_Activate.Property.nFlags))
    {
        return true;
    }
    return false;
}

bool CRoute::IsFullMode()
{
    return !IsPartMode();
}

void CRoute::EverySecond()
{
    //check timeout
	if (m_Activate.Property.nTimeOut > 0 && m_bJobStarted)
	{
		m_nCalltime++;
		if(m_nCalltime >= m_Activate.Property.nTimeOut)
		{
			//2, do disconnect (timeout)
			OnRouteState(CON_DIS_CONNECT);
			return;
		}
	}

    //check activate route
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
            t_ActivateRet ret = m_vDest[i].ret;
            ret.e_ConState = CON_DIS_CONNECT;
            OnRouteState(m_vDest[i].nNode, ret);
            LOG_DEBUG("%d no check for 30 seconds", m_RouteResult.nProcess);
        }
    }
}

void CRoute::OnRouteState(int nNode, t_ActivateRet ret)
{
    assert(0);
}

void CRoute::OnRouteState(CONSTATE conState)
{
    assert(0);
}

bool CRoute::IsDisConnect()
{
    if (m_RouteResult.e_ConState == CON_DIS_CONNECT)
        return true;

    return false;
}

bool CRoute::IsRtpOnly()
{
    return m_Activate.Property.bAudioStream;
}

e_ROUTE_TYPE CRoute::GetType()
{
    return m_eType;
}

void CRoute::BeCheckRoute(int nRequest)
{
    for (unsigned int i = 0; i < m_vDest.size(); i++)
    {
        if (m_vDest[i].ret.nRequest == nRequest)
        {
            m_vDest[i].nNoCheckSeconds = 0;
            break;
        }
    }
}

t_DestinationRoute* CRoute::GetDestination(int nNode)
{
	for (unsigned int i = 0; i < m_vDest.size(); i++)
	{
		if (m_vDest[i].nNode == nNode)
		{
			return &m_vDest[i];
		}
	}
    assert(0);
	return NULL;
}

void CRoute::UpdateResult(int nNode, t_ActivateRet ret, int response)
{
    e_CONNECT_REQUEST request = CON_ACTIVATE;
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
        if (m_vDest[i].nNode == nNode && m_vDest[i].ret.nRequest == ret.nRequest)
		{
            m_vDest[i].ret = ret;
			m_vDest[i].bActivate = true;

            //stop this request
            if (m_vDest[i].bRequest && m_vDest[i].eRequest == request)
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
        return "PARTCON";
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

void CRoute::ActivateNode(int nNode)
{
    t_DestinationRoute* pDest = GetDestination(nNode);
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
    CTSystem *pSystem = g_SDKServer.GetD1System(nNode);
    if (pSystem)
    {
        CAnnouncement act = m_Activate;
        act.vDest.clear();
        act.vDest.push_back(*pDest);

        pDest->ret.nRequest = CPublic::GetChRequest();
      
        CMsgConnection Connect(pSystem);
        Connect.Activate(act);

        LOG_DEBUG("%d Activate Node %d", m_RouteResult.nProcess, nNode);
    }
}
