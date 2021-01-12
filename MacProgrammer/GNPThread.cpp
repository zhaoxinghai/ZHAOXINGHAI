
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

void CGNPThread::Run(int type)
{
    //create
    if(type == D_D1_ETCS)
    {
        RunETCS();
    }
    else if(type == D_D1_INC)
    {
        RunINC();
    }
    //nError = m_nError;
    //eState = eRunState::STATE_FINISH;
}

void CGNPThread::RunETCS()
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
            ETCSTelnet(sock);
        }
        else
        {
            //m_pMain->D1 = ETCS_SOCKET_CON;
        }
    }
    else
    {
        //m_nError = ETCS_SOCKET_CREATE;
    }
    closesocket(sock);
}

void CGNPThread::RunINC()
{

}

void CGNPThread::ETCSTelnet(SOCKET sock)
{
    /*
    //telnet client
    std::string strBuf;
    bool bSendCmd = false;

    while (true)
    {
        char data = 0;
        if (recv(sock, &data, 1, 0) != 1)
        {
            m_nError = ETCS_SOCKET_RECV;
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
                m_nError = ETCS_SOCKET_SEND;
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
                    + D1MacList.at(D1MacIndex).toStdString() + std::string("\r\n");
            if(!ETCSWrite(sock,strSend.c_str(), strSend.length()))
            {
                m_nError = ETCS_SOCKET_SEND;
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
            m_nError = ETCS_OK;
        }
        else
        {
            m_nError = ETCS_RET_ERROR;
        }
        break;
    }
    */
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
