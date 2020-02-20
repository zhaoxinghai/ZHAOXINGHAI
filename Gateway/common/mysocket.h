
#ifndef SOCKET_H_
#define SOCKET_H_

#ifdef _MSC_VER

#include <Windows.h>
#pragma  comment (lib,"wsock32.lib")
#define MYSOCK SOCKET

#else

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>

#define MYSOCK int

#endif

#define PACKET_BUFLEN  4096 
#define PACKET_TTL     128

class CMySocket
{
public:
    CMySocket();
    CMySocket(MYSOCK sock, int type, int port);
    virtual ~CMySocket();

    //SOCK_STREAM|SOCK_DGRAM
    bool Create(int type = SOCK_STREAM);
    void Create(MYSOCK sock,int type,int port);

    //this is a broadcast socket
    bool SetBroadcast();

    //not auto close
    void SetNotAutoClose();

    //set the socket buffer
    bool SetBuffer(int nSendBuf,int nRecvBuf);

    //set timeout
    bool SetTimeout(int nSendMillis,int nRecvMillis);

    //set block mode
    bool SetBlock(bool bBlock);

    //connect(block)
    bool Connect(const char* ip, unsigned short port,int ntimeoutSec);
    bool ConnectSync(const char* ip, unsigned short port);

    //bind
    bool Bind(const char* ip, unsigned short port);
    bool Bind(unsigned short port);

    //listen
    bool Listen(int backlog = 5);

    //accept
    bool Accept(MYSOCK& s, char* fromip, int ipsize);
    
    //send data
    bool Write(const char *strWrite, int len);

    //receive data
    bool Read(unsigned char *strRead,int nRead);

    //join udp group
    bool JoinGroup(const char* ip);
    void LeaveGroup(const char* ip);

    //close socket
    int  Close();

    //set dest address
    bool SetUDPAddress(const char* ip, unsigned short port);
    sockaddr_in& GetUDPAddress();

    //for windows
    static int Init();
    static int Exit();

    static void CloseSocket(MYSOCK sock);

    //Get socket
    MYSOCK GetSocket();
    bool IsClose();

    //last error
    int GetLastError();

    void SetTTL(int nTTL = PACKET_TTL);

    bool SetReuseAddr();

private:
    bool            m_bAutoClose;
    int             m_type;
    MYSOCK          m_sock;
    sockaddr_in     m_MCastAddr;
    int             m_nPort;
};

#endif
