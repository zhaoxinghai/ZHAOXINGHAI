
#ifndef AUDIO_DEF_H
#define AUDIO_DEF_H

#include <fstream>
#include "d1adpcmcodec.h"
#include "cyclebuffer.h"

#ifndef _MSC_VER
//#include "speex/speex_resampler.h"
#endif

enum e_AUDIO_TYPE
{
    AUDIO_ADP,
    AUDIO_WAV
};

class CAudioFile
{
public:
    CAudioFile();
    ~CAudioFile();

    bool Open(std::string strPath);
    bool IsAdpEnough();
    void ReadAdpFrame(unsigned char* pData);
    void ReadAdpSilence(unsigned char* pData);

    static float GetSampleF(double sample);
    static short GetSampleS(long sample);

    //c apture sample to float
    static void S16Bit2Float(unsigned char src[], float dest[], int nFrames);
    static void S32Bit2Float(unsigned char src[], float dest[], int nFrames);

protected:

    bool CheckAudioType(std::string strPath);
    void AudioDecode();

    void DecodeWavFile();
    void Decode2Wav();

    void DecodeTo48kSample();
    void EncodeToAdp();

    //wav sample to 32 bit
    float Sample8BitTo32Bit(unsigned char src[], bool bMono);
    float SampleS16BitTo32Bit(unsigned char src[], bool bMono);
    float SampleS24BitTo32Bit(unsigned char src[], bool bMono);
    float SampleS32BitTo32Bit(unsigned char src[], bool bMono);

    VpAudioCore::D1AudioAdpcmDecoder   m_audioDecoder;
    VpAudioCore::D1AudioAdpcmEncoder   m_audioEncoder;

    e_AUDIO_TYPE    m_type;
    std::ifstream   m_hFile;
    int             m_nRemain;
    int             m_nChannel;
    int             m_nSamplesPerSec;
    int             m_nBitsPerSample;
    long int        m_q;

    CCycleBuffer    m_AdpBuffer;
    CCycleBuffer    m_PCMBuffer;
    CCycleBuffer    m_PCM48KBuffer;
    CCycleBuffer   *m_pBuf;

#ifndef _MSC_VER
    //SpeexResamplerState* m_st;
#endif
};

#endif
