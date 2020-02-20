#include "threadpool.h"  
#include "common.h"

ThreadPool::ThreadPool() 
{
    m_name = "ThreadPool";
    m_bRunning = false;
}

ThreadPool::~ThreadPool()
{
    if (m_bRunning)
    {
        stop();
    }
}

void ThreadPool::start(int numThreads)
{
    if(m_bRunning)
    {
        return;
    }
    m_bRunning = true;
    m_vThreads.reserve(numThreads);

    for (int i = 0; i < numThreads; ++i)
    {
        m_vThreads.push_back(std::thread(&ThreadPool::runInThread, this));
    }
}

void ThreadPool::stop()
{
    {
        std::unique_lock<std::mutex>  lock(m_mutex);
        m_bRunning = false;
        m_notEmpty.notify_all();
    }

    for (size_t i = 0; i < m_vThreads.size(); ++i)
    {
        m_vThreads[i].join();
    }
    m_vThreads.clear();
}

void ThreadPool::push(CAudioJob* pJob)
{
    std::unique_lock<std::mutex> lock(m_mutex);
 
    auto it = m_queue.begin();
    while (it != m_queue.end())
    {
        if ((*it) == pJob)
        {
            return;
        }
        else
        {
            it++;
        }
    }
    
    m_queue.push_back(pJob);
    m_notEmpty.notify_one();
}

CAudioJob* ThreadPool::take()
{
    std::unique_lock<std::mutex> lock(m_mutex);

    while (m_queue.empty() && m_bRunning)
    {
        m_notEmpty.wait(lock);
    }

    CAudioJob *task = NULL;
    if (!m_queue.empty())
    {
        task = m_queue.front();
        m_queue.pop_front();
    }
    return task;
}

void ThreadPool::runInThread()
{
    CCommon::SetThreadName("CThreadAudio");

    try
    {
        CMySocket::Init();
   
        while (m_bRunning)
        {
            CAudioJob* task = take();
            if ( (task != NULL) && m_bRunning)
            {
                task->Run();
            }
        }
        CMySocket::Exit();
    }
    catch (const std::exception& ex)
    {
        fprintf(stderr, "exception caught in ThreadPool %s\n", m_name.c_str());
        fprintf(stderr, "reason: %s\n", ex.what());
        abort();
    }
    catch (...)
    {
        fprintf(stderr, "exception caught in ThreadPool %s\n", m_name.c_str());
    }
}
