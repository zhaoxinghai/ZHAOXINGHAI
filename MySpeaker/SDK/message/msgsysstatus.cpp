
#include "msgbase.h"
#include "msgsysstatus.h"
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

    //device name(40 bytes)
    char szName[100] = { 0 };
    strncpy(szName, (char*)pBuf[4],sizeof(szName));
    pBuf += 40;

    //dwError
    int nError = ReadLONG(pBuf);

    /*
    if(nError == 1)
    {
        int ErrorCount = ReadLONG(10);
        for(int i = 0;i<10;i++)
        {
            //error type
            //error number1
            //error number2
            WriteLONG(0);
            WriteLONG(0);
            WriteLONG(0);
        }
    }
    */
}

void CMsgLifeSignal::GenerateMsg()
{
    m_vBuffer.clear();

    //sub mid
    WriteLONG(0);

    //device type
    WriteLONG(0);

    //device name(40 bytes)
    char szName[100] = { 0 };
    CTSystem* p = CService::GetInstance()->m_pSelfSystem;
    strncpy(szName, p->m_Node.strName.c_str(),sizeof(szName)-1);
    for (int i = 0; i < 39; i++)
    {
        m_vBuffer.push_back(szName[i]);
    }
    m_vBuffer.push_back('\0');

    //Error
    if (CService::GetInstance()->m_bLocalError)
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
