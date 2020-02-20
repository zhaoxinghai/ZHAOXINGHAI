
//this file can only include C++ standard header
//this file for all other project

#include "common.h"
#include "fcntl.h"
#include <iterator>
#include <cctype>
#include <algorithm>
#include <memory>

#ifdef _MSC_VER
#include <windows.h>  
#include <process.h>
#include <io.h> 

#pragma warning(disable:4996)

#else
#include <sys/prctl.h>
#include <sched.h>
#include <unistd.h>   
#include <dirent.h>  
#include <sys/stat.h>  
#include<sys/time.h>
#include <math.h>
#include<unistd.h>
#endif

void CCommon::Trace(std::string str)
{
    std::string str2 = str + "\n";

#ifdef _MSC_VER
    OutputDebugStringA(str2.c_str());
#else
    std::cout<<str2;
#endif
}

void CCommon::Trace(const char *szFormat, ...)
{
    char temp[800] = { 0 };
    va_list args;
    va_start(args, szFormat);
    vsnprintf(temp, 800, szFormat, args);
    va_end(args);
    std::string str(temp);
    Trace(str);
}

std::string CCommon::GetModulePath()
{
#ifdef _MSC_VER
    char strModule[256] = { 0 };
    GetModuleFileNameA(NULL, strModule, 256);
    std::string str(strModule);
    size_t pos = str.rfind('\\');
    std::string strTmp = str.substr(0, pos+1);
    return strTmp;
#else
    int len = 256;
    char path[len];
    int cnt = readlink("/proc/self/exe", path, len);

    int i;
    for (i = cnt; i >= 0; --i)
    {
        if (path[i] == '/')
        {
            path[i + 1] = '\0';
            break;
        }
    }
    return std::string(path);
#endif
}

std::string CCommon::GetHostIP(std::string strDefault)
{
#ifdef _MSC_VER
    char local[255] = { 0 };
    gethostname(local, sizeof(local));
    hostent* ph = gethostbyname(local);
    if (ph == NULL)
        return strDefault;

    in_addr addr;
    memcpy(&addr, ph->h_addr_list[0], sizeof(in_addr));

    std::string localIP;
    localIP.assign(inet_ntoa(addr));
    return localIP;

#else

    std::string strIP = strDefault;
    int sock;
    struct sockaddr_in mysin;
    struct ifreq ifr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        return strDefault;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, "eth0", sizeof(ifr.ifr_name) - 1);

    if (ioctl(sock, SIOCGIFADDR, &ifr) < 0)
    {
        close(sock);
        return strDefault;
    }

    memcpy(&mysin, &ifr.ifr_addr, sizeof(mysin));
    strIP = inet_ntoa(mysin.sin_addr);

    close(sock);
    return strIP;

#endif
}

void CCommon::Split(std::vector<std::string> &result, std::string str, std::string token)
{
    result.clear();
    if (str.length() == 0)
    {
        return;
    }
    std::string strFind = str;
    size_t curPos = strFind.find(token);
    while (curPos != std::string::npos)
    {
        std::string temp = strFind.substr(0, curPos);
        if (temp.length() > 0)
        {
            result.push_back(temp);
        }
        strFind = strFind.substr(curPos + 1, strFind.length() - curPos - 1);
        curPos = strFind.find(token);
    }
    if (strFind.length() > 0)
    {
        result.push_back(strFind);
    }
}

bool CCommon::IsAllNumber(std::string str)
{
    for (unsigned int i = 0; i < str.size(); i++)
    {
        char c = str[i];
        if (c < '0' || c > '9')
        {
            return false;
        }
    }
    return true;
}

std::string CCommon::StrFormat(const char *szFormat, ...)
{
    char temp[800] = { 0 };
    va_list args;
    va_start(args, szFormat);
    vsnprintf(temp, 800, szFormat, args);
    va_end(args);
    std::string str(temp);
    return str;
}

void CCommon::SleepMinisecond(int minisecond)
{
#ifdef _MSC_VER
    Sleep(minisecond);
#else
    usleep(minisecond * 1000);
#endif
}

