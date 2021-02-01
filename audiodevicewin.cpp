#ifdef _MSC_VER

#include "audiodevicewin.h"
#include "mylog.h"
#include "common.h"
#include "sdkerror.h"
#include <functiondiscoverykeys_devpkey.h>
#include <avrt.h>

#pragma comment(lib,"Winmm.lib") 
#pragma comment(lib, "Avrt.lib")

//#define WINMM_CLSID
const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);
const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);

#define EXIT_ON_ERROR(hres)    if (FAILED(hres)) { return false; }
#define SAFE_RELEASE(punk)     if ((punk) != NULL){ (punk)->Release(); (punk) = NULL; }

namespace TOP
{
    CAudioDevice::CAudioDevice()
    {
        m_pMMDeviceCaptureCollection = NULL;
        m_pMMDevicePlayCollection = NULL;
        m_pAudioClient = NULL;
        m_pRenderClient = NULL;
        m_pCaptureClient = NULL;
        m_hEvent = NULL;
        m_hTask = NULL;
        m_pwfx = NULL;
        m_pAudioStream = NULL;
        m_error = 0;
    }

    CAudioDevice::~CAudioDevice()
    {
        Release();
    }
    void CAudioDevice::Release()
    {
        SAFE_RELEASE(m_pMMDeviceCaptureCollection)
        SAFE_RELEASE(m_pMMDevicePlayCollection)
        SAFE_RELEASE(m_pAudioClient)
        SAFE_RELEASE(m_pCaptureClient)
        SAFE_RELEASE(m_pRenderClient)

        if (m_hEvent)
        {
            if(m_pAudioStream->IsRender())
            {
                CancelWaitableTimer(m_hEvent);
            }
            ::CloseHandle(m_hEvent);
            m_hEvent = NULL;
        }
    }
    void CAudioDevice::SetCaptureFrame(int nFrame)
    {
        m_nFrames = nFrame;
    }

    bool CAudioDevice::InitDevice(bool bCapture,CThreadDevice* pMySource)
    {
        m_bCapture = bCapture;
        m_pAudioStream = pMySource;

        if(!InitMediaDevice())
        {
            return false;
        }
        if(!InitFormat())
        {
            return false;
        }
        if(!InitPeriod())
        {
            return false;
        }
        if(!createTimerEvent())
        {
            return false;
        }
        if (m_bCapture)
        {
           //get capture client
           HRESULT hr = m_pAudioClient->GetService(IID_IAudioCaptureClient, (void**)&m_pCaptureClient);
           if (FAILED(hr))
           {
               LOG_ERROR("%s", "m_pAudioClient->GetService")
               m_error = ERROR_AUDIO_DEVICE;
               return false;
           }
        }
        else
        {
           //renderclient
           HRESULT hr = m_pAudioClient->GetService(IID_IAudioRenderClient,(void**)&m_pRenderClient);
           if (FAILED(hr))
           {
               LOG_ERROR("%s", "m_pAudioClient->GetService")
               m_error = ERROR_AUDIO_DEVICE;
               return false;
           }
        }
        return true;
    }

    bool CAudioDevice::StartPlayBack()
    {
        // Get the entire buffer for the initial fill operation.
        unsigned int bufferFrameCount = 0;
        BYTE* pData;
        HRESULT hr = m_pRenderClient->GetBuffer(bufferFrameCount, &pData);
        if (FAILED(hr))
        {
            LOG_ERROR("%s", "m_pRenderClient->GetBuffer")
            m_error = ERROR_AUDIO_DEVICE;
            return false;
        }

        // Load the initial data into the shared buffer.
        bool bLoadData = m_pAudioStream->LoadData(bufferFrameCount, pData);
        if (!bLoadData)
        {
            LOG_ERROR("%s", "pMySource->LoadData first")
            m_error = ERROR_AUDIO_DEVICE;
            return false;
        }

        DWORD flags = 0;
        hr = m_pRenderClient->ReleaseBuffer(bufferFrameCount, flags);
        if (FAILED(hr))
        {
            LOG_ERROR("%s", "m_pRenderClient->ReleaseBuffer")
            m_error = ERROR_AUDIO_DEVICE;
            return false;
        }

        // Start playing.
        hr = m_pAudioClient->Start();
        if (FAILED(hr))
        {
            LOG_ERROR("%s", "m_pAudioClient->Start")
            m_error = ERROR_AUDIO_DEVICE;
            return false;
        }

        LoopPlayBack();

        m_pAudioClient->Stop();
        if (FAILED(hr))
        {
            LOG_ERROR("%s", "m_pAudioClient->Stop")
            m_error = ERROR_AUDIO_DEVICE;
            return false;
        }
        return true;
    }

