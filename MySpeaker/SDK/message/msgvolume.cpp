
#include "msgvolume.h"
#include "sdk.h"
#include "interfacemsg.h"
#include "msgfactory.h"
#include "service.h"
#include "public.h"

#define IS_VOLUME(x) (x & 0x01)

CMsgVolume::CMsgVolume(CTSystem* pSys):CMsgBase(pSys)
{
    m_Mid = PAMSG_MID_VOLUME;
}

int CMsgVolume::SetVolume(bool bCapture, int nPort, int nVol, int nVolMin, int nVolMax)
{
    m_vBuffer.clear();

    //subid
    WriteLONG(SMID_SET_VOLUME);

    //capture
    WriteLONG(bCapture?1:0);

    //port
    WriteLONG(nPort);

    //volume
    WriteLONG(nVol);
    WriteLONG(nVolMin);
    WriteLONG(nVolMax);

    m_pSystem->OnSend(this);
   
    return 0;
}

int CMsgVolume::SetStepVolume(bool bCapture, int nPort, int nSetupVolumedB)
{
    m_vBuffer.clear();

    //subid
    WriteLONG(SMID_SET_STEP_VOLUME);

    //capture
    WriteLONG(bCapture ? 1 : 0);

    //port
    WriteLONG(nPort);

    //step volume
    WriteLONG(nSetupVolumedB);

    m_pSystem->OnSend(this);

    return 0;
}

void CMsgVolume::OnRespond(unsigned char* pBuf, const int len)
{
    m_SubMid = ReadLONG(pBuf);

    if (m_SubMid == SMID_SET_VOLUME)
    {
        OnSetVolume(pBuf, len);
    }
    else if (m_SubMid == SMID_SET_STEP_VOLUME)
    {
        OnSetStepVolume(pBuf, len);
    }
    else if(m_SubMid == SMID_LEVEL_METER_START)
    {
        OnStartLevelmeter(pBuf,len);
    }
    else if(m_SubMid == SMID_LEVEL_METER_STOP)
    {
        OnStopLevelmeter(pBuf,len);
    }
    else if(m_SubMid == SMID_LEVEL_METER_DATA)
    {
        OnLevelData(pBuf, len);
    }
    else if (m_SubMid == SMID_VOLUME_NOTIFY)
    {
        OnVolumeNotify(pBuf, len);
    }
    else
    {
        LOG_DEBUG("%s","CMsgVolume");
    }
}

void CMsgVolume::OnSetVolume(unsigned char* pBuf, const int len)
{
    int nCapture = ReadLONG(pBuf);
    pBuf += 4;

    int Port = ReadLONG(pBuf);
    pBuf += 4;

    int vol = ReadLONG(pBuf);
    pBuf += 4;

    int volMin = ReadLONG(pBuf);
    pBuf += 4;

    int volMax = ReadLONG(pBuf);

    CVolumeSetResult ret;
    ret.nTriggleNode = m_pSystem->GetNode();
    ret.bCapture = nCapture==1 ? true:false;
    ret.nPort = Port;
    ret.nVolume = vol;
    ret.nVolMin = volMin;
    ret.nVolMax = volMax;

    g_SDKServer.ExcuteCallback(&ret);
}

void CMsgVolume::OnSetStepVolume(unsigned char* pBuf, const int len)
{
    int nCapture = ReadLONG(pBuf);
    pBuf += 4;

    int Port = ReadLONG(pBuf);
    pBuf += 4;

    int volStep = ReadLONG(pBuf);

    CVolumeSetStepResult ret;
    ret.nTriggleNode = m_pSystem->GetNode();
    ret.bCapture = nCapture == 1 ? true : false;
    ret.nPort = Port;
    ret.nVolumeStep = volStep;
  
    g_SDKServer.ExcuteCallback(&ret);
}

void CMsgVolume::OnStartLevelmeter(unsigned char* pBuf, const int len)
{
    /*
    t_Levelmeter level;
    level.nCounter = 0;
    level.pSys = m_pSystem;
    level.de.type = pBuf[2];
    level.de.number = ReadLONG(&pBuf[3]);
    level.de.channel = pBuf[5];

    int handle = FindLevelHandle(m_pSystem,level.de);
    if(handle == -1)
    {
        //a new handle
        handle = GetLevelHandle();
        if(handle != -1)
        {
            pSer->m_mapLevelmeter[handle] = level;
        }
    }
    else
    {
        //exist handle
        pSer->m_mapLevelmeter[handle].nCounter = 0;
    }
    ReplyLevelmeter(handle,level.de);
    */
}

void CMsgVolume::OnLevelData(unsigned char* pBuf, const int len)
{

}

void CMsgVolume::OnStopLevelmeter(unsigned char* pBuf, const int len)
{
}

int CMsgVolume::FindLevelHandle(CTSystem* pSys,bool bCapture,int nPort)
{
    auto it = g_SDKServer.m_mapLevelmeter.begin();
    while(it != g_SDKServer.m_mapLevelmeter.end())
    {
        t_Levelmeter &level = (*it).second;
        if(level.pSys == pSys && level.bCapture == bCapture && level.nPort == nPort)
        {
            return (*it).first;
        }
        it++;
    }
    return -1;
}

int CMsgVolume::GetLevelHandle()
{
    for(int i = 0;i<24;i++)
    {
        auto it = g_SDKServer.m_mapLevelmeter.find(i);
        if(it == g_SDKServer.m_mapLevelmeter.end())
        {
            return i;
        }
    }
    return -1;
}

void CMsgVolume::SendLevelmeter(bool bCapture, int nPort, int nLevel)
{
    m_vBuffer.clear();

    //subid
    WriteLONG(SMID_LEVEL_METER_DATA);

    //capture
    WriteLONG(bCapture ? 1 : 0);

    //port
    WriteLONG(nPort);

    //level
    WriteLONG(nLevel);

    m_pSystem->m_pTransport->Send(this);
}

void CMsgVolume::OnVolumeNotify(unsigned char* pBuf,const int len)
{
    int nCapture = ReadLONG(pBuf);
    pBuf += 4;

    int nPort = ReadLONG(pBuf);
    pBuf += 4;

    int nVol = ReadLONG(pBuf);
    pBuf += 4;

    int nVolMin = ReadLONG(pBuf);
    pBuf += 4;

    int nVolMax = ReadLONG(pBuf);
    pBuf += 4;

    CVolumeNotifyResult ret;
    ret.nNode = m_pSystem->m_Node.nNode;
    ret.bCapture = nCapture == 1 ? true : false;
    ret.nPort = nPort;
    ret.nVolume = nVol;
    ret.nVolMin = nVolMin;
    ret.nVolMax = nVolMax;

    g_SDKServer.ExcuteCallback(&ret);
}

void CMsgVolume::SendVolumeNotify(bool bCapture, int nPort, int nVol, int nVolMin, int nVolMax)
{
    m_vBuffer.clear();

    //subid
    WriteLONG(SMID_VOLUME_NOTIFY);

    //capture
    WriteLONG(bCapture ? 1 : 0);

    //port
    WriteLONG(nPort);

    //volume
    WriteLONG(nVol);
    WriteLONG(nVolMin);
    WriteLONG(nVolMax);

    m_pSystem->OnSend(this);
}