void CCommon::WaitMinisecond(int minisecond, bool *bBreak)
{
    int minis = 0;
    while (!(*bBreak))
    {
        CCommon::SleepMinisecond(5);
        minis += 5;

        if (minis >= minisecond)
        {
            break;
        }
    }
}

bool CCommon::IsFileExist(std::string strPath)
{
    std::fstream file;
    file.open(strPath.c_str(), std::ios::in);
    if (!file.is_open())
    {
        return false;
    }
    else
    {
        file.close();
        return true;
    }
}

std::string CCommon::GetFileExt(std::string strPath)
{
    size_t pos = strPath.rfind(".");
    if (pos == std::string::npos)
    {
        return "";
    }
    std::string strExt = strPath.substr(pos, strPath.length() - pos);
    return strExt;
}

std::string CCommon::GetFileName(std::string strPath)
{
    size_t pos1 = strPath.rfind("\\");
    size_t pos2 = strPath.rfind("/");

    if (pos1 == std::string::npos && pos2 == std::string::npos)
    {
        return "";
    }
    std::string strExt;
    if (pos1 != std::string::npos)
    {
        strExt = strPath.substr(pos1 + 1, strPath.length() - pos1);
    }
    else if (pos2 != std::string::npos)
    {
        strExt = strPath.substr(pos2 + 1, strPath.length() - pos2);
    }
    return strExt;
}

void CCommon::CreateFolder(std::string strPath)
{
#ifdef _MSC_VER
    ::CreateDirectoryA(strPath.c_str(),NULL);
#else
    struct stat st;
    if (stat(strPath.c_str(), &st) == -1)
    {
        mkdir(strPath.c_str(), 777);
    }
#endif
}

std::string CCommon::GetString(std::string strToken, std::string &strSrc)
{
    std::string str = "";
    size_t Pos = strSrc.find(strToken);
    if (Pos == std::string::npos)
    {
        return str;
    }
    str = strSrc.substr(0, Pos);
    strSrc = strSrc.substr(Pos + strToken.length(), strSrc.length() - Pos);

    CCommon::TrimeLeft(str," ");
    CCommon::TrimeLeft(strSrc," ");
    return str;
}

void CCommon::TrimeRight(std::string &strSrc, std::string strToken)
{
    while(true)
    {
        size_t Pos = strSrc.rfind(strToken);
        if (Pos == std::string::npos)
            break;

        if (Pos == strSrc.length()-1)
        {
            strSrc = strSrc.substr(0, strSrc.length() - 1);
        }
        else
        {
            break;
        }
    }
}

void CCommon::TrimeLeft(std::string &strSrc, std::string strToken)
{
    while (true)
    {
        size_t Pos = strSrc.find(strToken);
        if (Pos == std::string::npos)
            break;

        if (Pos == 0)
        {
            strSrc = strSrc.substr(1, strSrc.length() - 1);
        }
        else
        {
            break;
        }
    }
}

std::string CCommon::strToLower(const std::string &str)
{
    std::string strTmp = str;
    transform(strTmp.begin(),strTmp.end(),strTmp.begin(),(int(*)(int))tolower);
    return strTmp;
}

bool CCommon::IsIPAddress(std::string strSrc)
{
    std::vector<std::string> vNo;
    CCommon::Split(vNo, strSrc, ".");
    if (vNo.size() != 4)
    {
        return false;
    }

    for (unsigned int i = 0; i < vNo.size(); i++)
    {
        if (vNo[i].length() > 3)
        {
            return false;
        }

        if (!CCommon::IsAllNumber(vNo[i]))
        {
            return false;
        }

        int temp = atoi(vNo[i].c_str());
        if (temp < 0 || temp>255)
        {
            return false;
        }
    }
    return true;
}

