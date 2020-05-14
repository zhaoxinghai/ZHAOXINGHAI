
#ifndef _INTERFACEMSG_H
#define _INTERFACEMSG_H

#include "sdk.h"
#include "mylog.h"
#include "common.h"
#include "threadrecv.h"
#include "threadsend.h"
#include "threadtimer.h"
#include "service.h"

class CInterface
{
public:
    CInterface();
    virtual ~CInterface();

    static bool RunMessage(CMsg* pMsg);

    static void SyncDataTime(t_DateTime* mytime);
  
    static void NormalAnnouncement(int nNode,CActivate* pActivate);
    static void LocalPlayAnnouncement(CActivatePlay *pActivate);
    static void LocalMicrAnnouncement(CActivateMicr *pActivate);
    static void StopAnnouncement(int chProcess);

    static void LocalRecord(int nRequest, int port, std::string strPath);

    static void SetVolume(bool bCapture,int port,float fVolume);
};

#endif
