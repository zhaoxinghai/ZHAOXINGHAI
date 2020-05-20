#ifdef _MSC_VER


#include "audiodevicewin.h"
#include "mylog.h"
#include "common.h"
#include "sdkerror.h"

#pragma comment(lib,"Winmm.lib") 

#define WINMM_CLSID \
const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator); \
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);   \
const IID IID_IAudioClient = __uuidof(IAudioClient);                 \
const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);     \
const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);

#define EXIT_ON_ERROR(hres)    if (FAILED(hres)) { return false; }
#define SAFE_RELEASE(punk)     if ((punk) != NULL){ (punk)->Release(); (punk) = NULL; }
#define REFTIMES_PER_SEC       10000000
#define REFTIMES_PER_MILLISEC  10000

CAudioDevice::CAudioDevice()
{
    m_pEnumerator = NULL;
    m_pDevice = NULL;
    m_pAudioClient = NULL;
    m_pRenderClient = NULL;
    m_pCaptureClient = NULL;
    m_hEvent = NULL;
    m_pwfx = NULL;
    m_pAudioStream = NULL;
    m_error = 0;
}

CAudioDevice::~CAudioDevice()
{
    /*
    if (m_pwfx)
    {
        CoTaskMemFree(m_pwfx);
    }
    SAFE_RELEASE(m_pEnumerator);
    SAFE_RELEASE(m_pDevice);
    SAFE_RELEASE(m_pAudioClient);
    SAFE_RELEASE(m_pCaptureClient);
    SAFE_RELEASE(m_pRenderClient);

    if (m_hEvent)
    {
        ::CloseHandle(m_hEvent);
        m_hEvent = NULL;
    }
    */
}

bool CAudioDevice::InitDevice(bool bCapture,int nChannel,int nBitsPerSamp)
{
    WINMM_CLSID

    m_bCapture = bCapture;
    m_nChannel = nChannel;
    m_nBitsPerSamp = nBitsPerSamp;

    if (!InitAudioClient())
    {
        LOG_ERROR("%s", "InitAudioClient");
        return false;
    }
    if (!InitPeriod())
    {
        LOG_ERROR("%s", "InitPeriod");
        return false;
    }

    //set event
    m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    HRESULT hr = m_pAudioClient->SetEventHandle(m_hEvent);
    if (FAILED(hr))
    {
        LOG_ERROR("%s", "m_pAudioClient->SetEventHandle");
        m_error = ERROR_AUDIO_DEVICE;
        return false;
    }

    if(m_bCapture)
    {
        //get capture client
        hr = m_pAudioClient->GetService(IID_IAudioCaptureClient,(void**)&m_pCaptureClient);
        if (FAILED(hr))
        {
            LOG_ERROR("%s", "m_pAudioClient->GetService");
            m_error = ERROR_AUDIO_DEVICE;
            return false;
        }
    }
    else
    {
        //renderclient
        hr = m_pAudioClient->GetService(
            IID_IAudioRenderClient,
            (void**)&m_pRenderClient);
        if (FAILED(hr))
        {
            LOG_ERROR("%s", "m_pAudioClient->GetService");
            m_error = ERROR_AUDIO_DEVICE;
            return false;
        }
    }
    return true;
}

bool CAudioDevice::StartPlayBack(CThreadDevice * pMySource)
{
    HRESULT hr;
    m_pAudioStream = pMySource;

    // Get the entire buffer for the initial fill operation.
    unsigned int bufferFrameCount = 0;
    BYTE *pData;
    hr = m_pRenderClient->GetBuffer(bufferFrameCount, &pData);
    if (FAILED(hr))
    {
        LOG_ERROR("%s", "m_pRenderClient->GetBuffer");
        m_error = ERROR_AUDIO_DEVICE;
        return false;
    }
   
    // Load the initial data into the shared buffer.
    bool bLoadData = pMySource->LoadData(bufferFrameCount, pData);
    if (!bLoadData)
    {
        LOG_ERROR("%s", "pMySource->LoadData first");
        m_error = ERROR_AUDIO_DEVICE;
        return false;
    }

    DWORD flags = 0;
    hr = m_pRenderClient->ReleaseBuffer(bufferFrameCount, flags);
    if (FAILED(hr))
    {
        LOG_ERROR("%s", "m_pRenderClient->ReleaseBuffer");
        m_error = ERROR_AUDIO_DEVICE;
        return false;
    }

    // Start playing.
    hr = m_pAudioClient->Start();
    if (FAILED(hr))
    {
        LOG_ERROR("%s", "m_pAudioClient->Start");
        m_error = ERROR_AUDIO_DEVICE;
        return false;
    }

    LoopPlayBack();

    m_pAudioClient->Stop();
    if (FAILED(hr))
    {
        LOG_ERROR("%s", "m_pAudioClient->Stop");
        m_error = ERROR_AUDIO_DEVICE;
        return false;
    }
    return true;
}

