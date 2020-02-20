
/*
* Date:     2016-8-4
* Author :  xinghai,zhao
* Content : this object is to supply audio stream's data source
*/

#ifndef AUDIO_RECEIVE_H
#define AUDIO_RECEIVE_H

#include "audiojob.h"

class CAudioReceive: public CAudioJob
{
public:
    CAudioReceive();
    virtual ~CAudioReceive();
    virtual void Run();
    virtual void Pause();
    virtual void Start();
    virtual void Stop();

    e_SEND_ENCODE  m_eFromCode;
    e_SEND_ENCODE  m_eToCode;

private:

    void RemoteData(unsigned char * pBuf);

    //read the audio's type
    bool InitNet();
    bool ExitNet();

    void pcm2adp(e_SEND_ENCODE eType);

    //receive adp data from net
    int                   m_sequenceNo;
};

#endif

