
#include "msgconnectrequest.h"
#include "sdkdefine.h"
#include "service.h"
#include "sdk.h"
#include "audiocapture.h"
#include "mylog.h"
#include "msgcheckroute.h"
#include "msgbusystate.h"
#include "common.h"
#include "msgconnection.h"
#include "routeremote.h"
#include "public.h"

CMsgConnectRequest::CMsgConnectRequest(CTSystem* pSys):CMsgBase(pSys)
{
    m_chProcess = 0;
    m_Mid = PAMSG_MID_CONNECT;
}

void CMsgConnectRequest::OnRespond(unsigned char* pBuf, const int len)
{
    m_SubMid = pBuf[0];

    switch (m_SubMid)
    {
    case CON_ACTIVATE:
    {
        Activate(pBuf, len);
        break;
    }
    case CON_DEACTIVATE:
    {
        DeActivate(pBuf,len);
        break;
    }
    case CON_REACTIVATE:
    {
        ReActivate(pBuf, len);
    	break;
    }
    case CON_INTERRUPT:
    {
        Interrupt(pBuf, len);
    	break;
    }
    case CON_ROUTESTATE:
    {
        RouteStatus(pBuf, len);
    	break;
    }
    case CON_CHECKROUTE:
    {
        CheckRoute(pBuf, len);
        break;
    }
    default:
    {
        LOG_DEBUG("CMsgConnectRequest %d", m_SubMid);
    }
    }
    g_SDKServer.m_routeManager.CleanRoute();
}

void CMsgConnectRequest::Activate(unsigned char * pBuf, const int len)
{
    unsigned char* p = pBuf;

    CAnnouncement act;
    act.nRequest = ReadLONG(p);p += 4;
    act.Property.nPriority = ReadLONG(p);p += 4;
    act.Property.nFlags = ReadLONG(p);p += 4;
    act.Property.nTimeOut = ReadLONG(p);p += 4;
    act.Property.nVolume = ReadLONG(p);p += 4;
    act.Property.nTriggleNode = ReadLONG(p);p += 4;
    act.Property.nTriggleType = ReadLONG(p);p += 4;
    act.Property.nTriggleNumber = ReadLONG(p);p += 4;
    act.Property.bAudioStream = ReadLONG(p) == 1 ? true : false;
    p += 4;

    if (act.Property.bAudioStream)
    {
        act.Property.nAudioNode = ReadLONG(p);p += 4;
        act.Property.nAudioChannal = ReadLONG(p);p += 4;
    }
    else
    {
        int nSrcNum = ReadLONG(p);p += 4;
        for (int i = 0; i < nSrcNum; i++)
        {
            t_Source src;
            src.eType = (e_SOURCE_TYPE)ReadLONG(p);p += 4;
            src.nNumber = ReadLONG(p);p += 4;
            act.vSrc.push_back(src);
        }
    }
    //read destination
    int nDestNum = ReadLONG(p);p += 4;
    for (int i = 0; i < nDestNum; i++)
    {
        t_Destination dest;
        dest.nNode = ReadLONG(p); p += 4;

        //max 32 bytes to represent 256 destinations
        unsigned char Bitarray[32] = { 0 };
        memcpy(&Bitarray[0], p, 32);
        p += 32;

        Change32BytetoVector(Bitarray, dest.vOutputPort);
        act.vDest.push_back(dest);
    }
    CRouteManager &Mgr = g_SDKServer.m_routeManager;
    CRouteRemote* pRoute = Mgr.GetRemoteRoute(m_pSystem->GetNode(), act.nRequest);
    if(pRoute != NULL)
    {
        //pRoute->SendRouteStatus();
        return;
    }

    act.nProcess = CPublic::GetChProcess();
    
    int nNode = m_pSystem->GetNode();
    pRoute = new CRouteRemote(nNode, &act);
    g_SDKServer.m_routeManager.AddRoute(pRoute);
    pRoute->BeActivate();
}

void CMsgConnectRequest::DeActivate(unsigned char * pBuf, const int /*len*/)
{
    int nRequest = ReadLONG(&pBuf[1]);
    int nProcess = ReadLONG(&pBuf[3]);

    CRouteManager &Mgr = g_SDKServer.m_routeManager;
    CRoute* pRoute = Mgr.GetRouteByProcessRequest(nProcess,nRequest);
    if (pRoute != NULL && pRoute->GetType() == ROUTE_REMOTE)
    {
        CRouteRemote* p = (CRouteRemote*)pRoute;
        p->BeDeActivate();
        p->OnRouteState(CON_DIS_CONNECT);
        Mgr.CleanRoute();
    }
}

void CMsgConnectRequest::Interrupt(unsigned char* pBuf, const int /*len*/)
{
    //chrequest
    int nRequest = ReadLONG(&pBuf[1]);
    int nProcess = ReadLONG(&pBuf[3]);

    CRouteManager &Mgr = g_SDKServer.m_routeManager;
    CRoute* pRoute = Mgr.GetRouteByProcessRequest(nProcess,nRequest);
    if (pRoute != NULL)
    {
        CRouteRemote* p = (CRouteRemote*)pRoute;
        p->BeInterrupt();
    }
}

void CMsgConnectRequest::CheckRoute(unsigned char* pBuf, const int /*len*/)
{
    int nRequest = ReadLONG(pBuf + 1);
    int nProcess = ReadLONG(pBuf + 3);

    CRouteManager &Mgr = g_SDKServer.m_routeManager;
    CRoute *pRoute = Mgr.GetRouteByRequest(nRequest);
    if (pRoute != NULL && !pRoute->IsDisConnect())
    {
        pRoute->BeCheckRoute(nRequest);

        CMsgCheckRoute check(m_pSystem);
        check.ReplayCheck(nRequest, nProcess, true);
    }
    else
    {
        CMsgCheckRoute check(m_pSystem);
        check.ReplayCheck(nRequest, nProcess, false);
    }
}

void CMsgConnectRequest::ReActivate(unsigned char* pBuf, const int len)
{
    //chrequest
    int nRequest = ReadLONG(&pBuf[1]);
    int nProcess = ReadLONG(&pBuf[3]);

    CRouteManager &Mgr = g_SDKServer.m_routeManager;
    CRoute* pRoute = Mgr.GetRouteByProcessRequest(nProcess,nRequest);
    if (pRoute != NULL)
    {
        CRouteRemote* pRo = (CRouteRemote*)pRoute;
        pRo->BeReActivate();
    }
}

void CMsgConnectRequest::RouteStatus(unsigned char* pBuf, const int /*len*/)
{
    //request
    int nRequest = ReadLONG(&pBuf[1]);
    int nProcess = ReadLONG(&pBuf[3]);

    CRouteManager &Mgr = g_SDKServer.m_routeManager;
    CRoute* pRoute = Mgr.GetRouteByProcessRequest(nProcess,nRequest);
    if (pRoute != NULL)
    {
        CRouteRemote* p = (CRouteRemote*)pRoute;
        p->BeRouteStatus();
    }
}

void CMsgConnectRequest::SendRouteStatus(t_ActivateRet ret, e_CONNECT_RESPONE eRespone)
{
    m_vBuffer.clear();

    //submid
    WriteLONG(eRespone);

    //request
    WriteLONG(ret.nRequest);

    //process
    WriteLONG(ret.nProcess);

    //conState
    WriteLONG(ret.e_ConState);

    //repeat index
    WriteLONG(ret.nRepeatIndex);

    //playing index
    WriteLONG(ret.nPlayIndex);

    m_pSystem->OnSend(this);
}
