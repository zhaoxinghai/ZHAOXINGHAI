
#ifndef _MSC_VER

#include "audiodevicealsa.h"
#include "audiojob.h"
#include "log.h"
#include "common.h"
#include "service.h"
#include "interface.h"
#include "public.h"

CAudioDevice::CAudioDevice()
{
    m_pHandle = NULL;
    m_bufferFrames = 0;
    m_peroidFrames = 0;
    m_pAudioStream = NULL;
    m_error = 0;
}

CAudioDevice::~CAudioDevice()
{
}

bool CAudioDevice::StartPlayBack(CThreadDevice *pMySource)
{
    m_pAudioStream = pMySource;

    LoopPlayBack();
    return true;
}

bool CAudioDevice::StartCapture(CThreadDevice *pMyDest)
{
    m_pAudioStream = pMyDest;

    LoopCapture();

    return true;
}

bool CAudioDevice::LoopCapture()
{
    char Data[1024 * 10];
    int nErrorCount = 0;

    while (true)
    {
        if (m_pAudioStream->IsStop())
        {
            break;
        }
        snd_pcm_sframes_t frames = snd_pcm_readi(m_pHandle, &Data[0], ED1AudioFrameSampleCount);
        if (frames == -EPIPE)
        {
            //EPIPE means overrun
            snd_pcm_prepare(m_pHandle);
            if(++nErrorCount > 10)
                break;
        }
        else if (frames < 0)
        {
            if(++nErrorCount > 10)
                break;
        }
        else
        {
            nErrorCount = 0;
        	//Copy the available capture data to the audio sink.
        	if(!m_pAudioStream->CopyData((unsigned char*)&Data[0], (int)frames))
        		break;
        }
    }
    return true;
}

bool CAudioDevice::LoopPlayBack()
{
    int frameSize = m_pAudioStream->GetChannel()*m_pAudioStream->GetBitsPerSample() / 8;
    int len = m_peroidFrames*frameSize;
    unsigned char Data[1024 * 10];
    int nErrorCount = 0;

    while (true)
    {
        if (m_pAudioStream->IsStop())
        {
            break;
        }
        //load data buffer
        memset(&Data[0], 0, len);
        bool bLoadData = m_pAudioStream->LoadData(m_peroidFrames, &Data[0]);
        if (!bLoadData)
        {
            break;
        }

        snd_pcm_sframes_t rc = snd_pcm_writei(m_pHandle, &Data[0], m_peroidFrames);

        if (rc == -EPIPE)
        {
            //EPIPE means underrun
            snd_pcm_prepare(m_pHandle);
            nErrorCount++;
        }
        else if (rc < 0)
        {
            nErrorCount++;
        }
        else if (rc != (snd_pcm_sframes_t)m_peroidFrames)
        {
            nErrorCount++;
        }
        else
        {
            nErrorCount = 0;
        }
        if(nErrorCount > 10)
        {
            break;
        }
    }
    return false;
}

bool CAudioDevice::InitDevice(bool bCapture,int nChannel,int nBitsPerSamp)
{
    m_bCapture = bCapture;
    m_nChannel = nChannel;
    m_nBitsPerSamp = nBitsPerSamp;

    int err;
    snd_pcm_hw_params_t *hwparams;
    snd_pcm_hw_params_alloca(&hwparams);
    
    if ((err = snd_pcm_open(&m_pHandle, "default",
    		m_bCapture?SND_PCM_STREAM_CAPTURE:SND_PCM_STREAM_PLAYBACK, 0)) < 0)
    {
        LOG_ERROR("%s", "snd_pcm_open");
        return false;
    }

    if ((err = set_hwparams(m_pHandle, hwparams)) < 0)
    {
    	LOG_ERROR("%s", "Setting of hwparams");
    	snd_pcm_close(m_pHandle);
        return false;
    }
    return true;
}

