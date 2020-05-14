
#include "service.h"
#include "routemanager.h"
#include "paerror.h"
#include "sdk.h"
#include "msgcommon.h"
#include "msgsynctime.h"
#include "common.h"
#include "mylog.h"
#include "msgconnection.h"
#include "msgdevice.h"
#include "audioplay.h"
#include "audiocapture.h"
#include "device.h"
#include "public.h"
#include "threadcapture.h"
#include "threadplay.h"
#include "audiodevicealsa.h"

//grabol callback function for app
PACallback  g_Callback;

void PA::InitArg(int count,char** arg)
{
    if(CCommon::IsMatch(count,arg,"-debug"))
    {
        CMyLog::GetInstance()->m_bDebug = true;
    }
}

bool PA::Init(t_Node &curNode, std::vector<t_Node> &vSystem)
{
    return CService::GetInstance()->Init(curNode,vSystem);
}

void PA::SetBaseAddress(int nBaseAddress)
{
    CService* pSer = CService::GetInstance();
    pSer->m_nBaseAddress = nBaseAddress;
}

void PA::Init(t_Configuration &c)
{
    CService* pSer = CService::GetInstance();
    pSer->m_fMicADC = CCommon::dB2FloatScale(c.fMicrophone_ADC);
    pSer->m_nMaxDebugLength = c.nMaxLogDebug;
    pSer->m_routeManager.m_nRemoteMaxCount = c.nMaxBgQueueCount;
    pSer->m_routeManager.m_nPlayingMaxCount = c.nMaxPlayingCount;
}

void PA::MonitorBusyState()
{
    CService* pSer = CService::GetInstance();
}

void PA::MonitorLifeSignal()
{
    CService* pSer = CService::GetInstance();
}

void PA::SetFileMAP(std::vector<t_FILEMAP> &vFileMap)
{
    CService::GetInstance()->InitFileMAP(vFileMap);
}

bool PA::GetFileMAP(int nChannel,int nTitle,t_FILEMAP &file)
{
    return CService::GetInstance()->GetFileMAP(nChannel,nTitle,file);
}

bool PA::RegisterCallback(PACallback callback)
{
    g_Callback = callback;
    return true;
}

int PA::AddSoundCardDevice(std::string strName,bool bCapture,std::vector<int> &vPortCh1, std::vector<int> &vPortCh2)
{
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
        CService::GetInstance()->m_vThreadPlay.push_back(pPlay);

        for (unsigned int i = 0; i < vPortCh1.size(); i++)
        {
            pServer->m_PortPlay[vPortCh1[i]].bUsed = false;
        }
        for (unsigned int i = 0; i < vPortCh2.size(); i++)
        {
            pServer->m_PortPlay[vPortCh2[i]].bUsed = false;
        }
    }
    return RESULT_VAL_OK;
}

void PA::InitVolume(float fSG,float fMic,float fLineIn,float fSpeaker,float fLineOut)
{
    CService* pSer = CService::GetInstance();
    pSer->m_VolumeScaleSG = CCommon::dB2FloatScale(fSG);
    pSer->m_fVolumeMic = pSer->m_fMicADC*CCommon::dB2FloatScale(fMic);
    pSer->m_fVolumeLineIn = CCommon::dB2FloatScale(fLineIn);
    pSer->m_fVolumeSpeaker = CCommon::dB2FloatScale(fSpeaker);
    pSer->m_fVolumeLineOut = CCommon::dB2FloatScale(fLineOut);
}

bool PA::Run()
{
    CService::GetInstance()->Start();
    return true;
}

bool PA::Exit(bool bWait)
{
    CService::GetInstance()->Exit(bWait);
    return true;
}

void PA::UpdateSysState(bool bLocalError)
{
    CService::GetInstance()->m_bLocalError = bLocalError;
}

///////////////////////////////////////////////////////////////////////////
///////Multi thread///////////////////////////////////////////////////////

void PA::SwitchMode(e_Mode eMode)
{
    LOG_DEBUG("%s","PA::SwitchMode");

    if(eMode == MODE_CONFIG)
    {
        CMyLog::GetInstance()->m_LogDebug.Stop();
        CMyLog::GetInstance()->m_LogError.Stop();

        auto pMsg = std::make_shared<CMsg>();
        pMsg->type = MSG_STOP_ALL_AUDIO;
        CService::GetInstance()->Push(pMsg);
    }
    else
    {
        CMyLog::GetInstance()->m_LogDebug.Start();
        CMyLog::GetInstance()->m_LogError.Start();
    }
}

int PA::SyncDataTime(t_DateTime mytm)
{
    auto pMsg = std::make_shared<CMsg>();
    pMsg.get()->type = MSG_SYNC_DATATIME;
    pMsg.get()->datatime = mytm;

    CService::GetInstance()->Push(pMsg);
    return RESULT_VAL_OK;
}

