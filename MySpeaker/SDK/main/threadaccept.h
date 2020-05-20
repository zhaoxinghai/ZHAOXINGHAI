
#ifndef ACCEPT_H
#define ACCEPT_H

#include "sdkdefine.h"
#include "mythread.h"
#include "mysocket.h"
#include "transport.h"
#include "mylog.h"

class CThreadAccept: public CMyThread
{
public:
    CThreadAccept();
    virtual ~CThreadAccept();

    void SetPort(int nPort);

    virtual void Run();
    virtual bool EndThread(bool bWait);

private:
    CMySocket   m_Socket;
    int         m_nPort;
};

#endif

