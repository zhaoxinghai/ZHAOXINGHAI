
/*
* Date:     2016-8-4
* Author :  xinghai,zhao
* Content : this object is to supply audio stream's data source
*/

#ifndef AUDIO_RECEIVE_H
#define AUDIO_RECEIVE_H

#ifndef _MSC_VER
#include <pcap.h>
#endif

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

private:

    void RemoteData(unsigned char * pBuf);

    //read the audio's type
    bool InitNet();
    bool ExitNet(bool bPacap);

    bool InitNetPcap();

    bool ReadPcap(unsigned char *strRead);

    //wav format
    //t_StreamParameter         m_wavFormat;

    //receive adp data from net
    int                       m_sequenceNo;

#ifndef _MSC_VER
    pcap_t                   *m_pcap_handle;
#endif

};

#endif