FILE* CCommon::OpenGPIO(int pin,bool bRead)
{
#ifdef _MSC_VER
    return NULL;
#endif

    char path[64] = { 0 };
    int ret = snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
    if (ret > 0)
    {
        FILE *fd = fopen(path, bRead ? "rb" : "wb");
        return fd;
    }
    else
    {
        return NULL;
    }
}

FILE* CCommon::OpenGPIO(std::string strPath, bool bRead)
{
#ifdef _MSC_VER
    return NULL;
#endif
    FILE *fd = fopen(strPath.c_str(), bRead ? "rb" : "r+");
    return fd;
}

int CCommon::ReadGPIO(FILE* file)
{
	if(file == NULL)
		return -1;

    char buf[10] = { 0 };
    fseek(file, 0, SEEK_SET);
    size_t iRead = fread(buf, sizeof(char),10,file);
    if (iRead <= 0)
    {
        perror("ReadGPIO Error");
        return -1;
    }
    return atoi(buf);
}

std::string CCommon::ReadGPIO2(FILE* file)
{
    std::string str = "";
    if (file == NULL)
        return str;

    char buf[100];
    memset(buf, 0, 100);
    fseek(file, 0, SEEK_SET);
    size_t iRead = fread(buf, sizeof(char), 100, file);
    if (iRead <= 0)
    {
        perror("ReadGPIO2 Error\n");
        return str;
    }
    str = buf;
    return str;
}

void CCommon::WriteGPIO(int /*pin*/, FILE* file, int value)
{
	if(file == NULL)
		return;

    fseek(file, 0, SEEK_SET);
    static const char values_str[] = "01";
    char c = values_str[value == 0 ? 0 : 1];
    fwrite(&c,sizeof(char),1,file);
    fflush(file);
}

void CCommon::WriteGPIO2(FILE* file, std::string value)
{
    if(file == NULL)
    {
        return;
    }
    fseek(file, 0, SEEK_SET);
    fwrite(value.c_str(),value.length(),1,file);
    fflush(file);
}

void CCommon::SetThreadName(std::string strName)
{
#ifdef _MSC_VER
    if (!::IsDebuggerPresent())
        return;

    const DWORD kVCThreadNameException = 0x406D1388;
    typedef struct tagTHREADNAME_INFO {
        DWORD dwType;  // Must be 0x1000.  
        LPCSTR szName;  // Pointer to name (in user addr space).  
        DWORD dwThreadID;  // Thread ID (-1=caller thread).  
        DWORD dwFlags;  // Reserved for future use, must be zero.  
    } THREADNAME_INFO;

    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = strName.c_str();
    info.dwThreadID = GetCurrentThreadId();
    info.dwFlags = 0;

    try
    {
        RaiseException(kVCThreadNameException, 0, sizeof(info) / sizeof(DWORD), 
            reinterpret_cast<DWORD_PTR*>(&info));
    }
    catch (const std::exception& /*ex*/)
    {
        abort();
    }
    catch (...)
    {
    }

#else
	prctl(PR_SET_NAME,strName.c_str());
#endif
}

#ifndef _MSC_VER

int CCommon::get_thread_policy(pthread_attr_t *attr)
{
  int policy;
  pthread_attr_getschedpolicy(attr,&policy);

  switch(policy)
  {
  case SCHED_FIFO:
    printf("policy= SCHED_FIFO\n");
    break;
  case SCHED_RR:
    printf("policy= SCHED_RR");
    break;
  case SCHED_OTHER:
    printf("policy=SCHED_OTHER\n");
    break;
  default:
    printf("policy=UNKNOWN\n");
    break;
  }
  return policy;
}

int CCommon::get_thread_priority(pthread_attr_t *attr)
{
  struct sched_param param;
  pthread_attr_getschedparam(attr,&param);

  printf("priority=%d",param.__sched_priority);
  return param.__sched_priority;
}

void CCommon::set_thread_policy(pthread_attr_t *attr,int policy)
{
  pthread_attr_setschedpolicy(attr,policy);
}
#endif

