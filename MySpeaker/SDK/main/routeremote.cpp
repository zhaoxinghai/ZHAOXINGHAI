
#include "routeremote.h"
#include "service.h"
#include "routemanager.h"
#include "msgconnection.h"
#include "msgconnectrequest.h"
#include "msgcheckroute.h"
#include "public.h"
#include "audioplay.h"
#include "audiocapture.h"
#include "audioreceive.h"
#include "common.h"

CRouteRemote::CRouteRemote(int nNode,CAnnouncement* pA)
    :CRouteLocal(pA)
{
    m_eType = ROUTE_REMOTE;
    m_pSystem = g_SDKServer.GetD1System(nNode);
    m_nCheckTimes = 0;
    m_nTimerCount10 = 0;
    m_RouteResult.nRequest = pA->nRequest;
    m_eRespone = CON_ON_ROUTESTATE;
    m_bBeInterrupt = false;
}

CRouteRemote::~CRouteRemote()
{
}

void CRouteRemote::BeActivate()
{
	if (m_Activate.Property.bAudioStream)
	{
        LOG_DEBUG("%d RTP BeActivate request %d,node(%d)",
                  m_RouteResult.nProcess,m_RouteResult.nRequest,m_pSystem->m_Node.nNode);

        m_eRespone = CON_ON_ACTIVATE;
		BeActivateRtp();
	}
	else
	{
        LOG_DEBUG("%d Remote BeActivate request %d,node(%d)",
                  m_RouteResult.nProcess,m_RouteResult.nRequest,m_pSystem->m_Node.nNode);

        m_eRespone = CON_ON_ACTIVATE;
		BeActivateLocal();
	}
}

void CRouteRemote::BeDeActivate()
{
    m_eRespone = CON_ON_DEACTIVATE;
    if(m_Activate.Property.bAudioStream)
    {
        LOG_DEBUG("%d RTP BeDeActivate, request %d",m_RouteResult.nProcess,m_RouteResult.nRequest);
    }
    else
    {
        LOG_DEBUG("%d Remote BeDeActivate, request %d",m_RouteResult.nProcess,m_RouteResult.nRequest);
        CRoute::DeActivate();
    }
}

void CRouteRemote::BeInterrupt()
{
    if(m_Activate.Property.bAudioStream)
    {
        LOG_DEBUG("%d RTP BeInterrupt, request %d",m_RouteResult.nProcess,m_RouteResult.nRequest);
    }
    else
    {
        LOG_DEBUG("%d Remote BeInterrupt, request %d",m_RouteResult.nProcess,m_RouteResult.nRequest);
    }
    m_bBeInterrupt = true;
    m_eRespone = CON_ON_INTERRUPT;
    bool bFree = ReleaseOutput();
    OnRouteState(CON_INTERRUPTED);

    if(bFree)
    {
        g_SDKServer.SendBusyState();
    }
}

void CRouteRemote::BeReActivate()
{
    m_bBeInterrupt = false;
    if(m_Activate.Property.bAudioStream)
    {
        LOG_DEBUG("%d RTP BeReActivate, request %d",m_RouteResult.nProcess,m_RouteResult.nRequest);
        m_eRespone = CON_ON_REACTIVATE;

        if(m_RouteResult.e_ConState == CON_INTERRUPTED
            || m_RouteResult.e_ConState == CON_RECONNECT_POSSIBLE)
        {
            ActivateOutput();
        }
        else
        {
            CMsgConnectRequest request(m_pSystem);
            request.SendRouteStatus(m_RouteResult, m_eRespone);
        }
    }
    else
    {
        LOG_DEBUG("%d Remote BeReActivate, request %d",m_RouteResult.nProcess,m_RouteResult.nRequest);
    }
}

