#ifdef _MSC_VER

#ifndef AUDIO_MSC_VER_H
#define AUDIO_MSC_VER_H

#include "sdkdefine.h"
#include <Audioclient.h>
#include <mmdeviceapi.h>
#include "audiodevice.h"
#include "threaddevice.h"

class CAudioDevice : public CAudioDeviceBase
{
public:
    CAudioDevice();
    ~CAudioDevice();

    virtual bool StartPlayBack(CThreadDevice *pMySource) final;
    virtual bool StartCapture(CThreadDevice *pMyDest) final;
    virtual bool LoopCapture() final;
    virtual bool LoopPlayBack() final;

    bool InitDevice(bool bCapture,int nChannel,int nBitsPerSamp);

protected:    
    bool InitAudioClient();
    bool InitPeriod();

    IMMDeviceEnumerator *m_pEnumerator;
    IMMDevice           *m_pDevice;
    IAudioClient        *m_pAudioClient;
    IAudioCaptureClient *m_pCaptureClient;
    IAudioRenderClient  *m_pRenderClient;
    HANDLE               m_hEvent;
    WAVEFORMATEX        *m_pwfx;
};

class CSound
{
public:
    CSound();
    ~CSound();

    static CSound* GetInstance();
    static void Release();

    void InitVolume();

private:
    static CSound*        g_Sound;
};

#endif
#endif  //_MSC_VER

