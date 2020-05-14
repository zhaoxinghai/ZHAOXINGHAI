
#include "mysocket.h"
#include "mylog.h"

CMySocket::CMySocket()
{
    m_bAutoClose = true;
    m_nPort = 0;

#ifdef _MSC_VER
    m_sock = INVALID_SOCKET;
#else
    m_sock = -1;
#endif

    m_type = SOCK_STREAM;
}

CMySocket::CMySocket(MYSOCK sock, int type, int port)
{
	m_bAutoClose = true;
    m_sock = sock;
    m_type = type;
    m_nPort = port;
}

CMySocket::~CMySocket()
{
    if (m_bAutoClose)
    {
        Close();
    }
}

void CMySocket::CloseSocket(MYSOCK sock)
{
#ifdef _MSC_VER

    if (sock != INVALID_SOCKET)
    {
        closesocket(sock);
    }

#else
    close(sock);
#endif
}

void CMySocket::SetNotAutoClose()
{
    m_bAutoClose = false;
}

bool CMySocket::Create(int type)
{
    m_type = type;
    int protocol = 0;

#ifdef _MSC_VER
    if (m_type == SOCK_STREAM)
    {
        protocol = IPPROTO_TCP;
    }
    else if (m_type == SOCK_DGRAM)
    {
        protocol = IPPROTO_UDP;
    }
#endif

    m_sock = socket(PF_INET, m_type, protocol);

#ifdef _MSC_VER
    if (m_sock == INVALID_SOCKET)
    {
        return false;
    }
#else
    if (m_sock < 0)
    {
        return false;
    }
#endif

    return true;
}

bool CMySocket::SetReuseAddr()
{
    int Flag = 1;
    if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR,
        (char *)(&Flag), sizeof(Flag)) == -1)
    {
        return false;
    }
    return true;
}

bool CMySocket::SetBroadcast()
{
#ifdef _MSC_VER
    bool optval = true;
#else
    int optval = 1;
#endif
    if (setsockopt(m_sock, SOL_SOCKET,
        SO_BROADCAST, (char *)&optval, sizeof(optval)) < 0)
    {
        return false;
    }
    return true;
}

bool CMySocket::SetBuffer(int nSendBuf,int nRecvBuf)
{
#ifdef _MSC_VER
    if(nSendBuf>=0 || nRecvBuf>=0)
        return true;
#else
	socklen_t optlen = sizeof(nSendBuf);

    int err = setsockopt(m_sock,SOL_SOCKET,SO_SNDBUF,(char*)&nSendBuf,optlen);
	if(err<0)
	{
		return false;
	}

    err = setsockopt(m_sock,SOL_SOCKET,SO_RCVBUF,(char*)&nRecvBuf,optlen);
	if(err<0)
	{
		return false;
	}
#endif
	return true;
}

bool CMySocket::SetTimeout(int nSendMillis, int nRecvMillis)
{
#ifdef _MSC_VER
    setsockopt(m_sock, SOL_SOCKET, SO_SNDTIMEO,(char *)&nSendMillis,sizeof(int));
    setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO,(char *)&nRecvMillis,sizeof(int));
#else
    long ssec = 0;
    long rsec = 0;
    if (nSendMillis > 1000)
    {
        ssec = nSendMillis / 1000;
        nSendMillis %= 1000;
    }
    if (nRecvMillis > 1000)
    {
        rsec = nRecvMillis / 1000;
        nRecvMillis %= 1000;
    }
    struct timeval stimeout = { ssec,nSendMillis };
    struct timeval rtimeout = { rsec,nRecvMillis };
    setsockopt(m_sock,SOL_SOCKET,SO_SNDTIMEO,(char *)&stimeout,sizeof(struct timeval));
    setsockopt(m_sock,SOL_SOCKET,SO_RCVTIMEO,(char *)&rtimeout,sizeof(struct timeval));
#endif
    return true;
}

bool CMySocket::SetBlock(bool bBlock)
{
    unsigned long mode;

#ifdef _MSC_VER

    mode = bBlock ? 0 : 1;
    ioctlsocket(m_sock, FIONBIO, &mode);
 
#else
    mode = fcntl(m_sock, F_GETFL, 0);
    if (bBlock)
    {
        if (fcntl(m_sock, F_SETFL, mode & ~O_NONBLOCK) < 0)
            return false;
    }
    else
    {
        if (fcntl(m_sock, F_SETFL, mode | O_NONBLOCK) < 0)
            return false;
    }
      
#endif
    return true;
}

