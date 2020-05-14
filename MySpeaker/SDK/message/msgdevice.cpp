
#include "msgdevice.h"
#include "sdk.h"
#include "interfacemsg.h"
#include "msgfactory.h"
#include "service.h"
#include "public.h"

#define IS_VOLUME(x) (x & 0x01)

CMsgDevice::CMsgDevice(CTSystem* pSys):CMsgBase(pSys)
{
    m_Mid = 0x5A;
}

int CMsgDevice::GetDeviceStatus(int nType, int number,int channel)
{
    t_VDevice tDevice;
    tDevice.type = (unsigned char)nType;
    tDevice.number = (unsigned char)number;
    tDevice.channel = (unsigned char)channel;
    return GetDeviceStatus(&tDevice);
}

int CMsgDevice::GetDeviceStatus(t_VDevice* pDevice)
{
    //subid
    m_vBuffer.push_back((unsigned char)0x00);

    //version
    m_vBuffer.push_back((unsigned char)0x01);

    //device type
    m_vBuffer.push_back(pDevice->type);
    
    //device number
    WriteLONG(pDevice->number);

    //device channel
    m_vBuffer.push_back(pDevice->channel);

    m_pSystem->OnSend(this);

    return 0;
}

int CMsgDevice::AudioControl(const t_VDevice &de, int volume)
{
    return AudioControl11(de,volume);
}

int CMsgDevice::AudioControl11(const t_VDevice &de, int volume)
{
    m_vBuffer.clear();

    //subid
    m_vBuffer.push_back((unsigned char)0x01);

    //version
    m_vBuffer.push_back((unsigned char)0x01);

    //device type
    m_vBuffer.push_back(de.type);

    //device number
    WriteLONG(de.number);

    //device channel
    m_vBuffer.push_back(de.channel);

    //CRC
    WriteLONG(0x00);

    //Flags
    WriteLONG(0x01);

    //volume
    WriteLONG(volume);

    //delay
    WriteLONG(0x00);

    //PEQ8
    for(int i = 0;i<8;i++)
    {
        WriteLONG(0x00);
        WriteLONG(0x00);
        WriteLONG(0x00);
    }
    //HighPass
    WriteLONG(0x00);
    WriteLONG(0x00);

    //LowPass
    WriteLONG(0x00);
    WriteLONG(0x00);

    //ShutdownTime
    WriteLONG(0x00);

    m_pSystem->OnSend(this);

    return 0;
}

void CMsgDevice::OnRespond(unsigned char* pBuf, const int len)
{
    /*
    m_SubMid = pBuf[0];

    if(m_Mid==PAMSG_MID_DEVICE2)
    {
        if(m_SubMid == PAMSG_SMID_LMSTART
                || m_SubMid == PAMSG_SMID_LMREFRESH)
        {
            OnLevelmeter(pBuf,len);
        }
        else if(m_SubMid == PAMSG_SMID_AUDIO_CONTROL)
        {
            OnAudioControl11(pBuf,len);
        }
        else if(m_SubMid == PAMSG_SMID_AUDIO_CONTROL_REPLY)
        {
            OnAudioControlReply11(pBuf,len);
        }
        else if(m_SubMid == PAMSG_SMID_DEVICE_STATE)
        {

        }
        else
        {
            LOG_DEBUG("%s","CMsgDevice");
        }
    }
    */
}

