
#ifndef MESSAGE_ASKVERSION_H
#define MESSAGE_ASKVERSION_H

#include "predefine.h"
#include "msgbase.h"

class CMsgCommon : public CMsgBase
{
public:
    CMsgCommon(CTSystem* pSys);
    virtual ~CMsgCommon();

    void OnRespond(unsigned char* pBuf, const int len);

    void AskVersion();
    void AskLogin();

    void OnVersion10(unsigned char* pBuf, const int len);
    void OnVersion11(unsigned char* pBuf, const int len);

    void OnLogin(unsigned char* pBuf, const int len);

    void OnTransportLine(unsigned char* pBuf, const int len);
    void TransportLine(bool bAction, std::vector<t_TL>& vTL);

    void OnTextIn(unsigned char* pBuf, const int len);
    void TextOutput(std::string strResult);

    //for special function
    void OnSpecialFunction(unsigned char* pBuf,const int len);
    void SpecialFunction(unsigned short number,bool bActivate);
};

#endif

