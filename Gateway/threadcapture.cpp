
#ifdef WIN32

#include "threadcapture.h"
#include "audiojob.h"
#include "public.h"
#include "paerror.h"
#include "audiobuffer.h"
#include "common.h"

CThreadCapture::CThreadCapture()
{
    m_strClassName = "CThreadCapture";
    m_bCapture = true;
    m_nRtpJobCount = 0;

    m_TempBufferLeft.SetSize(ED1AudioFrameSampleCount * 4 * 2);
    m_TempBufferRight.SetSize(ED1AudioFrameSampleCount * 4 * 2);

    memset(m_sampleLeft,0,sizeof(m_sampleLeft));
    memset(m_sampleRight,0,sizeof(m_sampleRight));
}

CThreadCapture::~CThreadCapture()
{
}

void CThreadCapture::Run()
{
#ifdef _MSC_VER
    ::CoInitialize(NULL);
#endif

    if(!m_audioDevice.InitDevice(true,GetChannel(),GetBitsPerSample()))
    {
        printf("CThreadCapture::Run-%s", "InitDevice");
        return;
    }
    /*
    auto msg = std::make_shared<CMsg>();
    msg->type = MSG_ALSA_INIT_FINISH;
    msg->nInt1 = 1;
    CService::GetInstance()->Push(msg);
    */
    CThreadDevice::Run();

#ifdef _MSC_VER
    CoUninitialize();
#endif

}

void CThreadCapture::Loop()
{
    m_audioDevice.StartCapture(this);

    if (m_audioDevice.m_error > 0)
    {
        printf("CThreadCapture Loop Error: %d",m_audioDevice.m_error);
        SleepM(60 * 1000);
    }

    m_fLevelLeft = -100.0f;
    m_fLevelRight = -100.0f;
}

bool CThreadCapture::CopyData(unsigned char * pData, int nFrames)
{
    int block = m_nBitsPerSample/8;
    int needlen = ED1AudioFrameSampleCount*block;
    //static float fMinVol = CService::GetInstance()->m_fMicADC
    //        *CCommon::dB2FloatScale(-60.0f);

#ifdef _MSC_VER
    unsigned char sampleLeft[4*ED1AudioFrameSampleCount];
    unsigned char sampleRight[4*ED1AudioFrameSampleCount];

    CPublic::GetChannelBuffer(sampleLeft, sampleRight, pData, nFrames,block);

    //double channel
    m_TempBufferLeft.write(sampleLeft, nFrames*block);
    m_TempBufferRight.write(sampleRight, nFrames*block);

    //buffer is not enough to send
    if (m_TempBufferLeft.getUsedSize() < needlen)
    {
        return true;
    }

    //is time to send this data
    m_TempBufferLeft.read((unsigned char*)&sampleLeft[0], needlen);
    m_TempBufferRight.read((unsigned char*)&sampleRight[0], needlen);

    //volume ajust
    /*
    if(m_pService->m_fVolumeMic<fMinVol)
        memset(sampleLeft,0,sizeof(sampleLeft));
    else
        SetLeftVolume(block,(unsigned char*)&sampleLeft[0]);

    if(m_pService->m_fVolumeLineIn<fMinVol)
        memset(sampleRight,0,sizeof(sampleRight));
    else
        SetRightVolume(block,(unsigned char*)&sampleRight[0]);

    m_pBufLeft->write((unsigned char*)&sampleLeft[0], needlen);
    m_pBufRight->write((unsigned char*)&sampleRight[0], needlen);
    */
#else
    if (nFrames != ED1AudioFrameSampleCount)
    {
        return false;
    }

    //get the channel data(16 bit)
    CPublic::GetChannelBuffer(m_sampleLeft, m_sampleRight, pData, nFrames,block);

    if(m_pService->m_fVolumeMic<fMinVol)
        memset(m_sampleLeft,0,sizeof(m_sampleLeft));
    else
        SetLeftVolume(block,(unsigned char*)&m_sampleLeft[0]);

    if(m_pService->m_fVolumeLineIn<fMinVol)
        memset(m_sampleRight,0,sizeof(m_sampleRight));
    else
        SetRightVolume(block,(unsigned char*)&m_sampleRight[0]);

    m_pBufLeft->write((unsigned char*)&m_sampleLeft[0], needlen);
    m_pBufRight->write((unsigned char*)&m_sampleRight[0], needlen);

#endif
    Signal();
    return true;
}

void CThreadCapture::PushRtpJob()
{
    std::unique_lock < std::mutex > lck(m_mutex);

    m_bLoop = true;
    m_nRtpJobCount++;

    m_cv.notify_one();
}

void CThreadCapture::PopRtpJob()
{
    std::unique_lock < std::mutex > lck(m_mutex);

    m_nRtpJobCount--;
    if(m_nRtpJobCount < 0)
    {
        printf("%s","CThreadCapture::PopRtpJob");
    }

    if (m_nRtpJobCount ==0 
        && !IsLeftRunning()
        && !IsRightRunning())
    {
        m_bLoop = false;
    }
}

int CThreadCapture::GetRtpJobCount()
{
    std::unique_lock < std::mutex > lck(m_mutex);

    return m_nRtpJobCount;
}

#endif
