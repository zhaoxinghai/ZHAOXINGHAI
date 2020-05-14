
#include "msgbase.h"
#include "msgsynctime.h"
#include "common.h"
#include "callback.h"
#include "sdk.h"
#include "service.h"

CMsgSyncTime::CMsgSyncTime(CTSystem* pSys):CMsgBase(pSys)
{
    //m_Mid = PAMSG_MID_SETRTC;
}

CMsgSyncTime::~CMsgSyncTime()
{

}

void CMsgSyncTime::OnRespond(unsigned char* pBuf, const int len)
{
    if (len < 7)
        return;

    unsigned char *p = pBuf;

    CDataTimeResult ret;
  
    ret.datatime.Sec = *p;
    p++;

    ret.datatime.Min = *p;
    p++;

    ret.datatime.Hour = *p;
    p++;

    ret.datatime.Day = *p;
    p++;

    ret.datatime.Month = *p;
    p++;

    ret.datatime.Year = ReadLONG(p);

    if (m_pSystem->m_Node.nNode != CService::GetInstance()->GetNode())
    {
        CService::GetInstance()->ExcuteCallback(&ret);
    }
}

void CMsgSyncTime::SyncDataTime(t_DateTime* mytime)
{
    m_vBuffer.push_back((unsigned char)mytime->Sec);
    m_vBuffer.push_back((unsigned char)mytime->Min);
    m_vBuffer.push_back((unsigned char)mytime->Hour);
    m_vBuffer.push_back((unsigned char)mytime->Day);
    m_vBuffer.push_back((unsigned char)mytime->Month);

    WriteLONG(mytime->Year);

    //m_pSystem->m_pTransport->SendBroadcast(this);
}
