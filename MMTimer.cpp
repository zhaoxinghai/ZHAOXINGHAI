#ifdef WIN32

#include "MMTimer.h"

static CMMTimer::g_MMTimer = new CMMTimer();
CMMTimer::CMMTimer()
{
    m_bRunningFlag=false;
    m_nID=0;
	m_nJobCount = 0;
}

CMMTimer::~CMMTimer()
{
    if (m_RunningFlag)
    {
        Stop();
    }
}

CMMTimer* CMMTimer::GetInstance()
{
	return g_MMTimer;
}

void CMMTimer::AddMediaJob()
{
	m_nJobCount++;
	Start();
}

void CMMTimer::RemoveMediaJob()
{
	--m_nJobCount;
	if(m_nJobCount==0)
	{
		Stop();
	}
}

void CMMTimer::TimeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
    CMMTimer *ptimer=(CMMTimer*)dwUser;
    ptimer->Signal();
}

void CMMTimer::Signal()
{
	std::unique_lock < std::mutex > lck(m_mutex);

    m_cv.notify_all();
}

bool CMMTimer::WaitSignal()
{
	std::unique_lock < std::mutex > lck(m_mutex);
    if (m_cv.wait_for(lck, std::chrono::milliseconds(100)) == std::cv_status::no_timeout)
    {
        return true;
    }
	else
	{
		return false;
	}
}

bool CMMTimer::Start()
{
    bool Result = true;
    if (!m_bRunningFlag)
    {
		m_nID = timeSetEvent(10,Resolution,TimeProc,(DWORD)(this), TIME_PERIODIC);
        if (m_nID != 0)
        {
            m_bRunningFlag = true;
        }
        else
        {
            Result=false;
        }
    }
    return Result;
}

void CMMTimer::Stop()
{
    if (m_bRunningFlag)
    {
        timeKillEvent(m_nID);
        m_bRunningFlag = false;
		m_nID = 0;
    }
}

#endif  //WIN32