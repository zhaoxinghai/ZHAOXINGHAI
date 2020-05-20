
#ifndef RECVMSG_H
#define RECVMSG_H

#include "sdkdefine.h"
#include "mythread.h"
#include "topsystem.h"

class CThreadRecv: public CMyThread
{
public:
    CThreadRecv(bool bTCP);
    virtual ~CThreadRecv();

    virtual void Run();
    virtual bool EndThread(bool bWait);

    void RunTCP();

    //for Protocol V10
    void RunUDP();

    //for Protocol V11
    void RunUDPMulticast();

    bool IsTCP();

    void AddScoket(MYSOCK sock);
    void RemoveSocket(MYSOCK sock);

    int GetSocketSize();

    void EnableMulticast(){m_bMulticast = true;};
    void SetMulticastAddress(std::string strAddr);
    void SetUDPPort(int nPort);

protected:

    int  SetSocket();
    void TCPReceive();
    void TCPReceive(MYSOCK sock);
    int  UDPReceive(MYSOCK sock);

    std::vector<MYSOCK>      m_vScoket;
    std::vector<MYSOCK>      m_vScoketTemp;

    fd_set             m_fdRead;
    MYSOCK             m_maxSock;
    CMySocket          m_Sock2;
    CMySocket          m_Sock;
    bool               m_bTCP;
    bool               m_bMulticast;
    std::string        m_strMulticast;
    int                m_nPort;
};

#endif

