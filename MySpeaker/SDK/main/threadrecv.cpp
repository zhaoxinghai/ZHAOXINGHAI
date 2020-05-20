
#include "threadrecv.h"
#include "service.h"
#include "mylog.h"
#include "common.h"

CThreadRecv::CThreadRecv(bool bTCP)
{
	m_maxSock = 0;
    m_bTCP = bTCP;
    m_bMulticast = false;
    m_nPort = 5966;
}

CThreadRecv::~CThreadRecv()
{
}

void CThreadRecv::AddScoket(MYSOCK sock)
{
    std::unique_lock < std::mutex > lck(m_mutex);

    m_vScoket.push_back(sock);
}

void CThreadRecv::RemoveSocket(MYSOCK sock)
{
    std::unique_lock < std::mutex > lck(m_mutex);

    std::vector<MYSOCK>::iterator iter = m_vScoket.begin();
    while (iter != m_vScoket.end())
    {
        if ((*iter)==sock)
        {
            iter = m_vScoket.erase(iter);
            return;
        }
        iter++;
    }
}

int CThreadRecv::GetSocketSize()
{
    std::unique_lock < std::mutex > lck(m_mutex); 

    int len = m_vScoket.size();

    return len;
}

int CThreadRecv::SetSocket()
{
	{
		std::unique_lock < std::mutex > lck(m_mutex);
		m_vScoketTemp = m_vScoket;
	}

    int iCount = 0;
    m_maxSock = 0;
    FD_ZERO(&m_fdRead);

    for (unsigned int i = 0;i < m_vScoketTemp.size();i++)
    {
        m_maxSock = std::max(m_maxSock, m_vScoketTemp[i]);
        FD_SET(m_vScoketTemp[i],&m_fdRead);
        iCount++;
    }
    return iCount;
}

bool CThreadRecv::EndThread(bool bWait)
{
    m_bActive = false;
    m_Sock.Close();
    return CMyThread::EndThread(bWait);
}

void CThreadRecv::Run()
{
	CCommon::SetThreadName("CThreadRecv");

    CMySocket::Init();

    if (m_bTCP)
    {
        RunTCP();
    }
    else
    {
        if (m_bMulticast)
        {
            RunUDPMulticast();
        }
        else
        {
            RunUDP();
        }
    }
    CMySocket::Exit();
}

void CThreadRecv::RunTCP()
{
    while (m_bActive)
    {
        int iCount = SetSocket();
        
        //no socket
        if (iCount == 0)
        {
            SleepM(1000);
            continue;
        }

		int ret = select((int)m_maxSock + 1, &m_fdRead, NULL, NULL, NULL);
		if (ret == 0)
		{
			//timeout,go on doing
			continue;
		}
		else if (ret == -1)
		{
            SleepM(1000);
			continue;
		}
		else
		{
			TCPReceive();
		}
    }
}

bool CThreadRecv::IsTCP()
{
    return m_bTCP;
}

void CThreadRecv::RunUDP()
{
    if (!m_Sock.Create(SOCK_DGRAM))
    {
        LOG_ERROR("%s", "sock.create");
        return;
    }

    if(!m_Sock.SetReuseAddr())
    {
        LOG_ERROR("%s","m_Socket.SetReuseAddr");
        return;
    }

    if (!m_Sock.Bind(m_nPort))
    {
        LOG_ERROR("%s", "sock.Bind");
        return;
    }

    int iRead = 0;
    while (m_bActive)
    {
        iRead = UDPReceive(m_Sock.GetSocket());
        if(iRead <= 0 && m_bActive)
        {
            SleepM(1000);
        }
    }
}

void CThreadRecv::SetMulticastAddress(std::string strAddr)
{ 
    m_strMulticast = strAddr; 
}

void CThreadRecv::SetUDPPort(int nPort)
{
    m_nPort = nPort;
}

void CThreadRecv::RunUDPMulticast()
{
    if (!m_Sock.Create(SOCK_DGRAM))
    {
        LOG_ERROR("%s", "RunUDPMulticast sock.create");
        return;
    }

    if(!m_Sock.SetReuseAddr())
    {
        LOG_ERROR("%s","m_Socket.SetReuseAddr");
        return;
    }

    if (!m_Sock.SetUDPAddress(m_strMulticast.c_str(), m_nPort))
    {
        LOG_ERROR("%s","RunUDPMulticast m_Sock.SetUDPAddress");
        return;
    }

    if (!m_Sock.JoinGroup(m_strMulticast.c_str()))
    {
        LOG_ERROR("%s", "RunUDPMulticast m_Sock.JoinGroup");
        return;
    }

    //bind port
    if (!m_Sock.Bind(m_nPort))
    {
        LOG_ERROR("%s", "RunUDP m_Sock.Bind");
        return;
    }

    int iRead = 0;
    while (m_bActive)
	{
		iRead = UDPReceive(m_Sock.GetSocket());
        if(iRead <= 0 && m_bActive)
		{
            SleepM(1000);
		}
	}
}

int CThreadRecv::UDPReceive(MYSOCK sock)
{
    //from
    struct sockaddr_in from;
    memset(&from, 0, sizeof(from));
#ifdef _MSC_VER
    int fromlen = sizeof(from);
#else
    socklen_t fromlen = sizeof(from);
#endif

    char szbuf[PACKET_BUFLEN];
    int iRead = recvfrom(sock, szbuf, PACKET_BUFLEN, 0,
        (struct sockaddr*)&from, &fromlen);

    if (iRead>0)
    {
        auto pMsg = std::make_shared<CMsg>();
        CMsg* p = pMsg.get();
        p->type = MSG_SOCK_UDP_RECV;
        memcpy(p->szbuffer, szbuf, iRead);

        strncpy(p->szIP, inet_ntoa(from.sin_addr),sizeof(p->szIP)-1);
        p->nInt1 = iRead;
        g_SDKServer.Push(pMsg);
    }
    return iRead;
}

void CThreadRecv::TCPReceive()
{
    for (unsigned int i = 0;i < m_vScoketTemp.size();i++)
    {
        if (FD_ISSET(m_vScoketTemp[i], &m_fdRead))
        {
            TCPReceive(m_vScoketTemp[i]);
        }
    }
}

void CThreadRecv::TCPReceive(MYSOCK sock)
{
    /*
    auto pMsg = std::make_shared<CMsg>();
    pMsg->socket = sock;
    int iRead = recv(sock, pMsg.get()->szbuffer, PACKET_BUFLEN, 0);

    if (iRead > 0)
    {
        pMsg->type = MSG_SOCK_TCP_RECV;
        pMsg->nInt1 = iRead;
        g_SDKServer.Push(pMsg);
    }
    else
    {
        RemoveSocket(sock);
        pMsg->type = MSG_SOCK_DISCONNECT;
        g_SDKServer.Push(pMsg);
    }
    */
}

