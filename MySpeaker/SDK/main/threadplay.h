
#ifndef THREAD_PLAY_H
#define THREAD_PLAY_H

#include "sdkdefine.h"
#include "mythread.h"
#include "audiodevice.h"
#include "threaddevice.h"
#include "service.h"

#ifdef _MSC_VER
#include "audiodevicewin.h"
#else
#include "audiodevicealsa.h"
#endif

class CService;
class CThreadPlay: public CThreadDevice
{
public:
    CThreadPlay();
    virtual ~CThreadPlay();
    virtual void Run();

    virtual void Loop();
    virtual bool LoadData(unsigned int nFrames, unsigned char* pData);

    void SetLeftVolume(int nFrame,unsigned char* pData);
    void SetRightVolume(int nFrame,unsigned char* pData);

    CAudioDevice  m_audioDevice;

    //temp
    unsigned char m_sampleLeft[1024 * 10];
    unsigned char m_sampleRight[1024 * 10];

    //mute
    bool          m_bMute;
};

#endif

