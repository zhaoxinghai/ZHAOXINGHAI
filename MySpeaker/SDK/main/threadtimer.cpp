
#include "common.h"
#include "service.h"
#include "threadtimer.h"

void CThreadTimer::Run()
{
	CCommon::SetThreadName("CProtocolTimer");

    while (m_bActive)
    {
        SleepM(200);

    	auto pMsg = std::make_shared<CMsg>();
        pMsg->type = MSG_EVERY_200_MINISECOND;
    	g_SDKServer.Push(pMsg);
    }
}
