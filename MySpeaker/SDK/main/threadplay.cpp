
#include "threadplay.h"
#include "audiojob.h"
#include "device.h"
#include "common.h"
#include "paerror.h"
#include "audiobuffer.h"
#include "mylog.h"
#include "service.h"
#include "sdk.h"
#include "public.h"

CThreadPlay::CThreadPlay()
{
	m_strClassName = "CThreadPlay";
    m_bCapture = false;
    memset(&m_sampleLeft[0], 0, sizeof(m_sampleLeft));
    memset(&m_sampleRight[0], 0, sizeof(m_sampleRight));
    m_bMute = false;
}

CThreadPlay::~CThreadPlay()
{
}

void CThreadPlay::Run()
{
#ifdef _MSC_VER
    ::CoInitialize(NULL);
#endif

    if(!m_audioDevice.InitDevice(false,GetChannel(),GetBitsPerSample()))
    {
        LOG_ERROR("CThreadPlay::Run-%s", "InitDevice");
        return;
    }

    auto msg = std::make_shared<CMsg>();
    msg->type = MSG_ALSA_INIT_FINISH;
    msg->nInt1 = 0;
    CService::GetInstance()->Push(msg);

    CThreadDevice::Run();

#ifdef _MSC_VER
    CoUninitialize();
#endif

}

void CThreadPlay::Loop()
{
    m_audioDevice.StartPlayBack(this);
    if (m_audioDevice.m_error > 0)
    {
        LOG_ERROR("CThreadPlay Loop Error: %d",m_audioDevice.m_error);
        SleepM(60 * 1000);
    }
    m_fLevelLeft = -100.0f;
    m_fLevelRight = -100.0f;
}

bool CThreadPlay::LoadData(unsigned int nFrames, unsigned char * pData)
{
    int block = m_nBitsPerSample/8;
    m_pBufLeft->read(&m_sampleLeft[0], block*nFrames);
    m_pBufRight->read(&m_sampleRight[0], block*nFrames);

    if(block == 2)
    {
        SetLeftVolume(nFrames,(unsigned char*)&m_sampleLeft[0]);
        SetRightVolume(nFrames,(unsigned char*)&m_sampleRight[0]);
    }
    CPublic::SetChannelBuffer(m_sampleLeft,m_sampleRight,pData,nFrames,block);

    return true;
}

void CThreadPlay::SetLeftVolume(int nFrame,unsigned char* pData)
{
    short* p = (short*)pData;
    CService* pSer = CService::GetInstance();

    if(m_bMute)
    {
        memset(p,0,2*nFrame);
    }
    else
    {
        if(pSer->m_fVolumeSpeaker > 0.99 && pSer->m_fVolumeSpeaker < 1.01)
        {
        }
        else
        {
            for(int i = 0;i<nFrame;i++)
            {
                p[i] = CAudioFile::GetSampleS(long(p[i]*pSer->m_fVolumeSpeaker));
            }
        }
    }
    LevelMeter(p,true);
}

void CThreadPlay::SetRightVolume(int nFrame,unsigned char* pData)
{
    short* p = (short*)pData;

    CService* pSer = CService::GetInstance();
    if(pSer->m_fVolumeLineOut > 0.99 && pSer->m_fVolumeLineOut < 1.01)
    {
    }
    else
    {
        for(int i = 0;i<nFrame;i++)
        {
            p[i] = CAudioFile::GetSampleS(long(p[i]*pSer->m_fVolumeLineOut));
        }
    }
    LevelMeter(p,false);
}
