

#include "msgfactory.h"
#include "service.h"

std::shared_ptr<CMsgBase> CMsgFactory::CreateMsgClass(unsigned char cMid, unsigned char sMid,CTSystem* pSystem)
{
    std::shared_ptr<CMsgBase> Msg;
    unsigned long nMid = cMid;

    switch (nMid)
    {
    case PAMSG_MID_LIFE_SIGNAL://every 10 seconds
        Msg = std::make_shared<CMsgLifeSignal>(pSystem);
        break;

    case PAMSG_MID_BUSY_STATE:
        Msg = std::make_shared<CMsgUsageReport>(pSystem);
        break;

    case PAMSG_MID_CONNECT:
        Msg = std::make_shared<CMsgConnection>(pSystem);
        break;

    //case PAMSG_MID_SETRTC:
        //Msg = std::make_shared<CMsgSyncTime>(pSystem);
        //break;

    //case PAMSG_MID_DEVICE:
    //case PAMSG_MID_DEVICE2:
        //Msg = std::make_shared<CMsgDevice>(pSystem);
        //break;
    default:
        Msg = std::make_shared<CMsgCommon>(pSystem);
    }
    Msg->SetSubMid(sMid);
    return Msg;
}
