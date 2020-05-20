
#ifndef MESSAGE_CONNECT_REQUEST_H
#define MESSAGE_CONNECT_REQUEST_H

#include "sdkdefine.h"
#include "msgbase.h"
#include "sdkconnect.h"
#include "route.h"
#include "msgid.h"

class CMsgConnectRequest : public CMsgBase
{
public:
    CMsgConnectRequest(CTSystem* pSys);

    void OnRespond(unsigned char* pBuf, const int len);
    
    void SendRouteStatus(t_ActivateRet ret, e_CONNECT_RESPONE eRespone);
  
private:
    void Activate(unsigned char* pBuf, const int len);
    void DeActivate(unsigned char* pBuf, const int len);
    void Interrupt(unsigned char* pBuf, const int len);
    void ReActivate(unsigned char* pBuf, const int len);
    void RouteStatus(unsigned char* pBuf, const int len);
    void CheckRoute(unsigned char* pBuf, const int len);

    unsigned short m_chProcess;
};

#endif

