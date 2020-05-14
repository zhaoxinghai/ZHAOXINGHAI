
#include "msgconnection.h"
#include "predefine.h"
#include "service.h"
#include "routemanager.h"
#include "sdk.h"
#include "audiocapture.h"
#include "mylog.h"
#include "msgcheckroute.h"
#include "msgconnectrequest.h"
#include "common.h"
#include "msgusagereport.h"

CMsgConnection::CMsgConnection(CTSystem* pSys):CMsgBase(pSys)
{
    m_Mid = PAMSG_MID_CONNECT;
}

int CMsgConnection::Activate(CActivate* pPACon)
{
    //submid
    m_vBuffer.push_back((unsigned char)0x00);

    //version
    m_vBuffer.push_back((unsigned char)0x01);

    //chrequest
    WriteLONG(pPACon->chRequest);

    //Priority
    m_vBuffer.push_back((unsigned char)pPACon->Priority);

    //Flags
    WriteLONG(pPACon->Flags);

    //timeout
    WriteLONG(pPACon->TimeOut);  

    //Level(int16)
    WriteLONG(pPACon->Level);

    //Originator
    WriteLONG(pPACon->nTriggleNode);    //0 to 998	CS number on the originator's system
    WriteLONG(pPACon->nTriggleType);  //0 - 400	node number of the originator
    WriteLONG(pPACon->nTriggleNumber);

    //SecHash
    m_vBuffer.push_back(0);
    m_vBuffer.push_back(0);
    m_vBuffer.push_back(0);
    m_vBuffer.push_back(0);
    m_vBuffer.push_back(0);
    m_vBuffer.push_back(0);
    m_vBuffer.push_back(0);
    m_vBuffer.push_back(0);

    if (IsRtpStream(pPACon))
    {
        WriteRtpSrc(pPACon);
    }
    else
    {
        WriteSrcEI(pPACon->vSrc);
    }

    //NumOfDstnEl
    WriteDestEI(pPACon->vDest);

    //reserved
    m_vBuffer.push_back(0x00);

    m_pSystem->OnSend(this);

    return 0;
}

void CMsgConnection::WriteSrcEI(std::vector<t_AudioSrc> &vSrc)
{
    unsigned char NumOfSrc = (unsigned char)vSrc.size();
    m_vBuffer.push_back(NumOfSrc);

    for (unsigned int i = 0;i < vSrc.size();i++)
    {
        WriteOneSrc(vSrc[i]);
    }
}

void CMsgConnection::WriteOneSrc(t_AudioSrc &src)
{
    m_vBuffer.push_back(src.Loop);
    m_vBuffer.push_back(src.Nextldx);

    WriteLONG(src.Num);
    m_vBuffer.push_back(src.Type);
    m_vBuffer.push_back((unsigned char)src.Title);
}

void CMsgConnection::WriteRtpSrc(CActivate* pPACon)
{
    CActivateMicr* pAc = (CActivateMicr*)pPACon;

    //src count
    m_vBuffer.push_back(1);

    //loop
    m_vBuffer.push_back(0);
    m_vBuffer.push_back(255);

    WriteLONG(pAc->nRtpNode);
    m_vBuffer.push_back(AS_TYPE_AN);
    m_vBuffer.push_back(pAc->nRtpChannel);
}

void CMsgConnection::WriteDestEI(std::vector<t_AudioDest> &vDest)
{
    unsigned short NumOfDest = (unsigned short)vDest.size();

    WriteLONG(NumOfDest);


    for (unsigned int i = 0;i < vDest.size();i++)
    {
        t_AudioDest &dest = vDest[i];

        WriteLONG(dest.Sys);

        
        //Type
        m_vBuffer.push_back((unsigned char)dest.Type);

        //output
        unsigned char szDest[32] = { 0 };
        ChangeVectorto32Byte(szDest,dest.vOutputPort);

        unsigned char begin = 0;
        unsigned char end = 0;
        TrimEmpty(szDest,begin,end);

        //offset
        m_vBuffer.push_back(begin);
        
        //len
        unsigned char len = 32 - begin - end;
        m_vBuffer.push_back(len);

        //data
        for (int i = begin;i < 32 - end;i++)
        {
            m_vBuffer.push_back(szDest[i]);
        }
    }
}