bool CMySocket::Connect(const char* ip, unsigned short port,int ntimeoutSec)
{
    struct sockaddr_in svraddr;
    svraddr.sin_family = AF_INET;
    svraddr.sin_addr.s_addr = inet_addr(ip);
    svraddr.sin_port = htons(port);

    SetBlock(false);
  
#ifdef _MSC_VER
    int addlen = sizeof(svraddr);
#else
    socklen_t addlen = sizeof(svraddr);
#endif

    int ret = connect(m_sock, (struct sockaddr *)&svraddr, addlen);

#ifdef _MSC_VER
    if (ret != SOCKET_ERROR)
    {
        //connected;
        SetBlock(true);
        return true;
    }
#else
    if (ret == 0)
    {
        SetBlock(true);
        return true;
    }
    else if (errno != EINPROGRESS)
    {
        return false;
    }
#endif

    fd_set fdr;
    fd_set fdw;
    FD_ZERO(&fdr);
    FD_ZERO(&fdw);
    FD_SET(m_sock, &fdr);
    FD_SET(m_sock, &fdw);

    timeval mytm = { ntimeoutSec,0};

    int result = select((int)m_sock + 1, &fdr, &fdw, NULL, &mytm);
    if (result > 0 && FD_ISSET(m_sock, &fdw))
    {

        int error = -1;
#ifdef _MSC_VER
        int optlen = sizeof(error);
        int sockoptret = getsockopt(m_sock, SOL_SOCKET, SO_ERROR, (char*)&error, &optlen);
#else
        socklen_t optlen = sizeof(error);
        int sockoptret = getsockopt(m_sock, SOL_SOCKET, SO_ERROR, &error, &optlen);
#endif

        if (sockoptret >= 0 && error==0)
        {
            SetBlock(true);
            return true;
        }
    }
    return false;
}

bool CMySocket::ConnectSync(const char * ip, unsigned short port)
{
    struct sockaddr_in svraddr;
    svraddr.sin_family = AF_INET;
    svraddr.sin_addr.s_addr = inet_addr(ip);
    svraddr.sin_port = htons(port);

#ifdef _MSC_VER
    int addlen = sizeof(svraddr);
#else
    socklen_t addlen = sizeof(svraddr);
#endif

    int ret = connect(m_sock, (struct sockaddr *)&svraddr, addlen);

#ifdef _MSC_VER
    if (ret != SOCKET_ERROR)
    {
        return true;
    }
#else
    if (ret == 0)
    {
        return true;
    }
#endif

    return false;
}

bool CMySocket::Bind(const char* ip, unsigned short port)
{
    SetReuseAddr();

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    //bind
#ifdef _MSC_VER
    int addrlen = sizeof(addr);
#else
    socklen_t addrlen = sizeof(addr);
#endif

    int ret = bind(m_sock, (const struct sockaddr *) &addr, addrlen);

#ifdef _MSC_VER
    if (ret == SOCKET_ERROR)
    {
        return false;
    }
#else
    if ( ret == -1)
    {
    	perror("bind");
        return false;
    }
#endif

    return true;
}

bool CMySocket::Bind(unsigned short port)
{
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    //bind
#ifdef _MSC_VER
    int addrlen = sizeof(addr);
#else
    socklen_t addrlen = sizeof(addr);
#endif

    int ret = bind(m_sock, (const struct sockaddr *) &addr, addrlen);

#ifdef _MSC_VER
    if (ret == SOCKET_ERROR)
    {
        return false;
    }
#else
    if ( ret == -1)
    {
    	perror("bind");
        return false;
    }
#endif

    return true;
}

bool CMySocket::Listen(int backlog)
{
    int ret = ::listen(m_sock, backlog);

#ifdef _MSC_VER
    if (ret == SOCKET_ERROR)
    {
        return false;
    }
#else
    if (ret == -1)
    {
        return false;
    }
#endif

    return true;
}

bool CMySocket::SetUDPAddress(const char* ip, unsigned short port)
{
#ifdef _MSC_VER
    bool opt = 1;
    int error = setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST,
        reinterpret_cast<char FAR *>(&opt), sizeof(opt));

    if (error != 0)
    {
        return false;
    }
#endif

    memset(&m_MCastAddr, 0, sizeof(m_MCastAddr));
    m_MCastAddr.sin_family = AF_INET;
    m_MCastAddr.sin_addr.s_addr = inet_addr(ip);
    m_MCastAddr.sin_port = htons(port);
    m_nPort = port;
    return true;
}

sockaddr_in& CMySocket::GetUDPAddress()
{
    return m_MCastAddr;
}

bool CMySocket::Read(unsigned char *strRead,int nRead)
{
    int read = 0;
    if (m_type == SOCK_STREAM)
    {
        while(read < nRead)
        {
            if(IsClose())
                break;

            int iRead = recv(m_sock, (char*)strRead, nRead, 0);
            if(iRead <= 0)
            {
                return false;
            }
            read += iRead;
        }
    }
    else
    {
        unsigned int addrlen = sizeof(m_MCastAddr);

#ifdef _MSC_VER
        read = recvfrom(m_sock, (char*)strRead, nRead,
            0, (struct sockaddr*)&m_MCastAddr, (int*)&addrlen);
#else

        read = recvfrom(m_sock, (void*)strRead, nRead,
            0, (struct sockaddr*)&m_MCastAddr, (unsigned int*)&addrlen);

#endif

    }
    return read == nRead;
}

