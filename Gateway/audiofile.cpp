
#include "audiofile.h"
#include "public.h"
#include "common.h"

CAudioFile::CAudioFile()
{
    m_type = AUDIO_ADP;
    m_nRemain = 0;
    m_q = 0;
    m_nChannel = 1;
    m_nSamplesPerSec = 0;
    m_nBitsPerSample = 0;
    m_pBuf = NULL;

    m_PCMBuffer.SetSize(4*192000*0.2);                  //200 mini seconds
    m_PCM48KBuffer.SetSize(10*4*ED1AudioAdpFrameCount); //10 frames
    m_AdpBuffer.SetSize(10*ED1AudioAdpFrameCount);      //10 frames

#ifndef _MSC_VER
    //m_st = NULL;
#endif
}

CAudioFile::~CAudioFile()
{
    m_hFile.close();

#ifndef _MSC_VER
    /*
    if(m_st != NULL)
    {
        speex_resampler_destroy(m_st);
        m_st = NULL;
    }
    */
#endif
}

float CAudioFile::GetSampleF(double sample)
{
    if (sample > 1.0f)
    {
        return 1.0f;
    }
    else if (sample < -1.0f)
    {
        return -1.0f;
    }
    else
    {
        return (float)sample;
    }
}

short CAudioFile::GetSampleS(long sample)
{
    if (sample > 32767)
    {
        return 32767;
    }
    else if (sample < -32768)
    {
        return -32768;
    }
    else
    {
        return (short)sample;
    }
}

void CAudioFile::S16Bit2Float(unsigned char src[], float dest[], int nFrames)
{
    short *p = (short*)&src[0];
    float scale = 32768.0f;
    for (int i = 0; i < nFrames; i++)
    {
        dest[i] = p[i] / scale;
    }
}

void CAudioFile::S32Bit2Float(unsigned char src[], float dest[], int nFrames)
{
    int *p = (int*)&src[0];
    float scale = 2147483648.0f;
    for (int i = 0; i < nFrames; i++)
    {
        dest[i] = p[i] / scale;
    }
}

float CAudioFile::Sample8BitTo32Bit(unsigned char src[], bool bMono)
{
    //8 bit wave is unsigned
    long int sample = 0;
    if(bMono)
    {
        sample = src[0] - 0x80;
    }
    else
    {
        sample = (src[0] + src[1])/2-0x80;
    }
    return sample/127.0f;
}

float CAudioFile::SampleS16BitTo32Bit(unsigned char src[], bool bMono)
{
    long int sample = 0;
    short* p = (short*)src;
    if(bMono)
    {
        sample = p[0];
    }
    else
    {
        sample = p[0]/2 + p[1]/2;
    }
    return sample/32768.0f;
}

float CAudioFile::SampleS24BitTo32Bit(unsigned char src[], bool bMono)
{
    long sample = 0;
    if(bMono)
    {
        sample = ((long)src[0]<<8) | ((long)src[1]<<16) | ((long)src[2]<<24);
    }
    else
    {
        long sample1 = ((long)src[0]<<8) | ((long)src[1]<<16) | ((long)src[2]<<24);
        long sample2 = ((long)src[3]<<8) | ((long)src[4]<<16) | ((long)src[5]<<24);
        sample = sample1/2 + sample2/2;
    }
    return sample/2147483648.0f;
}

float CAudioFile::SampleS32BitTo32Bit(unsigned char src[], bool bMono)
{
    long sample = 0;
    int* p = (int*)src;

    if(bMono)
    {
        sample = p[0];
    }
    else
    {
        sample = p[0]/2 + p[1]/2;
    }
    return sample/2147483648.0f;
}

bool CAudioFile::CheckAudioType(std::string strPath)
{
    size_t pos = strPath.rfind(".");
    if (pos == std::string::npos)
    {
        return false;
    }

    std::string strExt = strPath.substr(pos, strPath.length() - pos);
    std::string strTmp = CCommon::strToLower(strExt);
    if (strTmp == ".adp")
    {
        m_type = AUDIO_ADP;
    }
    else if(strTmp == ".wav")
    {
        m_type = AUDIO_WAV;
    }
    else
    {
        return false;
    }
    return true;
}

bool CAudioFile::Open(std::string strPath)
{
    //release before value
    m_hFile.close();

#ifndef _MSC_VER
    /*
    if(m_st != NULL)
    {
        speex_resampler_destroy(m_st);
        m_st = NULL;
    }*/
#endif

    //check audio type
    if (!CheckAudioType(strPath))
    {
        printf("audio file type error,path: %s", strPath.c_str());
        return false;
    }

    if (CCommon::IsFileExist(strPath))
    {
        m_hFile.open(strPath, std::ios::binary);
    }
    else
    {
        printf("audio file not exist,path: %s", strPath.c_str());
        return false;
    }

    //file can not open
    if (!m_hFile)
    {
        printf("audio file open error,path: %s", strPath.c_str());
        return false;
    }

    //get file size
    m_hFile.seekg(0, std::ios::end);
    m_nRemain = (unsigned int)m_hFile.tellg();
    m_hFile.seekg(0, std::ios::beg);

    //check audio format
    if (m_type == AUDIO_ADP)
    {
        if (!CPublic::CheckAdpFile(strPath))
        {
            printf("%s", "adp audio file format error");
            return false;
        }
    }
    else if(m_type == AUDIO_WAV)
    {
        if (!CPublic::CheckWavFile(strPath,m_nChannel,m_nBitsPerSample,m_nSamplesPerSec))
        {
            printf("%s", "wav audio file format error");
            return false;
        }
        printf("Open Wav,Channel-%d Bit-%d Rate-%d",
                  m_nChannel,m_nBitsPerSample,m_nSamplesPerSec);

        if(m_nSamplesPerSec == 48000)
        {
            m_pBuf = &m_PCM48KBuffer;
        }
        else
        {
            m_pBuf = &m_PCMBuffer;

    #ifndef _MSC_VER
            /*
            m_st = speex_resampler_init(
                        1,
                        m_nSamplesPerSec,
                        48000,
                        SPEEX_RESAMPLER_QUALITY_MIN,  //only support this quality
                        NULL);
            if(m_st == NULL)
            {
                printf("%s","speex_resampler_init error");
            }
            */
    #endif
        }
    }
    return true;
}

