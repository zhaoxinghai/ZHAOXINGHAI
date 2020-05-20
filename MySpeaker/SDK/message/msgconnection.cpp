
#include "msgconnection.h"
#include "sdkdefine.h"
#include "service.h"
#include "routemanager.h"
#include "sdk.h"
#include "audiocapture.h"
#include "mylog.h"
#include "msgcheckroute.h"
#include "common.h"
#include "msgbusystate.h"
#include "msgconnectrequest.h"

CMsgConnection::CMsgConnection(CTSystem* pSys):CMsgBase(pSys)
{
    m_Mid = PAMSG_MID_CONNECT;
}

int CMsgConnection::Activate(CAnnouncement & act)
{
    //submid
    WriteLONG(CON_ACTIVATE);

    //request
    WriteLONG(act.nRequest);

    //Priority
    WriteLONG(act.Property.nPriority);

    //Flags
    WriteLONG(act.Property.nFlags);

    //timeout
    WriteLONG(act.Property.nTimeOut);  

    //volume
    WriteLONG(act.Property.nVolume);

    //Originator
    WriteLONG(act.Property.nTriggleNode);
    WriteLONG(act.Property.nTriggleType);
    WriteLONG(act.Property.nTriggleNumber);

    //is audio stream
    WriteLONG(act.Property.bAudioStream ? 1:0);

    if (act.Property.bAudioStream)
    {
        WriteLONG(act.Property.nAudioNode);
        WriteLONG(act.Property.nAudioChannal);
    }
    else
    {
        WriteSrcEI(act.vSrc);
    }

    //NumOfDstnEl
    WriteDestEI(act.vDest);

    m_pSystem->OnSend(this);

    return 0;
}

void CMsgConnection::WriteSrcEI(std::vector<t_Source> &vSrc)
{
    unsigned char NumOfSrc = (unsigned char)vSrc.size();
    m_vBuffer.push_back(NumOfSrc);

    for (unsigned int i = 0;i < vSrc.size();i++)
    {
        WriteOneSrc(vSrc[i]);
    }
}

void CMsgConnection::WriteOneSrc(t_Source&src)
{
    WriteLONG(src.eType);
    WriteLONG(src.nNumber);
}

void CMsgConnection::WriteDestEI(std::vector<t_Destination> &vDest)
{
    unsigned short NumOfDest = (unsigned short)vDest.size();

    WriteLONG(NumOfDest);

    for (unsigned int i = 0;i < vDest.size();i++)
    {
        t_Destination &dest = vDest[i];

        WriteLONG(dest.nNode);

        //output
        unsigned char szDest[32] = { 0 };
        ChangeVectorto32Byte(szDest,dest.vOutputPort);

        //data
        for (int i = 0;i < 32;i++)
        {
            m_vBuffer.push_back(szDest[i]);
        }
    }
}

int CMsgConnection::ReActivate(int nRequest,int nProcess)
{
    //submid
    WriteLONG(CON_REACTIVATE);

    //request
    WriteLONG(nRequest);

    //process
    WriteLONG(nProcess);

    m_pSystem->OnSend(this);

    return 0;
}

int CMsgConnection::DeActivate(int nRequest, int nProcess)
{
    //submid
    WriteLONG(CON_DEACTIVATE);

    //request
    WriteLONG(nRequest);

    //process
    WriteLONG(nProcess);

    m_pSystem->OnSend(this);

    return 0;
}

int CMsgConnection::Interrupt(int nRequest, int nProcess)
{
    //submid
    m_vBuffer.push_back((unsigned char)0x03);

    //chrequest
    WriteLONG(nRequest);

    //chrequest
    WriteLONG(nProcess);

    m_pSystem->OnSend(this);

    return 0;
}

void CMsgConnection::OnRespond(unsigned char* pBuf, const int len)
{
    switch (m_SubMid)
    {
    case CON_ON_REACTIVATE:
    case CON_ON_INTERRUPT:
    case CON_ON_ACTIVATE:
    case CON_ON_DEACTIVATE:
    case CON_ON_ROUTESTATE:
    {
        OnRespond(pBuf, len ,(e_CONNECT_RESPONE)m_SubMid);
        break;
    }
    case CON_ON_CHECKROUTE:
    {
        OnCheckRoute(pBuf,len);
        break;
    }
    default:
    {
        LOG_ERROR("CMsgConnection %d", m_SubMid);
    }
    }
}

void CMsgConnection::OnRespond(unsigned char* pBuf, const int len, e_CONNECT_RESPONE response)
{
    t_ActivateRet ret;
    ret.nErrorCode = ReadLONG(pBuf);
    pBuf += 4;

    ret.nRequest = ReadLONG(pBuf);
    pBuf += 4;

    ret.nProcess = ReadLONG(pBuf);
    pBuf += 4;
    
    ret.nRepeatIndex = ReadLONG(pBuf);
    pBuf += 4;

    ret.nPlayIndex = ReadLONG(pBuf + 4);

    CRoute* pRoute = g_SDKServer.m_routeManager.GetRouteByRequest(ret.nRequest);
    if (pRoute != NULL && !pRoute->IsDisConnect())
    {
        pRoute->UpdateResult(m_pSystem->GetNode(), ret, (int)response);
    	switch(response)
    	{
    	case CON_ON_ACTIVATE:
    	{
            pRoute->OnActivate(m_pSystem->GetNode(), ret);
    		break;
    	}
    	case CON_ON_DEACTIVATE:
    	{
            pRoute->OnDeActivate(m_pSystem->GetNode(), ret);
    		break;
    	}
    	case CON_ON_REACTIVATE:
    	{
            pRoute->OnReActivate(m_pSystem->GetNode(), ret);
    		break;
    	}
    	case CON_ON_INTERRUPT:
    	{
            pRoute->OnInterrupt(m_pSystem->GetNode(), ret);
    		break;
    	}
    	case CON_ON_ROUTESTATE:
    	{
            pRoute->OnRouteState(m_pSystem->GetNode(), ret);
    		break;
    	}
    	default:
    	{
            LOG_ERROR("CMsgConnection::OnRespond, unknow submid ： %d",response);
    	}
    	}
        if(pRoute->IsDisConnect())
        {
            g_SDKServer.m_routeManager.CleanRoute();
        }
    }
    else
    {
        //deactivate not exist route
        if (ret.nErrorCode == 0)
        {
            CMsgConnection con(m_pSystem);
            con.DeActivate(ret.nRequest,ret.nProcess);
        }
    }
}

void CMsgConnection::OnCheckRoute(unsigned char * pBuf, const int /*len*/)
{
    unsigned char retVal = pBuf[1];
    if (retVal != 0)
    {
        return;
    }

    int nRequest = ReadLONG(pBuf + 2);
    int nProcess = ReadLONG(pBuf + 4);

    CRouteManager &Mgr = g_SDKServer.m_routeManager;
    CRoute* pRoute = Mgr.GetRouteByProcess(nProcess);
    if (pRoute != NULL && !pRoute->IsDisConnect())
    {
        if (pRoute->GetType() == ROUTE_REMOTE)
        {
            CRouteRemote* p = (CRouteRemote*)pRoute;
            p->OnCheckRoute();
        }
    }
    else
    {
        CMsgConnection con(m_pSystem);
        con.DeActivate(nRequest,nProcess);
    }
}