void CRouteRemote::BeRouteStatus()
{
    if(m_Activate.Property.bAudioStream)
    {
        LOG_DEBUG("%d RTP BeRouteState, request %d",m_RouteResult.nProcess,m_RouteResult.nRequest);
    }
    else
    {
        LOG_DEBUG("%d Remote BeRouteState, request %d",m_RouteResult.nProcess,m_RouteResult.nRequest);
    }
    m_eRespone = CON_ON_ROUTESTATE;
    CMsgConnectRequest request(m_pSystem);
    request.SendRouteStatus(m_RouteResult, m_eRespone);
}

int CRouteRemote::GetPhysicalPort()
{
    auto it = m_Activate.vSrc.begin();
    while(it != m_Activate.vSrc.end())
    {
        t_Source &src = (*it);
        if(src.eType == SOURCE_MIC)
        {
            return src.nNumber;
        }
        it++;
    }
    return -1;
}

bool CRouteRemote::IsOutputExist(std::vector<int> &vOutput)
{
	for(unsigned int i = 0;i<vOutput.size();i++)
	{
		if(!g_SDKServer.IsOutput(vOutput[i]))
		{
			return false;
		}
	}
	return true;
}

void CRouteRemote::BeActivateLocal()
{
	if (m_Activate.Property.nPort > 0)
	{
		BeActivatePhysical();
	}
	else
	{
		BeActivateVirtual();
	}
}

void CRouteRemote::BeActivateRtp()
{
    //only one destination
    if (m_vDest.size() == 1 && m_vDest[0].nNode == g_SDKServer.GetNode())
    {
        //check the port
        if (m_vDest[0].vOutputPort.size() == 0
            || !IsOutputExist(m_vDest[0].vOutputPort)
            || !CreateAudioJob())
        {
            LOG_ERROR("RTP BeActivate:%s", "Output Port Error");
            CMsgConnectRequest request(m_pSystem);
            m_RouteResult.e_ConState = CON_DIS_CONNECT;
            request.SendRouteStatus(m_RouteResult, m_eRespone);
            return;
        }
        GetOutput(m_vDest[0].vOutputPort);
        m_vDest.clear();
        ActivateOutput();
    }
    else
    {
        LOG_ERROR("RTP BeActivate:%s", "Only support one destination");
        CMsgConnectRequest request(m_pSystem);
        m_RouteResult.e_ConState = CON_DIS_CONNECT;
        request.SendRouteStatus(m_RouteResult, m_eRespone);
    }
}

void CRouteRemote::GetOutput(std::vector<int> &vOutput)
{
    for (unsigned int i = 0; i < vOutput.size(); i++)
    {
        t_Output out;
        out.nPort = vOutput[i];
        out.bConnect = false;
        m_vOutput.push_back(out);
    }
}

bool CRouteRemote::CreateAudioJob()
{
    if (m_Activate.Property.bAudioStream)
    {
        CAudioReceive *pJob = new CAudioReceive();  //checked

        pJob->SetChProcess(m_RouteResult.nProcess);

        pJob->SetRtpNode(m_Activate.Property.nAudioNode);
        pJob->SetRtpChannel(m_Activate.Property.nAudioChannal);

        g_SDKServer.m_routeManager.AddAudio(pJob);
        return true;
    }
    else
    {
        return CRouteLocal::CreateAudioJob();
    }
}

void CRouteRemote::OnRouteState(CONSTATE conState)
{
    bool bFree = false;
    if(m_RouteResult.e_ConState != conState)
    {
        m_RouteResult.e_ConState = conState;
        if(!CRoute::UpdateJob(conState))
        {
            DeActivate();
            OnRouteState(CON_DIS_CONNECT);
            return;
        }
        if(m_Activate.Property.bAudioStream)
        {
            LOG_DEBUG("%d RTP OnRouteState: %s",m_RouteResult.nProcess,
                  GetState(m_RouteResult.e_ConState).c_str());

            if (m_RouteResult.e_ConState == CON_DIS_CONNECT)
            {
                bFree = ReleaseOutput();
            }
        }
        else
        {
            LOG_DEBUG("%d Remote OnRouteState: %s",m_RouteResult.nProcess,
                      GetState(m_RouteResult.e_ConState).c_str());
        }

        //respond result to remote
        CMsgConnectRequest request(m_pSystem);
        request.SendRouteStatus(m_RouteResult, m_eRespone);
    }
    else
    {
        if(m_Activate.Property.bAudioStream)
        {
            //fro RTP job, always send the route state
            CMsgConnectRequest request(m_pSystem);
            request.SendRouteStatus(m_RouteResult, m_eRespone);
        }
    }

    if (ResetInput(conState) || bFree)
    {
        g_SDKServer.SendBusyState();
    }
}

