
#ifndef ROUTE_LOCAL_H
#define ROUTE_LOCAL_H

#include "route.h"

class CRouteLocal : public CRoute
{
public:
	CRouteLocal(CAnnouncement* pA);
	virtual ~CRouteLocal();

    void Activate() override;
    void OnActivate(int nNode, t_ActivateRet ret) override;
    void OnDeActivate(int nNode, t_ActivateRet ret) override;
    void OnInterrupt(int nNode, t_ActivateRet ret) override;
    void OnReActivate(int nNode, t_ActivateRet ret) override;
    void OnRouteState(int nNode, t_ActivateRet ret) override;

    void OnRouteState(CONSTATE conState) override;
    void EverySecond() override;

    void InterruptNode(int nNode);
    void ReActivateNode(int nNode);
    void DeActivateNode(int nNode);

    bool CreateAudioJob();

    void OnNodeState(int nNode, t_ActivateRet ret);
    void OnActivateFull(int nNode, t_ActivateRet ret);
    void OnActivatePart(int nNode, t_ActivateRet ret);

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

#endif

