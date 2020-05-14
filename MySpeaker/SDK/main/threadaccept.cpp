
#include "threadaccept.h"
#include "service.h"
#include "common.h"

CThreadAccept::CThreadAccept()
{
}

CThreadAccept::~CThreadAccept()
{
}

void CThreadAccept::Run()
{
    CCommon::SetThreadName("CThreadTelnetAccept");

    CMySocket::Init();

    if (!m_Socket.Create(SOCK_STREAM))
	{
        LOG_ERROR("Socket Create,Error code: %d", m_Socket.GetLastError());
		return;
	}

    if (!m_Socket.SetReuseAddr())
    {
        LOG_ERROR("Socket SetReuseAddr,Error code: %d", m_Socket.GetLastError());
        return;
    }

    if (!m_Socket.Bind(1120))
    {
        LOG_ERROR("%s", "Bind 1120");
        return;
    }

    if (!m_Socket.Listen())
    {
        LOG_ERROR("%s", "Listen");
        return;
    }

    while (m_bActive)
    {
        MYSOCK socket;
        char szIP[50];
        if (m_Socket.Accept(socket, szIP,sizeof(szIP)) && m_bActive)
        {
            auto pMsg = std::make_shared<CMsg>();

            strncpy(pMsg->szIP, szIP,sizeof(pMsg->szIP)-1);
            pMsg->type = MSG_SOCK_CONNECT;
            pMsg->socket = socket;
            CService::GetInstance()->Push(pMsg);
        }
        else
        {
            break;
        }
    }

    CMySocket::Exit();
}

bool CThreadAccept::EndThread(bool bWait)
{
    m_bActive = false;
    m_Socket.Close();
    return CMyThread::EndThread(bWait);
}