void CRouteRemote::EverySecond()
{
    CRouteLocal::EverySecond();

    if (IsDisConnect())
        return;

    //check the remote client every 10 seconds
    m_nTimerCount10++;
    if (m_nTimerCount10 >= 10)
    {
        m_nTimerCount10 = 0;
        m_nCheckTimes++;
        if (m_nCheckTimes < 3)
        {
            //submid:0x09
            CMsgCheckRoute check(m_pSystem);
            check.Check(m_RouteResult.nRequest, m_RouteResult.nProcess);
        }
        else
        {
            LOG_DEBUG("%d no check feedback for 30 seconds", m_RouteResult.nProcess);
            DeActivate();
            OnRouteState(CON_DIS_CONNECT);
        }
    }
}

void CRouteRemote::OnCheckRoute()
{
    m_nCheckTimes = 0;
}

void CRouteRemote::InterruptOutput(std::vector<int> &vConflictPort)
{
    if(!m_bJobStarted)
    {
        //when the job not start, don't need to interrupt.
        return;
    }

    LOG_DEBUG("%d RTP Interrupt Output",m_RouteResult.nProcess);

    if (IsPartMode())
    {
        InterruptOutputPartly(vConflictPort);
    }
    else
    {
        InterruptOutputFull();
    }
}

void CRouteRemote::InterruptOutputPartly(std::vector<int> &vConflictPort)
{
    bool bFree = false;
    unsigned int nConnectCount = 0;
    for (unsigned int i = 0; i < m_vOutput.size(); i++)
    {
        if (!m_vOutput[i].bConnect)
        {
            continue;
        }
        //free the conflict port
        if(CPublic::IsPortExist(vConflictPort,m_vOutput[i].nPort))
        {
            m_vOutput[i].bConnect = false;
            m_vOutput[i].bInterrupt = true;
            g_SDKServer.m_routeManager.FreePort(m_RouteResult.nProcess,
                false, m_vOutput[i].nPort);
            bFree = true;
        }
        if (m_vOutput[i].bConnect)
        {
            nConnectCount++;
        }
    }
   
    if (nConnectCount == m_vOutput.size())
    {
        OnRouteState(CON_FULL_CONNECT);
    }
    else if (nConnectCount>0)
    {
        OnRouteState(CON_PARTLY_CONNECT);
    }
    else
    {
        //0 connect
        if (IsReconnect())
        {
            OnRouteState(CON_INTERRUPTED);
        }
        else
        {
            //3, do disconnect (interrupt output)
            OnRouteState(CON_DIS_CONNECT);
        }
    }

    if (bFree)
    {
        auto pMsg = std::make_shared<CMsg>();
        pMsg->type = MSG_RESTORE_OUTPUT;
        g_SDKServer.Push(pMsg);
    }
}

void CRouteRemote::InterruptOutputFull()
{
    ReleaseOutput();
    if (IsReconnect())
    {
        OnRouteState(CON_INTERRUPTED);
    }
    else
    {
        //3, do disconnect (interrupt output)
        OnRouteState(CON_DIS_CONNECT);
    }
}

