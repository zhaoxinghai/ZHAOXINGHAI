
#include "threadcapture.h"
#include "audiojob.h"
#include "public.h"
#include "sdkerror.h"
#include "audiobuffer.h"
#include "common.h"
#include "sdk.h"
#include "service.h"
#include "libresample.h"

namespace TOP
{
    CThreadCapture::CThreadCapture()
    {
        m_strClassName = "CThreadCapture";
        m_bCapture = true;
        m_nPlayJobCount = 0;
        memset(m_sampleLeft, 0, sizeof(m_sampleLeft));
        memset(m_sampleRight, 0, sizeof(m_sampleRight));

        m_resamplefactor = 0;
        m_resamplehandle[0] = NULL;
        m_resamplehandle[1] = NULL;
        m_resamplefwidth[0] = 0;
        m_resamplefwidth[1] = 0;
    }

    CThreadCapture::~CThreadCapture()
    {
    }

    bool CThreadCapture::Init()
    {
        return true;
    }

    void CThreadCapture::Run()
    {
#ifdef WIN32
        ::CoInitializeEx(NULL,COINIT_MULTITHREADED);
#endif
        if(g_SDKServer.m_eStreamType==AUDIO_PCM)
        {
            m_AudioDevice.SetCaptureFrame(PCM_FRAME_SAMPLE_COUNT);
        }
        if (!m_AudioDevice.InitDevice(true,this))
        {
            LOG_ERROR("CThreadCapture::Run-%s", "InitDevice")
            return;
        }
        //init resample
        if(m_nSampleRate != PCM_FRAME_SAMPLE_RATE)
        {
            m_resamplefactor = ((double)PCM_FRAME_SAMPLE_RATE) / (double)m_nSampleRate;
            m_resamplehandle[0] = resample_open(1, m_resamplefactor, m_resamplefactor);
            m_resamplehandle[1] = resample_open(1, m_resamplefactor, m_resamplefactor);
            m_resamplefwidth[0] = resample_get_filter_width(m_resamplehandle[0]);
            m_resamplefwidth[1] = resample_get_filter_width(m_resamplehandle[1]);
        }
        CThreadDevice::Run();

        if (m_resamplehandle[0] != NULL)
        {
            resample_close(m_resamplehandle[0]);
        }
        if (m_resamplehandle[1] != NULL)
        {
            resample_close(m_resamplehandle[1]);
        }

#ifdef _MSC_VER
        CoUninitialize();
#endif
    }
    void CThreadCapture::Loop()
    {
        m_AudioDevice.StartCapture();

        if (m_AudioDevice.m_error > 0)
        {
            LOG_ERROR("CThreadCapture Loop Error: %d", m_AudioDevice.m_error);
            SleepM(10 * 1000);
        }
    }
    bool CThreadCapture::CopyData(unsigned char* pData, int nFrames)
    {
        unsigned char sampleLeft[4 * PCM_FRAME_SAMPLE_COUNT * 4];
        unsigned char sampleRight[4 * PCM_FRAME_SAMPLE_COUNT * 4];

        //split the left and right channel
        if(m_nChannel==2)
        {
            CPublic::GetChannelBuffer(sampleLeft, sampleRight, pData, nFrames, m_nBitsPerSample);
        }
        else
        {
            const int nBlock = m_nBitsPerSample/8;
            memcpy(sampleLeft,pData,nFrames*nBlock);
        }

        //sample to float
        if(m_nBitsPerSample!=32)
        {
            CPublic::PCMBuffer2Float(sampleLeft,nFrames,m_nBitsPerSample);
            if(m_nChannel==2)
            {
                CPublic::PCMBuffer2Float(sampleRight,nFrames,m_nBitsPerSample);
            }
        }
        //sample to 48k
        if(m_nSampleRate != PCM_FRAME_SAMPLE_RATE)
        {
            int nFrameOut = 0;
            int inBufferUsed = 0;
            float outFloat[4*1024];
            memset(outFloat,0,sizeof(outFloat));

            nFrameOut = resample_process(m_resamplehandle[0],m_resamplefactor,(float*)sampleLeft,
                    nFrames,0,&inBufferUsed,outFloat,4*1024);

            m_pBufLeft->write((unsigned char*)outFloat, nFrameOut*sizeof(float),false);

            if(m_nChannel==2)
            {
                memset(outFloat,0,sizeof(outFloat));
                nFrameOut = resample_process(m_resamplehandle[1],m_resamplefactor,(float*)sampleRight,
                        nFrames,0,&inBufferUsed,outFloat,4*1024);
            }
            m_pBufRight->write((unsigned char*)outFloat, nFrameOut*sizeof(float),false);
        }
        else
        {
            m_pBufLeft->write((unsigned char*)sampleLeft, nFrames*sizeof(float),false);
            if(m_nChannel==2)
            {
                m_pBufRight->write((unsigned char*)sampleRight, nFrames*sizeof(float),false);
            }
            else
            {
                m_pBufRight->write((unsigned char*)sampleLeft, nFrames*sizeof(float),false);
            }
        }

        //sample enough for one frame
        if (m_pBufLeft->usedsize()>=PCM_FRAME_SAMPLE_COUNT*sizeof(float))
        {
            Signal();
        }
        return true;
    }

    void CThreadCapture::AddPlayJob()
    {
        std::unique_lock < std::mutex > lck(m_mutex);

        m_bLoop = true;
        m_nPlayJobCount++;

        m_cv.notify_one();
    }

    void CThreadCapture::RemovePlayJob()
    {
        std::unique_lock < std::mutex > lck(m_mutex);

        m_nPlayJobCount--;
        if (m_nPlayJobCount < 0)
        {
            LOG_ERROR("%s", "CThreadCapture::PopRtpJob")
        }

        if (m_nPlayJobCount == 0
            && !IsLeftRunning()
            && !IsRightRunning())
        {
            m_bLoop = false;
        }
    }

    int CThreadCapture::GetPlayJobCount()
    {
        std::unique_lock < std::mutex > lck(m_mutex);

        return m_nPlayJobCount;
    }
}
