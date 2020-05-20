
#include "routenetwork.h"
#include "service.h"
#include "routemanager.h"
#include "msgconnection.h"
#include "msgconnectrequest.h"
#include "msgcheckroute.h"
#include "public.h"
#include "common.h"
#include "sdkcallback.h"
#include "sdk.h"

CRouteNetwork::CRouteNetwork(int nNode,CAnnouncement* pA)
    :CRoute(pA)
{
    m_pNetSystem = g_SDKServer.GetD1System(nNode);
    m_eType = ROUTE_NETWORK;
    m_bPresignal = false;

    /*
    //source(gong)
    if (m_pActivate->nGongChannel > 0 && m_pActivate->nGongTitle>0)
    {
        t_AudioSrc src;
        src.Num = m_pActivate->nGongChannel-1;
        src.Title = m_pActivate->nGongTitle - 1;
        m_pActivate->vSrc.insert(m_pActivate->vSrc.begin(), src);
    }
    */
	//one destination
    t_DestinationRoute dest;
	dest.nNode = nNode;
	m_vDest.push_back(dest);
}

CRouteNetwork::~CRouteNetwork()
{
}

void CRouteNetwork::ActivateNode(int nNode)
{
    m_vDest[0].bRequest = true;
    m_vDest[0].nRequestSeconds = 0;
    m_vDest[0].eRequest = CON_ACTIVATE;

    if (!m_pNetSystem->IsConnect())
    {
        return;
    }

    m_vDest[0].ret.nRequest = CPublic::GetChRequest();
    CMsgConnection Connect(m_pNetSystem);
    Connect.Activate(m_Activate);
}

void CRouteNetwork::OnRouteState(CONSTATE conState)
{
    if(m_RouteResult.e_ConState != conState)
    {
        LOG_DEBUG("%d Network OnRouteState   %s",
              m_RouteResult.nProcess,GetState(conState).c_str());

        m_RouteResult.e_ConState = conState;
        //m_RouteResult.nRequest = m_vDest[0].ret.nRequest;

        CAnnouncementResult ret;
        g_SDKServer.ExcuteCallback(&ret);
    }

    //current source index
    if(conState == CON_FULL_CONNECT || conState == CON_PARTLY_CONNECT)
    {
        CPlayIndexResult ret;
        ret.nRequest = m_RouteResult.nProcess;
        ret.nCurrentRepeat = m_RouteResult.nRepeatIndex;
        ret.nPlayIndex = m_RouteResult.nPlayIndex;

        g_SDKServer.ExcuteCallback(&ret);

        //gong signal
        //NetworkGongSignal(ret.nSourceIndex);
    }
}

void CRouteNetwork::NetworkGongSignal(int nSourceIndex)
{
    /*
    if(m_pActivate != NULL && m_pActivate->IsPresignal(nSourceIndex))
    {
        if(!m_bPresignal)
        {
            CPresignalResult ret;
            ret.nProcess = m_RouteResult.nProcess;
            ret.bPlaying = true;
            m_bPresignal = true;
            g_SDKServer.ExcuteCallback(&ret);
        }
    }
    else
    {
        if(m_bPresignal)
        {
            CPresignalResult ret;
            ret.nProcess = m_RouteResult.nProcess;
            ret.bPlaying = false;
            m_bPresignal = false;
            g_SDKServer.ExcuteCallback(&ret);
        }
    }
    */
}

void CRouteNetwork::EverySecond()
{
    CRoute::EverySecond();

    if (IsDisConnect())
    {
        return;
    }
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

            /*
            CAnnouncementResult ret = m_vDest[i].ret;
            ret.e_ConState = CON_DIS_CONNECT;
            OnRouteState(m_vDest[i].Sys, ret);
            LOG_DEBUG("%d network %d no check for 30 seconds",
                      m_RouteResult.nProcess,m_vDest[0].Sys);*/
        }
    }
}