void CRouteRemote::ActivateOutput()
{
    if (IsPartMode())
    {
        if (!IsPartNewOutputCanUsed())
        {
            OnOutputGroupState();
            return;
        }
    }
    else
    {
        if (!IsAllOutputCanUsed())
        {
            OnRouteState(CON_INTERRUPTED);
            return;
        }
    }
    TryToConnect();
}

void CRouteRemote::TryToConnect()
{
    if (IsOutputConflict())
    {
        g_SDKServer.m_routeManager.InterruptOutput(this);
    }

    //allocate free ports
    bool bAllocate = false;
    for (unsigned int i = 0; i < m_vOutput.size(); i++)
    {
        t_Output &output = m_vOutput[i];
        if(output.bConnect == false)
        {
            if (g_SDKServer.IsPortFree(false,output.nPort))
            {
                bAllocate = true;
                output.bConnect = true;
                g_SDKServer.m_routeManager.AllocatePort(m_RouteResult.nProcess,
                    false, output.nPort, 
                    m_Activate.Property.nPriority,
                    m_Activate.Property.nAudioChannal);
            }
        }
    }

    OnOutputGroupState();

    if(bAllocate)
    {
        g_SDKServer.SendBusyState();
    }
}

void CRouteRemote::OnOutputGroupState()
{
    unsigned int nConnectCount = 0;
    for (unsigned int i = 0; i < m_vOutput.size(); i++)
    {
        t_Output &output = m_vOutput[i];
        if(output.bConnect)
        {
            nConnectCount++;
            continue;
        }
    }

    if (nConnectCount == m_vOutput.size())
    {
        OnRouteState(CON_FULL_CONNECT);
    }
    else if (nConnectCount > 0)
    {
        OnRouteState(CON_PARTLY_CONNECT);
    }
    else
    {
        OnRouteState(CON_INTERRUPTED);
    }
}

bool CRouteRemote::IsNeed2Restore()
{
    for (unsigned int i = 0; i < m_vOutput.size(); i++)
    {
        t_Output &output = m_vOutput[i];
        if (!output.bConnect)
        {
            return true;
        }
    }
    return false;
}

bool CRouteRemote::RestoreOutput()
{
    if(m_bBeInterrupt)
    {
        //don't need to restore if the route be Interrupted.
        return false;
    }
    if (!IsNeed2Restore())
    {
        return false;
    }

    m_eRespone = CON_ON_ROUTESTATE;

    //full connect
    if (IsPartMode())
    {
        return RestoreOutputPartly();
    }
    else
    {
        return RestoreOutputFull();
    } 
}

bool CRouteRemote::RestoreOutputFull()
{
    if (!IsAllOutputCanUsed())
    {
        return false;
    }
    if(m_RouteResult.e_ConState == CON_RECONNECT_POSSIBLE)
    {
        return false;
    }
    else
    {
        LOG_DEBUG("%d RTP RestoreOutput(Full)",m_RouteResult.nProcess);

        m_RouteResult.e_ConState = CON_RECONNECT_POSSIBLE;
        CMsgConnectRequest request(m_pSystem);
        request.SendRouteStatus(m_RouteResult, CON_ON_ROUTESTATE);
        return true;
    }
}

bool CRouteRemote::RestoreOutputPartly()
{
    if (!IsPartNewOutputCanUsed())
    {
        return false;
    }

    if(m_bJobStarted)
    {
        if(!IsReconnect())
        {
            //the job started, but no reconnect
            return false;
        }
    }
    if(m_RouteResult.e_ConState == CON_RECONNECT_POSSIBLE)
    {
        return false;
    }
    else
    {
        LOG_DEBUG("%d RTP RestoreOutput(Part)",m_RouteResult.nProcess);
        m_RouteResult.e_ConState = CON_RECONNECT_POSSIBLE;
        CMsgConnectRequest request(m_pSystem);
        request.SendRouteStatus(m_RouteResult, CON_ON_ROUTESTATE);
        return true;
    }
}

