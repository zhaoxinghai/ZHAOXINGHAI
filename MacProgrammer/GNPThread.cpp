
#include "GNPThread.h"

CGNPThread::CGNPThread()
{
    D1State = STATE_NOT_RUN;
    D1FlashError = 0;
}

CGNPThread::~CGNPThread()
{

}

void CGNPThread::Init(MainWindow* p)
{
    m_pMain = p;
}

void CGNPThread::Run(int type,std::string strMac)
{
    //create
    if(type == D_D1_ETCS)
    {
        RunETCS(strMac);
    }
    else if(type == D_D1_INC)
    {
        RunINC(strMac);
    }
    D1State = STATE_FINISH;
}

void CGNPThread::RunETCS(std::string strMac)
{
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock != INVALID_SOCKET)
    {
        struct sockaddr_in svraddr;
        svraddr.sin_family = AF_INET;
        svraddr.sin_addr.s_addr = inet_addr("192.168.1.127");
        svraddr.sin_port = htons(23);
        int ret = ::connect(sock, (struct sockaddr *)&svraddr, sizeof(svraddr));
        if(ret != SOCKET_ERROR)
        {
            ETCSTelnet(sock,strMac);
        }
        else
        {
            D1FlashError = ETCS_SOCKET_CON;
        }
    }
    else
    {
        D1FlashError = ETCS_SOCKET_CREATE;
    }
    closesocket(sock);
}

void CGNPThread::RunINC(std::string strMac)
{

}

void CGNPThread::ETCSTelnet(SOCKET sock,std::string strMac)
{
    //telnet client
    std::string strBuf;
    bool bSendCmd = false;

    while (true)
    {
        char data = 0;
        if (recv(sock, &data, 1, 0) != 1)
        {
            D1FlashError = ETCS_SOCKET_RECV;
            break;
        }
        strBuf.push_back(data);

        //first login
        size_t pos = strBuf.find("Logon:");
        if(pos != std::string::npos)
        {
            std::string str = "etcs\r\n";
            if(!ETCSWrite(sock,str.c_str(), str.length()))
            {
                D1FlashError = ETCS_SOCKET_SEND;
                break;
            }
            strBuf.clear();
            continue;
        }
        if (data != '>')
        {
            continue;
        }
        //first send command
        if (!bSendCmd)
        {
            bSendCmd = true;
            std::string strSend = std::string("setmac ")
                    + strMac + std::string("\r\n");
            if(!ETCSWrite(sock,strSend.c_str(), strSend.length()))
            {
                D1FlashError = ETCS_SOCKET_SEND;
                break;
            }
            strBuf.clear();
            continue;
        }

        //get the result
        pos = strBuf.rfind('\n');
        if(pos != std::string::npos)
        {
            strBuf = strBuf.substr(0,pos);
        }
        //reselt
        pos = strBuf.find("OK");
        if(pos != std::string::npos)
        {
            D1FlashError = ETCS_OK;
        }
        else
        {
            D1FlashError = ETCS_RET_ERROR;
        }
        break;
    }
}

bool CGNPThread::ETCSWrite(SOCKET sock,const char *strWrite,int nWrite)
{
    int write = 0;
    while ( write < nWrite )
    {
        if (sock == INVALID_SOCKET)
        {
            return false;
        }
        int left = nWrite - write;
        int iWrite = send(sock, strWrite + write, left, 0);

        if (iWrite <= 0 )
        {
            return false;
        }
        write += iWrite;
    }
    return write == nWrite;
}
