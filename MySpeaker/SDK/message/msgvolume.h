
#ifndef MESSAGE_DEVICE_H
#define MESSAGE_DEVICE_H

#include "sdkdefine.h"
#include "msgbase.h"
#include "device.h"

class CMsgVolume : public CMsgBase
{
public:
    CMsgVolume(CTSystem* pSys);

    void OnRespond(unsigned char* pBuf, const int len);

    int SetVolume(bool bCapture, int nPort, int nVol, int nVolMin,int nVolMax);
    int SetStepVolume(bool bCapture, int nPort, int nSetupVolumedB);

    void OnSetVolume(unsigned char* pBuf, const int len);
    void OnSetStepVolume(unsigned char* pBuf,const int len);

    //levelmeter
    void OnStartLevelmeter(unsigned char* pBuf, const int len);
    void OnStopLevelmeter(unsigned char* pBuf, const int len);

    void OnLevelData(unsigned char* pBuf, const int len);
    void OnVolumeNotify(unsigned char* pBuf, const int len);

    int FindLevelHandle(CTSystem* pSys, bool bCapture, int nPort);
    int GetLevelHandle();

    void SendLevelmeter(bool bCapture, int nPort, int nLevel);
    void SendVolumeNotify(bool bCapture, int nPort,int nVol,int nVolMin,int nVolMax);
};

#endif