bool CRouteRemote::IsAllOutputCanUsed()
{
    for (unsigned int i = 0; i < m_vOutput.size(); i++)
    {
        t_Output &output = m_vOutput[i];
        if (!g_SDKServer.IsPortCanUsed(false,
            output.nPort,m_Activate.Property.nPriority))
        {
            return false;
        }
    }
    return true;
}

bool CRouteRemote::IsPartNewOutputCanUsed()
{
    for (unsigned int i = 0; i < m_vOutput.size(); i++)
    {
        t_Output &output = m_vOutput[i];
        if(output.bConnect == false)
        {
            if (g_SDKServer.IsPortCanUsed(false,
                output.nPort, m_Activate.Property.nPriority))
            {
                return true;
            }
        }
    }
    return false;
}

bool CRouteRemote::IsOutputConflict()
{
    for (unsigned int i = 0; i < m_vOutput.size(); i++)
    {
        t_Output &output = m_vOutput[i];
        if(output.bConnect == false)
        {
            if (!g_SDKServer.IsPortFree(false,output.nPort))
            {
                return true;
            }
        }
    }
    return false;
}

void CRouteRemote::BeActivatePhysical()
{
	CRouteLocal::Activate();
}

void CRouteRemote::BeActivateVirtual()
{
    /*
    GetIOMAP(pPlay->vIOMapPath);
    
    if(pPlay->vIOMapPath.size()==0 || !CPublic::IsAllAdpFormat(pPlay->vIOMapPath))
	{
        if(pPlay->vIOMapPath.size()==0)
        {
            LOG_ERROR("Remote BeActivate(Virtual):%s", "No AudioPath Error");
        }
        else
        {
            LOG_ERROR("Remote BeActivate(Virtual):%s", "AudioPath Format Error");
        }
        CMsgConnectRequest request(m_pSystem);
        m_RouteResult.e_ConState = CON_DIS_CONNECT;
        request.RouteStatus(m_RouteResult, m_eRespone);
		return;
	}
    m_RouteResult.LastSrcIdx = pPlay->vIOMapPath.size() - 1;
    */
	CRouteLocal::Activate();
}

void CRouteRemote::GetIOMAP(std::vector<t_FILEMAP> &vPath)
{
    /*
	vPath.clear();

	for (unsigned int i = 0; i<m_pActivate->vSrc.size(); i++)
	{
		int num = m_pActivate->vSrc[i].Num + 1;
		int title = m_pActivate->vSrc[i].Title+1;

        t_FILEMAP file;
        if(g_SDKServer.GetFileMAP(num,title,file))
		{
            vPath.push_back(file);
		}
        else
        {
#ifdef _MSC_VER
            std::string strPath = CCommon::GetModulePath() + "Presignal\\nofile.adp";
#else
            std::string strPath = CCommon::GetModulePath() + "Presignal/nofile.adp";
#endif
            file.vPath.push_back(strPath);
            vPath.push_back(file);
        }
	}
    */
}

bool CRouteRemote::ReleaseOutput()
{
    bool bFree = false;
    for (unsigned int i = 0; i < m_vOutput.size(); i++)
    {
        if (m_vOutput[i].bConnect)
        {
            bFree = true;
            m_vOutput[i].bConnect = false;
            g_SDKServer.m_routeManager.FreePort(m_RouteResult.nProcess,
                false, m_vOutput[i].nPort);
        }
    }

    if (bFree)
    {
        auto pMsg = std::make_shared<CMsg>();
        pMsg->type = MSG_RESTORE_OUTPUT;
        g_SDKServer.Push(pMsg);
    }
    return bFree;
}

void CRouteRemote::CurrentPlayIndex(int nIndex)
{
    m_RouteResult.nPlayIndex = nIndex;
    if (m_pSystem)
    {
        CMsgConnectRequest request(m_pSystem);
        request.SendRouteStatus(m_RouteResult, CON_ON_ROUTESTATE);
    }
}
