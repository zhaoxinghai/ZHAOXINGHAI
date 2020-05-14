
#include "noise.h"

CNoise::CNoise()
{
#ifndef _MSC_VER
    m_SpeexState = NULL;
#endif
}

CNoise::~CNoise()
{
#ifndef _MSC_VER
    if(m_SpeexState != NULL)
    {
        speex_preprocess_state_destroy(m_SpeexState);
        m_SpeexState = NULL;
    }
#endif
}

void CNoise::Init()
{
#ifndef _MSC_VER
    m_SpeexState = speex_preprocess_state_init(418, 48000);

    int denoise = 1;
    int noiseSuppress = -25;
    speex_preprocess_ctl(m_SpeexState, SPEEX_PREPROCESS_SET_DENOISE, &denoise);
    speex_preprocess_ctl(m_SpeexState, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noiseSuppress);

    int i;
    i = 0;
    speex_preprocess_ctl(m_SpeexState, SPEEX_PREPROCESS_SET_AGC, &i);
    i = 80000;
    speex_preprocess_ctl(m_SpeexState, SPEEX_PREPROCESS_SET_AGC_LEVEL, &i);
    i = 0;
    speex_preprocess_ctl(m_SpeexState, SPEEX_PREPROCESS_SET_DEREVERB, &i);
    float f = 0;
    speex_preprocess_ctl(m_SpeexState, SPEEX_PREPROCESS_SET_DEREVERB_DECAY, &f);
    f = 0;
    speex_preprocess_ctl(m_SpeexState, SPEEX_PREPROCESS_SET_DEREVERB_LEVEL, &f);
#endif
}

void CNoise::Exit()
{
#ifndef _MSC_VER
    if(m_SpeexState != NULL)
    {
        speex_preprocess_state_destroy(m_SpeexState);
        m_SpeexState = NULL;
    }
#endif
}

void CNoise::Reduce(unsigned char *pInData,int nFrame,short nNoise)
{
    short* pdata = (short*)pInData;

    for (int i=0; i < nFrame; i++)
    {
        if(pdata[i] > nNoise)
        {
            pdata[i] = pdata[i]- nNoise;
        }
        else if (pdata[i] < -nNoise)
        {
            pdata[i] = pdata[i] + nNoise;
        }
        else
        {
            pdata[i] = 0;
        }
    }
}

void CNoise::Reduce2(unsigned char *pInData)
{
#ifndef _MSC_VER
    if(m_SpeexState != NULL)
    {
        speex_preprocess_run(m_SpeexState, (spx_int16_t*)pInData);
    }
#endif
}