bool CMySocket::Write(const char *strWrite,int nWrite)
{
    int write = 0;
    if (m_type == SOCK_STREAM)
    {
        while ( write < nWrite )
        {
            if(IsClose())
                break;

            int left = nWrite - write;

#ifdef _MSC_VER
            int iWrite = send(m_sock, strWrite + write, left, 0);
#else
            int iWrite = send(m_sock, strWrite + write, left, MSG_NOSIGNAL);
#endif

            if (iWrite <= 0 )
            {
                return false;
            }
            write += iWrite;
        }
    }
    else
    {
        write = sendto(m_sock, strWrite, nWrite, 0,
                       (sockaddr*)&m_MCastAddr, sizeof(m_MCastAddr));
    }
    return write == nWrite;
}

MYSOCK CMySocket::GetSocket()
{
    return m_sock;
}

void CMySocket::Create(MYSOCK sock, int type, int port)
{
    Close();

    m_sock = sock;
    m_type = type;
    m_nPort = port;
}

int CMySocket::GetLastError()
{
#ifdef _MSC_VER
    return WSAGetLastError();
#else
    perror("GetLastError:");
    return errno;
#endif  
}

int  CMySocket::Close()
{
#ifdef _MSC_VER
    int ret = 0;
    if (m_sock != INVALID_SOCKET)
    {
        ret = closesocket(m_sock);
        m_sock = INVALID_SOCKET;
    }
    return ret;
#else
    int ret = close(m_sock);
    m_sock = -1;
    return ret;
#endif
}

bool CMySocket::IsClose()
{
#ifdef _MSC_VER
    if (m_sock == INVALID_SOCKET)
        return true;
#else
    if(m_sock == -1)
        return true;
#endif
    return false;
}

int CMySocket::Init()
{
#ifdef _MSC_VER
    WSADATA wsaData;
    unsigned short version = MAKEWORD(2, 0);
    int ret = WSAStartup(version, &wsaData);
    if (ret)
    {
        return -1;
    }
#endif

    return 0;
}
int CMySocket::Exit()
{
#ifdef _MSC_VER
    return (WSACleanup());
#else
    return 0;
#endif
}

bool CMySocket::JoinGroup(const char* ip)
{
    char Loop = 1;
    if (setsockopt(m_sock, IPPROTO_IP, IP_MULTICAST_LOOP,
        &Loop, sizeof(Loop)) == -1)
    {
        return false;
    }

    struct ip_mreq mreq;
    memset(&mreq, 0, sizeof(mreq));

    //setting group address
    mreq.imr_multiaddr.s_addr = inet_addr(ip);

    //setting source address
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
   
    if (setsockopt(m_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)) < 0)
    {
        return false;
    }
    return true;
}

void CMySocket::LeaveGroup(const char* ip)
{
#ifdef _MSC_VER
    if (m_sock == INVALID_SOCKET)
        return;
#else
    if (m_sock == -1)
        return;
#endif

    struct ip_mreq Mreq;
    memset(&Mreq, 0, sizeof(Mreq));

    Mreq.imr_multiaddr.s_addr = inet_addr(ip);
    Mreq.imr_interface.s_addr = htonl(INADDR_ANY);
 
    if (setsockopt(m_sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)(&Mreq), sizeof(Mreq))<0)
    {
        printf("LeaveGroup Error:%d",errno);
    }
}

bool CMySocket::Accept(MYSOCK& s, char* fromip,int ipsize)
{
    struct sockaddr_in cliaddr;

#ifdef _MSC_VER
    int addrlen = sizeof(cliaddr);
#else
    socklen_t addrlen = sizeof(cliaddr);
#endif

    MYSOCK sock = accept(m_sock, (struct sockaddr*)&cliaddr, &addrlen);

#ifdef _MSC_VER
    if (sock == INVALID_SOCKET)
    {
        return false;
    }
#endif

    //get ip address
    if (fromip != NULL)
    {
        snprintf(fromip, ipsize,"%s", inet_ntoa(cliaddr.sin_addr));
    }
    s = sock;
    return true;
}

void CMySocket::SetTTL(int nTTL /*= 128*/)
{
#ifdef _MSC_VER
    int addrlen = sizeof(nTTL);
#else
    socklen_t addrlen = sizeof(nTTL);
#endif
    setsockopt(m_sock, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&nTTL, addrlen);
}
