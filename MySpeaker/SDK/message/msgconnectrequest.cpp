
#include "msgconnectrequest.h"
#include "predefine.h"
#include "service.h"
#include "routemanager.h"
#include "sdk.h"
#include "audiocapture.h"
#include "mylog.h"
#include "msgcheckroute.h"
#include "msgusagereport.h"
#include "common.h"

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
        if (m_Mid == PAMSG_MID_CONNECT)
        {
            Activate10(pBuf, len);
        }
        else
        {
            Activate11(pBuf, len);
        }
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
    CService::GetInstance()->m_routeManager.CleanRoute();
}

void CMsgConnectRequest::Activate10(unsigned char * pBuf, const int len)
{
    if (len < 10)
        return;

    unsigned char* p = pBuf;

    //submid
    m_SubMid = *p;
    p++;

    CActivate ActivateRequest;
    ActivateRequest.chRequest = ReadLONG(p);
    p += 2;

    ActivateRequest.Priority = *p;
    p++;

    ActivateRequest.Flags = ReadLONG(p);
    p += 2;

    ActivateRequest.TimeOut = ReadLONG(p);
    p += 2;

    ActivateRequest.Level = ReadLONG(p);
    p += 2;

    int nSrcNum = *p;
    if (nSrcNum <= 0)
        return;

    p++;

    //read source
    for (int i = 0;i < nSrcNum;i++)
    {
        t_AudioSrc src;
        src.Loop = *p;
        p++;

        src.Nextldx = *p;
        p++;

        src.Num = ReadLONG(p);
        p += 2;

        src.Title = ReadLONG(p);
        p += 2;

        ActivateRequest.vSrc.push_back(src);
    }

    //read destination
    int nDestNum = *p;
    if (nDestNum <= 0)
        return;
    p++;

    for (int i = 0;i < nDestNum;i++)
    {
        t_AudioDest dest;

        dest.Sys = *p;
        p++;

        dest.Type = *p;
        p++;

        int Offset = *p;
        p++;

        int chlen = *p;
        p++;

        if(chlen > 32)
        {
            return;
        }
        //max 32 bytes to represent 256 destinations
        unsigned char Bitarray[32] = {0};
        memcpy(&Bitarray[Offset],p,chlen);
        p += chlen;

        Change32BytetoVector(Bitarray, dest.vOutputPort);

        ActivateRequest.vDest.push_back(dest);
    }

    CRouteManager &Mgr = CService::GetInstance()->m_routeManager;
    if(Mgr.IsRequestExist(m_pSystem->GetNode(),ActivateRequest.chRequest))
    {
        return;
    }
    if(Mgr.IsRequestFull())
    {
        return;
    }

    /*
    ActivateRequest.chProcess = PA::GetChProcess();
    CRouteRemote *pRoute = new CRouteRemote(m_AdrSrc.GetNode(),&ActivateRequest);
    if(pRoute->m_pSystem)
    {
        CService::GetInstance()->m_routeManager.AddRoute(pRoute);
        pRoute->BeActivate();
    }
    else
    {
        delete pRoute;
        pRoute = NULL;
    }
    */
}

void CMsgConnectRequest::Activate11(unsigned char * pBuf, const int len)
{
    unsigned char* p = pBuf;

    //submid
    m_SubMid = *p;
    if (len < 29)
    {
        LOG_ERROR("CMsgConnectRequest::Activate11,len:%d",len);
        return;
    }
    p++;

    //version
    //int version = *p;
    p++;

    CActivate ActivateRequest;
    ActivateRequest.chRequest = ReadLONG(p);
    p += 2;

    ActivateRequest.Priority = *p;
    p++;

    ActivateRequest.Flags = ReadLONG(p);
    p += 4;

    ActivateRequest.TimeOut = ReadLONG(p);
    p += 2;

    ActivateRequest.Level = ReadLONG(p);
    p += 2;

    ActivateRequest.nTriggleNode = ReadLONG(p);
    p += 2;

    ActivateRequest.nTriggleType = ReadLONG(p);
    p +=2;

    ActivateRequest.nTriggleNumber = ReadLONG(p);
    p +=2;

    //Sechash
    p += 8;

    int nSrcNum = *p;
    if (nSrcNum <= 0)
        return;

    p++;

    //read source
    for (int i = 0; i < nSrcNum; i++)
    {
        t_AudioSrc src;
        src.Loop = *p;
        p++;

        src.Nextldx = *p;
        p++;

        src.Num = ReadLONG(p);
        p += 2;

        src.Type = *p;
        p++;

        src.Title = *p;
        p++;

        ActivateRequest.vSrc.push_back(src);
    }

    //read destination
    int nDestNum = ReadLONG(p);
    if (nDestNum <= 0)
        return;
    p += 2;

    for (int i = 0; i < nDestNum; i++)
    {
        t_AudioDest dest;

        dest.Sys = ReadLONG(p);
        p += 2;

        dest.Type = *p;
        p++;

        int Offset = *p;
        p++;

        int chlen = *p;
        p++;

        if(chlen > 32)
        {
            return;
        }
        //max 32 bytes to represent 256 destinations
        unsigned char Bitarray[32] = { 0 };
        memcpy(&Bitarray[Offset], p, chlen);
        p += chlen;

        Change32BytetoVector(Bitarray, dest.vOutputPort);

        ActivateRequest.vDest.push_back(dest);
    }

    CRouteManager &Mgr = CService::GetInstance()->m_routeManager;
    if(Mgr.IsRequestExist(m_pSystem->GetNode(),ActivateRequest.chRequest))
    {
        return;
    }
    if(Mgr.IsRequestFull())
    {
        return;
    }

    /*
    ActivateRequest.chProcess = PA::GetChProcess();
    auto pRoute = new CRouteRemote(m_AdrSrc.GetNode(), &ActivateRequest);
    if(pRoute->m_pSystem)
    {
        CService::GetInstance()->m_routeManager.AddRoute(pRoute);
        pRoute->BeActivate();
    }
    else
    {
        delete pRoute;
        pRoute = NULL;
    }
    */
}

