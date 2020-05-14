
#ifndef THREAD_CAPTURE_H
#define THREAD_CAPTURE_H

#include "predefine.h"
#include "mythread.h"
#include "cyclebuffer.h"
#include "threaddevice.h"
#include "service.h"

#ifdef _MSC_VER
#include "audiodevicewin.h"
#else
#include "audiodevicealsa.h"
#endif

class CService;

//this is for mic capture
class CThreadCapture: public CThreadDevice
{
public:
    CThreadCapture(CService* pService);
    virtual ~CThreadCapture();

    virtual void Run();

    //audio device callback function
    virtual bool CopyData(unsigned char* pData, int nFrames);
    virtual void Loop();

    void PushRtpJob();
    void PopRtpJob();
    int GetRtpJobCount();

    void SetLeftVolume(int block,unsigned char* pData);
    void SetRightVolume(int block,unsigned char* pData);

private:
    unsigned char        m_sampleLeft[ED1AudioFrameSampleCount*4];
    unsigned char        m_sampleRight[ED1AudioFrameSampleCount*4];
    VpAudioCore::D1AudioAdpcmEncoder  m_audioEncode;
    CAudioDevice         m_audioDevice;
    CService*            m_pService;

    //this is temp buffer,when more than 418,send
    CCycleBuffer                m_TempBufferLeft;
    CCycleBuffer                m_TempBufferRight;

};

#endif

