
#ifndef MESSAGE_BUSYSTATE_H
#define MESSAGE_BUSYSTATE_H

#include "msgbase.h"

class CMsgBusystate : public CMsgBase
{
public:
    CMsgBusystate(CTSystem* pSys);
    virtual ~CMsgBusystate();

    void OnRespond(unsigned char* pBuf, const int len);

    void SendLocalSourceUsageReport11();
    void SendRemoteSourceUsageReport11();

private:
    void OnLocalSourceUsageReport11(unsigned char* pBuf, const int len);
    void OnRemoteSourceUsageReport11(unsigned char* pBuf, const int len);
};

#endif

