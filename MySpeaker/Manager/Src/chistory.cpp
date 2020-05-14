#include <QDebug>
#include <QString>
#include <QStringList>
#include <mutex>
#include "chistory.h"
#include "langstr.h"
#include "cqmlsourcemanage.h"
#include "clocalconfig.h"
#include "cqmlusermanage.h"
#include "public.h"

static std::mutex m_mutex;

CHistory* CHistory::pHistory = NULL;

CHistory* CHistory::GetInstance()
{
    if (pHistory == NULL)
    {
        m_mutex.lock();
        if (pHistory == NULL)
        {
            pHistory = new CHistory();
        }
        m_mutex.unlock();
    }
    return pHistory;
}

void CHistory::Release()
{
    if (NULL != pHistory)
    {
        m_mutex.lock();
        if (NULL != pHistory)
        {
            delete pHistory;
            pHistory = NULL;
        }
        m_mutex.unlock();
    }
}

CHistory::CHistory(QObject *parent) : QObject(parent)
{

}

void CHistory::init()
{

}

int CHistory::getHistoryListCount()
{
    std::vector<CPredefine*> vPredefine;

    CLogic::GetInstance()->GetHistoryList()->GetList(vPredefine);

    return vPredefine.size();
}

int CHistory::getHistoryIndex(unsigned int pos)
{
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetHistoryList()->GetList(vPredefine);

    if (pos >= vPredefine.size())
    {
        return -1;
    }

    return vPredefine[pos]->nIndex;
}

bool CHistory::isHistoryValid(unsigned int pos)
{
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetHistoryList()->GetList(vPredefine);
    QString tmpName;
    QString tmpId;

    if (pos >= vPredefine.size())
    {
        return false;
    }

    if (vPredefine[pos]->bAllUser == true)
    {
        return true;
    }


    tmpId = CQmlUserManage::GetInstance()->getLoginUserId();

    QStringList userIdList;
    userIdList = QString::fromStdString(vPredefine[pos]->strOwnerList).split(',');

    for (int j = 0; j < userIdList.size(); ++j)
    {
        if (userIdList.at(j) == tmpId)
        {
            return true;
        }
    }

    return false;
}

