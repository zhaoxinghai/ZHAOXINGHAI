
#ifndef MESSAGE_USAGE_REPORT_H
#define MESSAGE_USAGE_REPORT_H

#include "msgbase.h"

class CMsgUsageReport : public CMsgBase
{
public:
    CMsgUsageReport(CTSystem* pSys);
    virtual ~CMsgUsageReport();

    void OnRespond(unsigned char* pBuf, const int len);

    void SendLocalSourceUsageReport11();
    void SendRemoteSourceUsageReport11();

private:

    //V11 every 15 seconds
    void OnLocalSourceUsageReport11(unsigned char* pBuf, const int len);
    void OnRemoteSourceUsageReport11(unsigned char* pBuf, const int len);
};

#endif

