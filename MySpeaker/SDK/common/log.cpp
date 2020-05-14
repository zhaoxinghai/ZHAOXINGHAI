
#include "log.h"
#include "common.h"
#include <algorithm>

#ifdef _MSC_VER
#include <Windows.h>
#endif

CLog::CLog()
{
    m_bStop = false;
    m_bEveryDay = false;
    m_strPath[0] = '\0';
    m_nLogDays = 365;
    memset(m_strFormat,0,sizeof(m_strFormat));
    memset(m_strDir,0,sizeof(m_strDir));
}

CLog::~CLog()
{
}

void CLog::SetDir(const char* strPath)
{
    std::string str(strPath);
    CCommon::CreateFolder(str);
    strncpy(m_strDir,strPath,sizeof(m_strDir)-1);
}

void CLog::SetEveryDay(bool bEveryDay)
{
    m_bEveryDay = bEveryDay;
}

void CLog::SetNameFormat(const char* strFormat)
{
    strncpy(m_strFormat,strFormat,sizeof(m_strFormat)-1);

    //get the path
    if (!m_bEveryDay)
    {
        strncpy(m_strPath,m_strDir,strlen(m_strDir));
        strncat(m_strPath, m_strFormat,strlen(m_strFormat));
    }
    else
    {
        UpdatePath();
    }
}

void CLog::SetNameMatch(const char* strMatch)
{
    m_strNameMatch = strMatch;
}

void CLog::SetLogDays(int nDays)
{
    m_nLogDays = nDays;
}

void CLog::ReCreateFile()
{
    m_file.close();
    m_file.open(m_strPath, std::ios::app);

    CleanCache();
}

void CLog::Log(const char *szFormat, ...)
{
	std::unique_lock < std::mutex > lck(m_mutex);

    if(m_bStop)
    {
        return;
    }

    char temp[800] = { 0 };
    va_list args;
    va_start(args, szFormat);
    vsnprintf(temp, 800, szFormat, args);
    va_end(args);
   
    if (strlen(temp) > 300)
    {
        temp[299] = '\0';
    }
    PrintLog(temp);
}

void CLog::Start()
{
    std::unique_lock < std::mutex > lck(m_mutex);

    m_bStop = false;
}

void CLog::Stop()
{
    std::unique_lock < std::mutex > lck(m_mutex);

    m_bStop = true;
    m_file.close();
}

bool CLog::UpdatePath()
{
    struct tm now_time;
    time_t time_seconds = time(0);

#ifdef _MSC_VER
    localtime_s(&now_time, &time_seconds);
#else
    localtime_r(&time_seconds, &now_time);
#endif

    char name[200] = { 0 };
    snprintf(name, sizeof(name),m_strFormat,
        now_time.tm_year + 1900,
        now_time.tm_mon + 1,
        now_time.tm_mday);

    char logPath[260] = { 0 };
    snprintf(logPath, sizeof(logPath),"%s%s", m_strDir, name);

    if (strcmp(logPath, m_strPath) == 0)
    {
        return false;
    }
    else
    {
        strncpy(m_strPath,logPath,sizeof(m_strPath)-1);
        return true;
    }
}

void CLog::PrintLog(const char* strMsg)
{
    if (m_bEveryDay && UpdatePath())
    {
        ReCreateFile();
    }
    else if(m_file && !m_file.is_open())
    {
        ReCreateFile();
    }

    if (m_file && m_file.is_open())
    {
        m_file.write(strMsg, strlen(strMsg));
        m_file.flush();
    }
}

void CLog::CleanCache()
{
    if (!m_bEveryDay)
        return;

    int len = m_strNameMatch.length();
    std::vector<t_File> vNameAll;
    std::vector<std::string> vName;
    CCommon::dfsFolder(m_strDir, vNameAll);
    for (unsigned int i = 0;i<vNameAll.size();i++)
    {
        std::string str = vNameAll[i].strName.substr(0,len);
        if (str == m_strNameMatch)
        {
            vName.push_back(vNameAll[i].strName);
        }
    }

    if ((int)vName.size() <= m_nLogDays)
    {
        return;
    }
    std::sort(vName.begin(), vName.end());
    for (int i = vName.size()-m_nLogDays-1; i>=0; i--)
    {
        std::string strPath = m_strDir + vName[i];
        remove(strPath.c_str());
    }
}
