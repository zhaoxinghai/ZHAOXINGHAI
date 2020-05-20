
#include "service.h"
#include "routemanager.h"
#include "sdkerror.h"
#include "sdk.h"
#include "msgcommon.h"
#include "msgsynctime.h"
#include "common.h"
#include "mylog.h"
#include "msgconnection.h"
#include "msgvolume.h"
#include "audioplay.h"
#include "audiocapture.h"
#include "device.h"
#include "public.h"
#include "threadcapture.h"
#include "threadplay.h"
#include "audiodevicealsa.h"

void EnableDebug()
{
    CMyLog::GetInstance()->m_bDebug = true;
}

bool Init(t_Node &curNode, std::vector<t_Node> &vSystem)
{
    return g_SDKServer.Init(curNode,vSystem);
}

void SetBaseAddress(int nBaseAddress)
{
    g_SDKServer.m_nBaseAddress = nBaseAddress;
}

void MonitorBusyState()
{
    g_SDKServer.m_bMonitorBusyState;
}

void MonitorLifeSignal()
{
    g_SDKServer.m_bMonitorLifeSignal = true;
}

void SDK_API::SetFileMAP(std::vector<t_FILEMAP> &vFileMap)
{
    g_SDKServer.InitFileMAP(vFileMap);
}

bool GetFileMAP(int nChannel,int nTitle,t_FILEMAP &file)
{
    return g_SDKServer.GetFileMAP(nChannel,nTitle,file);
}

bool RegisterCallback(SDKCallback callback)
{
    //g_SDKServer.m_CallBack = callback;
    return true;
}

int AddSoundCardDevice(std::string strName,bool bCapture,std::vector<int> &vPortCh1, std::vector<int> &vPortCh2)
{
    /*
    CService* pServer = CService::GetInstance();

    //the first port of the channel will create audio buffer
    CAudioBuffer* pBufLeft = NULL;
    CAudioBuffer* pBufRight = NULL;
    if (vPortCh1.size() > 0)
    {
        pBufLeft = CAudioData::CreateBuf(bCapture, vPortCh1);
        pServer->m_vPortGroup.push_back(vPortCh1);
    }
    if (vPortCh2.size() > 0)
    {
        pBufRight = CAudioData::CreateBuf(bCapture, vPortCh2);
        pServer->m_vPortGroup.push_back(vPortCh2);
    }

    if (bCapture)
    {
        auto pCapture = std::make_shared<CThreadCapture>(pServer);
        pCapture->SetDeviceName(strName);
        pCapture->InitPort(vPortCh1, vPortCh2);
        pCapture->SetBuffer(pBufLeft,pBufRight);

        pServer->m_vThreadCapture.push_back(pCapture);

        for (unsigned int i = 0; i < vPortCh1.size(); i++)
        {
            pServer->m_PortCapture[vPortCh1[i]].bUsed = false;
        }
        for (unsigned int i = 0; i < vPortCh2.size(); i++)
        {
            pServer->m_PortCapture[vPortCh2[i]].bUsed = false;
        }
    }
    else
    {
        auto pPlay = std::make_shared<CThreadPlay>();
        pPlay->SetDeviceName(strName);
        pPlay->InitPort(vPortCh1, vPortCh2);
        pPlay->SetBuffer(pBufLeft, pBufRight);
        g_SDKServer.m_vThreadPlay.push_back(pPlay);

        for (unsigned int i = 0; i < vPortCh1.size(); i++)
        {
            pServer->m_PortPlay[vPortCh1[i]].bUsed = false;
        }
        for (unsigned int i = 0; i < vPortCh2.size(); i++)
        {
            pServer->m_PortPlay[vPortCh2[i]].bUsed = false;
        }
    }
    */
    return RESULT_VAL_OK;
}

void InitVolume(float fSG,float fMic,float fLineIn,float fSpeaker,float fLineOut)
{
    /*
    CService* pSer = CService::GetInstance();
    pSer->m_VolumeScaleSG = CCommon::dB2FloatScale(fSG);
    pSer->m_fVolumeMic = pSer->m_fMicADC*CCommon::dB2FloatScale(fMic);
    pSer->m_fVolumeLineIn = CCommon::dB2FloatScale(fLineIn);
    pSer->m_fVolumeSpeaker = CCommon::dB2FloatScale(fSpeaker);
    pSer->m_fVolumeLineOut = CCommon::dB2FloatScale(fLineOut);
    */
}

bool Run()
{
    g_SDKServer.Start();
    return true;
}

bool Exit(bool bWait)
{
    g_SDKServer.Exit(bWait);
    return true;
}

void UpdateSysState(bool bLocalError)
{
    g_SDKServer.m_bLocalError = bLocalError;
}

int SyncDataTime(t_DateTime mytm)
{
    auto pMsg = std::make_shared<CMsg>();
    pMsg.get()->type = MSG_SYNC_DATATIME;
    pMsg.get()->datatime = mytm;

    g_SDKServer.Push(pMsg);
    return RESULT_VAL_OK;
}

int StopLocalRecord(int chProcess)
{
    auto pMsg = std::make_shared<CMsg>();
    pMsg.get()->nInt1 = chProcess;
    pMsg.get()->type = MSG_STOP_LOCAL_RECORD;
    g_SDKServer.Push(pMsg);

    return RESULT_VAL_OK;
}

void VolumeAjust(int chProcess, float fVolumedB)
{
    auto pMsg = std::make_shared<CMsg>();
    pMsg->type = MSG_VOLUME_AJUST;
    pMsg->nInt1 = chProcess;
    pMsg->fVolume = fVolumedB;
    g_SDKServer.Push(pMsg);
}

