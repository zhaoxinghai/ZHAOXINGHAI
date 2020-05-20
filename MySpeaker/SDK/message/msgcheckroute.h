
#ifndef MESSAGE_CHECKROUTE_H
#define MESSAGE_CHECKROUTE_H

#include "sdkdefine.h"
#include "msgbase.h"

class CMsgCheckRoute : public CMsgBase
{
public:
    CMsgCheckRoute(CTSystem* pSys);
    virtual ~CMsgCheckRoute();

    void OnRespond(unsigned char* pBuf, const int len);

    void Check(unsigned short chRequest, unsigned short chProcess);
    void ReplayCheck(unsigned short chRequest, unsigned short chProcess, bool bOK);

private:
 
};

#endif

