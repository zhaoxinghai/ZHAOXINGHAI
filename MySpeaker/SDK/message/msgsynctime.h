
#ifndef MESSAGE_SYNCTIME_H
#define MESSAGE_SYNCTIME_H

#include "predefine.h"
#include "msgbase.h"

class CMsgSyncTime : public CMsgBase
{
public:
    CMsgSyncTime(CTSystem* pSys);
    virtual ~CMsgSyncTime();

    void OnRespond(unsigned char* pBuf, const int len);

    void SyncDataTime(t_DateTime* mytime);

private:
 
};

#endif

