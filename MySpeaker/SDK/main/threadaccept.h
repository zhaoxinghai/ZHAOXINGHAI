
#ifndef ACCEPT_H
#define ACCEPT_H

#include "predefine.h"
#include "mythread.h"
#include "mysocket.h"
#include "transport.h"
#include "mylog.h"

class CThreadAccept: public CMyThread
{
public:
    CThreadAccept();
    virtual ~CThreadAccept();

    virtual void Run();
    virtual bool EndThread(bool bWait);

private:
    CMySocket   m_Socket;
};

#endif