void PA::GetDevicesState(int nNode, t_VDevice dev)
{
    auto pMsg = std::make_shared<CMsg>();
    pMsg->type = MSG_GET_DEVICE_STATE;
    pMsg->datatime.Sec = nNode;
    pMsg->datatime.Min = dev.type;
    pMsg->datatime.Hour = dev.number;
    pMsg->datatime.Day = dev.channel;

    CService::GetInstance()->Push(pMsg);
}

void PA::LogDebug(const char *szFormat, ...)
{
    char temp[800] = { 0 };
    va_list args;
    va_start(args, szFormat);
    vsnprintf(temp, 800, szFormat, args);
    va_end(args);
    std::string str(temp);

    //time
    char tim[100] = { 0 };
    struct tm now_time;
    time_t time_seconds = time(0);
#ifdef _MSC_VER
    localtime_s(&now_time, &time_seconds);
#else
    localtime_r(&time_seconds, &now_time);
#endif
    snprintf(tim,sizeof(tim),"[DEBUG][%02d:%02d:%02d]  ",now_time.tm_hour, now_time.tm_min, now_time.tm_sec);

    std::string strTm(tim);
    str = "\n" + strTm + str;
    CMyLog::GetInstance()->PrintLog(LEVEL_DEBUG, str);
}

void PA::LogError(const char *szFormat, ...)
{
    char temp[800] = { 0 };
    va_list args;
    va_start(args, szFormat);
    vsnprintf(temp, 800, szFormat, args);
    va_end(args);
    std::string str(temp);

    //time
    char tim[100] = { 0 };
    struct tm now_time;
    time_t time_seconds = time(0);
#ifdef _MSC_VER
    localtime_s(&now_time, &time_seconds);
#else
    localtime_r(&time_seconds, &now_time);
#endif
    snprintf(tim,sizeof(tim),"[ERROR][%02d:%02d:%02d]  ",now_time.tm_hour, now_time.tm_min, now_time.tm_sec);

    std::string strTm(tim);
    str = "\n" + strTm + str;
    CMyLog::GetInstance()->PrintLog(LEVEL_ERROR, str);
}

unsigned short PA::GetChProcess()
{
    return CPublic::GetChProcess();
}

int PA::StopLocalRecord(int chProcess)
{
    auto pMsg = std::make_shared<CMsg>();
    pMsg.get()->nInt1 = chProcess;
    pMsg.get()->type = MSG_STOP_LOCAL_RECORD;
    CService::GetInstance()->Push(pMsg);

    return RESULT_VAL_OK;
}

void PA::VolumeAjust(int chProcess, float fVolumedB)
{
    auto pMsg = std::make_shared<CMsg>();
    pMsg->type = MSG_VOLUME_AJUST;
    pMsg->nInt1 = chProcess;
    pMsg->fVolume = fVolumedB;
    CService::GetInstance()->Push(pMsg);
}

void PA::SetVolume(bool bCapture,int nPort,float fVolumedB)
{
    auto pMsg = std::make_shared<CMsg>();
    pMsg->type = MSG_VOLUME_AJUST_PORT;
    pMsg->nInt1 = bCapture?1:0;
    pMsg->nInt2 = nPort;
    pMsg->fVolume = fVolumedB;
    CService::GetInstance()->Push(pMsg);
}

int PA::LocalRecord(int chRequest, int nPort, std::string strFilePath)
{
    auto pMsg = std::make_shared<CMsg>();

    pMsg->nInt1 = chRequest;
    pMsg->nInt2 = nPort;
    strncpy(pMsg->szbuffer, strFilePath.c_str(),sizeof(pMsg->szbuffer)-1);
    pMsg->type = MSG_LOCAL_RECORD;

    CService::GetInstance()->Push(pMsg);

	return RESULT_VAL_OK;
}

int PA::StopAnnouncement(int ID)
{
    auto pMsg = std::make_shared<CMsg>();
    pMsg->type = MSG_STOP_ANNOUNCEMENT;
    pMsg->nInt1 = ID;
    CService::GetInstance()->Push(pMsg);

    return RESULT_VAL_OK;
}

