#include <mutex>
#include <QDebug>

#include "cqmlpage.h"
#include "cqmlsourcemanage.h"
#include "crecord.h"

static std::mutex m_mutex2;

CQmlPage* CQmlPage::pQmlPage = NULL;

CQmlPage * CQmlPage::GetInstance()
{
    if (pQmlPage == NULL)
    {
        m_mutex2.lock();
        if (pQmlPage == NULL)
        {
            pQmlPage = new CQmlPage(NULL);
        }
        m_mutex2.unlock();
    }
    return pQmlPage;
}

void CQmlPage::Release()
{
    if (NULL != pQmlPage)
    {
        m_mutex2.lock();
        if (NULL != pQmlPage)
        {
            delete pQmlPage;
            pQmlPage = NULL;
        }
        m_mutex2.unlock();
    }
}


CQmlPage::CQmlPage(QObject *parent) : QObject(parent)
{

}

void CQmlPage::init()
{
    //printAllOptions();
}

int CQmlPage::livePageStartCall(int iPri, float iVolume,
        int iSingal, bool bReconnect, bool bPartly, int port)
{
   t_LiveOption opt;
   opt.nPreSignalTitle = iSingal;
   opt.nPriority = iPri;
   opt.fVolumedB = iVolume;
   opt.bPartlyConnect = bPartly;
   opt.bReconnect = bReconnect;
   opt.nLivePort = port;

   CLivePage live;
   m_callID = CLogic::GetInstance()->GetCallID();
   return live.StartCall(m_callID, opt);
}

int CQmlPage::livePageStopCall(int id)
{
    CLivePage live;
    live.StopCall(id);
	return 0;
}

int CQmlPage::liveMakePredefine(QString name, int iPri,
        float iVolume, int iSingal, bool bReconnect,
        bool bPartly, int port)
{
    t_LiveOption opt;
    opt.strName = name.toStdString();
    opt.nPreSignalTitle = iSingal;
    opt.nPriority = iPri;
    opt.fVolumedB = iVolume;
    opt.bPartlyConnect = bPartly;
    opt.bReconnect = bReconnect;
    opt.nLivePort = port;

    CLivePage live;
    return live.MakePredefine(opt);
}

int CQmlPage::liveMakeSchedule(int iPri, float iVolume,
        int iSingal, bool bReconnect, bool bPartly, int port)
{
    t_LiveOption opt;
    opt.nPreSignalTitle = iSingal;
    opt.nPriority = iPri;
    opt.fVolumedB = iVolume;
    opt.bPartlyConnect = bPartly;
    opt.bReconnect = bReconnect;
    opt.nLivePort = port;

    CLivePage live;
    live.MakeSchedule(opt, m_param);

    return 0;
}

int CQmlPage::networkPageStartCall(QString callType, int node, int iPri,
        float iVolume, int iSingal, bool bReconnect, bool bPartly,
        int repeat, int interval, bool gapTime)
{
    t_NetworkOption opt;

    opt.nNode = node;
    opt.nPreSignalTitle = iSingal;
    opt.nPriority = iPri;
    opt.bPartlyConnect = bPartly;
    opt.bReconnect = bReconnect;
    opt.fVolumedB = iVolume;
    opt.nRepeatCount = repeat;
    opt.nRepeatInterval = interval;
    opt.bGapTime = gapTime;
    opt.eType = NET_AUDIO;

    CNetworkPage network;
    std::vector<t_Audio> vAudio;
    CQmlSourceManage::GetInstance()->fillChannelAndTitle(vAudio);

    m_callID = CLogic::GetInstance()->GetCallID();
    if (callType == "Call")
    {
        network.StartCall(m_callID, vAudio, opt);
    }
    else if(callType == "PreListen")
    {
        network.PreListen(m_callID, vAudio, opt);
    }
    return 0;
}

