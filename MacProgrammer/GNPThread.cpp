
#include "GNPThread.h"
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <istream>

CGNPThread::CGNPThread()
{
    D1State = STATE_NOT_RUN;
    D1FlashError = 0;
    D1DeviceIndex = 0;
}

CGNPThread::~CGNPThread()
{

}

void CGNPThread::Init(MainWindow* p)
{
    m_pMain = p;
}

void CGNPThread::Run(std::string strMac)
{
    D1FlashError = ETCS_SOCKET_CON;

    if(D1DeviceIndex==0)
    {
        RunETCS(strMac);
    }
    else if(D1DeviceIndex == 1)
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
    D1FlashError = SSH_READFILE_ERROR;

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    HANDLE h = CreateFile(L"Output.txt",
            FILE_APPEND_DATA,
            FILE_SHARE_WRITE | FILE_SHARE_READ,
            &sa,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    BOOL ret = FALSE;
    DWORD flags = CREATE_NO_WINDOW;

    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdInput = NULL;
    si.hStdError = h;
    si.hStdOutput = h;

    std::string sParam = "/home/admin/applications/emeabox/macconfig " + strMac + "\nifconfig";
    std::wstring wsParam = utf8tounicode(sParam.c_str());

    std::wstring sCmd =  L"plink.exe -batch -ssh root@192.168.1.200 -pw HON*emea&1357 ";
    sCmd += wsParam;

    ret = CreateProcess(NULL, (LPWSTR)sCmd.c_str(), NULL, NULL, TRUE, flags, NULL, NULL, &si, &pi);
    WaitForSingleObject( pi.hProcess, INFINITE );

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    CloseHandle(h);
    CheckFileString(strMac);
}

std::string CGNPThread::GetModulePath()
{
    char strModule[256] = { 0 };
    GetModuleFileNameA(NULL, strModule, 256);
    std::string str(strModule);
    size_t pos = str.rfind('\\');
    std::string strTmp = str.substr(0, pos+1);
    return strTmp;
}

std::wstring CGNPThread::GetModulePathW()
{
    wchar_t strModule[256] = { 0 };
    GetModuleFileNameW(NULL, strModule, 256);
    std::wstring str(strModule);
    size_t pos = str.rfind(L'\\');
    std::wstring strTmp = str.substr(0, pos+1);
    return strTmp;
}

void CGNPThread::CheckFileString(std::string strMac)
{
    HANDLE hFile;
    hFile = CreateFile(L"Output.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    char ch[100];
    DWORD dwReads;
    ReadFile(hFile, ch, 100, &dwReads, NULL);
    ch[dwReads] = 0;
    CloseHandle(hFile);
    if(dwReads==100)
    {
        std::string str(ch);
        std::string sMac = str.substr(38,17);
        if(sMac == strMac)
        {
            D1FlashError = ETCS_OK;
        }
        else
        {
            D1FlashError = MAC_ADDRESS_ERROR;
        }
    }
}

std::string CGNPThread::StrFormat(const char *szFormat, ...)
{
    char temp[800] = { 0 };
    va_list args;
    va_start(args, szFormat);
    vsnprintf(temp, 800, szFormat, args);
    va_end(args);
    std::string str(temp);
    return str;
}

std::wstring CGNPThread::utf8tounicode(const char* src)
{
    wchar_t* m_wchar;
    int len = MultiByteToWideChar(CP_UTF8, 0, src, strlen(src), NULL, 0);
    m_wchar = new wchar_t[len + 1];        //checked
    MultiByteToWideChar(CP_UTF8, 0, src, strlen(src), m_wchar, len);
    m_wchar[len] = L'\0';
    std::wstring wstr = m_wchar;
    delete[]m_wchar;
    return wstr;
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
