
#ifndef ROUTE_REMOTE_H
#define ROUTE_REMOTE_H

#include "routelocal.h"

class CRouteRemote : public CRouteLocal
{
public:
    CRouteRemote(int nNode,CAnnouncement* pA);
    virtual ~CRouteRemote();
    
    void BeActivate();
    void BeDeActivate();
    void BeInterrupt();
    void BeReActivate();
    void BeRouteStatus();

    //only remote have this function
    void OnCheckRoute();
  
    virtual void OnRouteState(CONSTATE conState);
    virtual void EverySecond();
    virtual bool CreateAudioJob();
    virtual void CurrentPlayIndex(int nIndex);

    //output conflict,interrupt
    void InterruptOutput(std::vector<int> &vConflictPort);

    //output restore
    bool RestoreOutput();
    bool IsNeed2Restore();

    std::vector<t_Output>  m_vOutput;
    CTSystem             *m_pSystem;

protected:
    int GetPhysicalPort();

    bool IsOutputExist(std::vector<int> &vOutput);

    //remote audio source
    void BeActivateRtp();
    void ActivateOutput();

    void OnOutputGroupState();

    //local audio source
    void BeActivateLocal();

    void GetOutput(std::vector<int> &vOutput);
    bool ReleaseOutput();
    bool IsAllOutputCanUsed();
    bool IsPartNewOutputCanUsed();
    bool IsOutputConflict();

    //local audio source
	void BeActivatePhysical();
	void BeActivateVirtual();

	void CopyParameter(CAnnouncement* pMic);
    void GetIOMAP(std::vector<t_FILEMAP> &vPath);
	
    void InterruptOutputPartly(std::vector<int> &vConflictPort);
    void InterruptOutputFull();

    bool RestoreOutputFull();
    bool RestoreOutputPartly();
    void TryToConnect();

    //no feedback for 3 times(30 seconds)
    int          m_nCheckTimes;    
    int          m_nTimerCount10;

    bool         m_bBeInterrupt;
    e_CONNECT_RESPONE    m_eRespone;
};

#endif

