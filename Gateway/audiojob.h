
#ifndef AUDIO_JOB_H
#define AUDIO_JOB_H

#include <atomic>
#include "d1rtpframelayer.h"
#include "mysocket.h"
#include "audiofile.h"
#include "audiobuffer.h"
#include "audiodevice.h"
#include "g711.h"
#include "g722.h"
#include "libresample.h"
#include "opus/opus.h"

#define G722_BIT_RATE      64000   //64000/56000/48000
#define G711_FRAME_LEN     (8000/50)
#define G722_FRAME_LEN     (8000/50)

//this is fix for channel start
#define TRP_CHANNEL_START   10500

extern const int g_nWriteErrorCount;

enum  e_PLAY_TYPE
{
    LOCAL_ADP2NET,
    LOCAL_ADP2LISTEN,
    LOCAL_OPUS,
};

enum e_SEND_ENCODE
{
    ENCODE_ADP = 0,
    ENCODE_G711a,
    ENCODE_G711u,
    ENCODE_G722,
    ENCODE_OPUS,
    ENCODE_UNKNOW
};

class CAudioJob
{
public:
	CAudioJob();
    virtual ~CAudioJob();

    std::string GetClassName();

    virtual void Run() {};

    virtual void Pause();
    virtual void Start();
    virtual void Stop();

    void Finish();
    void FinishPause();
    bool IsNeed2Start();

    //this is the unique id for this job
    void SetChProcess(int nProcess);
    void SetRecordID(int nID);
    int GetChProcess();
    int GetRecordID();

    //pre-signal
    void SetPresignal(std::string strPath);

    //rtp node and channel
    void SetRtpNode(int nNode);
    void SetRtpChannel(int nChannel);
    int GetRtpChannel();
    int GetRtpNode();

    //void GongSignal(bool bPlay);
 
    bool Write(const char *strWrite,int nWrite);

    //when finish the job,return this error
    int GetError();

    bool IsStop() { return m_bStop; };
    bool IsManuStop() { return m_bManuStop; };

    bool IsRunning() {return m_bRunning;};
    void SetRunning(bool bRunning) { m_bRunning = bRunning; };

    //adp to pcm
    void Adp2PcmBuffer(unsigned char* pAdp, int nFrames);

    //resample from 48k to 8k
    void ResampleFrom48k();

    //resample from 8k/16k to 48k
    void Resampleto48k(e_SEND_ENCODE eType);

    //encoding to codes
    void Encoding(e_SEND_ENCODE eType);
    void Decoding(unsigned char* pBuf,int nFrame,e_SEND_ENCODE eType);

    int   m_error;
    e_SEND_ENCODE m_eAudioEncoding;

protected:
    //get the udpmulticast address
    std::string GetUdpAddress();

    //wait for more time to finish
    void WaitMiniSeconds(int nMiniSecond);

    void pcmToG711(bool bALaw);
    void pcmToG722();
    void pcmToOPUS();

    void G711topcm(unsigned char * pBuf,int iFrame,bool bALaw);
    void G722topcm(unsigned char * pBuf,int iFrame);

    std::string           m_className;
  
    //encode and decode rtp frame
    VpAudioCore::D1RtpFrameCreator     m_rtpFrameCreator;
    VpAudioCore::D1AudioAdpcmDecoder   m_audioDecoder;
    VpAudioCore::D1AudioAdpcmEncoder   m_audioEncode;

    int                   m_nWriteErrorCount;
    CMySocket             m_Socket;
    std::vector<int>      m_vPort;
    std::string           m_strPresignal;

    //call id
    int   m_chProcess;
    int   m_nRecordID;
    int   m_rtpNode;
    int   m_rtpChannel;
    bool  m_bRunning;
    bool  m_bStop;
    bool  m_bPause;
    bool  m_bManuStop;
    bool  m_bPlayGong;
    int   m_nBlockSize;
    float m_fVolumeScale;
    float m_fVolumeScaleSG;

    CCycleBuffer          m_pcm48kBuffer;   //this is 48k float buffer
    CCycleBuffer          m_pcm8kBuffer;    //this is 8k short buffer
    CCycleBuffer          m_pcm16kBuffer;    //this is 16k short buffer

    CCycleBuffer          m_pcmG711a;
    CCycleBuffer          m_pcmG711u;
    CCycleBuffer          m_pcmG722;

    void*                 m_resamplehandle;
    double                m_resamplefactor;
    int                   m_resamplefwidth;

    g722_encode_state_t   m_g722EncodeState;
    g722_decode_state_t   m_g722DecodeState;

    OpusEncoder          *m_Opus_encoder;
};

#endif

