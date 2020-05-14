
#ifndef _MYLOG_H
#define _MYLOG_H

#include "log.h"

//log level
enum e_LEVEL_TYPE
{ 
    LEVEL_ERROR,
    LEVEL_DEBUG
}; 

#define LOG_ERROR(szFormat, ...) \
CMyLog::GetInstance()->Log(LEVEL_ERROR,__FILE__, __FUNCTION__, __LINE__, szFormat, __VA_ARGS__);

#define LOG_DEBUG(szFormat, ...)\
CMyLog::GetInstance()->Log(LEVEL_DEBUG,__FILE__, __FUNCTION__, __LINE__, szFormat, __VA_ARGS__);

class CMyLog 
{ 
public: 
    CMyLog(); 
    virtual ~CMyLog();

    static CMyLog *inst;

    static CMyLog* GetInstance();

    static void Release();

    void  Log(e_LEVEL_TYPE  eLevel,
        const char *szSrcFile,
        const char *szFunction,
        int nLine,
        const char *szFormat, ...);

    void PrintLog(e_LEVEL_TYPE level, std::string strMsg);
   
    CLog   m_LogError;
    CLog   m_LogDebug;
    bool   m_bDebug;
}; 
 
#endif 
