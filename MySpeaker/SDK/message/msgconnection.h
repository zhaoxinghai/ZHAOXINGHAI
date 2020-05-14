
#ifndef MESSAGE_CONNECTION_H
#define MESSAGE_CONNECTION_H

#include "predefine.h"
#include "msgbase.h"
#include "connection.h"

enum e_RESPONE
{
    CON_ON_ACTIVATE = 0x80,
    CON_ON_DEACTIVATE = 0x81,
    CON_ON_REACTIVATE = 0x82,
    CON_ON_INTERRUPT = 0x83,
    CON_ON_ROUTESTATE = 0x84,
    CON_ON_CHECKROUTE = 0x89
};

class CMsgConnection : public CMsgBase
{
public:
    CMsgConnection(CTSystem* pSys);

    void OnRespond(unsigned char* pBuf, const int len);

    //command
    int Activate(CActivate* pPACon);
    int ReActivate(CReActivate* pReAct);

    int DeActivate(CDeActivate* pDe);
    int DeActivateViacsNum(unsigned short csNum);

    int DeActivateAll();

    int Interrupt(CDeActivate* pDe);

private:

    void WriteSrcEI(std::vector<t_AudioSrc> &vSrc);
    void WriteDestEI(std::vector<t_AudioDest> &vDest);
    void WriteOneSrc(t_AudioSrc &src);
    void WriteRtpSrc(CActivate* pPACon);

    void TrimEmpty(unsigned char* pDest, unsigned char &begin, unsigned char &end);

    //Is local audio source
    bool IsRtpStream(CActivate* pPACon);
    int  GetGongCount(CActivate* pPACon);

    //respond(route status)
    void OnRespond(unsigned char* pBuf, const int len,e_RESPONE response);

    void OnCheckRoute(unsigned char* pBuf, const int len);

    void OnDeActivateViacsNum(unsigned char* pBuf, const int len);
};

#endif

