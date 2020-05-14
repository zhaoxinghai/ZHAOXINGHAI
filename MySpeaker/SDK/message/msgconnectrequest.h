
#ifndef MESSAGE_CONNECT_REQUEST_H
#define MESSAGE_CONNECT_REQUEST_H

#include "predefine.h"
#include "msgbase.h"
#include "connection.h"

enum e_REQUEST
{
    CON_ACTIVATE = 0x00,
    CON_DEACTIVATE = 0x01,
    CON_REACTIVATE = 0x02,
    CON_INTERRUPT = 0x03,
    CON_ROUTESTATE = 0x04,
    CON_CHECKROUTE = 0x09
};

class CMsgConnectRequest : public CMsgBase
{
public:
    CMsgConnectRequest(CTSystem* pSys);

    void OnRespond(unsigned char* pBuf, const int len);
    
    void RouteStatus(CActivateResult &ret, unsigned char = 0x84);
  
private:

    void Activate10(unsigned char* pBuf, const int len);
    void Activate11(unsigned char* pBuf, const int len);
    
    void DeActivate(unsigned char* pBuf, const int len);
    void Interrupt(unsigned char* pBuf, const int len);
    void ReActivate(unsigned char* pBuf, const int len);
    void RouteStatus(unsigned char* pBuf, const int len);
    void CheckRoute(unsigned char* pBuf, const int len);

    unsigned short m_chProcess;
};

#endif

