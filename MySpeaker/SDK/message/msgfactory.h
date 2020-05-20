
#ifndef MESSAGE_FACTORY_H
#define MESSAGE_FACTORY_H

#include "sdkdefine.h"
#include "sdkcallback.h"
#include "msglifesignal.h"
#include "msgconnection.h"
#include "msgconnectrequest.h"
#include "msgcommon.h"
#include "msgvolume.h"
#include "msgcheckroute.h"
#include "msgbusystate.h"
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

