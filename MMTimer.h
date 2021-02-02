
#ifdef WIN32

#include <mmsystem.h>
#include <thread>
#include <mutex>
#include <memory>
#include <condition_variable>

#pragma comment(lib,"winmm.lib")

typedef void (*TIMERCALLBACK)(DWORD);

class CMMTimer
{
public:
    CMMTimer();
    ~CMMTimer();
	
	static CMMTimer* GetInstance();
	
	void AddMediaJob();
	void RemoveMediaJob();
	
    bool WaitSignal();
	void Signal();
	
private:

	static void CALLBACK TimeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

	static CMMTimer* g_MMTimer;

	bool Start();
    void Stop();
	
	int         m_nJobCount;
    bool        m_bRunningFlag;
    UINT        m_nID;
	
	std::mutex  m_mutex;
    std::condition_variable  m_cv;
};

#endif   //WIN32