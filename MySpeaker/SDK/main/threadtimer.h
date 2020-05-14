
#ifndef THREAD_TIMER_H
#define THREAD_TIMER_H

#include "predefine.h"
#include "mythread.h"

//for every one second timer
class CThreadTimer: public CMyThread
{
public:
    virtual void Run();
};

#endif

