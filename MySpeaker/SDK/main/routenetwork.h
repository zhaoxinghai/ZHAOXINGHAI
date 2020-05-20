
#ifndef ROUTE_NETWORK_H
#define ROUTE_NETWORK_H

#include "route.h"

class CRouteNetwork : public CRoute
{
public:
	CRouteNetwork(int nNode,CAnnouncement* pA);
    virtual ~CRouteNetwork();
 
    void ActivateNode(int nNode) override;
    void OnRouteState(CONSTATE conState) override;
    void EverySecond() override;

    void NetworkGongSignal(int nSourceIndex);

    //network system
    CTSystem     *m_pNetSystem;
    bool          m_bPresignal;
};

#endif

