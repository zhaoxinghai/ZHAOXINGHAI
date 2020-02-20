
#include "mythread.h"
#include "common.h"

#ifdef _MSC_VER
DWORD WINAPI CMyThread::ThreadProc(LPVOID param)
{
    CMyThread* pThis = (CMyThread*)param;
    pThis->Run();
    return 1;
}
#else
void* CMyThread::ThreadProc(void* arg)
{
    CMyThread* pThis = (CMyThread*)arg;

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    if (pThis->m_ePrority == THREAD_BELOW_NORMAL)
	{
    	CCommon::set_thread_policy(&attr,SCHED_OTHER);
    	setpriority(PRIO_PROCESS, 0, 3);
	}
	else if (pThis->m_ePrority == THREAD_HIGHEST)
	{
		CCommon::set_thread_policy(&attr,SCHED_FIFO);
	}
	else if (pThis->m_ePrority == THREAD_ABOVE_NORMAL)
	{
		CCommon::set_thread_policy(&attr,SCHED_OTHER);
		setpriority(PRIO_PROCESS, 0, -3);
	}
    pThis->Run();
    return NULL;
}
#endif

CMyThread::CMyThread()
{
    m_handle = 0;
    m_ePrority = THREAD_NORMAL;
    m_bActive = false;
}

CMyThread::~CMyThread()
{
}

bool CMyThread::BeginThread(e_ThPrority ePrority /*= THREAD_NORMAL*/)
{
    m_bActive = true;
    m_ePrority = ePrority;

#ifdef _MSC_VER

    m_handle = ::CreateThread(NULL, 0, CMyThread::ThreadProc, 
        this, CREATE_SUSPENDED, NULL);

    if (m_handle != 0)
    {
        if (ePrority == THREAD_BELOW_NORMAL)
        {
            ::SetThreadPriority(m_handle, THREAD_PRIORITY_BELOW_NORMAL);
        }
        else if (ePrority == THREAD_ABOVE_NORMAL)
        {
            ::SetThreadPriority(m_handle, THREAD_PRIORITY_ABOVE_NORMAL);
        }
        else if (ePrority == THREAD_HIGHEST)
        {
            ::SetThreadPriority(m_handle, THREAD_PRIORITY_HIGHEST);
        }
        ::ResumeThread(m_handle);
    }

#else
    int ret = pthread_create(&m_handle, NULL, CMyThread::ThreadProc, (void*)this);
    if(ret != 0)
    {
    	printf("create thread : %s",strerror(ret));
        return false;
    }
#endif
    return true;
}
bool CMyThread::EndThread(bool bWait)
{
    m_bActive = false;
   
    {
        std::unique_lock < std::mutex > lck(m_mutex);
        m_cv.notify_all();
    }
    
#ifdef _MSC_VER
    if (bWait)
    {
        ::WaitForSingleObject(m_handle, INFINITE);
    }
    if (m_handle)
    {
        CloseHandle(m_handle);
        m_handle = NULL;
    }
#else
    if(bWait)
    {
        int ret = pthread_join(m_handle,NULL);
        if(ret != 0)
        {
            printf("pthread_join: %s",strerror(ret));
        }
    }
#endif
    return true;
}

void CMyThread::SleepM(int minisecond)
{
#ifdef _MSC_VER
    Sleep(minisecond);
#else
    usleep(minisecond * 1000);
#endif
}
