
#include "mylog.h"
#include "common.h"
#include <assert.h>

#define TF_PATH    "/run/media/mmcblk0p1/"

CMyLog *CMyLog::inst = new CMyLog();                               //checked

CMyLog::CMyLog()
{
#ifdef _MSC_VER
    std::string str = CCommon::GetModulePath() + "Logs\\";
#else
    std::string str = std::string(TF_PATH) + "Logs/debug/";
#endif

    m_LogError.SetDir(str.c_str());
    m_LogError.SetEveryDay(true);
    m_LogError.SetNameFormat("error-%d.%.2d.%.2d.txt");
    m_LogError.SetNameMatch("error-");

    m_LogDebug.SetDir(str.c_str());
    m_LogDebug.SetEveryDay(true);
    m_LogDebug.SetNameFormat("debug-%d.%.2d.%.2d.txt");
    m_LogDebug.SetNameMatch("debug-");
    m_LogDebug.SetLogDays(30);

    m_bDebug = false;
}

CMyLog::~CMyLog()
{
}

CMyLog * CMyLog::GetInstance()
{
	return inst;
}

void CMyLog::Release()
{
    delete inst;
    inst = NULL;
}

void CMyLog::Log(
    e_LEVEL_TYPE  eLevel,
    const char *szSrcFile, 
    const char *szFunction,
    int nLine, 
    const char *szFormat, ...)
{
    //get filename
    std::string strFile(szSrcFile);
#ifdef _MSC_VER
    size_t pos = strFile.rfind("\\");
#else
    size_t pos = strFile.rfind("/");
#endif
    strFile = strFile.substr(pos+1, strFile.length() - pos - 1);
    std::string strFun(szFunction);
    char szline[10] = {0};
    snprintf(szline,sizeof(szline),"%d", nLine);
   
    std::string strLine(szline);

    //get the message
    char temp[800] = {0};
    va_list args;
    va_start(args, szFormat);
    vsnprintf(temp, 800, szFormat, args);
    va_end(args);
    std::string str(temp);
 
    char tim[100] = { 0 };

    struct tm now_time;
    time_t time_seconds = time(0);

#ifdef _MSC_VER
    localtime_s(&now_time, &time_seconds);
#else
    localtime_r(&time_seconds, &now_time);
#endif
    snprintf(tim, sizeof(tim),"%02d:%02d:%02d",now_time.tm_hour, now_time.tm_min, now_time.tm_sec);
  
    const char *type[2] =
    {
        "ERROR",
        "DEBUG"
    };

    std::string info(type[eLevel]);
    std::string stm(tim);

    std::string strMsg;

    if(eLevel == LEVEL_ERROR)
    {
        strMsg = "[" + info + "]"
                + "[" + tim + "]  "
                + "[" + strFun + "]  "
                + "[" + strLine + "]  "
                + str;
    }
    else
    {
        strMsg = "[" + info + "]"
                + "[" + tim + "]  "
                + str;
    }
    //text align
    if (strMsg.length() < 61)
    {
        strMsg += "                                                     ";
        strMsg = strMsg.substr(0, 60);
    }
    std::string str2 = "\n";
    PrintLog(eLevel, str2 + strMsg);
}

void CMyLog::PrintLog(e_LEVEL_TYPE level, std::string strMsg)
{
    if(m_bDebug)
    {
        CCommon::Trace("%s",strMsg.c_str());
        if (level == LEVEL_ERROR)
        {
            assert(0);
        }
    }
    else
    {
        if (level == LEVEL_ERROR)
        {
            m_LogError.Log("%s", strMsg.c_str());
        }
        else if (level == LEVEL_DEBUG)
        {
            m_LogDebug.Log("%s", strMsg.c_str());
        }
    }
}