unsigned int CCommon::dfsFolder(std::string folderPath, std::vector<t_File> &vFile)
{

#ifdef WIN32  
    unsigned int length = 0;
    HANDLE hFind;
    WIN32_FIND_DATAA wfd;
    std::string strFind = folderPath + "*";
    hFind = FindFirstFileA(strFind.c_str(), &wfd);
    if (INVALID_HANDLE_VALUE == hFind)
    {
        return 0;
    }
    do
    {
        if ('.' == wfd.cFileName[0])
        {
            continue;
        }
        if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
        }
        else
        {
            t_File t;
            t.mTime = 0;
            t.uLength = wfd.nFileSizeLow;
            t.strName = wfd.cFileName;
            vFile.push_back(t);

            length += wfd.nFileSizeLow;
        }
    } while (FindNextFileA(hFind, &wfd));
    FindClose(hFind);
    return length;

#else
    unsigned int length = 0;
    DIR *dp = opendir(folderPath.c_str());
    if (dp == NULL)
    {
        return 0;
    }

    struct dirent *entry = (struct dirent *)malloc(sizeof(struct dirent));
    struct dirent *result = (struct dirent *)malloc(sizeof(struct dirent));
    chdir(folderPath.c_str());
    while (true)
    {
        if (readdir_r(dp, entry, &result) != 0)
        {
            length = 0;
            break;
        }
        if (result == NULL)
        {
            break;
        }
        if (result->d_name[0] == '.')
        {
            continue;
        }
        struct stat statbuf;
        stat(result->d_name, &statbuf);

        t_File t;
        t.uLength = statbuf.st_size;
        t.strName = result->d_name;
        t.mTime = statbuf.st_mtime;
        vFile.push_back(t);
        length += t.uLength;
    }
    chdir("..");
    closedir(dp);
    free(entry);
    free(result);

    return length;
#endif
}

std::string CCommon::wchar2char(const wchar_t* wsrc)
{
#ifdef WIN32
    char *m_char;
    int len = WideCharToMultiByte(CP_UTF8, 0, wsrc, wcslen(wsrc), NULL, 0, NULL, NULL);
    m_char = new char[len + 1];            //checked
    WideCharToMultiByte(CP_UTF8, 0, wsrc, wcslen(wsrc), m_char, len, NULL, NULL);
    m_char[len] = '\0';
    std::string out = m_char;
    delete []m_char;
    return out;
#else
    std::wstring wstr = wsrc;

    size_t len = wstr.size() * 4;
    setlocale(LC_CTYPE, "");
    char *buf = new char[len];             //checked

    wcstombs(buf, wstr.c_str(), len);
    std::string out(buf);
    delete[] buf;
    return out;
#endif
}

std::wstring CCommon::char2wchar(const char* src)
{
#ifdef WIN32
    wchar_t *m_wchar;
    int len = MultiByteToWideChar(CP_UTF8, 0, src, strlen(src), NULL, 0);
    m_wchar = new wchar_t[len + 1];        //checked
    MultiByteToWideChar(CP_UTF8, 0, src, strlen(src), m_wchar, len);
    m_wchar[len] = L'\0';
    std::wstring wstr = m_wchar;
    delete []m_wchar;
    return wstr;
#else
    std::string str = src;
    size_t len = str.size() * 2;
    setlocale(LC_CTYPE, "");
    wchar_t *buf = new wchar_t[len];       //checked

    mbstowcs(buf, str.c_str(), len);
    std::wstring out(buf);
    delete[] buf;
    return out;
#endif
}

std::wstring CCommon::string2wstring(std::string str)
{
    std::wstring wstr = char2wchar(str.c_str());
    return wstr;
}

float CCommon::dB2FloatScale(float fdB)
{
    //n dB = 20*log(A1/A2), here A1/A2 is the scale
    //so A1/A2 =10^( n dB/20)

    float VolumeScale = pow(10.0f, fdB/20);
    return VolumeScale;
}

bool CCommon::IsMatch(int count,char** arg,const char*text)
{
    for(int i = 0;i<count;i++)
    {
        if(strcmp(arg[i],text)==0)
            return true;
    }
    return false;
}

