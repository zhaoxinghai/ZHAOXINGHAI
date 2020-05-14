
#ifndef AUDIO_JOB_H
#define AUDIO_JOB_H

#include <atomic>
#include "d1rtpframelayer.h"
#include "mysocket.h"
#include "audiofile.h"
#include "audiobuffer.h"
#include "audiodevice.h"

//this is fix for channel start
#define TRP_CHANNEL_START   10500

extern const int g_nWriteErrorCount;

enum  e_PLAY_TYPE
{
    LOCAL_ADP2NET,
    LOCAL_ADP2LISTEN
};

enum e_BUFFER_TYPE
{
    BUF_PRE_SIGNAL,
    BUF_AUDIO,
    BUF_END_SIGNAL
};

class CAudioJob
{
public:
	CAudioJob();
    virtual ~CAudioJob();

    virtual void Run();

    virtual void Pause();
    virtual void Start();
    virtual void Stop();

    void Finish();
    void FinishPause();
    bool IsNeed2Start();

    //update port
    void AllocatePort(bool bCapture, int nPort);
    void FreePort(bool bCapture, int nPort);

    //ajust volume
    void VolumeAjust(float fVolumeScale);
    void VolumeAjustSG(float fVolumeScale);
    void VolumeAjustEG(float fVolumeScale);

    //this is the unique id for this job
    void SetChProcess(int nProcess);
    void SetRecordID(int nID);
    int GetChProcess();
    int GetRecordID();

    //rtp node and channel
    void SetRtpNode(int nNode);
    void SetRtpChannel(int nChannel);
    int GetRtpChannel();
    int GetRtpNode();

    //send audio data by socket
    bool Write(const char *strWrite,int nWrite);

    //when finish the job,return this error
    int GetError();

    bool IsStop();
    bool IsManuStop();

    bool IsRunning();
    void SetRunning(bool bRunning);

    void EnableVLAN(bool bEnable);
    void SwitchVLANNet(MYSOCK socket,std::string strNet);

    void EnableSecurity(bool bEnable);

    //for security
    void SetPreSignalFile(t_FILEMAP &file);
    void SetEndSignalFile(t_FILEMAP &file);

    int         m_error;
    std::string m_className;
protected:
    //socket init and exit
    bool InitNet();
    bool ExitNet();

    //adp to pcm buffer
    void Adp2PcmBuffer(unsigned char* pAdp, int nFrames);

    //adp to pcm buffer with volume ajust
    void Adp2PcmBufferListening(unsigned char* pAdp, int nFrames,e_BUFFER_TYPE eType);

    //get the udpmulticast address
    std::string GetUdpAddress();

    //wait for more time to finish
    void WaitMiniSeconds(int nMiniSecond);

    //volume ajust
    void VolumeAjust(unsigned char *adpData,bool bSG);
 
    void LevelMeter(short Sample[]);
  
    //busy state
    void SendPreSignal(bool bPlay);
    void SendEndSignal(bool bPlay);
    void SendAudioSignal(bool bPlay);

    //encode and decode rtp frame
    VpAudioCore::D1RtpFrameCreator     m_rtpFrameCreator;
    VpAudioCore::D1AudioAdpcmDecoder   m_audioDecoder;
    VpAudioCore::D1AudioAdpcmEncoder   m_audioEncode;

    bool                  m_bEnableSecurity;
    int                   m_nWriteErrorCount;
    CMySocket             m_Socket;
    std::vector<int>      m_vPort;

    //for audio buffer
    std::mutex            m_mutexBuf;
    std::vector<CAudioBuffer*> m_vAudioBuffer;

    //VLAN
    std::mutex            m_mutexVLAN;
    bool                  m_bEnableVLAN;
    bool                  m_bResetVLANNet;
    MYSOCK                m_VLANSock;
    std::string           m_strVLANNet;

    //call id
    int   m_chProcess;
    int   m_nRecordID;
    int   m_rtpNode;
    int   m_rtpChannel;
    bool  m_bRunning;
    bool  m_bStop;
    bool  m_bPause;
    bool  m_bManuStop;
    int   m_nBlockSize;
    float m_fVolumeScale;
    float m_fVolumeScaleSG;
    float m_fVolumeScaleEG;

    //for security
    t_FILEMAP   m_PreSignalFile;
    t_FILEMAP   m_EndSignalFile;

    //busy state
    bool m_bAudioSignal;
    bool m_bPreSignal;
    bool m_bEndSignal;
};

#endif