int CQmlPage::networkPageStartLiveCall(QString callType, int node, int channel,
        int title, int iPri, float iVolume, int iSingal, bool bReconnect,
        bool bPartly, int repeat, int interval, bool gapTime)
{
    t_NetworkOption opt;

    opt.nNode = node;
    opt.nPreSignalTitle = iSingal;
    opt.nPriority = iPri;
    opt.bPartlyConnect = bPartly;
    opt.bReconnect = bReconnect;
    opt.fVolumedB = iVolume;
    opt.nRepeatCount = repeat;
    opt.nRepeatInterval = interval;
    opt.bGapTime = gapTime;
    opt.eType = NET_PHYSICAL;

    CNetworkPage network;
    std::vector<t_Audio> vAudio;
    t_Audio tmp;
    tmp.nChannel = channel;
    tmp.nTitle = title;
    vAudio.push_back(tmp);

    m_callID = CLogic::GetInstance()->GetCallID();
    if (callType == "Call")
    {
        network.StartCall(m_callID, vAudio, opt);
    }
    else if(callType == "PreListen")
    {
        network.PreListen(m_callID, vAudio, opt);
    }
    return 0;
}

int CQmlPage::networkPageStopCallFromId(QString callType, int id)
{
    CNetworkPage network;
    if (callType == "Call")
    {
        network.StopCall(id);
    }
    else if(callType == "PreListen")
    {
        network.StopPreListen(id);
    }
    return 0;
}

int CQmlPage::networkMakePredefine(int node, QString name, int iPri,
        float iVolume, int iSingal, bool bReconnect, bool bPartly,
        int repeat, int interval, bool gapTime)
{
    t_NetworkOption opt;

    opt.nNode = node;
    opt.strName = name.toStdString();
    opt.nPreSignalTitle = iSingal;
    opt.nPriority = iPri;
    opt.bPartlyConnect = bPartly;
    opt.bReconnect = bReconnect;
    opt.fVolumedB = iVolume;
    opt.nRepeatCount = repeat;
    opt.nRepeatInterval = interval;
    opt.bGapTime = gapTime;
    opt.eType = NET_AUDIO;

    CNetworkPage network;
    std::vector<t_Audio> vAudio;
    CQmlSourceManage::GetInstance()->fillChannelAndTitle(vAudio);

    network.MakePredefine(vAudio, opt);

    return 0;
}

int CQmlPage::networkLiveMakePredefine(int node, int channel, int title,
        QString name, int iPri, float iVolume, int iSingal, bool bReconnect,
        bool bPartly, int repeat, int interval, bool gapTime)
{
    t_NetworkOption opt;

    opt.nNode = node;
    opt.strName = name.toStdString();
    opt.nPreSignalTitle = iSingal;
    opt.nPriority = iPri;
    opt.bPartlyConnect = bPartly;
    opt.bReconnect = bReconnect;
    opt.fVolumedB = iVolume;
    opt.nRepeatCount = repeat;
    opt.nRepeatInterval = interval;
    opt.bGapTime = gapTime;
    opt.eType = NET_PHYSICAL;

    CNetworkPage network;
    std::vector<t_Audio> vAudio;
    t_Audio tmp;
    tmp.nChannel = channel;
    tmp.nTitle = title;
    vAudio.push_back(tmp);

    network.MakePredefine(vAudio, opt);

    return 0;
}


int CQmlPage::networkMakeSchedule(int node, int iPri, float iVolume, int iSingal,
        bool bReconnect, bool bPartly, int repeat, int interval, bool gapTime)
{
    t_NetworkOption opt;

    opt.nNode = node;
    opt.nPreSignalTitle = iSingal;
    opt.nPriority = iPri;
    opt.bPartlyConnect = bPartly;
    opt.bReconnect = bReconnect;
    opt.fVolumedB = iVolume;
    opt.nRepeatCount = repeat;
    opt.nRepeatInterval = interval;
    opt.bGapTime = gapTime;
    opt.eType = NET_AUDIO;

    CNetworkPage network;
    std::vector<t_Audio> vAudio;
    CQmlSourceManage::GetInstance()->fillChannelAndTitle(vAudio);

    network.MakeSchedule(vAudio, opt, m_param);

    return 0;
}

int CQmlPage::networkLiveMakeSchedule(int node, int channel, int title,
        int iPri, float iVolume, int iSingal, bool bReconnect,
        bool bPartly, int repeat, int interval, bool gapTime)
{
    t_NetworkOption opt;

    opt.nNode = node;
    opt.nPreSignalTitle = iSingal;
    opt.nPriority = iPri;
    opt.bPartlyConnect = bPartly;
    opt.bReconnect = bReconnect;
    opt.fVolumedB = iVolume;
    opt.nRepeatCount = repeat;
    opt.nRepeatInterval = interval;
    opt.bGapTime = gapTime;
    opt.eType = NET_PHYSICAL;

    CNetworkPage network;
    std::vector<t_Audio> vAudio;
    t_Audio tmp;
    tmp.nChannel = channel;
    tmp.nTitle = title;
    vAudio.push_back(tmp);

    network.MakeSchedule(vAudio, opt, m_param);

    return 0;
}

