
#ifndef DEFINE_H
#define DEFINE_H

#include <assert.h>
#include "callback.h"
#include "device.h"
#include "connection.h"
#include "mysocket.h"

//audio dsp is double channel
#define AUDIO_DOUBLE_CHANNEL  1

//interface message
#define MSG_NORMAL_ANNOUNCEMENT             1
#define MSG_LOCAL_PLAY_ANNOUNCEMENT         4
#define MSG_LOCAL_MIC_ANNOUNCEMENT          5
#define MSG_STOP_ANNOUNCEMENT               6
#define MSG_STOP_ALL_AUDIO                  7

#define MSG_LOCAL_RECORD                    8
#define MSG_STOP_LOCAL_RECORD               9
#define MSG_SYNC_DATATIME                   11
#define MSG_TRANSPORT_LINE                  12
#define MSG_DEVICE_VOLUME_AJUST             13

//network message
#define MSG_SOCK_CONNECT                    33
#define MSG_SOCK_DISCONNECT                 34
#define MSG_SOCK_UDP_RECV                   35
#define MSG_SOCK_TCP_RECV                   36                  

//audiojob message
#define MSG_AUDIO_FINISH                    37
#define MSG_EVERY_200_MINISECOND            38
#define MSG_VOLUME_AJUST                    39

#define MSG_PLAY_INDEX                      41

#define MSG_RESTORE_INPUT                   42
#define MSG_RESTORE_OUTPUT                  43

#define MSG_DECODE_FILE                     44

#define MSG_AUDIO_FINISH_PAUSE              47
#define MSG_AUDIO_FINISH_CLEAN              48
#define MSG_VOLUME_AJUST_PORT               50

#define MSG_GET_DEVICE_STATE                52
#define MSG_ALSA_INIT_FINISH                57

#define MSG_SEND_USAGE_REPORT               58
#define MSG_GONG_SIGNAL                     59
#define MSG_AUDIO_SIGNAL                    60

#define MSG_SEND_THREAD_RUN                 61
#define MSG_AUDIO_CONTROL                   62
#define MSG_TEXT_OUT                        63
#define MSG_VOL_REPLY                       64

#define MSG_LAMP_TEST                       68

struct t_UsedPriority
{
    t_UsedPriority()
    {
        bUsed = false;
        nPriority = 255;
        nRtpChannel = 0;
    }
    bool   bUsed;
    int    nPriority;
    int    nRtpChannel;
};

class CMsg
{
public:
    CMsg()
    {
        type = 0;
        pointer = NULL;
        nInt1 = 0;
        nInt2 = 0;
        socket = 0;
        memset(&szbuffer, 0, sizeof(szbuffer));
        memset(&szIP, 0, sizeof(szIP));
    }
    virtual ~CMsg()
    {
    }
    int         type;
    int         nInt1;
    int         nInt2;
    char        szIP[30];
    MYSOCK      socket;
    void*       pointer;
 
    union
    {
    	char           szbuffer[PACKET_BUFLEN];
        t_DateTime     datatime;
        t_UsedPriority usedpriority;
        float          fVolume;
        t_VDevice      device;
        t_VolReply     volreply;
        t_VLANConfig   vlanconfig;
    };
};

struct t_LocalCall
{
    t_LocalCall()
    {
        CS = 0;
        CSNode = 0;
        AI = 0;
        Title = 0;
        ConState = 0;
        Priority = 0;
        DstRTP = 0;
    }

    t_LocalCall(const t_LocalCall& c)
    {
        this->CS = c.CS;
        this->CSNode = c.CSNode;
        this->AI = c.AI;
        this->Title = c.Title;
        this->ConState = c.ConState;
        this->Priority = c.Priority;
        this->DstRTP = c.DstRTP;
        this->vOutput.assign(c.vOutput.begin(),c.vOutput.end());
    }

    unsigned short  CS;
    unsigned short  CSNode;
    unsigned char   AI;
    unsigned char   Title;
    unsigned char   ConState;
    unsigned char   Priority;
    unsigned char   DstRTP;

    std::vector<int> vOutput;
};

struct t_RemoteCall
{
    t_RemoteCall()
    {
        CS = 0;
        CSNode = 0;
        RTPChannel = 0;
        RTPNode = 0;
        flags = 0;
    }

    t_RemoteCall(const t_RemoteCall &c)
    {
        this->CS = c.CS;
        this->CSNode = c.CSNode;
        this->RTPChannel = c.RTPChannel;
        this->RTPNode = c.RTPNode;
        this->flags = c.flags;
        this->vOutput.assign(c.vOutput.begin(),c.vOutput.end());
    }

    unsigned short  CS;
    unsigned short  CSNode;
    unsigned char   RTPChannel;
    unsigned short  RTPNode;
    unsigned char   flags;

    std::vector<int> vOutput;
};

//usage report extension
struct t_PADevice
{
    unsigned short Net;           // network number
    unsigned short Node;          // node number
    unsigned short Type;          // device type
    unsigned short Number;        // device number
    unsigned short Channel;       // device channel
    unsigned short Title;         // title
};

struct t_UsageReportExt
{
    t_UsageReportExt()
    {
        memset(&destination, 0, sizeof(destination));
        memset(&source, 0, sizeof(source));

        destination.Type = AS_TYPE_AO;
        source.Type = AS_TYPE_AI;
    }
    t_PADevice destination;
    t_PADevice source;
};

struct t_Output
{
    t_Output()
    {
        nPort = -1;
        bConnect = false;
        bInterrupt = false;
    }
    int  nPort;
    bool bConnect;
    bool bInterrupt;  //when this true,this port can't reconnect
};

#endif
