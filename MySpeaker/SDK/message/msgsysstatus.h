
#ifndef MESSAGE_SYSSTATUS_H
#define MESSAGE_SYSSTATUS_H

#include "predefine.h"
#include "msgbase.h"

class CMsgLifeSignal : public CMsgBase
{
public:
    CMsgLifeSignal(CTSystem* pSys);

    void OnRespond(unsigned char* pBuf, const int len);

    void Broadcast();

private:

    e_DEVICE_TYPE GetDeviceType(int Val);

    void GenerateMsg();
};

#endif