QString CHistory::getHistoryStrValue(int index, QString key)
{
    QString value;
    bool ret1;
    unsigned int port;
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetHistoryList()->GetList(vPredefine);

    for (unsigned int i = 0; i < vPredefine.size(); i++)
    {
        if (index == vPredefine[i]->nIndex)
        {
            if (key == "Name")
            {
                if (vPredefine[i]->pConnect->bPreListening == true)
                {
                    value =  PRELISTEN_NAME;
                }
                else
                {
                    value = QString::fromStdString(vPredefine[i]->strName);
                }
            }
            else if (key == "Time")
            {
                QString tmp;
                QString tmp1;

                tmp = QString("%1:%2:%3").arg(
                            vPredefine[i]->pConnect->datetime.Hour, 2, 10, QChar('0')).arg(
                            vPredefine[i]->pConnect->datetime.Min, 2, 10, QChar('0')).arg(
                            vPredefine[i]->pConnect->datetime.Sec, 2, 10, QChar('0'));

                tmp1 = QString("%1.%2.%3").arg(
                            vPredefine[i]->pConnect->datetime.Day, 2, 10, QChar('0')).arg(
                            vPredefine[i]->pConnect->datetime.Month, 2, 10, QChar('0')).arg(
                            vPredefine[i]->pConnect->datetime.Year, 2, 10, QChar('0'));

                value = tmp + "\n" + tmp1;
            }
            else if (key == "Status")
            {
                value =  DIS_STATUS;
            }
            else if (key == "Gap Time")
            {
                value = QString("%1:%2").arg(vPredefine[i]->pConnect->nLoopInterval/60, 2, 10, QChar('0')).arg(vPredefine[i]->pConnect->nLoopInterval%60, 2, 10, QChar('0'));
            }
            else if (key == "Interval")
            {
                value = QString("%1").arg(vPredefine[i]->pConnect->nLoopInterval);
            }
            else if (key == "Pre-signal")
            {
                if (vPredefine[i]->pConnect->nGongChannel == 31)
                {
                    value = QString("%1").arg(vPredefine[i]->pConnect->nPreSignal);
                }
                else if ((vPredefine[i]->pConnect->nGongChannel == 0) && (vPredefine[i]->pConnect->nPreSignal == 0))
                {
                    value = "--";
                }
                else
                {
                    value = QString("Channel%1-Title%2.adp").arg(vPredefine[i]->pConnect->nGongChannel).arg(vPredefine[i]->pConnect->nPreSignal);
                }
            }    
            else if (key == "Volume")
            {
                value = QString("%1").arg(vPredefine[i]->pConnect->fVolumedB) + "dB";
            }
            else if (key == "Source")
            {
                if (vPredefine[i]->pConnect->eType == CONNECT_LOCAL)
                {
                    if (vPredefine[i]->pConnect->vIOMapPath.size() > 0)
                    {
                       bool ret1 = vPredefine[i]->pConnect->bIsUSB;
                       if (ret1 == true)
                       {
                           value = LangStr::GetInstance()->GetLangString("ETCS_USB");
                       }
                       else
                       {
                           value = LangStr::GetInstance()->GetLangString("ETCS_LOCAL");
                       }
                    }

                }
                else if (vPredefine[i]->pConnect->eType == CONNECT_NETWORK)
                {
                    value = CQmlSourceManage::GetInstance()->getNodeName(vPredefine[i]->pConnect->nNode);
                }
                else if (vPredefine[i]->pConnect->eType == CONNECT_LIVE)
                {
                   port = vPredefine[i]->pConnect->nLivePort;
                   if (port == 41)
                   {
                       value = LangStr::GetInstance()->GetLangString("ETCS_MIC");
                   }
                   else if (port == 42)
                   {
                       value = LangStr::GetInstance()->GetLangString("ETCS_LINEIN");
                   }
                }
            }
            else if (key == "SourceList")
            {
                if (vPredefine[i]->pConnect->eType == CONNECT_LIVE)
                {
                    port = vPredefine[i]->pConnect->nLivePort;
                    if (port == 41)
                    {
                        value = LangStr::GetInstance()->GetLangString("ETCS_MIC");
                    }
                    else if (port == 42)
                    {
                        value = LangStr::GetInstance()->GetLangString("ETCS_LINEIN");
                    }
                }
                else
                {
                    int count = getHistorySourceCount(index);

                    for (int k = 0; k < count; k++)
                    {
                        if (k < count - 1)
                        {
                            value += getHistorySource(index, k) + ";";
                        }
                        else if (k == count - 1)
                        {
                            value += getHistorySource(index, k);
                        }
                    }
                }
            }
            else if (key == "Zone")
            {
                QString tmp;

                for  (int k = 0; k < getHistoryZoneCount(index); k++)
                {
                    if (k < getHistoryZoneCount(index) - 1)
                    {
                        tmp += QString::fromStdString(vPredefine[i]->pConnect->vZone[k]->strName) + ";";
                    }
                    else if (k == getHistoryZoneCount(index) - 1)
                    {
                        tmp += QString::fromStdString(vPredefine[i]->pConnect->vZone[k]->strName);
                    }
                }

                value = tmp;
            }
            else if (key == "Description")
            {
                value = QString::fromStdString(vPredefine[i]->strDesc);
            }

            return value;
        }
    }

    return "";
}

int CHistory::getHistoryIntValue(int index, QString key)
{
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetHistoryList()->GetList(vPredefine);

    for (unsigned int i = 0; i < vPredefine.size(); i++)
    {
        if (index == vPredefine[i]->nIndex)
        {
            if (key == "Cycle")
            {
                return vPredefine[i]->pConnect->nLoopCount;
            }
            else if (key == "Current Cycle")
            {
                return vPredefine[i]->pConnect->nLoopNow;
            }
            else if (key == "Priority")
            {
                return vPredefine[i]->pConnect->nPriority;
            }
            else if  (key == "nGongChannel")
            {
                return vPredefine[i]->pConnect->nGongChannel;
            }
            else if  (key == "nPreSignal")
            {
                return vPredefine[i]->pConnect->nPreSignal;
            }
            else if (key == "errorNum")
            {
                return vPredefine[i]->nErrorCode;
            }
        }
    }
    return 0;
}

