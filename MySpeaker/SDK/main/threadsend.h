
#ifndef SENDMSG_H
#define SENDMSG_H

#include "predefine.h"
#include "mythread.h"
#include "mysocket.h"
#include "device.h"

//the message from mainthead to sendthread
struct t_SendMsg
{
    t_SendMsg()
    {
        bTCP = false;
        bBroadcast = false;
        sock = 0;
        memset(szIP,0,sizeof(szIP));
        memset(szData, 0, sizeof(szData));
        len = 0;
    }
    t_SendMsg(const t_SendMsg *pMsg)
    {
        this->bTCP = pMsg->bTCP;
        this->bBroadcast = pMsg->bBroadcast;
        this->sock = pMsg->sock;
        this->len = pMsg->len;
        strncpy(this->szIP, pMsg->szIP,sizeof(this->szIP)-1);
        memcpy(this->szData, pMsg->szData, pMsg->len);
    }
    bool     bTCP;
    bool     bBroadcast;
    MYSOCK   sock;
    char     szIP[100];
    char     szData[PACKET_BUFLEN];
    int      len;
};

class CThreadSend: public CMyThread
{
public:
    CThreadSend();
    ~CThreadSend();

    virtual void Run();
    //send message
    void SendMsg(std::shared_ptr<t_SendMsg> pMsg);

protected:

    void RunMessage(t_SendMsg* pMsg);

    //send to one net
    bool Send(MYSOCK sock, const char* buf, int len);
    bool SendTo(MYSOCK sock,const char* ip, const char* buf, int len);

    //broadcast
    bool SendBroadcast(const char* buf, int len);
    bool SendMulticast(const char* ip, const char* buf, int len);

private:
  
    std::queue<std::shared_ptr<t_SendMsg>>   m_queueMsg;
    CMySocket                           m_sockBroadcast;
    CMySocket                           m_sockUDP;
};

#endif