bool CAudioFile::IsAdpEnough()
{
    if(m_type == AUDIO_ADP)
    {
        if(m_nRemain >= ED1AudioAdpFrameCount)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        AudioDecode();
        if(m_AdpBuffer.getUsedSize()>=ED1AudioAdpFrameCount)
        {
            return true;
        }
        else
        {
            m_AdpBuffer.setEmpty();
            m_PCMBuffer.setEmpty();
            m_PCM48KBuffer.setEmpty();
#ifndef _MSC_VER
            /*
            if(m_st != NULL)
            {
                speex_resampler_destroy(m_st);
                m_st = NULL;
            }
            */
#endif
            return false;
        }
    }
}

void CAudioFile::ReadAdpFrame(unsigned char* pData)
{
    if(m_type == AUDIO_ADP)
    {
        m_hFile.read((char*)pData, ED1AudioAdpFrameCount);
        m_nRemain -= ED1AudioAdpFrameCount;
    }
    else
    {
        m_AdpBuffer.read(pData,ED1AudioAdpFrameCount);
    }
}

void CAudioFile::ReadAdpSilence(unsigned char* pData)
{

}

void CAudioFile::AudioDecode()
{
    if(m_type == AUDIO_WAV)
    {
        DecodeWavFile();
    }
}

void CAudioFile::DecodeWavFile()
{
    Decode2Wav();

    if(m_nSamplesPerSec != 48000)
    {
        DecodeTo48kSample();
    }
    EncodeToAdp();
}

void CAudioFile::Decode2Wav()
{
    if(m_pBuf == NULL)
        return;

    float fSample;
    unsigned char sample_byte[10];
    int block = m_nChannel*m_nBitsPerSample/8;
    while (m_nRemain > block && m_pBuf->getFreeSize() >= 4)
    {
        m_hFile.read((char*)sample_byte, block);
        m_nRemain -= block;

        if (m_nChannel == 2)
        {
            if (m_nBitsPerSample == 8)
            {
                fSample = Sample8BitTo32Bit(sample_byte,false);
            }
            else if (m_nBitsPerSample == 16)
            {
                fSample = SampleS16BitTo32Bit(sample_byte, false);
            }
            else if(m_nBitsPerSample == 24)
            {
                fSample = SampleS24BitTo32Bit(sample_byte, false);
            }
            else if(m_nBitsPerSample == 32)
            {
                fSample = SampleS32BitTo32Bit(sample_byte, false);
            }
        }
        else
        {
            if (m_nBitsPerSample == 8)
            {
                fSample = Sample8BitTo32Bit(sample_byte, true);
            }
            else if (m_nBitsPerSample == 16)
            {
                fSample = SampleS16BitTo32Bit(sample_byte, true);
            }
            else if(m_nBitsPerSample == 24)
            {
                fSample = SampleS24BitTo32Bit(sample_byte, true);
            }
            else if(m_nBitsPerSample == 32)
            {
                fSample = SampleS32BitTo32Bit(sample_byte, true);
            }
        }
        m_pBuf->write((unsigned char*)&fSample, 4);
    }
}

void CAudioFile::EncodeToAdp()
{
    int needlen = 4*ED1AudioFrameSampleCount;
    float sampleFloat[ED1AudioFrameSampleCount];
    unsigned char sampleAdp[ED1AudioFrameEncodedByteCount];

    while(m_PCM48KBuffer.getUsedSize() >= needlen
          && (m_AdpBuffer.getUsedSize() < 2*ED1AudioAdpFrameCount))
    {
        m_PCM48KBuffer.read((unsigned char*)sampleFloat,needlen);

        m_audioEncoder.Encode(sampleAdp,sampleFloat);

        m_AdpBuffer.write(sampleAdp,ED1AudioAdpFrameCount);
    }
}

//this function is from adpcm_converter project
void CAudioFile::DecodeTo48kSample()
{
#ifndef _MSC_VER

    //if(m_st == NULL)
    //    return;

    float inSample[100];
    float outSample[1000];
    int needlen = 100*sizeof(float);
    unsigned int inLen = 100;

    while(m_PCMBuffer.getUsedSize() >= needlen
          && m_PCM48KBuffer.getUsedSize() < (4*4*ED1AudioFrameSampleCount))
    {

        unsigned int outLen = 1000;
        m_PCMBuffer.read((unsigned char*)&inSample[0],needlen);

        /*
        int ret = speex_resampler_process_float(m_st, 0,
                    inSample, &inLen, outSample, &outLen);

        if (ret == RESAMPLER_ERR_SUCCESS && outLen>0)
        {
            m_PCM48KBuffer.write((unsigned char*)&outSample[0],outLen*sizeof(float));
        }
        */
    }

#endif

}

