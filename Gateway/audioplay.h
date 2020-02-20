
/*
* Date:     2016-8-4
* Author :  xinghai,zhao
* Content : this object is to supply audio stream's data source
*/

#ifndef AUDIO_PLAY_H
#define AUDIO_PLAY_H

#ifdef WIN32

#include "audiojob.h"

class CAudioPlay: public CAudioJob
{
public:
    CAudioPlay(e_PLAY_TYPE type);
    virtual ~CAudioPlay();
    virtual void Run();

    virtual void Pause();
    virtual void Start();
    virtual void Stop();

    void Loop();

    e_PLAY_TYPE GetType();
    void SetRepeatCount(int nRepeat,int nGapTime);
    void SetAudioPath(std::vector<std::string> &vPath);
    void SetEncodeType(e_SEND_ENCODE eType);
    void GetListeningPort(std::vector<int> &vPort);

protected:    

    //send rtp data to net(audioplay run)
    void SendRtpFrame();

    //read the audio's type
    bool InitNet();
    bool ExitNet();

    //decode file to audio buffer(m_param.nBlockAlign)
    void DecodeFile2Buffer();
    void DecodeAdpFile2AdpBuffer();

    bool OpenNewFile();

    //buffer
    CCycleBuffer              m_adpAudiobufferSG;
    CCycleBuffer              m_adpAudiobuffer;

    //the play file
    CAudioFile                m_audioFile;
    bool                      m_bHaveGong;
  
    //receive adp data from net
    int                       m_sequenceNo;
    std::string               m_strNofilePath;

    //the job type
    e_PLAY_TYPE               m_type;
    bool                      m_bAudioSignal;
    bool                      m_bDecodingSignal;

    int                       m_nCurPlayIndex;
    bool                      m_bPlayFinish;
    std::vector<std::string>  m_vAudioPath;
    std::mutex                m_mutex;

    int                       m_nRepeatCount; //255 is for ever
    int                       m_nCurrentRepeat;

    int                       m_nSilenceFrames;
    bool                      m_bSendSilence;
    int                       m_nSendSilenceCount;
    unsigned char             m_SilenceADP[ED1AudioFrameEncodedByteCount];

    CThreadDevice*            m_pThreadDevice;
};

#endif
#endif

