
#ifndef COMMON_H
#define COMMON_H

//this file can only include C++ standard header
//this file for all other project

#ifdef _MSC_VER
#include <Windows.h>
#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <memory.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>

#endif

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

struct t_File
{
    std::string  strName;
    unsigned int uLength;
    unsigned int mTime;
};

class CCommon
{
public:

    static void Trace(std::string str);
    static void Trace(const char *szFormat, ...);

    static std::string GetModulePath();
    static std::string GetHostIP(std::string strDefault);

    static void SleepMinisecond(int minisecond);
    static void WaitMinisecond(int minisecond, bool *bBreak);

    static void Split(std::vector<std::string> &vResult, std::string str, std::string token);

    static bool IsAllNumber(std::string str);

    static std::string StrFormat(const char *szFormat, ...);

    static bool IsFileExist(std::string strPath);
    static void CreateFolder(std::string strPath);
    static std::string GetFileExt(std::string strPath);
    static std::string GetFileName(std::string strPath);

    //trim std::string by token
    static std::string GetString(std::string strToken, std::string &strSrc);

    static void TrimeRight(std::string &strSrc, std::string strToken);
    static void TrimeLeft(std::string &strSrc, std::string strToken);
    static std::string strToLower(const std::string &str);
    static std::string strReplace(std::string& str, const std::string& to_replaced, const std::string& newchars);

    static bool IsIPAddress(std::string strSrc);
    static void Copy(std::string src, std::string dst);

    //GPIO
    static FILE* OpenGPIO(int pin, bool bRead);
    static FILE* OpenGPIO(std::string strPath, bool bRead);
    static int ReadGPIO(FILE* file);
    static std::string ReadGPIO2(FILE* file);
    static void WriteGPIO(int pin, FILE* file, int value);
    static void WriteGPIO2(FILE* file, std::string value);

    static void SetThreadName(std::string strName);

    //volume from dB to floatscale
    static float dB2FloatScale(float fdB);

    static unsigned int dfsFolder(std::string folderPath, std::vector<t_File> &vFile);
    static void CleanCache(std::string strDir,unsigned int nMaxLen,unsigned int nDelLen);

    static std::string wchar2char(const wchar_t* wsrc);

    static std::wstring char2wchar(const char* src);

    static std::wstring string2wstring(std::string str);

    //match text
    static bool IsMatch(int count,char** arg,const char*text);

    static std::string ReadFileString(std::string strFilePath);

    //sync
    static void msync();

    static std::string EnCrypt(std::string S);
    static std::string DeCrypt(std::string S);

    static long long GetSysTimeMicros();
};

class CPrintTime
{
public:
    CPrintTime(std::string strName = "time")
    {
		m_strName = strName;
        m_begin = std::chrono::high_resolution_clock::now();
    }
    ~CPrintTime()
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
        int mytime = (int)std::chrono::duration_cast<std::chrono::milliseconds>(end - m_begin).count();
        if (mytime > 1)
        {
            std::string str = CCommon::StrFormat("%s:%d", m_strName.c_str(), mytime);
            CCommon::Trace(str);
        }
    }

protected:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_begin;
    std::string m_strName;
};

//print the function interval time
#define PRINT_TIME(str) \
static std::chrono::time_point<std::chrono::high_resolution_clock> begin = std::chrono::high_resolution_clock::now();\
std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();\
int time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();\
begin = end;\
CCommon::Trace("%s: %d\n", str.c_str(),time);

#endif

