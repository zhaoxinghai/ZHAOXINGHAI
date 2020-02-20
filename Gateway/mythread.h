
#ifndef _MYTHREAD_H_
#define _MYTHREAD_H_

#include <thread>
#include <mutex>
#include <memory>
#include <condition_variable>

#ifdef _MSC_VER

#include <Windows.h>
typedef HANDLE     threadhandle;

#else

#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
typedef pthread_t  threadhandle;

#endif

enum e_ThPrority
{
    THREAD_BELOW_NORMAL,
    THREAD_NORMAL,
    THREAD_ABOVE_NORMAL,
    THREAD_HIGHEST           //realtime
};

class CMyThread
{
public:
    CMyThread();
    virtual ~CMyThread();
    virtual void Run() = 0;
    virtual bool EndThread(bool bWait);

    bool BeginThread(e_ThPrority ePrority = THREAD_NORMAL);    
    void SleepM(int minisecond);

    e_ThPrority         m_ePrority;

protected:
    threadhandle        m_handle;
    bool                m_bActive;

    std::mutex          m_mutex;
    std::condition_variable  m_cv;

#ifdef _MSC_VER
    static DWORD WINAPI ThreadProc(LPVOID param);
#else
    static void* ThreadProc(void* arg);
#endif

};

#endif 
