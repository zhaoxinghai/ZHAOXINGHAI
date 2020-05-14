
/*
* Date:     2016-8-4
* Author :  xinghai,zhao
* Content : this object is process audio stream's from micophone capture,
*           such as record,play to the net destination.
*/

#ifndef AUDIO_CAPTURE2_H
#define AUDIO_CAPTURE2_H

#include "audiojob.h"
#include "threaddevice.h"

class CAudioCapture2: public CAudioJob
{
public:
    CAudioCapture2(int nPort);
    virtual ~CAudioCapture2();

    virtual void Run();
	virtual void Pause();
	virtual void Stop();

    void SetVARecord(bool bRecord);

    int GetPort();

    //for pre-signal(only for audiocapture)
    void OpenGongAudio();

protected:

    void Loop();
    void MicrAnnouncement();

    bool SendRtpFrame();
  
    bool GetPreSignalAdpFrame(unsigned char* pAdpData);
    bool GetCaptureAdpFrame(unsigned char* pAdpData);

    void MicroSignl(bool bMic);

    void StartVARecord();
    void VARecord(unsigned char* pAdpData);
    void StopVARecord();

    std::ifstream    m_hFilePreSignal;
    int              m_nPreSignalRemain;

    std::ofstream    m_hFile;        //record file
    std::mutex       m_mutex;
    std::string      m_strFilePath;

    int              m_nPort;
    CThreadDevice*   m_pThreadDevice;

    CCycleBuffer     m_Pcmbuffer;
    CCycleBuffer     m_PcmBufferOut;

    bool             m_bMicopen;
    int              m_nSrcIndex;

    bool             m_bVARecord;
    int              m_nVARecordFrames;
};

#endif

