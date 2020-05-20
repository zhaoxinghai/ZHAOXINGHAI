
#include "msgbase.h"
#include "msglifesignal.h"
#include "service.h"
#include "common.h"

CMsgLifeSignal::CMsgLifeSignal(CTSystem* pSys):CMsgBase(pSys)
{
    m_Mid = PAMSG_MID_LIFE_SIGNAL;
}

e_DEVICE_TYPE CMsgLifeSignal::GetDeviceType(int Val)                                            // MOD-V6005
{
    switch (Val)
    {
    case 0:return DEVICE_IP_SPEAKER_A20;
    default:  return DEVICE_UNKNOW;
    }
}

void CMsgLifeSignal::OnRespond(unsigned char* pBuf, const int len)
{
    //device type
    int type = ReadLONG(pBuf);
    pBuf += 4;

    //dwError
    int nError = ReadLONG(pBuf);
    pBuf += 4;

    if(nError == 1)
    {
        int ErrorCount = ReadLONG(pBuf);
        pBuf += 4;

        for(int i = 0;i< ErrorCount;i++)
        {
            int type = ReadLONG(pBuf); pBuf += 4;
            int number1 = ReadLONG(pBuf); pBuf += 4;
            int number2 = ReadLONG(pBuf); pBuf += 4;
        }
    }
}

void CMsgLifeSignal::GenerateMsg()
{
    m_vBuffer.clear();

    //sub mid
    WriteLONG(0);

    //device type
    WriteLONG(0);

    //Error
    if (g_SDKServer.m_bLocalError)
    {
        WriteLONG(0x01);
    }
    else
    {
        WriteLONG(0x00);
    }
    //Error count
    WriteLONG(1);

    //Error elements
    for(int i = 0;i<1;i++)
    {
        WriteLONG(0);   //error type
        WriteLONG(0);   //error number1
        WriteLONG(0);   //error number2
    }
}

void CMsgLifeSignal::Broadcast()
{
    GenerateMsg();

    m_pSystem->m_pTransport->SendBroadcast(this,true);
}