int PA::NormalAnnouncement(int nNode,CActivate *pActivate)
{
    if (nNode == 0)
    {
        return -1;
    }
    //check audio destination
    if (pActivate->vDest.size() == 0)
    {
        LOG_ERROR("%d %s", pActivate->chProcess, "destination empty");
        return ERROR_DEST_EMPYT;
    }

    CActivate* p = new CActivate(pActivate);                   //checked
      
    auto pMsg = std::make_shared<CMsg>();
    pMsg->type = MSG_NORMAL_ANNOUNCEMENT;
    pMsg->nInt1 = nNode;
    pMsg->pointer = p;
    CService::GetInstance()->Push(pMsg);

    return RESULT_VAL_OK;
}
int PA::LocalPlayAnnouncement(CActivatePlay *pActivate)
{  
    //check audio destination
    if (pActivate->vDest.size() == 0)
    {
        LOG_ERROR("%d %s", pActivate->chProcess, "destination empty");
        return ERROR_DEST_EMPYT;
    }

    CActivatePlay* p = new CActivatePlay(pActivate);           //checked
    //p->nRtpNode = CService::GetInstance()->GetNode();

    auto pMsg = std::make_shared<CMsg>();
    pMsg->type = MSG_LOCAL_PLAY_ANNOUNCEMENT;
    pMsg->pointer = p;

    CService::GetInstance()->Push(pMsg);
    return RESULT_VAL_OK;
}
int PA::LocalMicrAnnouncement(CActivateMicr *pActivate)
{
    //check audio destination
    if (pActivate->vDest.size() == 0)
    {
        LOG_ERROR("%d %s", pActivate->chProcess, "destination empty");
        return ERROR_DEST_EMPYT;
    }
    CActivateMicr* p = new CActivateMicr(pActivate);           //checked
    //p->nRtpNode = CService::GetInstance()->GetNode();

    auto pMsg = std::make_shared<CMsg>();
    pMsg->type = MSG_LOCAL_MIC_ANNOUNCEMENT;
    pMsg->pointer = p;

    CService::GetInstance()->Push(pMsg);
    return RESULT_VAL_OK;
}

bool PA::CheckAdpFile(std::string strPath)
{
	return CPublic::CheckAdpFile(strPath);
}

bool PA::CheckWavFile(std::string strPath)
{
    int nChannel = 0;
    int nDeep = 0;
    int nRate = 0;
    return CPublic::CheckWavFile(strPath,nChannel,nDeep,nRate);
}

void PA::TransportLine(int nNode, bool bAction, std::vector<t_TL> vTL)
{
    auto pMsg = std::make_shared<CMsg>();
    auto p = new std::vector<t_TL>;                                 //checked

    for (unsigned int i = 0; i < vTL.size(); i++)
    {
        p->push_back(vTL[i]);
    }
    pMsg->type = MSG_TRANSPORT_LINE;
    pMsg->nInt1 = nNode;
    pMsg->nInt2 = (int)bAction;
    pMsg->pointer = p;

    CService::GetInstance()->Push(pMsg);
}

float PA::GetLevelmeter(bool bCapture,int nPort)
{
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
}

bool PA::IsAudioJobOverRun(int &nRunningCount)
{
    CService* pSer = CService::GetInstance();
    return pSer->IsAudioJobOverRun(nRunningCount);
}

std::string PA::TelnetCommand(std::string strCmd)
{
    CService* pSer = CService::GetInstance();
    std::string strRet = "";
    if(strCmd == "constat")
    {
        strRet = pSer->m_routeManager.Constat();
    }
    return strRet;
}

void PA::TextOutput(MYSOCK tcpSock,std::string strResult)
{
    auto pMsg = std::make_shared<CMsg>();
    pMsg->socket = tcpSock;
    strncpy(pMsg->szbuffer, strResult.c_str(),sizeof(pMsg->szbuffer)-1);
    pMsg->type = MSG_TEXT_OUT;

    CService::GetInstance()->Push(pMsg);
}

void PA::AudioControl(int nNode,const t_VDevice &de,float fVolume)
{
    auto pMsg = std::make_shared<CMsg>();
    pMsg->nInt1 = nNode;
    pMsg->nInt2 = int(fVolume*10);
    pMsg->device = de;
    pMsg->type = MSG_AUDIO_CONTROL;

    CService::GetInstance()->Push(pMsg);
}

void PA::ReplyAudioControl(bool IsTCP,int nNode,MYSOCK tcpSock,t_VolReply &reply)
{
    auto pMsg = std::make_shared<CMsg>();
    pMsg->nInt1 = IsTCP?1:0;
    pMsg->nInt2 = nNode;
    pMsg->socket = tcpSock;
    pMsg->volreply = reply;
    pMsg->type = MSG_VOL_REPLY;

    CService::GetInstance()->Push(pMsg);
}

void PA::EnableCCCF(bool bOpen)
{
    CService::GetInstance()->m_bCCCF = bOpen;
}

void PA::LampTest(bool bActivate)
{
    if(CService::GetInstance()->m_bCCCF)
    {
        auto pMsg = std::make_shared<CMsg>();
        pMsg->nInt1 = bActivate?1:0;
        pMsg->type = MSG_LAMP_TEST;

        CService::GetInstance()->Push(pMsg);
    }
}

void PA::EnableSecurity(bool bEnable)
{
    CService::GetInstance()->m_bEnableSecurity = bEnable;
}