bool CAudioDevice::StartCapture(CThreadDevice *pMyDest)
{
    m_pAudioStream = pMyDest;

    // Start recording.
    HRESULT hr = m_pAudioClient->Start();
    if (FAILED(hr))
    {
        LOG_ERROR("%s", "m_pAudioClient->Start");
        m_error = ERROR_AUDIO_DEVICE;
        return false;
    }

    LoopCapture();
   
    // Stop capture.
    hr = m_pAudioClient->Stop(); 
    return true;
}

bool CAudioDevice::InitAudioClient()
{
    WINMM_CLSID
    //DeviceEnumerator
    HRESULT hr = CoCreateInstance(
        CLSID_MMDeviceEnumerator,
        NULL,
        CLSCTX_ALL,
        IID_IMMDeviceEnumerator,
        (void**)&m_pEnumerator);
    if (FAILED(hr))
    {
        LOG_ERROR("%s", "CLSID_MMDeviceEnumerator-CoCreateInstance");
        return false;
    }

    //device
    hr = m_pEnumerator->GetDefaultAudioEndpoint(
        m_bCapture?eCapture:eRender, eConsole, &m_pDevice);

    if (FAILED(hr))
    {
        LOG_ERROR("%s", "m_pEnumerator->GetDefaultAudioEndpoint");
        return false;
    }

    //audio client
    hr = m_pDevice->Activate(
        IID_IAudioClient,
        CLSCTX_ALL,
        NULL,
        (void**)&m_pAudioClient);
    if (FAILED(hr))
    {
        LOG_ERROR("%s", "m_pDevice->Activate");
        return false;
    }

    hr = m_pAudioClient->GetMixFormat(&m_pwfx);
    if (FAILED(hr))
    {
        LOG_ERROR("%s", "m_pAudioClient->GetMixFormat");
        return false;
    }

    return true;
}

bool CAudioDevice::InitPeriod()
{
    //set format
    m_pwfx->cbSize = 0;
    m_pwfx->wFormatTag = WAVE_FORMAT_PCM;
    m_pwfx->nChannels = m_nChannel;
    m_pwfx->nSamplesPerSec = SAMPLING_RATE;
    m_pwfx->wBitsPerSample = m_nBitsPerSamp;
    m_pwfx->nBlockAlign = (m_pwfx->wBitsPerSample*m_pwfx->nChannels) / 8;
    m_pwfx->nAvgBytesPerSec = m_pwfx->nBlockAlign*m_pwfx->nSamplesPerSec;

    WAVEFORMATEX *pFormat = NULL;
    HRESULT hr = m_pAudioClient->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED,m_pwfx,&pFormat);
    if(FAILED(hr))
    {
        LOG_ERROR("%s", "m_pAudioClient->IsFormatSupported");
        return false;
    }

    //min period
    REFERENCE_TIME periodMin;
    hr = m_pAudioClient->GetDevicePeriod(NULL, &periodMin);
    if (FAILED(hr))
    {
        LOG_ERROR("%s", "m_pAudioClient->GetDevicePeriod");
        return false;
    }
 
    //160 sample's period
    REFERENCE_TIME periodRequest = REFTIMES_PER_SEC*160/SAMPLING_RATE;
    if (periodRequest < periodMin)
    {
        periodRequest = periodMin;
    }

    AUDCLNT_SHAREMODE aShare = m_bCapture ? AUDCLNT_SHAREMODE_EXCLUSIVE : AUDCLNT_SHAREMODE_SHARED;
    hr = m_pAudioClient->Initialize(
        aShare,
        AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
        periodRequest,
        periodRequest,
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
        LOG_ERROR("%s", "m_pAudioClient->Initialize");
        return false;
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

        ::WaitForSingleObject(m_hEvent, 1000);

        // Get the available data in the shared buffer.
        hr = m_pCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, NULL, NULL);
        EXIT_ON_ERROR(hr)

        // Copy the available capture data to the audio sink.
        bool bCopy = m_pAudioStream->CopyData((unsigned char*)pData, numFramesAvailable);
        if (!bCopy)
        {
            return false;
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
    BYTE *pData;

    unsigned int bufferFrameCount;
    hr = m_pAudioClient->GetBufferSize(&bufferFrameCount);
    EXIT_ON_ERROR(hr);
    int frameLen = m_nChannel*m_nBitsPerSamp/8;
    
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
        memset(pData, 0, numFramesAvailable*frameLen);
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

///////////////////////////////////////////////////////////////

CSound* CSound::g_Sound = new CSound();  //checked
CSound::CSound()
{

}

CSound::~CSound()
{

}

CSound* CSound::GetInstance()
{
    return g_Sound;
}

void CSound::Release()
{
    if(g_Sound != NULL)
    {
        delete g_Sound;
        g_Sound = NULL;
    }
}

void CSound::InitVolume()
{

}

#endif  //_MSC_VER


