
#ifndef THREAD_CAPTURE_H
#define THREAD_CAPTURE_H

#ifdef WIN32

#include "mythread.h"
#include "cyclebuffer.h"
#include "threaddevice.h"
#include "audiodevicewin.h"

//this is for mic capture
class CThreadCapture: public CThreadDevice
{
public:
    CThreadCapture();
    virtual ~CThreadCapture();

    virtual void Run();

    //audio device callback function
    virtual bool CopyData(unsigned char* pData, int nFrames);
    virtual void Loop();

    void PushRtpJob();
    void PopRtpJob();
    int GetRtpJobCount();

private:
    unsigned char        m_sampleLeft[ED1AudioFrameSampleCount*4];
    unsigned char        m_sampleRight[ED1AudioFrameSampleCount*4];
    VpAudioCore::D1AudioAdpcmEncoder  m_audioEncode;
    CAudioDevice         m_audioDevice;

    //this is temp buffer,when more than 418,send
    CCycleBuffer                m_TempBufferLeft;
    CCycleBuffer                m_TempBufferRight;

};

#endif
#endif

