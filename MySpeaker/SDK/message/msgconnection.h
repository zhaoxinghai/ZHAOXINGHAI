
#ifndef MESSAGE_CONNECTION_H
#define MESSAGE_CONNECTION_H

#include "sdkdefine.h"
#include "msgbase.h"
#include "sdkconnect.h"
#include "route.h"
#include "msgid.h"

class CMsgConnection : public CMsgBase
{
public:
    CMsgConnection(CTSystem* pSys);

    void OnRespond(unsigned char* pBuf, const int len);

    //command
    int Activate(CAnnouncement &act);
    int ReActivate(int nRequest, int nProcess);
    int DeActivate(int nRequest, int nProcess);
    int Interrupt(int nRequest, int nProcess);

private:

    void WriteSrcEI(std::vector<t_Source> &vSrc);
    void WriteDestEI(std::vector<t_Destination> &vDest);
    void WriteOneSrc(t_Source&src);

    //respond(route status)
    void OnRespond(unsigned char* pBuf, const int len,e_CONNECT_RESPONE response);

    void OnCheckRoute(unsigned char* pBuf, const int len);
};

#endif