    bool CAudioDevice::StartCapture()
    {
        // Ask MMCSS to temporarily boost the thread priority
        // to reduce glitches while the low-latency stream plays.
        DWORD taskIndex = 0;
        m_hTask = AvSetMmThreadCharacteristics(TEXT("Pro Audio"), &taskIndex);
        if (m_hTask == NULL)
        {
           return false;
        }

        // Start recording.
        HRESULT hr = m_pAudioClient->Start();
        if (SUCCEEDED(hr))
        {
            LoopCapture();
            m_pAudioClient->Stop();
        }
        else
        {
            LOG_ERROR("%s", "m_pAudioClient->Start")
            m_error = ERROR_AUDIO_DEVICE;
        }

        if (m_hTask != NULL)
        {
            AvRevertMmThreadCharacteristics(m_hTask);
            m_hTask = NULL;
        }
        return true;
    }

    bool CAudioDevice::InitFormat()
    {
        HRESULT hr = m_pAudioClient->GetMixFormat(&m_pwfx);
        if (FAILED(hr))
        {
            LOG_ERROR("%s", "m_pAudioClient->GetMixFormat")
            return false;
        }
        m_pAudioStream->m_nChannel = m_pwfx->nChannels;
        m_pAudioStream->m_nSampleRate = m_pwfx->nSamplesPerSec;
        m_pAudioStream->m_nBitsPerSample = m_pwfx->wBitsPerSample;
        /*
        //default 48000,16bit
        for(int k = 1;k<=2;k++)
        {
            int nSampleRate = (k==1) ? 48000:44100;
            for(int i = 1;i<=2;i++)
            {
                int nChannel = (i==1)?2:1;
                for(int j = 1;j<=2;j++)
                {
                    int nBitSample = j*16;
                    if(SetFormat(nChannel,nSampleRate,nBitSample))
                    {
                        return true;
                    }
                }
            }
        }
        */
        return true;
    }
    bool CAudioDevice::SetFormat(int nChannel,int nSampleRate,int nBitSample)
    {
        //set format
        m_pwfx->cbSize = 0;
        m_pwfx->wFormatTag = WAVE_FORMAT_PCM;
        m_pwfx->nChannels = nChannel;
        m_pwfx->nSamplesPerSec = nSampleRate;
        m_pwfx->wBitsPerSample = nBitSample;
        m_pwfx->nBlockAlign = (m_pwfx->wBitsPerSample * m_pwfx->nChannels) / 8;
        m_pwfx->nAvgBytesPerSec = m_pwfx->nBlockAlign * m_pwfx->nSamplesPerSec;

        m_pAudioStream->m_nChannel = nChannel;
        m_pAudioStream->m_nSampleRate = nSampleRate;
        m_pAudioStream->m_nBitsPerSample = nBitSample;

        WAVEFORMATEX* pFormat = NULL;
        HRESULT hr = m_pAudioClient->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED, m_pwfx, &pFormat);
        if (FAILED(hr))
        {
            LOG_ERROR("%s", "m_pAudioClient->IsFormatSupported")
            return false;
        }
        if(pFormat==NULL)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool CAudioDevice::InitPeriod()
    {
        HRESULT hr = m_pAudioClient->GetDevicePeriod(
                    &m_hnsDefaultDevicePeriod,&m_phnsMinimumDevicePeriod);
        if (FAILED(hr))
        {
            LOG_ERROR("%s", "m_pAudioClient->GetDevicePeriod")
            return false;
        }
        if(m_bCapture && m_pAudioStream->IsRender())
        {
            //capture the playback
            hr = m_pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
                AUDCLNT_STREAMFLAGS_LOOPBACK,
                m_hnsDefaultDevicePeriod,
                0,
                m_pwfx,
                NULL);

            if (FAILED(hr))
            {
                LOG_ERROR("capture the playback, m_pAudioClient->Initialize:%d", hr)
                return false;
            }
        }
        else
        {
            hr = m_pAudioClient->Initialize(
                        AUDCLNT_SHAREMODE_SHARED,
                        AUDCLNT_STREAMFLAGS_EVENTCALLBACK|AUDCLNT_STREAMFLAGS_NOPERSIST,
                        m_hnsDefaultDevicePeriod,
                        0,
                        m_pwfx,
                        NULL);

            switch (hr)
            {
            case S_OK:
                break;

            case AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED:
                // skip this case because it is not an error
                break;

            default:
                LOG_ERROR("capture mic m_pAudioClient->Initialize:%d", GetLastError())
                return false;
            }
        }
        return true;
    }

    bool CAudioDevice::LoopCapture()
    {
        unsigned int numFramesAvailable;

        HRESULT hr = S_FALSE;
        DWORD flags = 0;
        BYTE* pData;

        while (true)
        {
            if (m_pAudioStream->IsStop())
            {
                break;
            }

            ::WaitForSingleObject(m_hEvent, 20);

            // Get the available data in the shared buffer.
            hr = m_pCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, NULL, NULL);
            EXIT_ON_ERROR(hr)

			// Copy the available capture data to the audio sink.
            if(numFramesAvailable>0)
            {
                m_pAudioStream->CopyData((unsigned char*)pData, numFramesAvailable);
            }
            hr = m_pCaptureClient->ReleaseBuffer(numFramesAvailable);
            EXIT_ON_ERROR(hr)
        }
        return true;
    }

    bool CAudioDevice::LoopPlayBack()
    {
        HRESULT hr = S_FALSE;
        DWORD flags = 0;
        BYTE* pData;

        unsigned int bufferFrameCount;
        hr = m_pAudioClient->GetBufferSize(&bufferFrameCount);
        EXIT_ON_ERROR(hr)
        int frameLen = m_pAudioStream->m_nChannel * (PCM_FRAME_SAMPLE_DEEP / 8);

        while (true)
        {
            if (m_pAudioStream->IsStop())
            {
                break;
            }
            ::WaitForSingleObject(m_hEvent, 1000);

            // See how much buffer space is available.
            unsigned int numFramesPadding;
            hr = m_pAudioClient->GetCurrentPadding(&numFramesPadding);
            EXIT_ON_ERROR(hr)

            unsigned int numFramesAvailable = bufferFrameCount - numFramesPadding;

            // Grab all the available space in the shared buffer.
            hr = m_pRenderClient->GetBuffer(numFramesAvailable, &pData);
            EXIT_ON_ERROR(hr)

            //load data from the audio source.
            memset(pData, 0, numFramesAvailable * frameLen);
            bool bLoadData = m_pAudioStream->LoadData(numFramesAvailable, pData);

            //all data have been play
            if (!bLoadData)
            {
                break;
            }
            hr = m_pRenderClient->ReleaseBuffer(numFramesAvailable, flags);
            EXIT_ON_ERROR(hr)
        }
        return true;
    }
    bool CAudioDevice::InitMediaDevice()
    {
        std::string strID = m_pAudioStream->GetDeviceName();
        std::wstring str = Common::utf8tounicode(strID.c_str());
        if(m_bCapture)
        {
            m_pIMMDevice = CSoundDevice::GetInstance()->GetCaptureDevice(str);
        }
        else
        {
            m_pIMMDevice = CSoundDevice::GetInstance()->GetPlayDevice(str);
        }
        if(m_pIMMDevice==NULL)
        {
            LOG_ERROR("%s","m_pIMMDevice==NULL.");
            return false;
        }

        HRESULT hr = m_pIMMDevice->pIMMDevice->Activate(
                        IID_IAudioClient,CLSCTX_ALL,NULL,(void**)&m_pAudioClient);

        if (FAILED(hr))
        {
            LOG_ERROR("%s", "m_pDevice->Activate")
            return false;
        }
        return true;
    }
    bool CAudioDevice::createTimerEvent()
    {
        if(m_bCapture && m_pAudioStream->IsRender())
        {
            //capture the playback depends on the media timmer.
            m_hEvent = CreateWaitableTimer(NULL, FALSE, NULL);
            if(m_hEvent == NULL)
            {
                LOG_ERROR("%s", "CreateWaitableTimer Error.")
                return false;
            }
            LARGE_INTEGER liFirstFire;
            liFirstFire.QuadPart = -m_hnsDefaultDevicePeriod / 2; // negative means relative time
            BOOL bOK = SetWaitableTimer(m_hEvent,&liFirstFire,10,NULL, NULL, FALSE);
            if(!bOK)
            {
                LOG_ERROR("%s", "SetWaitableTimer Error.")
                return false;
            }
        }
        else
        {
            m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
            HRESULT hr = m_pAudioClient->SetEventHandle(m_hEvent);
            if (FAILED(hr))
            {
               LOG_ERROR("%s", "m_pAudioClient->SetEventHandle")
               m_error = ERROR_AUDIO_DEVICE;
               return false;
            }
        }
        return true;
    }
}

#endif  //_MSC_VER