int CQmlPage::baseStartCall(QString SrcType, QString callType, int iPri, float iVolume,
        int iPreSingal,int iEndSignal,bool bReconnect, bool bPartly, int repeat, int interval, bool gapTime)
{
    t_LocalOption opt;
    opt.nPreSignalTitle = iPreSingal;
    opt.nEndSignalTitle = iEndSignal;
    opt.nPriority = iPri;
    opt.bPartlyConnect = bPartly;
    opt.bReconnect = bReconnect;
    opt.fVolumedB = iVolume;
    opt.nRepeatCount = repeat;
    opt.nRepeatInterval = interval;
    opt.bGapTime = gapTime;

    m_callID = CLogic::GetInstance()->GetCallID();

    if (SrcType == "USB")
    {
        std::vector<std::string> vPath;
        CQmlSourceManage::GetInstance()->fillPathListFromUsbSourceList(vPath);

        CUSBPage usblocal;
        if (callType == "Call")
        {
            return usblocal.StartCall(m_callID, vPath, opt);
        }
        else if(callType == "PreListen")
        {
            return usblocal.PreListen(m_callID, vPath, opt);
        }
    }
    else if(SrcType == "Local")
    {
        std::vector<t_FILEMAP> vPath;
        CQmlSourceManage::GetInstance()->fillLocalSource(vPath);

        CLocalPage local;
        if (callType == "Call")
        {
            return local.StartCall(m_callID, vPath, opt);
        }
        else if(callType == "PreListen")
        {
            return local.PreListen(m_callID, vPath, opt);
        }
    }
    return 0;
}

int CQmlPage::baseStopCallFromId(QString callType, int id)
{
    CLocalPage local;
    if (callType == "Call")
    {
        local.StopCall(id);
    }
    else if(callType == "PreListen")
    {
        local.StopPreListen(id);
    }
    return 0;
}

int CQmlPage::baseMakePredefine(QString SrcType, QString name, int iPri, float iVolume,
        int iSingal, bool bReconnect, bool bPartly, int repeat, int interval, bool gapTime)
{
    t_LocalOption opt;
    opt.strName = name.toStdString();
    opt.nPreSignalTitle = iSingal;
    opt.nPriority = iPri;
    opt.bPartlyConnect = bPartly;
    opt.bReconnect = bReconnect;
    opt.fVolumedB = iVolume;
    opt.nRepeatCount = repeat;
    opt.nRepeatInterval = interval;
    opt.bGapTime = gapTime;

    std::vector<std::string> vPath;
    if (SrcType == "USB")
    {
        CQmlSourceManage::GetInstance()->fillPathListFromUsbSourceList(vPath);

        CUSBPage local;
        local.MakePredefine(vPath, opt);
    }
    else if(SrcType == "Local")
    {
        //to do
        //CQmlSourceManage::GetInstance()->fillLocalSource(vPath);

        //CLocalPage local;
        //local.MakePredefine(vPath, opt);
    }
    else if (SrcType == "Record")
    {
        //to do
        //opt.bIsRecord = true;
        //CRecord::GetInstance()->fillRecordSource(vPath);

        //CLocalPage local;
        //local.MakePredefine(vPath, opt);
    }


    return 0;
}

int CQmlPage::baseMakeSchedule(QString SrcType, int iPri, float iVolume, int iSingal,
        bool bReconnect, bool bPartly, int repeat, int interval, bool gapTime)
{
    t_LocalOption opt;
    opt.nPreSignalTitle = iSingal;
    opt.nPriority = iPri;
    opt.bPartlyConnect = bPartly;
    opt.bReconnect = bReconnect;
    opt.fVolumedB = iVolume;
    opt.nRepeatCount = repeat;
    opt.nRepeatInterval = interval;
    opt.bGapTime = gapTime;

    if (SrcType == "USB")
    {
        std::vector<std::string> vPath;
        CQmlSourceManage::GetInstance()->fillPathListFromUsbSourceList(vPath);

        CUSBPage local;
        local.MakeSchedule(vPath, opt, m_param);
    }
    else if(SrcType == "Local")
    {
        std::vector<t_FILEMAP> vPath;
        //to do
        //CQmlSourceManage::GetInstance()->fillLocalSource(vPath);

        //CLocalPage local;
        //local.MakeSchedule(vPath, opt, m_param);
    }
    else if (SrcType == "Record")
    {
        std::vector<t_FILEMAP> vPath;
        opt.bIsRecord = true;
        //CRecord::GetInstance()->fillRecordSource(vPath);

        CLocalPage local;
        local.MakeSchedule(vPath, opt, m_param);
    }
    return 0;
}

