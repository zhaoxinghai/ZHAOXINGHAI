#ifndef THREADPOOL_H  
#define THREADPOOL_H  

#include <thread>  
#include <mutex>
#include <functional>  
#include <string>  
#include <condition_variable>  
#include <deque>  
#include <vector>  
#include <memory>  
#include "audiojob.h"

#define THREAD_POOL_COUNT 20

class ThreadPool
{
public:
    typedef std::function<void()> Task;

    ThreadPool();
    virtual ~ThreadPool();

    void start(int numThreads);
    void stop();

    void push(CAudioJob* pJob);
    bool IsExist(CAudioJob* pJob);

private:
    void runInThread();
    CAudioJob* take();

    std::mutex               m_mutex;
    std::condition_variable  m_notEmpty;

    std::string              m_name;
    std::vector<std::thread> m_vThreads;
    std::deque<CAudioJob*>   m_queue;
    bool                     m_bRunning;
};

#endif // THREADPOOL_H  
