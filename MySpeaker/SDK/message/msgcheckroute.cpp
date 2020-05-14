
#include "msgbase.h"
#include "msgcheckroute.h"
#include "common.h"
#include "callback.h"
#include "define.h"
#include "service.h"
#include "routemanager.h"

CMsgCheckRoute::CMsgCheckRoute(CTSystem* pSys):CMsgBase(pSys)
{
    m_Mid = PAMSG_MID_CONNECT;
    m_SubMid = 0;
}

CMsgCheckRoute::~CMsgCheckRoute()
{

}

void CMsgCheckRoute::OnRespond(unsigned char* /*pBuf*/, const int /*len*/)
{
}

void CMsgCheckRoute::Check(unsigned short chRequest, unsigned short chProcess)
{
    //submid
    m_vBuffer.push_back((unsigned char)0x09);

    //chrequest
    WriteLONG(chRequest);

    //chprocess
    WriteLONG(chProcess);

    m_pSystem->OnSend(this);
}

void CMsgCheckRoute::ReplayCheck(unsigned short chRequest, unsigned short chProcess, bool bOK)
{
    //submid
    m_vBuffer.push_back((unsigned char)0x89);

    //retval
    if (bOK)
    {
        m_vBuffer.push_back(0);
    }
    else
    {
        m_vBuffer.push_back(1);
    }
    //chrequest
    WriteLONG(chRequest);

    //chprocess
    WriteLONG(chProcess);

    m_pSystem->OnSend(this);
}
