
#ifndef ROUTE_H
#define ROUTE_H

#include "d1system.h"
#include "connection.h"
#include "callback.h"
#include "audiojob.h"
#include "routemanager.h"
#include "msgconnection.h"
#include "msgconnectrequest.h"

enum e_ROUTE_TYPE
{
    ROUTE_NETWORK,
    ROUTE_LOCAL,
    ROUTE_REMOTE
};

struct t_AudioDestRoute : public t_AudioDest
{
    t_AudioDestRoute()
    {
        nNoCheckSeconds = 0;
        nRequestSeconds = 0;
        bActivate = false;
        bDeActivate = false;
        bRequest = false;
        eRequest = CON_ACTIVATE;
    }
    t_AudioDestRoute(const t_AudioDest &dest)
    {
        this->Sys = dest.Sys;
        this->Type = dest.Type;
        this->vOutputPort = dest.vOutputPort;

        nRequestSeconds = 0;
        nNoCheckSeconds = 0;
        bActivate = false;
        bDeActivate = false;
        bRequest = false;
        eRequest = CON_ACTIVATE;
    }
    bool             bRequest;
    e_REQUEST        eRequest;
    int              nRequestSeconds;

    bool             bActivate;
    bool             bDeActivate;
    int              nNoCheckSeconds;
    CActivateResult  ret;
};

class CRouteManager;

class CRouteBase
{
public:
    CRouteBase() {};
    virtual ~CRouteBase() {};

	//protocol response
	virtual void OnActivate(int nNode, CActivateResult &ret) = 0;
	virtual void OnDeActivate(int nNode,CActivateResult &ret) = 0;
	virtual void OnInterrupt(int nNode, CActivateResult &ret) = 0;
	virtual void OnReActivate(int nNode, CActivateResult &ret) = 0;
	virtual void OnRouteState(int nNode, CActivateResult &ret) = 0;

	virtual void OnRouteState(CONSTATE conState) = 0;
	virtual void EverySecond() = 0;
};

class CRoute : public CRouteBase
{
public:
	CRoute(CActivate* pA);
	virtual ~CRoute();

    //for local activate
    virtual void Activate();
	virtual void DeActivate();
    virtual void BeCheckRoute(unsigned short chRequest);
    virtual void EverySecond();

    virtual void CurrentPlayIndex(int /*nIndex*/) {};

    virtual void ActivateNode(int nNode);
    void InterruptNode(int nNode);
    void ReActivateNode(int nNode);
    void DeActivateNode(int nNode);

    bool IsDisConnect();
    e_ROUTE_TYPE GetType();
    bool IsRtpOnly();
    int GetRtpNode();
    int GetRtpChannel();
    bool IsReconnect();
    void UpdateResult(int nNode, CActivateResult &ret, e_RESPONE response);

    void GetSource(std::string &strPre,std::string &strSrc,bool bStart);
    void GetDest(std::string szDest[]);
    void GetDestRtp(std::string szDest[]);
    void GetDestNet(std::string szDest[]);

    static std::string GetState(CONSTATE conState);
    static std::string GetPresignal(int nPre);

    CRouteManager        *m_pData;
    CActivateResult       m_RouteResult;
    CActivate*            m_pActivate;
    std::vector<t_AudioDestRoute>  m_vDest;

    bool m_bJobFinish;
    int  m_nPort;

protected:

    // bit 0
    bool IsPartlyMode();
    bool IsFullMode();

    // bit 5
    bool IsReconnectRequest();

    //job state
    bool UpdateJob(CONSTATE &conState);

    //get the dest
    t_AudioDestRoute* GetDestination(int nNode);

    e_ROUTE_TYPE   m_eType;
    bool           m_bRtpOnly;
    int            m_nRtpNode;
    int            m_nRtpChannel;

    //calltime
    int            m_nCalltime;
    bool           m_bPause;
    bool           m_bJobStarted;
    long long      m_lStartedTime;
};

//network activate
class CRouteNetwork : public CRoute
{
public:
	CRouteNetwork(int nNode,CActivate* pA);
    virtual ~CRouteNetwork();
 
    virtual void Activate();
    virtual void ActivateNode(int nNode);

    virtual void OnActivate(int nNode, CActivateResult &ret);
    virtual void OnDeActivate(int nNode,CActivateResult &ret);
    virtual void OnInterrupt(int nNode, CActivateResult &ret);
    virtual void OnReActivate(int nNode, CActivateResult &ret);
    virtual void OnRouteState(int nNode, CActivateResult &ret);

    virtual void OnRouteState(CONSTATE conState);
    virtual void EverySecond();

    void NetworkGongSignal(int nSourceIndex);

    //network system
    CTSystem     *m_pNetSystem;
    bool           m_bPresignal;
};


//this is local play activate
class CRouteLocal : public CRoute
{
public:
	CRouteLocal(CActivate* pA);
	virtual ~CRouteLocal();

    virtual void Activate();

    virtual void OnActivate(int nNode, CActivateResult &ret);
    virtual void OnDeActivate(int nNode,CActivateResult &ret);
    virtual void OnInterrupt(int nNode, CActivateResult &ret);
    virtual void OnReActivate(int nNode, CActivateResult &ret);
    virtual void OnRouteState(int nNode, CActivateResult &ret);

    virtual void OnRouteState(CONSTATE conState);
    virtual void EverySecond();

    virtual bool CreateAudioJob();

    void OnNodeState(int nNode,CActivateResult &ret);
    void OnActivateFull(int nNode, CActivateResult &ret);
    void OnActivatePart(int nNode, CActivateResult &ret);

    //interrupt and restore
    void InterruptInput();
    bool RestoreInput();
	void ReActivateExcept(int nNode);

    void OnGroupState();
    bool ResetInput(CONSTATE conState);

    bool IsInterrupt();

    int   m_nInterruptdom;
    bool  m_bInputConnect;
    bool  m_bCreateJob;

    bool  m_bInputInterrupt;
    bool  m_bDelayActivate;   //the input is using, delay activate
};


//this is remote activate (receive)
class CRouteRemote : public CRouteLocal
{
public:
    CRouteRemote(int nNode, CActivate* pA);
    virtual ~CRouteRemote();
    
    void BeActivate();
    void BeDeActivate();
    void BeInterrupt();
    void BeReActivate(t_AudioSrc &src);
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
    //is rtp activate
    bool IsRtpActivate();
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

	void CopyParameter(CActivateMicr* pMic);
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
    e_RESPONE    m_eRespone;
};

#endif