int CQmlPage::predefinePreListen(float vol)
{
    t_PredefineOption opt;

    opt.fVolumedB = vol;

    CPredefinePage predefine;
    m_callID = CLogic::GetInstance()->GetCallID();
    return predefine.PreListen(opt, m_callID);
}

int CQmlPage::predefineStopPreListen(int id)
{
    CPredefinePage predefine;
    predefine.StopPreListen(id);

    return 0;
}

int CQmlPage::predefineCall(float vol)
{
    int errorCode;
    t_PredefineOption opt;
    std::vector<int> vCallID;
    opt.fVolumedB = vol;

    CPredefinePage predefine;
    errorCode = predefine.StartCall(opt, vCallID);
    if(errorCode == 0 && vCallID.size()==1)
    {
        m_callID = vCallID[0];
    }
    return errorCode;
}

int CQmlPage::predefineStopCallFromId(int id)
{
    CPredefinePage predefine;
    predefine.StopCall(id);

    return 0;
}

int CQmlPage::predefineVolumeControl()
{
    CPredefinePage predefine;
    predefine.VolumeControl();
    return 0;
}

int CQmlPage::predefineMakeSchedule(float vol)
{
    t_PredefineOption opt;
    opt.fVolumedB = vol;

    CPredefinePage predefine;
    predefine.MakeSchedule(opt, m_param);

    return 0;
}

int CQmlPage::vaLiveStartCall()
{
    t_LiveOption opt;
    opt.nPreSignalTitle = 0;
    opt.nPriority = 20;
    opt.fVolumedB = 0;
    opt.bPartlyConnect = true;
    opt.bReconnect = true;
    opt.nLivePort = PORT_MIC;
    opt.bReconnectRequest = false;
    opt.bVA = true;

    CLivePage live;
    m_callID = CLogic::GetInstance()->GetCallID();
    live.StartCall(m_callID, opt);

    return 0;
}

int CQmlPage::vaLiveStopCall(int id)
{
    CLivePage live;
    live.StopCall(id);
    return 0;
}

int CQmlPage::live()
{
    m_callID = CLogic::GetInstance()->GetCallID();
    return CLogic::GetInstance()->Live(m_callID);
}

int CQmlPage::evacuation()
{
    m_callID = CLogic::GetInstance()->GetCallID();
    return CLogic::GetInstance()->Evacuation(m_callID);
}

int CQmlPage::alert()
{
    m_callID = CLogic::GetInstance()->GetCallID();
    return CLogic::GetInstance()->Alert(m_callID);
}

bool CQmlPage::isEVAExist()
{
    return CLogic::GetInstance()->IsEVAExist();
}

bool CQmlPage::isAlertExist()
{
    return CLogic::GetInstance()->IsAlertExist();
}

int  CQmlPage::GetEVAErrorCode()
{
    return CLogic::GetInstance()->GetEVAErrorCode();
}

int  CQmlPage::GetAlertErrorCode()
{
    return CLogic::GetInstance()->GetAlertErrorCode();
}

int CQmlPage::getCallId()
{
    return m_callID;
}

int CQmlPage::setScheduleParam(int hour, int min, QString name,
        bool onlyOnce, int cycle, int gapTime)
{
    m_param.startDateTime.Hour = hour;
    m_param.startDateTime.Min = min;
    m_param.strName = name.toStdString();
    m_param.bOnlyOnce = onlyOnce;
    m_param.nLoopCount = cycle;
    m_param.nLoopInterval = gapTime;

    for (int i = 0; i < 7; i++)
    {
        m_param.szDay[i] = m_day[i];
    }

    return 0;
}

int CQmlPage::setMday(int day, bool value)
{
    if (day > 6 || day < 0)
    {
        return -1;
    }

    m_day[day] = value;

    return 0;
}

