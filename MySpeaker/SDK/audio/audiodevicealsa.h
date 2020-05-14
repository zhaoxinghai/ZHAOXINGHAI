
#ifndef _MSC_VER

#ifndef AUDIO_LINUX_H
#define AUDIO_LINUX_H

#include "predefine.h"
#include "audiodevice.h"
#include <alsa/asoundlib.h>

#define ADC_LEVEL_LEFT        (24)
#define ADC_LEVEL_RIGHT       (24)
#define PGA_LEVEL_LEFT        0
#define PGA_LEVEL_RIGHT       0

#define PCM_LEFT              127
#define PCM_RIGHT             127

class CSound
{
public:
    CSound();
    ~CSound();

    static CSound* GetInstance();
    static void Release();

    void InitVolume();
    void SetMicVolume(float fDB);

private:
    static CSound*        g_Sound;

    snd_mixer_elem_t     *m_element_PCM;
    snd_mixer_t          *m_mixerPlay;

    snd_mixer_t          *m_mixerCapture;
    snd_mixer_elem_t     *m_element_ADC_Level;
    snd_mixer_elem_t     *m_element_PGA_Level;
};

class CAudioDevice : public CAudioDeviceBase
{
public:
    CAudioDevice();
    virtual ~CAudioDevice();

    virtual bool StartPlayBack(CThreadDevice *pMySource);
    virtual bool StartCapture(CThreadDevice *pMyDest);
    virtual bool LoopCapture();
    virtual bool LoopPlayBack();

    bool InitDevice(bool bCapture,int nChannel,int nBitsPerSamp);

protected:
    int set_hwparams(snd_pcm_t *handle,snd_pcm_hw_params_t *params);

    snd_pcm_t             *m_pHandle;
    snd_pcm_uframes_t      m_peroidFrames;
    snd_pcm_uframes_t      m_bufferFrames;
};

#endif

#endif  //_MSC_VER

