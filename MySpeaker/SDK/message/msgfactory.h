
#ifndef MESSAGE_FACTORY_H
#define MESSAGE_FACTORY_H

#include "predefine.h"
#include "callback.h"
#include "msgsysstatus.h"
#include "msgconnection.h"
#include "msgcommon.h"
#include "msgdevice.h"
#include "msgcheckroute.h"
#include "msgdevice.h"
#include "msgusagereport.h"
#include "msgsysstatus.h"
#include "msgsynctime.h"
#include "mylog.h"

class CMsgFactory
{
public:
    static std::shared_ptr<CMsgBase> CreateMsgClass(
        unsigned char cMid,
        unsigned char sMid,
        CTSystem* pSystem);
};

#endif