std::string CCommon::ReadFileString(std::string strFilePath)
{
    std::ifstream file;
    file.open(strFilePath.c_str(), std::ios::binary);
    if (!file.is_open())
    {
        return "";
    }
    file.seekg(0, std::ios::end);
    int len = (unsigned int)file.tellg();
    file.seekg(0, std::ios::beg);

    std::shared_ptr<char> p;
    p.reset(new char[len+2]);
    p.get()[len] = 0;
    p.get()[len+1] = 0;

    file.read(p.get(), len);
    file.close();

    std::string str;
    str = p.get();
    return str;
}

void CCommon::msync()
{
#ifndef _MSC_VER
    sync();
#endif
}

void CCommon::Copy(std::string src, std::string dst)
{
    if (src == dst)
         return;

    remove(dst.c_str());
    std::ifstream in(src,std::ios::binary);
    std::ofstream out(dst,std::ios::binary);
    if (!in.is_open())
         return;

    if(!out.is_open())
        return;

    char buf[2048];
    long long totalBytes = 0;
    while(in)
    {
        in.read(buf, 2048);
        out.write(buf, in.gcount());
        totalBytes += in.gcount();
    }
    in.close();
    out.close();
}

bool CompareTime(const t_File &value1,const t_File &value2)
{
    return value1.mTime < value2.mTime;
}

void CCommon::CleanCache(std::string strDir,unsigned int nMaxLen,unsigned int nDelLen)
{
    std::vector<t_File> vFile;
    unsigned int len = CCommon::dfsFolder(strDir, vFile);
    if(len < nMaxLen)
    {
        return;
    }
#ifndef _MSC_VER
    unsigned int overLen = len - nMaxLen;

    //remove newer files
    struct timeval tv;
    gettimeofday(&tv,NULL);
    for (unsigned int i = 0; i < vFile.size(); i++)
    {
        if(vFile[i].mTime>(unsigned int)tv.tv_sec)
        {
            std::string str = strDir + vFile[i].strName;
            printf("remove file(newer):%s",vFile[i].strName.c_str());
            remove(str.c_str());
        }
    }

    //remove older files
    std::sort(vFile.begin(),vFile.end(),CompareTime);
    unsigned int count = 0;
    for (unsigned int i = 0; i < vFile.size(); i++)
    {
        std::string str = strDir + vFile[i].strName;
        printf("remove file:%s",vFile[i].strName.c_str());
        remove(str.c_str());

        count += vFile[i].uLength;
        if(count >= (nDelLen + overLen))
        {
            break;
        }
    }

#endif

}


std::string CCommon::strReplace(std::string& str, const std::string& to_replaced, const std::string& newchars)
{
    std::string::size_type pos = 0;
    while(true)
    {
        pos = str.find(to_replaced,pos);
        if(pos == std::string::npos)
        {
            break;
        }
        str.replace(pos,to_replaced.length(),newchars);
        pos += newchars.length();
    }
    return str;
}

std::string CCommon::EnCrypt(std::string S)
{
    int len = S.length();
    std::string Result;
    Result = S + S;
    for (int i = 0; i < len; i++)
    {
        Result[i]     = S[i]+1;
        Result[i+len] = S[i]+2;
    }
    return Result;
}

std::string  CCommon::DeCrypt(std::string S)
{
    int len = S.length()/2;
    std::string Result;
    Result.resize(len);

    for (int i = 0; i < len; i++)
    {
        Result[i] = S[i]-1;
    }
    return Result;
}

long long CCommon::GetSysTimeMicros()
{
#ifdef _WIN32
#define EPOCHFILETIME   (116444736000000000UL)
    FILETIME ft;
    LARGE_INTEGER li;
    long long tt = 0;
    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    tt = (li.QuadPart - EPOCHFILETIME) /10;
    return tt;
#else
    timeval tv;
    gettimeofday(&tv, 0);
    return (long long)tv.tv_sec * 1000000 + (long long)tv.tv_usec;
#endif
}