bool CHistory::getHistoryBoolValue(int index, QString key)
{
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetHistoryList()->GetList(vPredefine);

    for (unsigned int i = 0; i < vPredefine.size(); i++)
    {
        if (index == vPredefine[i]->nIndex)
        {
            if (key == "Reconnect")
            {
                if (IS_RECONNECT(vPredefine[i]->pConnect->Flags) == FLAGS_RECONNECT)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if (key == "Partly Connect")
            {
                if (IS_PARTIAL_CONNECTION(vPredefine[i]->pConnect->Flags) == FLAGS_PARTIAL_CONNECT)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if (key == "isLive")
            {
                return vPredefine[i]->pConnect->eType == CONNECT_LIVE;
            }
            else if (key == "isError")
            {
                return vPredefine[i]->bIsError;
            }
        }
     }

    return false;
}

int CHistory::getHistoryZoneCount(int index)
{
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetHistoryList()->GetList(vPredefine);

    for (unsigned int i = 0; i < vPredefine.size(); i++)
    {
        if (vPredefine[i]->nIndex == index)
        {
            return (vPredefine[i]->pConnect->vZone).size();
        }
    }

    return 0;
}

QString CHistory::getHistoryZone(int index, int pos)
{
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetHistoryList()->GetList(vPredefine);

    for (unsigned int i = 0; i < vPredefine.size(); i++)
    {
        if (vPredefine[i]->nIndex == index)
        {
            return QString::fromStdString(vPredefine[i]->pConnect->vZone[pos]->strName);
        }
    }

    return "";
}

int CHistory::getHistorySourceCount(int index)
{
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetHistoryList()->GetList(vPredefine);

    for (unsigned int i = 0; i < vPredefine.size(); i++)
    {
        if (vPredefine[i]->nIndex == index)
        {
            if (vPredefine[i]->pConnect->eType == CONNECT_LOCAL
                    || vPredefine[i]->pConnect->eType == CONNECT_NETWORK)
            {
                return (vPredefine[i]->pConnect->vIOMapPath).size();
            }
            else if (vPredefine[i]->pConnect->eType == CONNECT_LIVE)
            {
                return 1;
            }
        }
    }

    return 0;
}

QString CHistory::getHistorySource(int index, int pos)
{
    QString fileName;

    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetHistoryList()->GetList(vPredefine);

    for (unsigned int i = 0; i < vPredefine.size(); i++)
    {
        if (vPredefine[i]->nIndex == index)
        {
            if (vPredefine[i]->pConnect->eType == CONNECT_LOCAL)
            {
                if (vPredefine[i]->pConnect->bIsUSB == true)
                {
                    fileName = QString::fromStdString(vPredefine[i]->pConnect->vIOMapPath[pos].GetAudioPath()).section("/", -1);
                    return fileName;
                }
                else
                {
                    fileName = QString::fromStdString(vPredefine[i]->pConnect->vIOMapPath[pos].GetAudioPath()).section("/", -1);
                    if (fileName == "channel31-title1.adp")
                    {
                        return LangStr::GetInstance()->GetLangString("ETCS_SIGNAL_1");
                    }
                    else if (fileName == "channel31-title2.adp")
                    {
                        return LangStr::GetInstance()->GetLangString("ETCS_SIGNAL_2");
                    }
                    else if (fileName == "channel31-title3.adp")
                    {
                        return LangStr::GetInstance()->GetLangString("ETCS_SIGNAL_3");
                    }
                    else
                    {
                        //to do
                        return "";
                        //return CQmlSourceManage::GetInstance()->getLocalDescFromFilename(fileName);
                    }
                }
            }
            else if (vPredefine[i]->pConnect->eType == CONNECT_NETWORK)
            {
                return "channel"+
                        QString::number(vPredefine[i]->pConnect->vIOMapPath[pos].nChannel) +
                        "-" +
                        "title" +
                        QString::number(vPredefine[i]->pConnect->vIOMapPath[pos].nTitle);
            }
            else if (vPredefine[i]->pConnect->eType == CONNECT_LIVE)
            {

            }
        }
    }

    return "";
}

int CHistory::localCall(int index)
{
    t_LocalOption opt;
    opt = getHistoryLocalCallParam(index);

    if(opt.bIsUSB)
    {
        std::vector<std::string> vPath;
        GetHistoryLocalPath(index,vPath);
        return commonUSBCall(opt, vPath);
    }
    else
    {
        std::vector<t_FILEMAP> vPath;
    }
}

int CHistory::networkCall(int index)
{
    t_NetworkOption opt;
    opt = getHistoryNetworkCallParam(index);

    std::vector<t_Audio> vAudio;
    getHistoryNetworkSourcePath(index,vAudio);

    return commonNetworkCall(opt, vAudio);
}

int CHistory::liveCall(int index)
{
    t_LiveOption opt;
    opt = getHistoryLiveCallParam(index);
    return commonLiveCall(opt);
}

int CHistory::commonUSBCall(t_LocalOption &opt, std::vector<std::string> &vPath)
{
#if defined(_HISTORY_) && (_HISTORY_ > 0)
    qDebug() << "common usb call nPreSignal" << opt.nPreSignal;
    qDebug() << "common usb call nPriority" << opt.nPriority;
    qDebug() << "common usb call bPartlyConnect" << opt.bPartlyConnect;
    qDebug() << "common usb call bReconnect" << opt.bReconnect;
    qDebug() << "common usb call fVolumedB" << opt.fVolumedB;
    qDebug() << "common usb call nRepeatCount" << opt.nRepeatCount;
    qDebug() << "common usb call nRepeatInterval" << opt.nRepeatCount;
    qDebug() << "common usb call bGapTime" << opt.bGapTime;
#endif

    CUSBPage local;
    m_callID = CLogic::GetInstance()->GetCallID();
    local.StartCall(m_callID, vPath, opt);

    return 0;
}

int CHistory::commonLocalCall(t_LocalOption &opt, std::vector<t_FILEMAP> &vPath)
{
#if defined(_HISTORY_) && (_HISTORY_ > 0)
    qDebug() << "common local call nPreSignal" << opt.nPreSignal;
    qDebug() << "common local call nPriority" << opt.nPriority;
    qDebug() << "common local call bPartlyConnect" << opt.bPartlyConnect;
    qDebug() << "common local call bReconnect" << opt.bReconnect;
    qDebug() << "common local call fVolumedB" << opt.fVolumedB;
    qDebug() << "common local call nRepeatCount" << opt.nRepeatCount;
    qDebug() << "common local call nRepeatInterval" << opt.nRepeatCount;
    qDebug() << "common local call bGapTime" << opt.bGapTime;
#endif

    CLocalPage local;
    m_callID = CLogic::GetInstance()->GetCallID();
    local.StartCall(m_callID, vPath, opt);

    return 0;
}

int CHistory::commonNetworkCall(t_NetworkOption &opt, std::vector<t_Audio> &vAudio)
{
#if defined(_HISTORY_) && (_HISTORY_ > 0)
    qDebug() << "common network call nPreSignal" << opt.nPreSignal;
    qDebug() << "common network call nPriority" << opt.nPriority;
    qDebug() << "common network call bPartlyConnect" << opt.bPartlyConnect;
    qDebug() << "common network call bReconnect" << opt.bReconnect;
    qDebug() << "common network call fVolumedB" << opt.fVolumedB;
    qDebug() << "common network call nRepeatCount" << opt.nRepeatCount;
    qDebug() << "common network call nRepeatInterval" << opt.nRepeatCount;
    qDebug() << "common network call bGapTime" << opt.bGapTime;
#endif

    CNetworkPage network;
    m_callID = CLogic::GetInstance()->GetCallID();
    network.StartCall(m_callID, vAudio, opt);

    return 0;
}

int CHistory::commonLiveCall(t_LiveOption &opt)
{
#if defined(_HISTORY_) && (_HISTORY_ > 0)
    qDebug() << "common live call nPreSignal" << opt.nPreSignal;
    qDebug() << "common live call nPriority" << opt.nPriority;
    qDebug() << "common live call fVolumedB" << opt.fVolumedB;
    qDebug() << "common live call bPartlyConnect" << opt.bPartlyConnect;
    qDebug() << "common live call bReconnect" << opt.bReconnect;
    qDebug() << "common live call nLivePort" << opt.nLivePort;
#endif

   CLivePage live;
   m_callID = CLogic::GetInstance()->GetCallID();
   live.StartCall(m_callID, opt);

   return 0;
}


int CHistory::commonStopLocalCall(int id)
{
    CLocalPage local;
    local.StopCall(id);
    return 0;
}

int CHistory::commonStopNetworkCall(int id)
{
    CNetworkPage network;
    network.StopCall(id);

    return 0;
}

int CHistory::commonStopLiveCall(int id)
{
    CLivePage live;
    live.StopCall(id);
    return 0;
}

int CHistory::historyCall(int index)
{
    m_callID = CLogic::GetInstance()->GetCallID();
    return CLogic::GetInstance()->GetHistoryList()->StartCall(index, m_callID);
}

void CHistory::GetHistoryLocalPath(int index,std::vector<std::string> &vPath)
{
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetHistoryList()->GetList(vPredefine);

    for (unsigned int i = 0; i < vPredefine.size(); i++)
    {
        if (vPredefine[i]->nIndex == index)
        {
            vPath = vPredefine[i]->pConnect->vLocalPath;
            break;
        }
    }
}

void CHistory::GetHistoryIOMapPath(int index,std::vector<t_FILEMAP> &vPath)
{
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetHistoryList()->GetList(vPredefine);

    for (unsigned int i = 0; i < vPredefine.size(); i++)
    {
        if (vPredefine[i]->nIndex == index)
        {
            vPath = vPredefine[i]->pConnect->vIOMapPath;
            break;
        }
    }
}

void CHistory::getHistoryNetworkSourcePath(int index,std::vector<t_Audio> &vAudio)
{
    std::vector<t_Audio> tmp;
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetHistoryList()->GetList(vPredefine);

    for (unsigned int i = 0; i < vPredefine.size(); i++)
    {
        if (vPredefine[i]->nIndex != index)
        {
            continue;
        }
        std::vector<t_FILEMAP> &vIOMapPath = vPredefine[i]->pConnect->vIOMapPath;
        for(unsigned int k = 0;k<vIOMapPath.size();k++)
        {
            t_Audio audio;
            audio.nChannel = vIOMapPath[k].nChannel;
            audio.nTitle = vIOMapPath[k].nTitle;
            vAudio.push_back(audio);
        }
    }
}

int CHistory::getHistoryLivePort(int index)
{
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetHistoryList()->GetList(vPredefine);

    for (unsigned int i = 0; i < vPredefine.size(); i++)
    {
        if (vPredefine[i]->nIndex == index)
        {
            return vPredefine[i]->pConnect->nLivePort;
        }
    }

    return 0;
}

t_LocalOption CHistory::getHistoryLocalCallParam(int index)
{
    t_LocalOption opt;
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetHistoryList()->GetList(vPredefine);

    for (unsigned int i = 0; i < vPredefine.size(); i++)
    {
        if (vPredefine[i]->nIndex == index)
        {
            opt.nPreSignalTitle = vPredefine[i]->pConnect->nPreSignal;
            opt.nPriority = vPredefine[i]->pConnect->nPriority;
            opt.bPartlyConnect = IS_PARTIAL_CONNECTION(vPredefine[i]->pConnect->Flags);
            opt.bReconnect =  IS_RECONNECT(vPredefine[i]->pConnect->Flags);
            opt.fVolumedB = vPredefine[i]->pConnect->fVolumedB;
            opt.nRepeatCount = vPredefine[i]->pConnect->nLoopCount;
            opt.nRepeatInterval = vPredefine[i]->pConnect->nLoopInterval;
            opt.bGapTime = vPredefine[i]->pConnect->bGapTime;
            opt.bIsUSB = vPredefine[i]->pConnect->bIsUSB;
        }
    }

    return  opt;
}

t_NetworkOption CHistory::getHistoryNetworkCallParam(int index)
{
    t_NetworkOption opt;
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetHistoryList()->GetList(vPredefine);

    for (unsigned int i = 0; i < vPredefine.size(); i++)
    {
        if (vPredefine[i]->nIndex == index)
        {
            opt.nNode = vPredefine[i]->pConnect->nNode;
            opt.nPreSignalTitle = vPredefine[i]->pConnect->nPreSignal;
            opt.nPriority = vPredefine[i]->pConnect->nPriority;
            opt.bPartlyConnect = IS_PARTIAL_CONNECTION(vPredefine[i]->pConnect->Flags);
            opt.bReconnect = IS_RECONNECT(vPredefine[i]->pConnect->Flags);
            opt.fVolumedB = vPredefine[i]->pConnect->fVolumedB;
            opt.nRepeatCount = vPredefine[i]->pConnect->nLoopCount;
            opt.nRepeatInterval = vPredefine[i]->pConnect->nLoopInterval;
            opt.bGapTime = vPredefine[i]->pConnect->bGapTime;
        }
    }

    return  opt;
}

t_LiveOption CHistory::getHistoryLiveCallParam(int index)
{
    t_LiveOption opt;
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetHistoryList()->GetList(vPredefine);

    for (unsigned int i = 0; i < vPredefine.size(); i++)
    {
        if (vPredefine[i]->nIndex == index)
        {
            opt.nPreSignalTitle = vPredefine[i]->pConnect->nPreSignal;
            opt.nPriority = vPredefine[i]->pConnect->nPriority;
            opt.fVolumedB = vPredefine[i]->pConnect->fVolumedB;
            opt.bPartlyConnect = IS_PARTIAL_CONNECTION(vPredefine[i]->pConnect->Flags);
            opt.bReconnect = IS_RECONNECT(vPredefine[i]->pConnect->Flags);
            opt.nLivePort = vPredefine[i]->pConnect->nLivePort;
        }
    }

    return opt;
}

void CHistory::CleanRecent()
{
    CLogic::GetInstance()->CleanRecent();
}
