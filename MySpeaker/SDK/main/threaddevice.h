
//this is base class for audio device thread
#ifndef THREAD_DEVICE_H
#define THREAD_DEVICE_H

#include "predefine.h"
#include "device.h"
#include "mythread.h"
#include "audiodevice.h"
#include "audiojob.h"
#include "define.h"

struct t_LevelMeter
{
    t_LevelMeter()
    {
        Reset();
    }
    void Reset()
    {
        levelMeterUp = 0;
        MeterCountUp = 0;
        levelMeterDown = 0;
        MeterCountDown = 0;
    }

    long long levelMeterUp;
    int       MeterCountUp;
    long long levelMeterDown;
    int       MeterCountDown;
};

class CThreadDevice: public CMyThread
{
public:
    CThreadDevice();
    virtual ~CThreadDevice();

    virtual void Run();

    virtual void Loop() = 0;
    
    virtual bool LoadData(unsigned int /*nFrames*/, unsigned char* /*pData*/) { return false; };
    virtual bool CopyData(unsigned char* /*pData*/, int /*nFrames*/) { return false; };

    void SetDeviceName(std::string strName) { m_strDeviceName = strName; };
    std::string GetDeviceName() { return m_strDeviceName; };
    void InitPort(std::vector<int> &vPortLeft,std::vector<int> &vPortRight);
    void SetBuffer(CAudioBuffer* pBufLeft, CAudioBuffer* pBufRight);
    
    bool IsStop() { return !m_bLoop; };
    int GetChannel() { return m_nChannel; };
    int GetBitsPerSample() { return m_nBitsPerSample; };

    //main thread
    bool OccupyPort(int nPort);
    void FreePort(int nPort);
    bool IsPortExist(int nPort);

    //audio play and audio capture thread wait signal(100 ms timeout)
    bool WaitSignal();
    unsigned int GetSignalCount();
    void SetVolume(float fVolume);

    float         m_fLevelLeft;
    float         m_fLevelRight;

protected:
    //load and copy data signal
    void Signal();

    bool IsLeftRunning();
    bool IsRightRunning();

    void LevelMeter(short szSample[],bool bLeftCh);

protected:
    std::string                 m_strClassName;
    std::string                 m_strDeviceName;
    bool                        m_bCapture;
    int                         m_nBitsPerSample;
    int                         m_nChannel;   //1 or 2
    std::map<int,bool>          m_mapPortLeft;
    std::map<int, bool>         m_mapPortRight;

    int                         m_nRtpJobCount;
    bool                        m_bLoop;

    //signal
    std::mutex                  m_mutexSignal;
    std::condition_variable     m_cvSignal;
    unsigned int                m_nSignalCount;

    //audio buffer
    CAudioBuffer*               m_pBufLeft;
    CAudioBuffer*               m_pBufRight;

    t_LevelMeter                m_levelLeft;
    t_LevelMeter                m_levelRight;
};

#endif

