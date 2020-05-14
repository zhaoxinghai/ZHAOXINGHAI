
#include "d1system.h"
#include "msgfactory.h"
#include "sdk.h"
#include "service.h"
#include "mylog.h"
#include "common.h"

#define LF      (char)10
#define CR      (char)13

CTSystem::CTSystem(t_Node &Node)
{
	m_bIsSelfSystem = false;
    m_bIsConnect = false;
    m_bLocalError = false;
    m_bIsInit = false;
    m_bIsConfig = false;
    m_bMonitorInput = false;
    m_nUsageCount = 0;

#ifdef _MSC_VER
    m_sock = INVALID_SOCKET;
#else
    m_sock = -1;
#endif

    m_pTransport = std::make_shared<CTransport>(this);
    m_IsTCP = false;
    m_nSysStatusSeconds = 0;
    m_nCommunicationSeconds = 0;
    m_Usage.strIP = m_Node.strIP;
}

CTSystem::~CTSystem()
{
}

int CTSystem::GetNode()
{
    return m_Node.nNode;
}

void CTSystem::OnSend(CMsgBase* pMsg)
{
    m_pTransport->Send(pMsg);
}

bool CTSystem::IsInit()
{
    return m_bIsInit;
}

bool CTSystem::IsConnect()
{
    return m_bIsConnect;
}

void CTSystem::SetDisConnnect()
{
    m_bIsInit = true;
    m_pTransport->Clear();
    m_bIsConnect = false;

    m_nSysStatusSeconds = 0;
    m_nCommunicationSeconds = 0;

    if (m_IsTCP)
    {
        CMySocket::CloseSocket(m_sock);
    }

    CConnectResult ret;
    ret.strIP = m_Node.strIP;
    ret.bTCP = m_IsTCP;
    ret.sock = m_sock;
    ret.bConnect = false;
    ret.bStandalone = CService::GetInstance()->IsStandAlone();
    CService::GetInstance()->ExcuteCallback(&ret);
}

void CTSystem::SetConnect()
{
    m_bIsInit = true;
    m_bIsConnect = true;

	CConnectResult ret;
    ret.strIP = m_Node.strIP;
    ret.bTCP = m_IsTCP;
    ret.type = m_Node.eType;
	ret.bConnect = true;
    ret.bStandalone = CService::GetInstance()->IsStandAlone();
    CService::GetInstance()->ExcuteCallback(&ret);
}

bool CTSystem::SetSocket(MYSOCK sock)
{
    m_sock = sock;
    return true;
}

void CTSystem::OnLocalError(bool bError)
{
    if(m_bLocalError == bError)
        return;

    m_bLocalError = bError;

    CSysStateResult ret;
    ret.bErrorChange = true;
    ret.strIP = m_Node.strIP;
    ret.IsError = bError;
    CService::GetInstance()->ExcuteCallback(&ret);
}

t_UsageReportExt* CTSystem::GetReportByDestID(int id, std::vector<t_UsageReportExt>& vReportExt)
{
    for (unsigned int i = 0; i < vReportExt.size(); i++)
    {
        if (id == vReportExt[i].destination.Number)
        {
            return &vReportExt[i];
        }
    }
    return NULL;
}

void CTSystem::SourceUsageLocal(std::vector<t_LocalCall>& vLocalReport)
{
    CService* pSer = CService::GetInstance();
    ClearUsage(true);

    for (unsigned int i = 0; i < vLocalReport.size(); i++)
    {
        t_LocalCall &call = vLocalReport[i];

        t_UsageReport Usage;
        Usage.bInput = true;
        Usage.bLocalSource = true;
        Usage.ConState = call.ConState;
        Usage.nPort = call.AI + 1;  //OK
        Usage.nTitle = call.Title;
        Usage.nRtpNode = 1;//GetNode();
        Usage.nRtpChannel = call.DstRTP;
        Usage.nPriority = call.Priority;
        Usage.nCSNode = call.CSNode;
        Usage.nCSNumber = call.CS;

        pSer->SourceUsageUpdate(Usage);
        m_Usage.vBusyInfo.push_back(Usage);

        //output
        for (unsigned int j = 0; j < call.vOutput.size(); j++)
        {
            Usage.bInput = false;
            Usage.nPort = call.vOutput[j]; //OK
            m_Usage.vBusyInfo.push_back(Usage);
        }
    }
}

void CTSystem::SourceUsageRemote(std::vector<t_RemoteCall>& vRemoteReport)
{
    CService* pSer = CService::GetInstance();
    ClearUsage(false);

    //rtp local output
    for (unsigned int i = 0; i < vRemoteReport.size(); i++)
    {
        t_RemoteCall &call = vRemoteReport[i];
        for (unsigned int j = 0; j < call.vOutput.size(); j++)
        {
            t_UsageReport Usage;
            Usage.bInput = false;
            Usage.bLocalSource = false;
            Usage.nPort = call.vOutput[j];  //OK
            Usage.nCSNode = call.CSNode;
            Usage.nCSNumber = call.CS;
            Usage.nRtpNode = call.RTPNode;
            Usage.nRtpChannel = call.RTPChannel;

            //Get Source
            t_UsageReport* p = pSer->GetSourceUsage(Usage.nRtpNode,Usage.nRtpChannel);
            if(p != NULL)
            {
                Usage.nPriority = p->nPriority;
            }
            else
            {
                Usage.nPriority = 0;
            }
            m_Usage.vBusyInfo.push_back(Usage);
        }
    }

    if(m_nUsageCount != 0 || m_Usage.vBusyInfo.size()!=0)
    {
        pSer->ExcuteCallback(&m_Usage);
    }
    m_nUsageCount = m_Usage.vBusyInfo.size();
}

void CTSystem::SetSelfSystem()
{
	m_bIsSelfSystem = true;
}

t_UsageReport* CTSystem::GetSourceUsage(int nRTPChannel)
{
    for(unsigned int i = 0;i<m_Usage.vBusyInfo.size();i++)
    {
        t_UsageReport &usage = m_Usage.vBusyInfo[i];
        if(usage.bLocalSource
                && usage.bInput
                && usage.nRtpChannel == nRTPChannel)
        {
            return &usage;
        }
    }
    return NULL;
}

void CTSystem::SourceUsageUpdate(t_UsageReport &srcUsage)
{
    bool bUpdate = false;
    for(unsigned int i = 0;i<m_Usage.vBusyInfo.size();i++)
    {
        t_UsageReport &usage = m_Usage.vBusyInfo[i];
        if(usage.nPriority == 0
                && !usage.bInput
                && usage.nRtpNode == srcUsage.nRtpNode
                && usage.nRtpChannel == srcUsage.nRtpChannel)
        {
            usage.nPriority = srcUsage.nPriority;
            bUpdate = true;
        }
    }
    if(bUpdate)
    {
        CService::GetInstance()->ExcuteCallback(&m_Usage);
    }
}

void CTSystem::ClearUsage(bool bLocalSource)
{
    auto it = m_Usage.vBusyInfo.begin();
    while(it != m_Usage.vBusyInfo.end())
    {
        if((*it).bLocalSource == bLocalSource)
        {
            it = m_Usage.vBusyInfo.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void CTSystem::TextOutput(const std::string strResult)
{
    std::vector<std::string> vLine;
    CCommon::Split(vLine,strResult,"\n");
    CMsgCommon msg(this);
    for(unsigned int i = 0;i<vLine.size();i++)
    {
        CCommon::TrimeRight(vLine[i],"\r");
        msg.TextOutput(vLine[i]);
    }
}

