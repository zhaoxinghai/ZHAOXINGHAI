
/*
* Date:     2016-9-8
* Author :  xinghai,zhao
* Content : audio device's interface
*/

#ifndef AUDIO_DEVICE_H
#define AUDIO_DEVICE_H

#include "sdkdefine.h"
#include "threaddevice.h"

#define  SAMPLING_RATE   48000
#define  SAMPLING_DEEP   16      //16 or 24

class CAudioStream
{
public:
	virtual ~CAudioStream(){};
    virtual bool IsStop() = 0;
    virtual bool LoadData(unsigned int /*nFrames*/, unsigned char* /*pData*/) { return false; }
    virtual bool CopyData(unsigned char* /*pData*/, int /*nFrames*/) { return false; }
};

class CThreadDevice;
class CAudioDeviceBase
{
public:
	virtual ~CAudioDeviceBase(){};
    virtual bool StartPlayBack(CThreadDevice *pMySource) = 0;
    virtual bool StartCapture(CThreadDevice *pMyDest) = 0;
    virtual bool LoopCapture() = 0;
    virtual bool LoopPlayBack() = 0;

    CThreadDevice       *m_pAudioStream;
    bool                 m_bCapture;
    int                  m_nChannel;
    int                  m_nBitsPerSamp;
    int                  m_error;
};

#endif