void CMsgConnectRequest::DeActivate(unsigned char * pBuf, const int /*len*/)
{
    unsigned short chRequest = ReadLONG(&pBuf[1]);
    unsigned short chProcess = ReadLONG(&pBuf[3]);

    CRouteManager &Mgr = CService::GetInstance()->m_routeManager;
    CRoute* pRoute = Mgr.GetRouteByProcessRequest(chProcess,chRequest);
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
    unsigned short chRequest = ReadLONG(&pBuf[1]);
    unsigned short chProcess = ReadLONG(&pBuf[3]);

    CRouteManager &Mgr = CService::GetInstance()->m_routeManager;
    CRoute* pRoute = Mgr.GetRouteByProcessRequest(chProcess,chRequest);
    if (pRoute != NULL)
    {
        CRouteRemote* p = (CRouteRemote*)pRoute;
        p->BeInterrupt();
    }
}

void CMsgConnectRequest::CheckRoute(unsigned char* pBuf, const int /*len*/)
{
    unsigned short chRequest = ReadLONG(pBuf + 1);
    unsigned short chProcess = ReadLONG(pBuf + 3);

    CRouteManager &Mgr = CService::GetInstance()->m_routeManager;
    CRoute *pRoute = Mgr.GetRouteByRequest(chRequest);
    if (pRoute != NULL && !pRoute->IsDisConnect())
    {
        pRoute->BeCheckRoute(chRequest);

        CMsgCheckRoute check(m_pSystem);
        check.ReplayCheck(chRequest, chProcess, true);
    }
    else
    {
        CMsgCheckRoute check(m_pSystem);
        check.ReplayCheck(chRequest, chProcess, false);
    }
}

void CMsgConnectRequest::ReActivate(unsigned char* pBuf, const int len)
{
    if (len < 11)
    {
        return;
    }

    //chrequest
    unsigned short chRequest = ReadLONG(&pBuf[1]);
    unsigned short chProcess = ReadLONG(&pBuf[3]);

    CRouteManager &Mgr = CService::GetInstance()->m_routeManager;
    CRoute* pRoute = Mgr.GetRouteByProcessRequest(chProcess,chRequest);
    if (pRoute != NULL)
    {
        unsigned char* p = &pBuf[5];
        t_AudioSrc src;

        src.Loop = *p;
        p++;

        src.Nextldx = *p;
        p++;

        src.Num = ReadLONG(p);
        p += 2;

        src.Title = ReadLONG(p);
        p += 2;

        CRouteRemote* pRo = (CRouteRemote*)pRoute;
        pRo->BeReActivate(src);
    }
}

void CMsgConnectRequest::RouteStatus(unsigned char* pBuf, const int /*len*/)
{
    //chrequest
    unsigned short chRequest = ReadLONG(&pBuf[1]);
    unsigned short chProcess = ReadLONG(&pBuf[3]);

    CRouteManager &Mgr = CService::GetInstance()->m_routeManager;
    CRoute* pRoute = Mgr.GetRouteByProcessRequest(chProcess,chRequest);
    if (pRoute != NULL)
    {
        CRouteRemote* p = (CRouteRemote*)pRoute;
        p->BeRouteStatus();
    }
}

void CMsgConnectRequest::RouteStatus(CActivateResult &ret, unsigned char nMid)
{
    m_vBuffer.clear();

    //submid
    m_vBuffer.push_back(nMid);

    //retVal
    m_vBuffer.push_back(ret.RetVal);

    //chrequest
    WriteLONG(ret.chRequest);

    //chprocess
    m_chProcess = ret.chProcess;
    WriteLONG(m_chProcess);

    //conState
    m_vBuffer.push_back(ret.e_ConState);

    //SrcIdx
    m_vBuffer.push_back(ret.SrcIdx);

    //LastSrcIdx
    m_vBuffer.push_back(ret.LastSrcIdx);

    m_pSystem->OnSend(this);
}
