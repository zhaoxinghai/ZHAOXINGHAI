
#include "threadsend.h"
#include "define.h"
#include "service.h"
#include "common.h"

CThreadSend::CThreadSend()
{
}

CThreadSend::~CThreadSend()
{
}

void CThreadSend::Run()
{
	CCommon::SetThreadName("CThreadSend");

    CMySocket::Init();
    m_sockUDP.Create(SOCK_DGRAM);

    //for udp broadcast
    if (!m_sockBroadcast.Create(SOCK_DGRAM))
    {
        LOG_ERROR("%s", "m_sockBroadcast.Create");
        return;
    }

    if (!m_sockBroadcast.SetBroadcast())
    {
        LOG_ERROR("%s", "m_sockBroadcast.Create");
        return;
    }
    m_sockBroadcast.SetTTL();
    //thread run
    auto pMsg = std::make_shared<CMsg>();
    pMsg->type = MSG_SEND_THREAD_RUN;
    CService::GetInstance()->Push(pMsg);

    while (m_bActive)
    {
        std::unique_lock < std::mutex > lck(m_mutex);
        m_cv.wait(lck);

        while (!m_queueMsg.empty())
        {
            std::shared_ptr<t_SendMsg> pMsg = m_queueMsg.front();
            m_queueMsg.pop();

            RunMessage(pMsg.get());
        }
    }

    //Release
    while (!m_queueMsg.empty())
    {
        std::shared_ptr<t_SendMsg> pMsg = m_queueMsg.front();
        m_queueMsg.pop();
    }

    m_sockUDP.Close();
    m_sockBroadcast.Close();
    CMySocket::Exit();
}

void CThreadSend::RunMessage(t_SendMsg * pMsg)
{
    if (pMsg->bTCP)
	{
        Send(pMsg->sock, &pMsg->szData[0], pMsg->len);
	}
    else if (pMsg->bBroadcast)
    {
        SendMulticast(pMsg->szIP, &pMsg->szData[0], pMsg->len);
    }
    else
	{
		SendTo(m_sockUDP.GetSocket(), pMsg->szIP, &pMsg->szData[0], pMsg->len);
	}
}

void CThreadSend::SendMsg(std::shared_ptr<t_SendMsg> pMsg)
{
    if(CService::GetInstance()->IsExit())
        return;

    std::unique_lock < std::mutex > lck(m_mutex);

    m_queueMsg.push(pMsg);

    m_cv.notify_one();
}

bool CThreadSend::Send(MYSOCK sock, const char* buf, int len)
{
    int count = 0;
    while (count < len)
    {

#ifdef _MSC_VER
        int write = send(sock, buf + count, len - count, 0);
#else
        int write = send(sock, (buf + count), len - count, MSG_NOSIGNAL);
#endif

        if (write <= 0)
        {
            return false;
        }
        count += write;
    }
    return true;
}

bool CThreadSend::SendTo(MYSOCK sock,const char* ip, const char* buf, int len)
{
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(1120);
  
    int ret = sendto(sock, buf, len, 0,(sockaddr*)&addr, sizeof(addr));
    if (ret != len)
    {
        return false;
    }
    return true;
}

bool CThreadSend::SendBroadcast(const char* buf, int len)
{
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    addr.sin_port = htons(1120);

    int ret = sendto(m_sockBroadcast.GetSocket(), buf, len, 0, (sockaddr*)&addr, sizeof(addr));
    if (ret != len)
    {
        return false;
    }
    return true;
}

bool CThreadSend::SendMulticast(const char* ip, const char* buf, int len)
{
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(1120);

    int ret = sendto(m_sockBroadcast.GetSocket(), buf, len, 0, (sockaddr*)&addr, sizeof(addr));
    if (ret != len)
    {
        return false;
    }
    return true;
}
