
#include "threaddevice.h"
#include "audiojob.h"
#include "device.h"
#include "common.h"
#include "sdkerror.h"
#include "mylog.h"
#include "service.h"

CThreadDevice::CThreadDevice()
{
	m_strClassName = "CThreadDevice";
    m_bLoop = false;
    m_bCapture = false;
    m_nRtpJobCount = 0;
    m_nChannel = 2;
    m_nBitsPerSample = SAMPLING_DEEP;

    m_pBufLeft = NULL;
    m_pBufRight = NULL;
    m_nSignalCount = 0;

    m_fLevelLeft = -100.0f;
    m_fLevelRight = -100.0f;
}

CThreadDevice::~CThreadDevice()
{    
}

void CThreadDevice::Run()
{
	CCommon::SetThreadName(m_strClassName);

    while (m_bActive)
    {
        if(!m_bLoop)
        {
            std::unique_lock < std::mutex > lck(m_mutex);
            m_cv.wait(lck);
        }

        if (!m_bActive)
            break;

        Loop();
    }
}

void CThreadDevice::Loop()
{
}

void CThreadDevice::InitPort(std::vector<int> &vPortLeft, std::vector<int> &vPortRight)
{
    if (vPortLeft.size() > 0 && vPortRight.size() > 0)
        m_nChannel = 2;
    else
        m_nChannel = 1;

    for (unsigned int i = 0; i < vPortLeft.size(); i++)
    {
        m_mapPortLeft[vPortLeft[i]] = false;
    }
    for (unsigned int i = 0; i < vPortRight.size(); i++)
    {
        m_mapPortRight[vPortRight[i]] = false;
    }
}

void CThreadDevice::SetBuffer(CAudioBuffer* pBufLeft, CAudioBuffer* pBufRight)
{
    m_pBufLeft = pBufLeft;
    m_pBufRight = pBufRight;
}

bool CThreadDevice::OccupyPort(int nPort)
{
    std::unique_lock < std::mutex > lck(m_mutex);

    //channel left
    auto iter = m_mapPortLeft.find(nPort);
    if (iter != m_mapPortLeft.end())
    {
        m_bLoop = true;
        if(iter->second)
        {
            LOG_ERROR("%s","CThreadDevice::OccupyPort Left Error");
        }
        iter->second = true; 
    }
    else
    {
        //channel right
        iter = m_mapPortRight.find(nPort);
        if (iter != m_mapPortRight.end())
        {
            m_bLoop = true;
            if(iter->second)
            {
                LOG_ERROR("%s","CThreadDevice::OccupyPort Right Error");
            }
            iter->second = true;
        }
    }
    m_cv.notify_one();
    return true;
}

void CThreadDevice::FreePort(int nPort)
{
    std::unique_lock < std::mutex > lck(m_mutex);

    //channel left
    auto iter = m_mapPortLeft.find(nPort);
    if (iter != m_mapPortLeft.end())
    {
        if(!iter->second)
        {
            LOG_ERROR("%s","CThreadDevice::FreePort Left Error");
        }
        iter->second = false;
    }
    else
    {
        //channel right
        iter = m_mapPortRight.find(nPort);
        if (iter != m_mapPortRight.end())
        {
            if(!iter->second)
            {
                LOG_ERROR("%s","CThreadDevice::FreePort Right Error");
            }
            iter->second = false;
        }
    }

    //reset the audio buffer
    if (!IsLeftRunning())
    {
        m_pBufLeft->reset();
    }
    if (!IsRightRunning())
    {
        m_pBufRight->reset();
    }

    if (m_nRtpJobCount == 0
        && !IsLeftRunning()
        && !IsRightRunning())
    {
        m_bLoop = false;
    }
}

bool CThreadDevice::IsLeftRunning()
{
    auto iter = m_mapPortLeft.begin();
    while (iter != m_mapPortLeft.end())
    {
        if (iter->second)
        {
            return true;
        }
        iter++;
    }
    return false;
}

bool CThreadDevice::IsRightRunning()
{
    auto iter = m_mapPortRight.begin();
    while (iter != m_mapPortRight.end())
    {
        if (iter->second)
        {
            return true;
        }
        iter++;
    }
    return false;
}

bool CThreadDevice::IsPortExist(int nPort)
{
    //channel left
    auto iter = m_mapPortLeft.find(nPort);
    if (iter != m_mapPortLeft.end())
    {
        return true;
    }
    //channel right
    iter = m_mapPortRight.find(nPort);
    if (iter != m_mapPortRight.end())
    {
        return true;
    }
    return false;
}

unsigned int CThreadDevice::GetSignalCount()
{
	std::unique_lock < std::mutex > lck(m_mutexSignal);
	return m_nSignalCount;
}

bool CThreadDevice::WaitSignal()
{
    std::unique_lock < std::mutex > lck(m_mutexSignal);
    if (m_cvSignal.wait_for(lck, std::chrono::milliseconds(100)) == std::cv_status::no_timeout)
    {
        return true;
    }
    return false;
}

void CThreadDevice::Signal()
{
    std::unique_lock < std::mutex > lck(m_mutexSignal);

    m_nSignalCount++;
    m_cvSignal.notify_all();
}

void CThreadDevice::LevelMeter(short szSample[],bool bLeftCh)
{
    static long long LEVEL_SAMPLE = (418*46);
    static long long RMS_TOTAL = 32768 * 32768 * LEVEL_SAMPLE;

    t_LevelMeter* pLevel = NULL;
    if(bLeftCh)
        pLevel = &m_levelLeft;
    else
        pLevel = &m_levelRight;

    for(int i = 0;i<418;i++)
    {
        if(szSample[i]>0)
        {
            pLevel->levelMeterUp += (szSample[i]*szSample[i]);
            pLevel->MeterCountUp++;
        }
        else
        {
            short tmp = abs(szSample[i]);
            pLevel->levelMeterDown += (tmp*tmp);
            pLevel->MeterCountDown++;
        }
    }

    //200 mini seconds
    if((pLevel->MeterCountUp + pLevel->MeterCountDown) >= LEVEL_SAMPLE)
    {
        long long level = pLevel->levelMeterUp+pLevel->levelMeterDown;
        double tmp = level*1.0/RMS_TOTAL;
        if(bLeftCh)
        {
            m_fLevelLeft = std::max(-80.0f,10.0f * float(log10(tmp)));
        }
        else
        {
            m_fLevelRight = std::max(-80.0f,10.0f * float(log10(tmp)));
        }
        pLevel->Reset();
    }  
}
