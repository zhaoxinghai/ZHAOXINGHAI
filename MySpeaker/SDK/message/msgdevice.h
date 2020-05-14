
#ifndef MESSAGE_DEVICE_H
#define MESSAGE_DEVICE_H

#include "predefine.h"
#include "msgbase.h"
#include "device.h"

class CMsgDevice : public CMsgBase
{
public:
    CMsgDevice(CTSystem* pSys);

    void OnRespond(unsigned char* pBuf, const int len);

    //command
    int GetDeviceStatus(t_VDevice* pDevice);
    int GetDeviceStatus(int nType, int number, int channel);

    int AudioControl(const t_VDevice &de, int volume);
    int AudioControl10(const t_VDevice &de, int volume);
    int AudioControl11(const t_VDevice &de, int volume);

    //OnRespond
    void OnDeviceStatus(unsigned char* pBuf, const int len);

    void OnAudioControl11(unsigned char* pBuf, const int len);
    void OnAudioControlReply11(unsigned char* pBuf,const int len);

    void OnLevelmeter(unsigned char* pBuf, const int len);
    int FindLevelHandle(CTSystem* pSys,const t_VDevice &de);
    int GetLevelHandle();
    void ReplyLevelmeter(int handle,const t_VDevice &de);

    //request
    void DeviceStatusRequest(unsigned char* pBuf, const int len);
    //void DeviceStatus(CDeviceStateResult *pState);

    void Levelmeter(int handle,float fValue);

    void ReplyAudioControl(const t_VDevice &de,float fVolme,float fMin,float fMax);
    void ReplyAudioControl11(const t_VDevice &de,float fVolme,float fMin,float fMax);
};

#endif

