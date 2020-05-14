
#include "interfacemsg.h"
#include "threadrecv.h"
#include "threadsend.h"
#include "threadtimer.h"
#include "service.h"
#include "mylog.h"
#include "common.h"
#include "routemanager.h"
#include "paerror.h"
#include "sdk.h"
#include "msgcommon.h"
#include "msgsynctime.h"
#include "msgconnection.h"
#include "msgdevice.h"
#include "device.h"
#include "public.h"

#ifndef _MSC_VER
#include "audiodevicealsa.h"
#endif

#if defined(DLL_EXPORTS)

#include <windows.h>

BOOL APIENTRY DllMain( HMODULE /*hModule*/,
                       int  ul_reason_for_call,
                       LPVOID /*lpReserved*/
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif

CInterface::CInterface()
{
}

CInterface::~CInterface()
{
}

bool CInterface::RunMessage(CMsg * pMsg)
{
    bool bExccute = true;
    switch (pMsg->type)
    {
    case MSG_NORMAL_ANNOUNCEMENT:
    {
        int nNode = pMsg->nInt1;
        CActivate* p = (CActivate*)pMsg->pointer;
        CInterface::NormalAnnouncement(nNode,p);

        delete p;
        p = NULL;
        pMsg->pointer = NULL;
        break;
    }
    case MSG_LOCAL_PLAY_ANNOUNCEMENT:
    {
        CActivatePlay* p = (CActivatePlay*)pMsg->pointer;
        CInterface::LocalPlayAnnouncement(p);

        delete p;
        p = NULL;
        pMsg->pointer = NULL;
        break;
    }
    case MSG_LOCAL_MIC_ANNOUNCEMENT:
    {
        CActivateMicr* p = (CActivateMicr*)pMsg->pointer;
        CInterface::LocalMicrAnnouncement(p);

        delete p;
        p = NULL;
        pMsg->pointer = NULL;
        break;
    }
    case MSG_STOP_ANNOUNCEMENT:
    {
        CInterface::StopAnnouncement(pMsg->nInt1);
        break;
    }
    case MSG_LOCAL_RECORD:
    {
        std::string strPath(pMsg->szbuffer);
        CInterface::LocalRecord(pMsg->nInt1,pMsg->nInt2, strPath);
        break;
    }
    case MSG_TEXT_OUT:
    {
        std::string strText(pMsg->szbuffer);
        CTSystem* pSys = CService::GetInstance()->GetTCPD1SystemBySock(pMsg->socket);
        if(pSys != NULL && pSys->IsConnect())
        {
            pSys->TextOutput(strText);
        }
        break;
    }
    case MSG_STOP_LOCAL_RECORD:
    {
        int nChProcess = CService::GetInstance()->m_routeManager.StopRecord(pMsg->nInt1);
        LOG_DEBUG("%d StopRecord,ID %d",nChProcess,pMsg->nInt1);
        break;
    }
    case MSG_SYNC_DATATIME:
    {
        CInterface::SyncDataTime(&pMsg->datatime);
        break;
    }
    case MSG_VOLUME_AJUST:
    {
        float fScale = CCommon::dB2FloatScale(pMsg->fVolume);
        CService::GetInstance()->m_routeManager.VolumeAjustAudio(pMsg->nInt1, fScale);
        break;
    }
    case MSG_VOLUME_AJUST_PORT:
    {
        SetVolume(pMsg->nInt1==1,pMsg->nInt2,pMsg->fVolume);
        break;
    }
    /*
    case MSG_TRANSPORT_LINE:
    {
        int nNode = pMsg->nInt1;
        bool bAction = pMsg->nInt2==1 ? true:false;
        std::vector<t_TL> *p = (std::vector<t_TL>*)pMsg->pointer;
        
        CTSystem* pSys = CService::GetInstance()->GetD1System(nNode);
        if (pSys)
        {
            CMsgCommon msg(pSys);
            msg.TransportLine(bAction,*p);
        }

        delete p;
        pMsg->pointer = NULL;
        break;
    }
    */
    case MSG_STOP_ALL_AUDIO:
    {
        CService::GetInstance()->m_routeManager.StopAllAudio();
        break;
    }
    default:
    {
        bExccute = false;
        LOG_ERROR("%s","Interface RunMessage");
    }
    }
    return bExccute;
}

void CInterface::SetVolume(bool bCapture,int port,float fVolume)
{
    CService* pSer = CService::GetInstance();

    float fScale = CCommon::dB2FloatScale(fVolume);
    if(bCapture)
    {
        if(port==31)
        {
            pSer->m_VolumeScaleSG = fScale;
            pSer->m_routeManager.VolumeAjustAudioSG(fScale);
        }
        else if(port==41)
        {
            pSer->m_fVolumeMic = pSer->m_fMicADC*fScale;
        }
        else if(port==42)
        {
            pSer->m_fVolumeLineIn = fScale;
        }
    }
    else  //playback
    {
        if(port==41)
        {
            pSer->m_fVolumeSpeaker = fScale;
        }
        else if(port==42)
        {
            pSer->m_fVolumeLineOut = fScale;
        }
    }
}

void CInterface::SyncDataTime(t_DateTime* mytime)
{
    //CMsgSyncTime Msg(CService::GetInstance()->GetSelfD1System());
    //Msg.SyncDataTime(mytime);
}

void CInterface::NormalAnnouncement(int nNode,CActivate *pActivate)
{
    CActivateResult ret;
    ret.chProcess = pActivate->chProcess;
    ret.e_ConState = CON_DIS_CONNECT;

    CTSystem *pSystem = CService::GetInstance()->GetD1System(nNode);
    if (pSystem == NULL)
    {
        ret.nErrorCode = ERROR_SYSTEM_NOEXIST;
        ret.strErrorDesc = "system not exist";
        LOG_ERROR("%d %s", ret.chProcess, ret.strErrorDesc.c_str());
        CService::GetInstance()->ExcuteCallback(&ret);
        return;
    }
    //a new route
    auto pRoute = new CRouteNetwork(nNode,pActivate);  //checked
    CService::GetInstance()->m_routeManager.AddRoute(pRoute);
    pRoute->Activate();
}

void CInterface::LocalPlayAnnouncement(CActivatePlay *pActivate)
{
    /*
    CService* pSer = CService::GetInstance();
    if(pActivate->bPreListen)
    {
        CActivateResult ret;
        ret.chProcess = pActivate->chProcess;

        if(pSer->m_routeManager.GetCapturePlayAudio()>=THREAD_POOL_COUNT)
        {
            LOG_DEBUG("LocalPlayAnnouncement,%s","Audio thread pool full");
            ret.e_ConState = CON_DIS_CONNECT;
            pSer->ExcuteCallback(&ret);
            return;
        }
        //interrupt output
        std::vector<int> &vPort = pActivate->vDest[0].vOutputPort;
        pSer->m_routeManager.InterruptAllOutput(vPort[0]);

        //start play
        CAudioPlay* Job = new CAudioPlay(LOCAL_ADP2LISTEN);  //checked

        Job->EnableSecurity(CService::GetInstance()->m_bEnableSecurity);
        Job->SetRepeatCount(pActivate->nRepeatCount);

        t_FILEMAP prefile;
        if(pSer->GetFileMAP(pActivate->nGongChannel, pActivate->nGongTitle,prefile))
        {
            Job->SetPreSignalFile(prefile);
        }
        t_FILEMAP endfile;
        if(pSer->GetFileMAP(pActivate->nEndSignalChannel, pActivate->nEndSignalTitle,endfile))
        {
            Job->SetEndSignalFile(endfile);
        }
        Job->SetAudioPath(pActivate->vIOMapPath);

        float fScale = CCommon::dB2FloatScale((pActivate->Level)/ 10.0f);
        Job->VolumeAjust(fScale);
        Job->VolumeAjustSG(pSer->m_VolumeScaleSG);
        Job->VolumeAjustEG(pSer->m_VolumeScaleEG);
        Job->SetRtpChannel(pActivate->nRtpChannel);
        Job->SetChProcess(pActivate->chProcess);

        pSer->m_routeManager.AddAudio(Job);

        pSer->m_routeManager.AllocatePort(pActivate->chProcess,
                false, vPort[0], 1,pActivate->nRtpChannel);

        pSer->m_routeManager.StartAudio(pActivate->chProcess);

        ret.e_ConState = CON_FULL_CONNECT;
        pSer->ExcuteCallback(&ret);
    }
    else
    {
        auto pRoute = new CRouteLocal(pActivate);  //checked
        pSer->m_routeManager.AddRoute(pRoute);
        pRoute->Activate();
    }
    */
}

void CInterface::LocalMicrAnnouncement(CActivateMicr *pActivate)
{
    auto pRoute = new CRouteLocal(pActivate);  //checked
    CService::GetInstance()->m_routeManager.AddRoute(pRoute);
    pRoute->Activate();
}

void CInterface::StopAnnouncement(int chProcess)
{
    LOG_DEBUG("StopAnnouncement %d",chProcess);

    CRoute *pRoute = CService::GetInstance()->m_routeManager.GetRouteByProcess(chProcess);
    if (pRoute)
    {
        pRoute->DeActivate();
        pRoute->OnRouteState(CON_DIS_CONNECT);
        CService::GetInstance()->m_routeManager.CleanRoute();
    }
    else
    {
        CService::GetInstance()->m_routeManager.StopAudio(chProcess);
    }
}

void CInterface::LocalRecord(int nRequest,int port,std::string strPath)
{
    CLocalRecordResult ret;
    ret.chRequest = nRequest;
    ret.strFilePath = strPath;

    CService* pSer = CService::GetInstance();
    if(pSer->m_routeManager.GetCapturePlayAudio()>=THREAD_POOL_COUNT)
    {
        LOG_DEBUG("LocalRecord,%s","Audio thread pool full");
        ret.nErrorCode = ERROR_THREAD_POOL_FULL;
        CService::GetInstance()->ExcuteCallback(&ret);
        return;
    }

    //allocate port
    if (CService::GetInstance()->IsPortBusy(true, port))
    {
        CService::GetInstance()->m_routeManager.InterruptInput(port);
        
        //is busy?
        if (CService::GetInstance()->IsPortBusy(true, port))
        {
            LOG_ERROR("%s","not possible to occur");
            ret.nErrorCode = ERROR_LOCAL_MICROPHONE_BUSY;
            CService::GetInstance()->ExcuteCallback(&ret);
            return;
        }
    }

    ret.chProcess = PA::GetChProcess();
    LOG_DEBUG("%d LocalRecord,ID %d",ret.chProcess,ret.chRequest);
    CAudioCapture* pJob = new CAudioCapture(CAPTURE_RECORD, port);  //checked
    pJob->SetRecordPath(strPath);
    pJob->SetChProcess(ret.chProcess);
    pJob->SetRecordID(ret.chRequest);

    CService::GetInstance()->m_routeManager.AddAudio(pJob);
    CService::GetInstance()->m_routeManager.AllocatePort(ret.chProcess,true, port, 1, 0);
    CService::GetInstance()->m_routeManager.StartAudio(ret.chProcess);
}
