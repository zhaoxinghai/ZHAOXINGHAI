#ifdef _MSC_VER

#ifndef AUDIO_MSC_VER_H
#define AUDIO_MSC_VER_H

#include "sdkdefine.h"
#include <Audioclient.h>
#include <mmdeviceapi.h>
#include "audiodevice.h"
#include "threaddevice.h"
#include "sounddevicewin.h"

namespace TOP
{
    class CAudioDevice : public CAudioDeviceBase
    {
    public:
        CAudioDevice();
        ~CAudioDevice();

        virtual bool StartPlayBack() final;
        virtual bool StartCapture() final;
        virtual bool LoopCapture() final;
        virtual bool LoopPlayBack() final;

        void SetCaptureFrame(int nFrame);

        bool InitDevice(bool bCapture,CThreadDevice* pMySource);
        void Release();

    protected:
        bool InitPeriod();
        bool InitFormat();
        bool SetFormat(int nChannel,int nSampleRate,int nBitSample);

        bool createTimerEvent();
        bool InitMediaDevice();

        IMMDeviceCollection*  m_pMMDeviceCaptureCollection;
        IMMDeviceCollection*  m_pMMDevicePlayCollection;

        t_IMMDevice          *m_pIMMDevice;
        IAudioClient*         m_pAudioClient;
        IAudioCaptureClient*  m_pCaptureClient;
        IAudioRenderClient*   m_pRenderClient;
        HANDLE                m_hEvent;

        WAVEFORMATEX*         m_pwfx;
        int                   m_nFrames;

        HANDLE                m_hTask;
        REFERENCE_TIME        m_hnsDefaultDevicePeriod;
        REFERENCE_TIME        m_phnsMinimumDevicePeriod;
    };
}

#endif
#endif  //_MSC_VER