int CQmlPage::clearscheduleParam()
{
    m_param.startDateTime.Hour = 0;
    m_param.startDateTime.Min = 0;
    m_param.strName = "";
    m_param.bOnlyOnce = false;
    m_param.nLoopCount = 0;
    m_param.nLoopInterval = 0;

    for (int i = 0; i < 7; i++)
    {
        m_param.szDay[i] = false;
    }

    return 0;
}

int CQmlPage::clearMday()
{
    for (int i = 0; i < 7; i++)
    {
        m_day[i] = false;
    }

    return 0;
}

int CQmlPage::getLocalIntOption(QString SrcType, QString optType)
{
    t_LocalOption opt;

    if (SrcType == "USB")
    {
        opt.bIsRecord = false;

        CUSBPage local;
        local.GetOption(opt);
    }
    else if (SrcType == "Record")
    {
        opt.bIsRecord = true;
        CLocalPage local;
        local.GetOption(opt);
    }
    else if (SrcType == "Local")
    {
        CLocalPage local;
        opt.bIsRecord = false;
        local.GetOption(opt);
    }
    else
    {
        return 0;
    }

    if (optType == "Cycle")
    {
        return opt.nRepeatCount;
    }
    else if (optType == "Gap Time")
    {
        return opt.nRepeatInterval;
    }
    else if (optType == "Priority")
    {
        return opt.nPriority;
    }
    else if (optType == "Presignal")
    {
        return opt.nPreSignalTitle;
    }

    return 0;

}

bool CQmlPage::getLocalBoolOption(QString SrcType,QString optType)
{
    t_LocalOption opt;
    if (SrcType == "USB")
    {
        opt.bIsRecord = false;

        CUSBPage local;
        local.GetOption(opt);
    }
    else if (SrcType == "Record")
    {
        opt.bIsRecord = true;

        CLocalPage local;
        local.GetOption(opt);
    }
    else if (SrcType == "Local")
    {
        opt.bIsRecord = false;

        CLocalPage local;
        local.GetOption(opt);
    }
    else
    {
        return false;
    }

    if (optType == "Regain")
    {
        return opt.bReconnect;
    }
    else if (optType == "Partial")
    {
        return opt.bPartlyConnect;
    }

    return 0;
}

float CQmlPage::getLocalFloatOption(QString SrcType, QString optType)
{
    t_LocalOption opt;

    if (SrcType == "USB")
    {
        CUSBPage local;
        opt.bIsRecord = false;
        local.GetOption(opt);
    }
    else if (SrcType == "Record")
    {
        CLocalPage local;
        opt.bIsRecord = true;
        local.GetOption(opt);
    }
    else if (SrcType == "Local")
    {
        CLocalPage local;
        opt.bIsRecord = false;
        local.GetOption(opt);
    }
    else
    {
        return 0;
    }

    if (optType == "Volume")
    {
        return opt.fVolumedB;
    }

    return 0;
}

int CQmlPage::setLocalOption(QString SrcType, int iPri, float iVolume, int iSingal,
        bool bReconnect, bool bPartly, int repeat, int interval)
{
    t_LocalOption opt;
    opt.nPreSignalTitle = iSingal;
    opt.nPriority = iPri;
    opt.bPartlyConnect = bPartly;
    opt.bReconnect = bReconnect;
    opt.fVolumedB = iVolume;
    opt.nRepeatCount = repeat;
    opt.nRepeatInterval = interval;

    if (SrcType == "USB")
    {
        opt.bIsRecord = false;
        CUSBPage local;
        local.SetOption(opt);
    }
    else if (SrcType == "Record")
    {
        opt.bIsRecord = true;
        CLocalPage local;
        local.SetOption(opt);
    }
    else if(SrcType == "Local")
    {
        opt.bIsRecord = false;
        CLocalPage local;
        local.SetOption(opt);
    }
    return 0;
}

int CQmlPage::setLocalVolume(QString SrcType, float iVolume)
{
    t_LocalOption opt;

    if (SrcType == "USB")
    {
        CUSBPage usblocal;
        opt.bIsRecord = false;
        usblocal.GetOption(opt);

        opt.fVolumedB = iVolume;
        usblocal.SetOption(opt);
    }
    else if (SrcType == "Record")
    {
        CLocalPage local;
        opt.bIsRecord = true;
        local.GetOption(opt);

        opt.fVolumedB = iVolume;
        local.SetOption(opt);
    }
    else if (SrcType == "Local")
    {
        CLocalPage local;
        opt.bIsRecord = false;
        local.GetOption(opt);

        opt.fVolumedB = iVolume;
        local.SetOption(opt);
    }
    return 0;
}

