
#include "route.h"
#include "service.h"
#include "routemanager.h"
#include "msgconnection.h"
#include "msgconnectrequest.h"
#include "msgcheckroute.h"
#include "public.h"
#include "common.h"
#include "callback.h"
#include "sdk.h"

CRouteNetwork::CRouteNetwork(int nNode,CActivate* pA):CRoute(pA)
{
    /*
    m_pNetSystem = CService::GetInstance()->GetD1System(nNode);
    m_eType = ROUTE_NETWORK;
    m_bPresignal = false;

    //source(gong)
    if (m_pActivate->nGongChannel > 0 && m_pActivate->nGongTitle>0)
    {
        t_AudioSrc src;
        src.Num = m_pActivate->nGongChannel-1;
        src.Title = m_pActivate->nGongTitle - 1;
        m_pActivate->vSrc.insert(m_pActivate->vSrc.begin(), src);
    }

	//one destination
    t_AudioDestRoute dest;
	dest.Sys = nNode;
	m_vDest.push_back(dest);
    */
}

CRouteNetwork::~CRouteNetwork()
{
}

void CRouteNetwork::Activate()
{
    m_vDest[0].bRequest = true;
    m_vDest[0].nRequestSeconds = 0;
    m_vDest[0].eRequest = CON_ACTIVATE;

    if (!m_pNetSystem->IsConnect())
    {
        return;
    }
    m_pActivate->chRequest = CPublic::GetChRequest();
    m_vDest[0].ret.chRequest = m_pActivate->chRequest;
    CMsgConnection Connect(m_pNetSystem);
    Connect.Activate(m_pActivate);
}

void CRouteNetwork::ActivateNode(int nNode)
{
    Activate();
}

void CRouteNetwork::OnActivate(int /*nNode*/, CActivateResult &ret)
{
    m_RouteResult.SrcIdx = ret.SrcIdx;
    OnRouteState(ret.e_ConState);
}

void CRouteNetwork::OnDeActivate(int /*nNode*/,CActivateResult &ret)
{
    m_RouteResult.SrcIdx = ret.SrcIdx;
	OnRouteState(ret.e_ConState);
}

void CRouteNetwork::OnInterrupt(int /*nNode*/, CActivateResult &ret)
{
    m_RouteResult.SrcIdx = ret.SrcIdx;
	OnRouteState(ret.e_ConState);
}

void CRouteNetwork::OnReActivate(int /*nNode*/, CActivateResult &ret)
{
    m_RouteResult.SrcIdx = ret.SrcIdx;
	OnRouteState(ret.e_ConState);
}

void CRouteNetwork::OnRouteState(int /*nNode*/, CActivateResult &ret)
{
    m_RouteResult.SrcIdx = ret.SrcIdx;
	OnRouteState(ret.e_ConState);
}

void CRouteNetwork::OnRouteState(CONSTATE conState)
{
    if(m_RouteResult.e_ConState != conState)
    {
        LOG_DEBUG("%d Network OnRouteState   %s",
              m_RouteResult.chProcess,GetState(conState).c_str());

        m_RouteResult.e_ConState = conState;
        m_RouteResult.chRequest = m_vDest[0].ret.chRequest;
        CService::GetInstance()->ExcuteCallback(&m_RouteResult);
    }

    //current source index
    if(conState == CON_FULL_CONNECT || conState == CON_PARTLY_CONNECT)
    {
        CSourceIndexResult ret;
        ret.chProcess = m_RouteResult.chProcess;
        ret.nSourceIndex = m_RouteResult.SrcIdx;
        CService::GetInstance()->ExcuteCallback(&ret);

        //gong signal
        NetworkGongSignal(ret.nSourceIndex);
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
            ret.chProcess = m_RouteResult.chProcess;
            ret.bPlaying = true;
            m_bPresignal = true;
            CService::GetInstance()->ExcuteCallback(&ret);
        }
    }
    else
    {
        if(m_bPresignal)
        {
            CPresignalResult ret;
            ret.chProcess = m_RouteResult.chProcess;
            ret.bPlaying = false;
            m_bPresignal = false;
            CService::GetInstance()->ExcuteCallback(&ret);
        }
    }
    */
}

void CRouteNetwork::EverySecond()
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
            LOG_DEBUG("%d network %d no check for 30 seconds",
                      m_RouteResult.chProcess,m_vDest[0].Sys);
        }
    }
}