void CMsgConnection::TrimEmpty(unsigned char* pDest, unsigned char &begin, unsigned char &end)
{
    //get begin
    begin = 0;
    for (int i = 0;i < 31;i++)
    {
        if (pDest[i] == 0)
        {
            begin++;
        }
        else
        {
            break;
        }
    }

    //get end
    end = 0;
    for (int i = 31;i >= 0;i--)
    {
        if (pDest[i] == 0)
        {
            end++;
        }
        else
        {
            break;
        }
    }
}

bool CMsgConnection::IsRtpStream(CActivate * pPACon)
{
    if (pPACon->type == ACTIVATE_MICR
        || pPACon->type == ACTIVATE_PLAY)
    {
        return true;
    }
    return false;
}

int CMsgConnection::GetGongCount(CActivate* pPACon)
{
	for(unsigned int i = 0;i<pPACon->vSrc.size();i++)
	{
		if(pPACon->vSrc[i].Type==AS_TYPE_AI && pPACon->vSrc[i].Num==30)
		{
			return pPACon->vSrc[i].Title;
		}
	}
	return 0;
}


int CMsgConnection::ReActivate(CReActivate* pReAct)
{
    //submid
    m_vBuffer.push_back((unsigned char)0x02);

    //chrequest
    WriteLONG(pReAct->chRequest);

    //chrequest
    WriteLONG(pReAct->chProcess);

    //srcEI
    m_vBuffer.push_back(0);    //loop
    m_vBuffer.push_back(255);

    unsigned short num = 0;

    //1000 0000 0000 0000     symbol
    num |= (1 << 15);

    //0000 0111 1000 0000     set system number bits
    num |= pReAct->nRtpNode << 7;

    // set rtp channel number
    num |= pReAct->nRtpChannel;

    WriteLONG(num);
    WriteLONG(0);

    m_pSystem->OnSend(this);

    return 0;
}

int CMsgConnection::DeActivate(CDeActivate* pDe)
{
    //submid
    m_vBuffer.push_back((unsigned char)0x01);

    //chrequest
    WriteLONG(pDe->chRequest);

    //chrequest
    WriteLONG(pDe->chProcess);

    m_pSystem->OnSend(this);

    return 0;
}

int CMsgConnection::DeActivateViacsNum(unsigned short csNum)
{
    //submid
    m_vBuffer.push_back((unsigned char)0x0F);

    //version
    m_vBuffer.push_back((unsigned char)0x01);

    //cs number
    WriteLONG(csNum);

    m_pSystem->OnSend(this);
    
    return 0;
}

int CMsgConnection::DeActivateAll()
{
    //submid
    m_vBuffer.push_back((unsigned char)0x06);

    m_pSystem->OnSend(this);

    return 0;
}

int CMsgConnection::Interrupt(CDeActivate* pDe)
{
    //submid
    m_vBuffer.push_back((unsigned char)0x03);

    //chrequest
    WriteLONG(pDe->chRequest);

    //chrequest
    WriteLONG(pDe->chProcess);

    m_pSystem->OnSend(this);

    return 0;
}

void CMsgConnection::OnRespond(unsigned char* pBuf, const int len)
{
    m_SubMid = pBuf[0];
    
    if (m_SubMid & 0x80)
    {
        switch (m_SubMid)
        {
        case CON_ON_REACTIVATE:
        case CON_ON_INTERRUPT:
        case CON_ON_ACTIVATE:
        case CON_ON_DEACTIVATE:
        case CON_ON_ROUTESTATE:
        {
        	OnRespond(pBuf, len ,(e_RESPONE)m_SubMid);
        	break;
        }
        case CON_ON_CHECKROUTE:
        {
            OnCheckRoute(pBuf,len);
        	break;
        }
        case 0x87:  //destination usage report,every 5 seconds
        case 0x8D:  //V11 local usage report
        case 0x8E:  //V11 remote usage report
        {
            CMsgUsageReport usage(m_pSystem);
            usage.SetMid(m_Mid);
            usage.OnRespond(pBuf, len);
            break;
        }
        case 0x8F:
            OnDeActivateViacsNum(pBuf, len);
            break;
        case 0x91:
        {
        	//i don't know what thing to do
            //LOG_DEBUG("CMsgConnection %d, i don't know what to do?", m_SubMid);
        	break;
        }
        default:
        {
            LOG_ERROR("CMsgConnection %d", m_SubMid);
        }
        }
    }
    else
    {
        //this is request
        CMsgConnectRequest request(m_pSystem);
        request.SetMid(m_Mid);
        request.OnRespond(pBuf, len);
    }
}