int CQmlPage::getLiveIntOption(QString optType)
{
    t_LiveOption opt;
    CLivePage live;
    live.GetOption(opt);


    if (optType == "Priority")
    {
        return opt.nPriority;
    }
    else if (optType == "Presignal")
    {
        return opt.nPreSignalTitle;
    }
    else if (optType == "Port")
    {
        return opt.nLivePort;
    }

    return 0;
}

bool CQmlPage::getLiveBoolOption(QString optType)
{
    t_LiveOption opt;

    CLivePage live;

    live.GetOption(opt);

    if (optType == "Regain")
    {
        return opt.bReconnect;
    }
    else if (optType == "Partial")
    {
        return opt.bPartlyConnect;
    }

    return false;
}

float CQmlPage::getLiveFloatOption(QString optType)
{
    t_LiveOption opt;

    CLivePage live;

    live.GetOption(opt);

    if (optType == "Volume")
    {
        return opt.fVolumedB;
    }

    return 0;
}

int CQmlPage::setLiveOption(int iPri, float iVolume, int iSingal,
        bool bReconnect, bool bPartly, int port)
{
    t_LiveOption opt;

    opt.nPriority = iPri;
    opt.fVolumedB = iVolume;
    opt.nPreSignalTitle = iSingal;
    opt.bReconnect = bReconnect;
    opt.bPartlyConnect = bPartly;
    opt.nLivePort = port;

    CLivePage live;
    live.SetOption(opt);

    return 0;
}

int CQmlPage::setLiveVolume(float iVolume)
{
    t_LiveOption opt;
    CLivePage live;

    live.GetOption(opt);
    opt.fVolumedB = iVolume;

    live.SetOption(opt);

    return 0;
}

int CQmlPage::getNetworkIntOption(QString optType)
{
    t_NetworkOption opt;
    CNetworkPage network;

    network.GetOption(opt);

    if (optType == "Cycle")
    {
        return opt.nRepeatCount;
    }
    else if (optType == "Gap Time")
    {
        return opt.nRepeatInterval;
    }
    else if (optType == "Priority")
    {
        return opt.nPriority;
    }
    else if (optType == "Presignal")
    {
        return opt.nPreSignalTitle;
    }

    return 0;
}

bool CQmlPage::getNetworkBoolOption(QString optType)
{
    t_NetworkOption opt;
    CNetworkPage network;

    network.GetOption(opt);

    if (optType == "Regain")
    {
        return opt.bReconnect;
    }
    else if (optType == "Partial")
    {
        return opt.bPartlyConnect;
    }

    return false;
}

float CQmlPage::getNetworkFloatOption(QString optType)
{
    t_NetworkOption opt;
    CNetworkPage network;

    network.GetOption(opt);

    if (optType == "Volume")
    {
        return opt.fVolumedB;
    }

    return 0;
}

int CQmlPage::setNetworkOption(int node, int iPri, float iVolume, int iSingal,
        bool bReconnect, bool bPartly, int repeat, int interval)
{
    t_NetworkOption opt;

    opt.nNode = node;
    opt.nPreSignalTitle = iSingal;
    opt.nPriority = iPri;
    opt.bPartlyConnect = bPartly;
    opt.bReconnect = bReconnect;
    opt.fVolumedB = iVolume;
    opt.nRepeatCount = repeat;
    opt.nRepeatInterval = interval;

    CNetworkPage network;
    network.SetOption(opt);

    return 0;
}

int CQmlPage::setNetworkVolume(float iVolume)
{
    t_NetworkOption opt;
    CNetworkPage network;

    network.GetOption(opt);
    opt.fVolumedB = iVolume;

    network.SetOption(opt);

    return 0;
}

int CQmlPage::printAllOptions()
{
    t_LocalOption localOpt;

    CUSBPage usblocal;
    localOpt.bIsRecord = false;
    usblocal.GetOption(localOpt);

    CLocalPage local;
    localOpt.bIsRecord = true;
    local.GetOption(localOpt);

    localOpt.bIsRecord = false;
    local.GetOption(localOpt);

    t_LiveOption liveOpt;
    CLivePage live;
    live.GetOption(liveOpt);

    t_NetworkOption networkOpt;
    CNetworkPage network;
    network.GetOption(networkOpt);

    return 0;
}