void SetVolume(int nNode, bool bCapture, int nPort, float fVolumedB)
{
    auto pMsg = std::make_shared<CMsg>();
    pMsg->type = MSG_VOLUME_AJUST_PORT;
    pMsg->nInt1 = bCapture?1:0;
    pMsg->nInt2 = nPort;
    pMsg->fVolume = fVolumedB;
    g_SDKServer.Push(pMsg);
}

void SetStepVolume(int nNode, bool bCapture, int nPort, float fSetupVolumedB)
{

}

int LocalRecord(int chRequest, int nPort, std::string strFilePath)
{
    auto pMsg = std::make_shared<CMsg>();

    pMsg->nInt1 = chRequest;
    pMsg->nInt2 = nPort;
    strncpy(pMsg->szbuffer, strFilePath.c_str(),sizeof(pMsg->szbuffer)-1);
    pMsg->type = MSG_LOCAL_RECORD;

    g_SDKServer.Push(pMsg);

	return RESULT_VAL_OK;
}

int StopAnnouncement(int ID)
{
    auto pMsg = std::make_shared<CMsg>();
    pMsg->type = MSG_STOP_ANNOUNCEMENT;
    pMsg->nInt1 = ID;
    g_SDKServer.Push(pMsg);

    return RESULT_VAL_OK;
}

int NormalAnnouncement(int nNode, CAnnouncement *pActivate)
{
    if (nNode == 0)
    {
        return -1;
    }
    //check audio destination
    if (pActivate->vDest.size() == 0)
    {
        LOG_ERROR("%d %s", pActivate->nProcess, "destination empty");
        return ERROR_DEST_EMPYT;
    }

    CAnnouncement* p = new CAnnouncement(pActivate);                   //checked
      
    auto pMsg = std::make_shared<CMsg>();
    pMsg->type = MSG_NORMAL_ANNOUNCEMENT;
    pMsg->nInt1 = nNode;
    pMsg->pointer = p;
    g_SDKServer.Push(pMsg);

    return RESULT_VAL_OK;
}
int LocalPlayAnnouncement(CAnnouncement *pActivate)
{  
    //check audio destination
    if (pActivate->vDest.size() == 0)
    {
        LOG_ERROR("%d %s", pActivate->nProcess, "destination empty");
        return ERROR_DEST_EMPYT;
    }

    CAnnouncement* p = new CAnnouncement(pActivate);           //checked
    p->Property.nAudioNode = g_SDKServer.GetNode();

    auto pMsg = std::make_shared<CMsg>();
    pMsg->type = MSG_LOCAL_PLAY_ANNOUNCEMENT;
    pMsg->pointer = p;

    g_SDKServer.Push(pMsg);
    return RESULT_VAL_OK;
}
int LocalMicrAnnouncement(CAnnouncement *pActivate)
{
    //check audio destination
    if (pActivate->vDest.size() == 0)
    {
        LOG_ERROR("%d %s", pActivate->nProcess, "destination empty");
        return ERROR_DEST_EMPYT;
    }
    CAnnouncement* p = new CAnnouncement(pActivate);           //checked
    p->Property.nAudioNode = g_SDKServer.GetNode();

    auto pMsg = std::make_shared<CMsg>();
    pMsg->type = MSG_LOCAL_MIC_ANNOUNCEMENT;
    pMsg->pointer = p;

    g_SDKServer.Push(pMsg);
    return RESULT_VAL_OK;
}

bool CheckAdpFile(std::string strPath)
{
	return CPublic::CheckAdpFile(strPath);
}

bool SDK_API::CheckWavFile(std::string strPath)
{
    int nChannel = 0;
    int nDeep = 0;
    int nRate = 0;
    return CPublic::CheckWavFile(strPath,nChannel,nDeep,nRate);
}

float GetLevelmeter(bool bCapture,int nPort)
{
    /*
    CService* pSer = CService::GetInstance();
    float fValue = -80.0f;
    if(!pSer->IsPortBusy(bCapture,nPort))
    {
        return fValue;
    }
    if(bCapture)
    {
        if(nPort == 41)
        {
            fValue = pSer->m_vThreadCapture[0]->m_fLevelLeft;
        }
        else if(nPort == 42)
        {
            fValue = pSer->m_vThreadCapture[0]->m_fLevelRight;
        }
    }
    else
    {
        if(nPort == 41)
        {
            fValue = pSer->m_vThreadPlay[0]->m_fLevelLeft;
        }
        else if(nPort == 42)
        {
            fValue = pSer->m_vThreadPlay[0]->m_fLevelRight;
        }
    }
    return fValue;
    */
    return 0;
}

void TextOutput(MYSOCK tcpSock,std::string strResult)
{
    /*
    auto pMsg = std::make_shared<CMsg>();
    pMsg->socket = tcpSock;
    strncpy(pMsg->szbuffer, strResult.c_str(),sizeof(pMsg->szbuffer)-1);
    pMsg->type = MSG_TEXT_OUT;

    g_SDKServer.Push(pMsg);
    */
}

void LampTest(bool bActivate)
{
    if(g_SDKServer.m_bCCCF)
    {
        auto pMsg = std::make_shared<CMsg>();
        pMsg->nInt1 = bActivate?1:0;
        pMsg->type = MSG_LAMP_TEST;

        g_SDKServer.Push(pMsg);
    }
}