void CMsgConnection::OnRespond(unsigned char* pBuf, const int len, e_RESPONE response)
{
    if (len != 9)
        return;

    unsigned char retVal = pBuf[1];
   
    unsigned long chRequest = ReadLONG(pBuf + 2);
    unsigned long chProcess = ReadLONG(pBuf + 4);
    unsigned char conState = pBuf[6];
    unsigned char SrcIdx = pBuf[7];
    unsigned char LastSrcIdx = pBuf[8];

    //result
	CActivateResult ret;
    ret.nErrorCode = 0;
	ret.strErrorDesc = "OK";
    ret.RetVal = retVal;
	ret.chRequest = chRequest;
	ret.chProcess = chProcess;
	ret.e_ConState = (CONSTATE)conState;
    if(ret.e_ConState == CON_PARTLY_DEFECT)
        ret.e_ConState = CON_PARTLY_CONNECT;

	ret.SrcIdx = SrcIdx;
	ret.LastSrcIdx = LastSrcIdx;

    if (retVal != 0)
        return;

    CRouteManager &Mgr = CService::GetInstance()->m_routeManager;
    CRoute* pRoute = Mgr.GetRouteByRequest(chRequest);
    if (pRoute != NULL && !pRoute->IsDisConnect())
    {
        //pRoute->UpdateResult(m_AdrSrc.GetNode(), ret, response);
    	switch(response)
    	{
    	case CON_ON_ACTIVATE:
    	{
            //pRoute->OnActivate(m_AdrSrc.GetNode(), ret);
    		break;
    	}
    	case CON_ON_DEACTIVATE:
    	{
            //pRoute->OnDeActivate(m_AdrSrc.GetNode(), ret);
    		break;
    	}
    	case CON_ON_REACTIVATE:
    	{
            //pRoute->OnReActivate(m_AdrSrc.GetNode(), ret);
    		break;
    	}
    	case CON_ON_INTERRUPT:
    	{
            //pRoute->OnInterrupt(m_AdrSrc.GetNode(), ret);
    		break;
    	}
    	case CON_ON_ROUTESTATE:
    	{
            //pRoute->OnRouteState(m_AdrSrc.GetNode(), ret);
    		break;
    	}
    	default:
    	{
            LOG_ERROR("CMsgConnection::OnRespond, unknow submid ： %d",response);
    	}
    	}
        if(pRoute->IsDisConnect())
        {
            CService::GetInstance()->m_routeManager.CleanRoute();
        }
    }
    else
    {
        //deactivate not exist route
        CDeActivate de;
        de.chRequest = chRequest;
        de.chProcess = chProcess;
        CMsgConnection con(m_pSystem);
        con.DeActivate(&de);
    }
}

void CMsgConnection::OnCheckRoute(unsigned char * pBuf, const int /*len*/)
{
    unsigned char retVal = pBuf[1];
    if (retVal != 0)
    {
        return;
    }

    unsigned short chRequest = ReadLONG(pBuf + 2);
    unsigned short chProcess = ReadLONG(pBuf + 4);

    CRouteManager &Mgr = CService::GetInstance()->m_routeManager;
    CRoute* pRoute = Mgr.GetRouteByProcess(chProcess);
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
        CDeActivate de;
        de.chRequest = chRequest;
        de.chProcess = chProcess;
        CMsgConnection con(m_pSystem);
        con.DeActivate(&de);
    }
}

void CMsgConnection::OnDeActivateViacsNum(unsigned char* /*pBuf*/, const int /*len*/)
{
    /*
    if (len != 5)
        return;

    unsigned short csNum = ReadLONG(pBuf + 2);
    unsigned char retVal = pBuf[4];

    if (retVal == 0)
    {
        LOG_DEBUG("csNum:%d %s OK", csNum, "OnDeActivateViacsNum");
    }
    else
    {
        LOG_DEBUG("csNum:%d %s Error", csNum, "OnDeActivateViacsNum");
    }

    CDeActivateViacsNumResult ret;
    ret.nErrorCode = retVal;
    ret.strErrorDesc = "OK";
    CService::GetInstance()->ExcuteCallback(pCall, &ret);
    */
}
