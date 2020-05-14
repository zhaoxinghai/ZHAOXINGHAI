
#ifndef THREAD_CONNECT_H
#define THREAD_CONNECT_H

#include "predefine.h"
#include "mythread.h"

//for tcp reconnect
class CThreadConnect : public CMyThread
{
public:
    virtual void Run();

    void Connect();

    //lock
    void AddIP(std::string strIP);
    void RemoveIP(std::string strIP);
    void GetVIP(std::vector<std::string> &vIP);

    std::vector<std::string>     m_vIP;
};

#endif