int CAudioDevice::set_hwparams(
    snd_pcm_t *handle,
    snd_pcm_hw_params_t *params)
{
    int err;
    int dir = 0;

    //fill parameters with default values
    err = snd_pcm_hw_params_any(handle, params);
    if (err < 0)
    {
    	LOG_ERROR("%s", "snd_pcm_hw_params_any");
        return err;
    }

    //set the interleaved read/write format
    err = snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err < 0)
    {
    	LOG_ERROR("%s", "snd_pcm_hw_params_set_access");
        return err;
    }

    //set the sample format 16 bit little-endian
    snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;
    if(m_nBitsPerSamp == 24)
    {
        format = SND_PCM_FORMAT_S24_LE;
    }
    err = snd_pcm_hw_params_set_format(handle, params,format);
    if (err < 0)
    {
    	LOG_ERROR("%s", "snd_pcm_hw_params_set_format");
        return err;
    }

    //set the count of channels
    err = snd_pcm_hw_params_set_channels(handle, params, m_nChannel);
    if (err < 0)
    {
    	LOG_ERROR("%s", "snd_pcm_hw_params_set_channels");
        return err;
    }

    //set the sampling rate
    unsigned int rate = SAMPLING_RATE;
    err = snd_pcm_hw_params_set_rate_near(handle, params, &rate, &dir);
    if (err < 0)
    {
    	LOG_ERROR("%s", "snd_pcm_hw_params_set_rate_near");
        return err;
    }

    //set the period size
    snd_pcm_uframes_t period = 418;
    err = snd_pcm_hw_params_set_period_size_near(handle, params, &period, &dir);
    if (err < 0)
    {
        LOG_ERROR("%s", "snd_pcm_hw_params_set_period_time_near");
        return err;
    }
 
    //set buffer size
    snd_pcm_uframes_t bufsize = 418*2;
    err = snd_pcm_hw_params_set_buffer_size_near(handle, params, &bufsize);
    if (err < 0)
    {
        LOG_ERROR("%s", "snd_pcm_hw_params_set_buffer_size_near");
        return err;
    }
  
    //write the parameters to device
    err = snd_pcm_hw_params(handle, params);
    if (err < 0)
    {
    	LOG_ERROR("%s", "Unable to set hw params");
        return err;
    }

    //get one period
    snd_pcm_hw_params_get_period_size(params, &m_peroidFrames,&dir);
    if(m_bCapture)
    {
        //LOG_DEBUG("alsa capture period frames:%d", m_peroidFrames);
    }
    else
    {
        //LOG_DEBUG("alsa playback period frames:%d", m_peroidFrames);
    }
    //get buffer size
    snd_pcm_hw_params_get_buffer_size(params, &m_bufferFrames);

    if(m_bCapture)
    {
        //LOG_DEBUG("alsa capture buffer frames:%d", m_bufferFrames);
    }
    else
    {
        //LOG_DEBUG("alsa playback buffer frames:%d", m_bufferFrames);
    }
    return 0;
}

CSound* CSound::g_Sound = new CSound();  //checked
CSound::CSound()
{
    m_mixerPlay = NULL;
    m_mixerCapture = NULL;

    m_element_ADC_Level = NULL;
    m_element_PGA_Level = NULL;
    m_element_PCM = NULL;
}

CSound::~CSound()
{
    snd_mixer_close(m_mixerPlay);
    snd_mixer_close(m_mixerCapture);
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
    snd_mixer_open(&m_mixerPlay, SND_PCM_STREAM_PLAYBACK);
    snd_mixer_attach(m_mixerPlay, "default");
    snd_mixer_selem_register(m_mixerPlay, NULL, NULL);
    snd_mixer_load(m_mixerPlay);

    //find pcm element
    snd_mixer_elem_t *element = snd_mixer_first_elem(m_mixerPlay);
    while(element)
    {
        if ( strcmp("PCM", snd_mixer_selem_get_name (element)) == 0)
        {
            m_element_PCM = element;
            snd_mixer_selem_set_playback_volume(m_element_PCM,SND_MIXER_SCHN_FRONT_LEFT,PCM_LEFT);
            snd_mixer_selem_set_playback_volume(m_element_PCM,SND_MIXER_SCHN_FRONT_RIGHT,PCM_RIGHT);
            break;
        }
        element = snd_mixer_elem_next(element);
    }

    //capture
    snd_mixer_open(&m_mixerCapture, SND_PCM_STREAM_CAPTURE);
    snd_mixer_attach(m_mixerCapture, "default");
    snd_mixer_selem_register(m_mixerCapture, NULL, NULL);
    snd_mixer_load(m_mixerCapture);

    //find pcm element
    element = snd_mixer_first_elem(m_mixerCapture);
    while(element)
    {
        if ( strcmp( "ADC Level", snd_mixer_selem_get_name (element)) == 0)
        {
            m_element_ADC_Level = element;
            snd_mixer_selem_set_capture_volume(m_element_ADC_Level,SND_MIXER_SCHN_FRONT_LEFT,ADC_LEVEL_LEFT);
            snd_mixer_selem_set_capture_volume(m_element_ADC_Level,SND_MIXER_SCHN_FRONT_RIGHT,ADC_LEVEL_RIGHT);
        }
        else if ( strcmp( "PGA Level", snd_mixer_selem_get_name (element)) == 0)
        {
            m_element_PGA_Level = element;
            snd_mixer_selem_set_capture_volume(m_element_PGA_Level,SND_MIXER_SCHN_FRONT_LEFT,PGA_LEVEL_LEFT);
            snd_mixer_selem_set_capture_volume(m_element_PGA_Level,SND_MIXER_SCHN_FRONT_RIGHT,PGA_LEVEL_RIGHT);
        }
        element = snd_mixer_elem_next(element);
    }

    if(m_element_PCM == NULL || m_element_ADC_Level== NULL || m_element_PGA_Level == NULL)
    {
        LOG_ERROR("%s","CSound::InitVolume Error");
    }
}

#endif  //_MSC_VER



