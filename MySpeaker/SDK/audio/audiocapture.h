
/*
* Date:     2016-8-4
* Author :  xinghai,zhao
* Content : this object is process audio stream's from micophone capture,
*           such as record,play to the net destination.
*/

#ifndef AUDIO_CAPTURE_H
#define AUDIO_CAPTURE_H

#include "audiojob.h"
#include "threaddevice.h"

enum e_CAPTURE_TYPE
{
    CAPTURE_NET_ANNOUNCEMENT,  //from micr to net
    CAPTURE_NET_RECORD,
    CAPTURE_RECORD     //record
};

class CAudioCapture: public CAudioJob
{
public:
    CAudioCapture(e_CAPTURE_TYPE type, int nPort);
    virtual ~CAudioCapture();

    virtual void Run();
	virtual void Pause();
	virtual void Start();
	virtual void Stop();

    void SetRecordPath(std::string strPath);
    std::string GetRecordPath();

    void SetVARecord(bool bRecord);

    e_CAPTURE_TYPE GetType();
    int GetPort() { return m_nPort; };

    //for pre-signal(only for audiocapture)
    void OpenGongAudio();

protected:
    bool InitNet();
    bool ExitNet();

    void Loop();

    void LocalRecord();
    void MicrAnnouncement();

    bool OnRecord();
    bool SendRtpFrame();
  
    bool GetGongAdpFrame(unsigned char* pAdpData);
    bool GetCaptureAdpFrame(unsigned char* pAdpData);

    void MicroSignl(bool bMic);

    void StartVARecord();
    void VARecord(unsigned char* pAdpData);
    void StopVARecord();

    std::ifstream    m_hFilePreSignal;
    int              m_nPreSignalRemain;

    std::ofstream    m_hFile;        //record file
    std::mutex       m_mutex;

    std::string      m_strFilePath;  //for record
    e_CAPTURE_TYPE   m_type;
    int              m_nPort;
    CThreadDevice*   m_pThreadDevice;

    CCycleBuffer     m_Pcmbuffer;
    CCycleBuffer     m_PcmBufferOut;

    bool             m_bMicopen;
    bool             m_bHaveGong;
    int              m_nSrcIndex;

    bool             m_bVARecord;
    int              m_nVARecordFrames;
};

#endif