void CMsgDevice::OnDeviceStatus(unsigned char* pBuf, const int len)
{
    /*
    if (len < 8)
    {
        return;
    }
    CDeviceStateResult DeviceState;
    DeviceState.address = m_AdrSrc.GetAdr();
    CPublic::GetDateTime(DeviceState.datetime);

    unsigned char* p = pBuf+1;

    //version
    if (m_ProtocolVersion == PROTOCOL_V11)
    {
        DeviceState.nVersion = *p;
        p++;
    }

    //retval
    DeviceState.eRetVal = e_StateRetVal(*p);
    p++;

    //device
    DeviceState.device.type = *p;
    p++;

    if (m_ProtocolVersion == PROTOCOL_V11)
    {
        DeviceState.device.number = ReadLONG(p);
        p += 2;
    }
    else
    {
        DeviceState.device.number = *p;
        p++;
    }
    DeviceState.device.channel = *p;
    p++;
    
    //controlstate
    int temp = *p;
    p++;
    if (temp > 4)
    {
        LOG_ERROR("%s","CMsgDevice::OnDeviceStatus temp > 4")
        return;
    }
    DeviceState.eControlState = (e_ControlState)temp;
   
    //audiostate
    temp = *p;
    p++;
    if (temp > 10)
    {
        LOG_ERROR("%s","CMsgDevice::OnDeviceStatus temp > 10")
        return;
    }
    DeviceState.eAudioState = (e_AudioState)temp;
   
    //busystate
    temp = *p;
    if (temp > 2)
    {
        LOG_ERROR("%s","CMsgDevice::OnDeviceStatus temp > 2");
        return;
    }
    DeviceState.eBusyState = (e_BusyState)temp;
    CService::GetInstance()->ExcuteCallback(&DeviceState);
    */
}

void CMsgDevice::OnAudioControl11(unsigned char* pBuf, const int len)
{
    if (len != 74)
        return;

    CAudioControlResult ret;
    ret.nNode = m_pSystem->GetNode();
    ret.tcpSocket = m_pSystem->m_sock;
    ret.IsTCP = m_pSystem->IsTCP();

    //int version = pBuf[1];
    ret.device.type = pBuf[2];
    ret.device.number = ReadLONG(&pBuf[3]);
    ret.device.channel = pBuf[5];
    //int CRC = ReadLONG(&pBuf[6]);
    short Flags = ReadLONG(&pBuf[10]);
    short volume = ReadLONG(&pBuf[12]);
    ret.fVolume = volume/10.0f;
    if(IS_VOLUME(Flags))
    {
        CService::GetInstance()->ExcuteCallback(&ret);
    }
}

void CMsgDevice::DeviceStatusRequest(unsigned char* pBuf, const int len)
{
    if (len != 4)
        return;

    /*
    //etcs not support device state
    CDeviceStateResult ret;
    ret.eRetVal = NOT_INSTALLED_HIGHER;
    ret.eControlState = CONTROL_OK;
    ret.eAudioState = AUDIO_OK;

    ret.device.type = pBuf[1];
    ret.device.number = pBuf[2];
    ret.device.channel = pBuf[3];

    DeviceStatus(&ret);
    */
}

void CMsgDevice::OnLevelmeter(unsigned char* pBuf, const int len)
{
    CService* pSer = CService::GetInstance();
    if(m_SubMid == PAMSG_SMID_LMSTART)
    {
        if (len != 6)
            return;

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
    }
    else if(m_SubMid == PAMSG_SMID_LMREFRESH)
    {
        int nNumOfHandles = pBuf[2];
        for(int i = 0;i<nNumOfHandles;i++)
        {
            int Handle = pBuf[3+i];
            auto it = pSer->m_mapLevelmeter.find(Handle);
            if(it != pSer->m_mapLevelmeter.end())
            {
                t_Levelmeter &level = (*it).second;
                level.nCounter = 0;
            }
        }
    }
    else
    {
        LOG_DEBUG("SubMid:%d",m_SubMid);
    }
}

void CMsgDevice::ReplyLevelmeter(int handle,const t_VDevice &de)
{
    m_vBuffer.clear();

    //sub mid
    m_vBuffer.push_back(0x90);

    //retval
    if(handle != -1)
    {
        m_vBuffer.push_back(0);
    }
    else
    {
        m_vBuffer.push_back(1);
    }
    //handle
    m_vBuffer.push_back((unsigned char)handle);

    //device
    m_vBuffer.push_back(de.type);
    WriteLONG(de.number);
    m_vBuffer.push_back(de.channel);

    m_pSystem->m_pTransport->Send(this);
}

