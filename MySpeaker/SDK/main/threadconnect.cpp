
#include "mysocket.h"
#include "common.h"
#include "service.h"
#include "threadconnect.h"


void CThreadConnect::Run()
{
	CCommon::SetThreadName("CThreadConnect");

    CMySocket::Init();
    int second = 4;

    while (m_bActive)
    {
        second++;
  
        //for reconnect
        if (second == 5)
        {
            second = 0;
            Connect();
        }
        
        for (int i = 0; i < 10; i++)
        {
            if (!m_bActive)
                break;

            CCommon::SleepMinisecond(100);
        }
    }

    CMySocket::Exit();
}

void CThreadConnect::Connect()
{
    vector<string> vIP;
    GetVIP(vIP);

    for (unsigned int i = 0;i < vIP.size();i++)
    {
        if (!m_bActive)
            break;

        CMySocket sock;
        sock.SetNotAutoClose();

        //create socket error
        if (!sock.Create(SOCK_STREAM))
        {
            continue;
        }

        //connect
        if (sock.Connect(vIP[i].c_str(), 1120))
        {
            auto pMsg = std::make_shared<CMsg>();

            pMsg->type = MSG_SOCK_CONNECT;
            strcpy(pMsg->szIP, vIP[i].c_str());
            pMsg->socket = sock.GetSocket();
            CService::GetInstance()->Push(pMsg);
        }
        else
        {
            sock.Close();
        }
    }
}

void CThreadConnect::AddIP(string strIP)
{
    std::unique_lock < std::mutex > lck(m_mutex);

    for (unsigned int i = 0;i < m_vIP.size();i++)
    {
        if (m_vIP[i] == strIP)
            return;
    }

    m_vIP.push_back(strIP);
}

void CThreadConnect::RemoveIP(string strIP)
{
    std::unique_lock < std::mutex > lck(m_mutex);

    vector<string>::iterator iter = m_vIP.begin();
    while (iter != m_vIP.end())
    {
        if ((*iter) == strIP)
        {
            iter = m_vIP.erase(iter);
            return;
        }
        iter++;
    }
}

void CThreadConnect::GetVIP(vector<string>& vIP)
{
    std::unique_lock < std::mutex > lck(m_mutex);

    vIP = m_vIP;
}
