
#ifndef ROUTE_H
#define ROUTE_H

#include "topsystem.h"
#include "sdkconnect.h"
#include "sdkcallback.h"
#include "msgconnection.h"
#include "msgid.h"

enum e_ROUTE_TYPE
{
    ROUTE_NETWORK,
    ROUTE_LOCAL,
    ROUTE_REMOTE
};

enum CONSTATE
{
    CON_FULL_CONNECT = 0,
    CON_PARTLY_CONNECT,
    CON_INTERRUPTED,
    CON_RECONNECT_POSSIBLE,
    CON_DIS_CONNECT,
    CON_UNKNOW
};

struct t_ActivateRet
{
    CONSTATE e_ConState;
    int      nErrorCode;
    int      nRequest;
    int      nProcess;
    int      nRepeatIndex;
    int      nPlayIndex;
};

struct t_DestinationRoute : public t_Destination
{
    t_DestinationRoute()
    {
        nNoCheckSeconds = 0;
        nRequestSeconds = 0;
        bActivate = false;
        bDeActivate = false;
        bRequest = false;
        eRequest = CON_ACTIVATE;
    }
    t_DestinationRoute(const t_Destination &dest)
    {
        //this->Sys = dest.nNode;
        this->vOutputPort = dest.vOutputPort;

        nRequestSeconds = 0;
        nNoCheckSeconds = 0;
        bActivate = false;
        bDeActivate = false;
        bRequest = false;
        eRequest = CON_ACTIVATE;
    }
    bool             bRequest;
    e_CONNECT_REQUEST eRequest;
    int              nRequestSeconds;

    bool             bActivate;
    bool             bDeActivate;
    int              nNoCheckSeconds;
    t_ActivateRet    ret;
};

class CRoute
{
public:
	CRoute(CAnnouncement* pA);
	virtual ~CRoute();

    virtual void OnActivate(int nNode, t_ActivateRet ret);
    virtual void OnDeActivate(int nNode, t_ActivateRet ret);
    virtual void OnInterrupt(int nNode, t_ActivateRet ret);
    virtual void OnReActivate(int nNode, t_ActivateRet ret);

    //for local activate
    virtual void Activate();
    virtual void ActivateNode(int nNode);
	virtual void DeActivate();
    virtual void BeCheckRoute(int nRequest);
    virtual void EverySecond();
    virtual void OnRouteState(int nNode,t_ActivateRet ret);
    virtual void OnRouteState(CONSTATE conState);

    //virtual void CurrentPlayIndex(int /*nIndex*/) {};
    e_ROUTE_TYPE GetType();

    bool IsDisConnect();
    bool IsRtpOnly();
    bool IsReconnect();
    void UpdateResult(int nNode, t_ActivateRet ret, int response);

    bool                  m_bJobFinish;
    t_ActivateRet         m_RouteResult;
    CAnnouncement         m_Activate;

    std::vector<t_DestinationRoute>  m_vDest;

protected:
    bool IsPartMode();
    bool IsFullMode();

    bool UpdateJob(CONSTATE&conState);

    //get the dest
    t_DestinationRoute* GetDestination(int nNode);
    std::string GetState(CONSTATE conState);

    e_ROUTE_TYPE   m_eType;

    //calltime
    int            m_nCalltime;
    bool           m_bJobStarted;
    long long      m_lStartedTime;
};

#endif