int CMsgDevice::FindLevelHandle(CTSystem* pSys,const t_VDevice &de)
{
    CService* pSer = CService::GetInstance();
    auto it = pSer->m_mapLevelmeter.begin();
    while(it != pSer->m_mapLevelmeter.end())
    {
        t_Levelmeter &level = (*it).second;
        if(level.pSys == pSys && level.de == de)
        {
            return (*it).first;
        }
        it++;
    }
    return -1;
}

int CMsgDevice::GetLevelHandle()
{
    CService* pSer = CService::GetInstance();
    for(int i = 0;i<24;i++)
    {
        auto it = pSer->m_mapLevelmeter.find(i);
        if(it == pSer->m_mapLevelmeter.end())
        {
            return i;
        }
    }
    return -1;
}

void CMsgDevice::Levelmeter(int handle,float fValue)
{
    m_vBuffer.clear();

    //sub mid
    m_vBuffer.push_back(0x93);

    //version
    m_vBuffer.push_back(1);

    //NumOfData
    m_vBuffer.push_back(1);

    //handle
    m_vBuffer.push_back(handle);

    //data
    short value = (short)(fValue*10);
    WriteLONG(value);

    m_pSystem->m_pTransport->Send(this);
}

void CMsgDevice::OnAudioControlReply11(unsigned char* pBuf,const int len)
{
    if(len != 87)
        return;

    CAudioReplyResult ret;
    ret.nNode = m_pSystem->m_Node.nNode;

    //int version = pBuf[1];

    //retval
    int retval = pBuf[2];
    if(retval != 0)
        return;

    //device
    ret.device.type = pBuf[3];
    ret.device.number = ReadLONG(&pBuf[4]);
    ret.device.channel = pBuf[6];

    short volume = ReadLONG(&pBuf[7]);
    short volMin = ReadLONG(&pBuf[9]);
    short volMax = ReadLONG(&pBuf[11]);

    ret.fVolume = volume/10.0f;
    ret.fVolMin = volMin/10.0f;
    ret.fVolMax = volMax/10.0f;

    CService::GetInstance()->ExcuteCallback(&ret);
}

void CMsgDevice::ReplyAudioControl(const t_VDevice &de,float fVolme,float fMin,float fMax)
{
    ReplyAudioControl11(de,fVolme,fMin,fMax);
}

void CMsgDevice::ReplyAudioControl11(const t_VDevice &de,float fVolme,float fMin,float fMax)
{
    m_vBuffer.clear();

    //sub mid
    m_vBuffer.push_back(0x81);

    //version
    m_vBuffer.push_back(0x01);

    //retval
    m_vBuffer.push_back(0);

    //device
    m_vBuffer.push_back(de.type);
    WriteLONG(de.number);
    m_vBuffer.push_back(de.channel);

    //vol
    WriteLONG(short(fVolme*10));
    WriteLONG(short(fMin*10));
    WriteLONG(short(fMax*10));

    WriteLONG(0x00);  //VolAlarm
    WriteLONG(0x00);  //VolBase
    WriteLONG(0x00);  //VolAV
    WriteLONG(0x00);  //VolProg1
    WriteLONG(0x00);  //VolProg2
    WriteLONG(0x00);  //VolConnect

    WriteLONG(0x00);  //Flags
    WriteLONG(0x00);  //Delay

    WriteLONG(0x00);  //PEQ8
    WriteLONG(0x00);  //PEQ8
    WriteLONG(0x00);  //PEQ8

    WriteLONG(0x00);  //HighPass
    WriteLONG(0x00);  //HighPass

    WriteLONG(0x00);  //LowPass
    WriteLONG(0x00);  //LowPass

    WriteLONG(0x00);  //shutdown delay time

    m_pSystem->m_pTransport->Send(this);
}

