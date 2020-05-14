
#ifndef _LOG_H
#define _LOG_H

#include <mutex>
#include <fstream>
#include <ostream>
#include <stdarg.h>
#include <string.h>

class CLog
{ 
public: 
    CLog();
    virtual ~CLog();

    //1,set the folder
    void SetDir(const char* strPath);
    std::string GetDir(){ return std::string(m_strDir);};

    //2,every day a new log file
    void SetEveryDay(bool bEveryDay);

    //3,if everyday is true: "Error-%d.%d.%d.txt"
    //  if everyday is false: "your file name"
    void SetNameFormat(const char* strFormat);
    void SetNameMatch(const char* strMatch);

    //4,Save how many days?
    void SetLogDays(int nDays);

    void Log(const char *szFormat, ...);
    void Start();
    void Stop();

protected: 

    bool UpdatePath();
    void ReCreateFile();
    void PrintLog(const char* strMsg);

    void CleanCache();

    bool        m_bEveryDay;
    char        m_strDir[100];
    char        m_strFormat[100];
    std::string      m_strNameMatch;
    char        m_strPath[360];
    int         m_nLogDays;

    std::ofstream  m_file;
    std::mutex     m_mutex;
    bool           m_bStop;
}; 
 
#endif 
