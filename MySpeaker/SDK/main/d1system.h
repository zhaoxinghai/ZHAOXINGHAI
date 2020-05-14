
#ifndef D1SYSTEM_H
#define D1SYSTEM_H

#include "transport.h"
#include "predefine.h"
#include "mysocket.h"
#include "msgbase.h"
#include "define.h"

class CTransport;
class CMsgBase;
class CTSystem
{
public:
    CTSystem(t_Node &Node);
    virtual ~CTSystem();

    //send this message
    void OnSend(CMsgBase* pMsg);

    bool IsInit();
    bool IsConnect();
    void SetDisConnnect();
    void SetConnect();
    bool SetSocket(MYSOCK sock);

    void OnLocalError(bool bError);

    void SourceUsageLocal(std::vector<t_LocalCall>& vLocalReport);
    void SourceUsageRemote(std::vector<t_RemoteCall>& vRemoteReport);
    void ClearUsage(bool bLocalSource);

    int GetNode();

    bool IsConfigSystem() { return m_bIsConfig; };
    void SetConfigSystem() { m_bIsConfig = true; };
    void SetSelfSystem();
    void SetTCP(bool bTCP){m_IsTCP = bTCP;};
    bool IsTCP(){return m_IsTCP;};

	void TextOutput(const std::string strResult);
public:
    std::shared_ptr<CTransport> m_pTransport;

    bool            m_bIsConfig;
    bool            m_bIsSelfSystem;
    bool            m_bLocalError;
    t_Node          m_Node;
    MYSOCK          m_sock;
    bool            m_IsTCP;
    int             m_nSysStatusSeconds;      //only for UDP
    int             m_nCommunicationSeconds;  //only for TCP

    t_UsageReport* GetSourceUsage(int nRTPChannel);
    void SourceUsageUpdate(t_UsageReport &srcUsage);

protected:
    t_UsageReportExt* GetReportByDestID(int id, std::vector<t_UsageReportExt>& vReportExt);

    bool m_bIsConnect;
    bool m_bIsInit;

    //usage report and extension
    unsigned int                m_nUsageCount;
    CUsageReportResult          m_Usage;
    bool                        m_bMonitorInput;
};

#endif

