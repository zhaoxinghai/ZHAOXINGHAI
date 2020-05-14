
/*
* Date:     2016-8-4
* Author :  xinghai,zhao
* Content : this object is to supply audio stream's data source
*/

#ifndef AUDIO_PLAY_H
#define AUDIO_PLAY_H

#include "audiojob.h"

class CAudioPlay: public CAudioJob
{
public:
    CAudioPlay(e_PLAY_TYPE type);
    virtual ~CAudioPlay();
    virtual void Run();

    void Loop();

    e_PLAY_TYPE GetType();
    void SetRepeatCount(int nRepeat);
    void SetAudioPath(std::vector<t_FILEMAP> &vPath);

    void GetListeningPort(std::vector<int> &vPort);

protected:    

    //send rtp data to net(audioplay run)
    void SendRtpFrame();

    //decode file to audio buffer(m_param.nBlockAlign)
    void DecodeFile2Buffer();
    void DecodeAdpFile2AdpBuffer();
    void DecodeAdpFile2AdpBuffer2();  //for security

    bool OpenNewFile();

    //buffer
    CCycleBuffer              m_adpAudiobufferSG;
    CCycleBuffer              m_adpAudiobuffer;
    CCycleBuffer              m_adpAudiobufferEG;

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
    std::vector<t_FILEMAP>    m_vIOMapPath;
    std::mutex                m_mutex;

    int                       m_nRepeatCount; //255 is for ever
    int                       m_nCurrentRepeat;

    CThreadDevice*            m_pThreadDevice;
};

#endif

