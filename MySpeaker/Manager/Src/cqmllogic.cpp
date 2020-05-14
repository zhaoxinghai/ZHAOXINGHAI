#include <QDebug>
#include <QModelIndex>
#include <QVariant>
#include <QCoreApplication>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QDesktopWidget>
#include <QApplication>
#include <QRegExp>
#include <QPixmap>
#include <QRect>
#include <QWidget>
#include <QScreen>
#include <QGuiApplication>
#include <QDir>
#include <QList>
#include <QDateTime>

#include <mutex>
#include <string.h>

#ifdef WIN32

#else
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <net/route.h>
#include <netinet/in.h>
#include <errno.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#endif

#include "cqmllogic.h"
#include "cqmlpage.h"
#include "cqmlactivatemodel.h"
#include "cqmlpredefinemodel.h"
#include "cqmlsourcemanage.h"
#include "cqmlusermanage.h"
#include "ListModel.h"
#include "version.h"
#include "connection.h"
#include "cgpiocontrol.h"
#include "public.h"
#include "langstr.h"
#include "clocalconfig.h"
#include "czonemanage.h"
#include "cfontSetting.h"
#include "cimageBrowser.h"
#include "cudisk.h"
#include "chistory.h"
#include "crecord.h"
#include "cschedulemanage.h"
#include "ckeyboard.h"
#include "cqmlfaultmodel.h"

#ifdef win32
#pragma comment(lib, "IPHLPAPI.lib")
#include <iphlpapi.h>
#include <stdio.h>
#endif

static std::mutex m_mutex;

CQmlLogic* CQmlLogic::pQmlLogic = NULL;

CQmlLogic * CQmlLogic::GetInstance()
{
	if (pQmlLogic == NULL)
	{
		m_mutex.lock();
		if (pQmlLogic == NULL)
		{
			pQmlLogic = new CQmlLogic(NULL);
		}
		m_mutex.unlock();
	}
	return pQmlLogic;
}

void CQmlLogic::Release()
{
	if (NULL != pQmlLogic)
	{
		m_mutex.lock();
		if (NULL != pQmlLogic)
		{
			delete pQmlLogic;
			pQmlLogic = NULL;
		}
		m_mutex.unlock();
	}	
}

static void CallBackLib(const CResultBase* pResult)
{
    CResultBase* pNew = CLogic::GetInstance()->CreateResult(pResult);
	CQmlLogic::GetInstance()->emitData(pNew);
}

void CallBackUI(const CResultBase* pResult)
{
    CQmlLogic::GetInstance()->analysisResult(pResult);
}

CQmlLogic::CQmlLogic(QObject *parent) : QObject(parent),
    m_activateModel(new Models::ListModel(new CQmlActivateModel())),
    m_faultModel(new Models::ListModel(new CQmlFaultModel())),
    m_predefineModel(new Models::ListModel(new CQmlPredefineModel()))
{

}

void CQmlLogic::init()
{
    m_realWidth = 480;
    m_realHeight = 800;

    initRealResolution();
    memset(m_hwVersion, 0, sizeof(m_hwVersion));
    initProtocl();
    initVlan();

    clearFaultListModel();
    createFaultListModel();
}

void CQmlLogic::initProtocl()
{
    CLogic::GetInstance()->RegisterCallback(CallBackLib);
    CLogic::GetInstance()->RegisterCallbackUI(CallBackUI);

    connect(this, SIGNAL(transitData(CResultBase*)), this, SLOT(dealData(CResultBase*)));
}

void CQmlLogic::initVlan()
{
    QString vlanValid = CLocalConfig::GetInstance()->readConfig("Config", "VlanValid");
    QString vlanID = CLocalConfig::GetInstance()->readConfig("Config", "VlanID");
    QString vlanPriority = CLocalConfig::GetInstance()->readConfig("Config", "VlanPriority");
    QString vlanIp = CLocalConfig::GetInstance()->readConfig("Config", "VlanIP");
    QString vlanMask = CLocalConfig::GetInstance()->readConfig("Config", "VlanMask");

    if ("true" != vlanValid)
    {
        vlanID = "-1";
    }

    setVlan(vlanID.toInt(), vlanPriority.toInt(), vlanIp, vlanMask);
}



void CQmlLogic::runLogic()
{
    CLogic::GetInstance()->Run();
}

int CQmlLogic::createActivateListModel()
{
    QList<Models::ListItem *> items;
    QList<Models::ListItem *> items1;
    QString name;
    QString zone;

    std::vector<CConnect*> vEmergency;
    std::vector<CConnect*> vConnect;

    CLogic::GetInstance()->GetActivateList()->GetEmergency(vEmergency);
    CLogic::GetInstance()->GetActivateList()->GetActivates(vConnect);

    unsigned int size = vEmergency.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (vEmergency[i]->bPreListening == true)
        {
            continue;
        }

        if (vEmergency[i]->bPreListening == true)
        {
            name = PRELISTEN_NAME;
            zone = PRELISTEN_ZONE;
        }
        else
        {
            name = QString::fromStdString(vEmergency[i]->strName);
            zone = getActivateZones(vEmergency[i]->vZone);
        }

        items.insert(i, new CQmlActivateModel(vEmergency[i]->nID,
                                               getActivateTime(vEmergency[i]->datetime),
                                               name,
                                               getActivateStatus(vEmergency[i]->eConState),
                                               zone,
                                               getEmergency(vEmergency[i]->bEmergency)));

    }

    m_activateModel->appendRows(items);

    size = vConnect.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (vConnect[i]->bPreListening == true)
        {
            continue;
        }

        if (vConnect[i]->bPreListening == true)
        {
            name = PRELISTEN_NAME;
            zone = PRELISTEN_ZONE;
        }
        else
        {
            name = QString::fromStdString(vConnect[i]->strName);
            zone = getActivateZones(vConnect[i]->vZone);
        }
        items1.insert(i, new CQmlActivateModel(vConnect[i]->nID,
                                              getActivateTime(vConnect[i]->datetime),
                                              name,
                                              getActivateStatus(vConnect[i]->eConState),
                                              zone,
                                              getEmergency(vConnect[i]->bEmergency)));

    }   

    m_activateModel->appendRows(items1);
	return 0;
}

int CQmlLogic::clearActivateListModel()
{
    m_activateModel->removeRows(0, m_activateModel->rowCount());

    return 0;
}

int CQmlLogic::getActivateCount()
{
    return m_activateModel->rowCount();
}

int CQmlLogic::updateActivate(CConnect *pConnect)
{
    QVariant id;
    QVariant status;

    int count = m_activateModel->rowCount();
    for (int i = 0; i < count; i++)
    {
       id = m_activateModel->data(m_activateModel->index(i, 0), 1);
       status = m_activateModel->data(m_activateModel->index(i, 0), 4);
       if ((id.toInt() == pConnect->nID) && (status.toString() != getActivateStatus(pConnect->eConState)))
       {
            m_activateModel->setData(m_activateModel->index(i, 0), getActivateStatus(pConnect->eConState) ,4);
            break;
       }
    }

    return 0;
}

int CQmlLogic::DeleteActivate(int nID)
{
    CLogic::GetInstance()->GetActivateList()->DeleteActivate(nID);

    return 0;
}

int CQmlLogic::delActivate(int nID)
{
    QVariant id;
    int count = m_activateModel->rowCount();

    for (int i = 0; i < count; i++)
    {
       id = m_activateModel->data(m_activateModel->index(i, 0), 1);
       if (id.toInt() == nID)
       {
            m_activateModel->removeRow(i);
       }
    }

    return 0;
}

int CQmlLogic::clearAllActivate()
{
    CLogic::GetInstance()->GetActivateList()->DeleteAllActivate();

    return 0;
}

int CQmlLogic::clearAllUsbActivate()
{
    CLogic::GetInstance()->GetActivateList()->DeleteAllUSBActivate();

    return 0;
}

bool CQmlLogic::isActivateImgHide()
{
    QVariant status;
    int size = 0;

    size = getActivateCount();
    if (size == 0)
    {
       return true;
    }

    for (int i = 0; i < size; i++)
    {
        status = m_activateModel->data(m_activateModel->index(i, 0), 4);
        if (status.toString() == FULL_STATUS || status.toString() == PART_STATUS)
        {
            return false;
        }
    }

    return true;
}

Models::ListModel *CQmlLogic::getActivateModel()
{
    return m_activateModel;
}

QString CQmlLogic::getActivateStrValue(int id, QString name)
{
    int ok = 0;
    bool ret;
    int port;
    QString value;

    std::vector<CConnect*> vEmergency;
    std::vector<CConnect*> vConnect;

    CLogic::GetInstance()->GetActivateList()->GetEmergency(vEmergency);
    CLogic::GetInstance()->GetActivateList()->GetActivates(vConnect);

    unsigned int size = 0;

    if (ok == 0)
    {
        size = vConnect.size();
        for (unsigned int i = 0; i < size; i++)
        {
            if (vConnect[i]->nID == id)
            {
                ok = 1;
                if (name == "Name")
                {
                    if (vConnect[i]->bPreListening == true)
                    {
                        value =  PRELISTEN_NAME;
                    }
                    else
                    {
                        value = QString::fromStdString(vConnect[i]->strName);
                    }
                }
                else if (name == "Time")
                {
                    value = getListTime(vConnect[i]->datetime);
                }
                else if (name == "Status")
                {
                    switch (vConnect[i]->eConState)
                    {
                        case CON_INTERRUPTED:
                            value = WAIT_STATUS;
                            break;
                        case CON_FULL_CONNECT:
                            value = FULL_STATUS;
                            break;
                        case CON_PARTLY_CONNECT:
                            value = PART_STATUS;
                            break;
                        case CON_DIS_CONNECT:
                            value = DIS_STATUS;
                            break;
                        default:
                            value = "";
                            break;
                    }
                }
                else if (name == "Gap Time")
                {
                    value = QString("%1:%2").arg(vConnect[i]->nLoopInterval/60, 2, 10, QChar('0')).arg(vConnect[i]->nLoopInterval%60, 2, 10, QChar('0'));
                }
                else if (name == "Pre-signal")
                {
                    if (vConnect[i]->nGongChannel == 31)
                    {
                        value = QString("%1").arg(vConnect[i]->nPreSignal);
                    }
                    else if ((vConnect[i]->nGongChannel == 0) && (vConnect[i]->nPreSignal == 0))
                    {
                        value = "--";
                    }
                    else
                    {
                        value = QString("Channel%1-Title%2.adp").arg(vConnect[i]->nGongChannel).arg(vConnect[i]->nPreSignal);
                    }
                }
                else if (name == "Volume")
                {
                    value = QString("%1").arg(vConnect[i]->fVolumedB) + "dB";
                }
                else if (name == "SourceType")
                {
                    if (vConnect[i]->eType == CONNECT_LOCAL)
                    {
                        if (vConnect[i]->vIOMapPath.size() > 0)
                        {
                           if (vConnect[i]->bIsUSB)
                           {
                               value = LangStr::GetInstance()->GetLangString("ETCS_USB");
                           }
                           else
                           {
                               value = LangStr::GetInstance()->GetLangString("ETCS_LOCAL");
                           }
                        }

                    }
                    else if (vConnect[i]->eType == CONNECT_NETWORK)
                    {
                        value = CQmlSourceManage::GetInstance()->getNodeName(vConnect[i]->nNode);
                    }
                    else if (vConnect[i]->eType == CONNECT_LIVE)
                    {
                       port = vConnect[i]->nLivePort;
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

                return value;
            }
        }
    }

    if (ok == 0)
    {
        size = vEmergency.size();
        for (unsigned int i = 0; i < size; i++)
        {
            if (vEmergency[i]->nID == id)
            {
                if (vEmergency[i]->nID == id)
                {
                    ok = 1;
                    if (name == "Name")
                    {
                        if (vEmergency[i]->bPreListening == true)
                        {
                            value =  PRELISTEN_NAME;
                        }
                        else
                        {
                            value = QString::fromStdString(vEmergency[i]->strName);
                        }
                    }
                    else if (name == "Time")
                    {
                        value = getListTime(vEmergency[i]->datetime);
                    }
                    else if (name == "Status")
                    {
                        switch (vEmergency[i]->eConState)
                        {
                            case CON_INTERRUPTED:
                                value = WAIT_STATUS;
                                break;
                            case CON_FULL_CONNECT:
                                value = FULL_STATUS;
                                break;
                            case CON_PARTLY_CONNECT:
                                value = PART_STATUS;
                                break;
                            case CON_DIS_CONNECT:
                                value = DIS_STATUS;
                                break;
                            default:
                                value = "";
                                break;
                        }
                    }
                    else if (name == "Gap Time")
                    {
                        value = QString("%1:%2").arg(vEmergency[i]->nLoopInterval/60, 2, 10, QChar('0')).arg(vEmergency[i]->nLoopInterval%60, 2, 10, QChar('0'));
                    }
                    else if (name == "Pre-signal")
                    {
                        if (vEmergency[i]->nGongChannel == 31)
                        {
                            value = QString("%1").arg(vEmergency[i]->nPreSignal);
                        }
                        else if ((vEmergency[i]->nGongChannel == 0) && (vEmergency[i]->nPreSignal == 0))
                        {
                            value = "--";
                        }
                        else
                        {
                            value = QString("Channel%1-Title%2.adp").arg(vEmergency[i]->nGongChannel).arg(vEmergency[i]->nPreSignal);
                        }
                    }
                    else if (name == "Volume")
                    {
                        value = QString("%1").arg(vEmergency[i]->fVolumedB) + "dB";
                    }
                    else if (name == "SourceType")
                    {
                        if (vEmergency[i]->eType == CONNECT_LOCAL)
                        {
                            if (vEmergency[i]->vIOMapPath.size() > 0)
                            {
                               if (vEmergency[i]->bIsUSB)
                               {
                                   value = LangStr::GetInstance()->GetLangString("ETCS_USB");
                               }
                               else
                               {
                                   value = LangStr::GetInstance()->GetLangString("ETCS_LOCAL");
                               }
                            }

                        }
                        else if (vEmergency[i]->eType == CONNECT_NETWORK)
                        {
                            value = CQmlSourceManage::GetInstance()->getNodeName(vEmergency[i]->nNode);
                        }
                        else if (vEmergency[i]->eType == CONNECT_LIVE)
                        {
                           port = vEmergency[i]->nLivePort;
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
                    return value;
            }
        }
    }
    }

    return "";
}

bool CQmlLogic::getActivateBoolValue(int id, QString name)
{
    int ok = 0;
    std::vector<CConnect*> vEmergency;
    std::vector<CConnect*> vConnect;

    CLogic::GetInstance()->GetActivateList()->GetEmergency(vEmergency);
    CLogic::GetInstance()->GetActivateList()->GetActivates(vConnect);

    unsigned int size = 0;

    if (ok == 0)
    {
        size = vConnect.size();
        for (unsigned int i = 0; i < vConnect.size(); i++)
        {
            if (vConnect[i]->nID == id)
            {
                ok = 1;
                if (name == "prelisten")
                {
                    return vConnect[i]->bPreListening;
                }
                else if (name == "isLive")
                {
                    return vConnect[i]->eType == CONNECT_LIVE;
                }
                else if(name == "haveGong")
                {
                    if ((vConnect[i]->nGongChannel == 0) && (vConnect[i]->nPreSignal == 0))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
                else if (name == "Reconnect")
                {
                    if (IS_RECONNECT(vConnect[i]->Flags) == FLAGS_RECONNECT)
                    {
                        return true;
                        //value = LangStr::GetInstance()->GetLangString("ETCS_YES");
                    }
                    else
                    {
                        return false;
                        //value = LangStr::GetInstance()->GetLangString("ETCS_NO");
                    }
                }
                else if (name == "Partly Connect")
                {
                    if (IS_PARTIAL_CONNECTION(vConnect[i]->Flags) == FLAGS_PARTIAL_CONNECT)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
    }

    if (ok == 0)
    {
        size = vEmergency.size();
        for (unsigned int i = 0; i < size; i++)
        {
            if (vEmergency[i]->nID == id)
            {
                ok = 1;
                if (name == "prelisten")
                {
                    return vEmergency[i]->bPreListening;
                }
                else if (name == "isLive")
                {
                    return vEmergency[i]->eType == CONNECT_LIVE;
                }
                else if(name == "haveGong")
                {
                    if ((vEmergency[i]->nGongChannel == 0) && (vEmergency[i]->nPreSignal == 0))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
                else if (name == "Reconnect")
                {
                    if (IS_RECONNECT(vEmergency[i]->Flags) == FLAGS_RECONNECT)
                    {
                        return true;
                        //value = LangStr::GetInstance()->GetLangString("ETCS_YES");
                    }
                    else
                    {
                        return false;
                        //value = LangStr::GetInstance()->GetLangString("ETCS_NO");
                    }
                }
                else if (name == "Partly Connect")
                {
                    if (IS_PARTIAL_CONNECTION(vEmergency[i]->Flags) == FLAGS_PARTIAL_CONNECT)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

int CQmlLogic::getActivateIntValue(int id, QString name)
{
    int ok = 0;
    std::vector<CConnect*> vEmergency;
    std::vector<CConnect*> vConnect;

    CLogic::GetInstance()->GetActivateList()->GetEmergency(vEmergency);
    CLogic::GetInstance()->GetActivateList()->GetActivates(vConnect);

    unsigned int size = 0;

    if (ok == 0)
    {
        size = vConnect.size();
        for (unsigned int i = 0; i < vConnect.size(); i++)
        {
            if (vConnect[i]->nID == id)
            {
                ok = 1;
                if (name == "Cycle")
                {
                    return vConnect[i]->nLoopCount;
                }
                else if (name == "Current Cycle")
                {
                    return vConnect[i]->nLoopNow;
                }
                else if (name == "Priority")
                {
                    return vConnect[i]->nPriority;
                }
            }
        }
    }

    if (ok == 0)
    {
        size = vEmergency.size();
        for (unsigned int i = 0; i < size; i++)
        {
            if (vEmergency[i]->nID == id)
            {
                ok = 1;
                if (vEmergency[i]->nID == id)
                {
                    ok = 1;
                    if (name == "Cycle")
                    {
                        return vEmergency[i]->nLoopCount;
                    }
                    else if (name == "Current Cycle")
                    {
                        return vEmergency[i]->nLoopNow;
                    }
                    else if (name == "Priority")
                    {
                        return vEmergency[i]->nPriority;
                    }
                }
            }
        }
    }
    return true;
}

int CQmlLogic::getActivateZoneCount(int id)
{
    std::vector<CConnect*> vEmergency;
    std::vector<CConnect*> vConnect;

    CLogic::GetInstance()->GetActivateList()->GetEmergency(vEmergency);
    CLogic::GetInstance()->GetActivateList()->GetActivates(vConnect);

    unsigned int size;

    size = vEmergency.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (vEmergency[i]->nID == id)
        {
            return (vEmergency[i]->vZone).size();
        }
    }

    size = vConnect.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (vConnect[i]->nID == id)
        {
            return (vConnect[i]->vZone).size();
        }
    }

    return 0;
}

QString CQmlLogic::getActivateZone(int id, int index)
{
    std::vector<CConnect*> vEmergency;
    std::vector<CConnect*> vConnect;

    CLogic::GetInstance()->GetActivateList()->GetEmergency(vEmergency);
    CLogic::GetInstance()->GetActivateList()->GetActivates(vConnect);

    unsigned int size;

    size = vEmergency.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (vEmergency[i]->nID == id)
        {
            return QString::fromStdString(vEmergency[i]->vZone[index]->strName);
        }
    }

    size = vConnect.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (vConnect[i]->nID == id)
        {
            return QString::fromStdString(vConnect[i]->vZone[index]->strName);
        }
    }

    return "";
}

int CQmlLogic::getActivateSourceCount(int id)
{
    std::vector<CConnect*> vEmergency;
    std::vector<CConnect*> vConnect;

    CLogic::GetInstance()->GetActivateList()->GetEmergency(vEmergency);
    CLogic::GetInstance()->GetActivateList()->GetActivates(vConnect);

    unsigned int size;

    size = vEmergency.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (vEmergency[i]->nID== id)
        {
            if (vEmergency[i]->eType == CONNECT_LOCAL
                    || vEmergency[i]->eType == CONNECT_NETWORK)
            {
                return (vEmergency[i]->vIOMapPath).size();
            }
            else if (vEmergency[i]->eType == CONNECT_LIVE)
            {
                return 1;
            }
        }
    }

    size = vConnect.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (vConnect[i]->nID== id)
        {
            if (vConnect[i]->eType == CONNECT_LOCAL
                    || vConnect[i]->eType == CONNECT_NETWORK)
            {
                return (vConnect[i]->vIOMapPath).size();
            }
            else if (vConnect[i]->eType == CONNECT_LIVE)
            {
                return 1;
            }
        }
    }

    return 0;
}

QString CQmlLogic::getActivateSource(int id, int index)
{
    QString fileName;
    std::vector<CConnect*> vEmergency;
    std::vector<CConnect*> vConnect;

    CLogic::GetInstance()->GetActivateList()->GetEmergency(vEmergency);
    CLogic::GetInstance()->GetActivateList()->GetActivates(vConnect);

    unsigned int size;

    size = vEmergency.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (vEmergency[i]->nID == id)
        {
            if (vEmergency[i]->eType == CONNECT_LOCAL)
            {
                if (vEmergency[i]->bIsUSB == true)
                {
                    fileName = getFileNameFromPath(QString::fromStdString(vEmergency[i]->vIOMapPath[index].GetAudioPath()));
                    return fileName;
                }
                else
                {
                    fileName = getFileNameFromPath(QString::fromStdString(vEmergency[i]->vIOMapPath[index].GetAudioPath()));
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
            else if (vEmergency[i]->eType == CONNECT_NETWORK)
            {
                return "channel"+
                        QString::number(vEmergency[i]->vIOMapPath[index].nChannel) +
                        "-" +
                        "title" +
                        QString::number(vEmergency[i]->vIOMapPath[index].nTitle);
            }
            else if (vEmergency[i]->eType == CONNECT_LIVE)
            {

            }
        }
    }

    size = vConnect.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (vConnect[i]->nID == id)
        {
            if (vConnect[i]->eType == CONNECT_LOCAL)
            {
                if (vConnect[i]->bIsUSB == true)
                {
                    fileName = getFileNameFromPath(QString::fromStdString(vConnect[i]->vIOMapPath[index].GetAudioPath()));
                    return fileName;
                }
                else
                {
                    fileName = getFileNameFromPath(QString::fromStdString(vConnect[i]->vIOMapPath[index].GetAudioPath()));
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
            else if (vConnect[i]->eType == CONNECT_NETWORK)
            {
                return "channel"+
                        QString::number(vConnect[i]->vIOMapPath[index].nChannel) +
                        "-" +
                        "title" +
                        QString::number(vConnect[i]->vIOMapPath[index].nTitle);
            }
            else if (vConnect[i]->eType == CONNECT_LIVE)
            {

            }
        }
    }

    return "";
}

int CQmlLogic::clearFaultListModel()
{
    m_faultModel->removeRows(0, m_faultModel->rowCount());

    return 0;
}

int CQmlLogic::createFaultListModel()
{
    std::vector<CSysFault*> vFault;

    CLogic::GetInstance()->GetSysFaultList()->GetList(vFault);

    for (unsigned int i = 0; i < vFault.size(); i++)
    {
        m_faultModel->insertRow(0, new CQmlFaultModel(vFault[i]->nIndex,
                                                     vFault[i]->bLocal,
                                                     getListTime(vFault[i]->datetime),
                                                     QString::fromStdString(vFault[i]->strName),
                                                     GetFaultDesc(vFault[i]),
                                                     GetFaultKey(vFault[i]),
                                                     vFault[i]->index.nNumber,
                                                     vFault[i]->index.nChannel)
                                                     );
    }

	return 0;
}

int CQmlLogic::getFaultListModelCount()
{
    return m_faultModel->rowCount();
}

int CQmlLogic::insertFault(CSysFault*  pFault)
{
    m_faultModel->insertRow(0, new CQmlFaultModel(pFault->nIndex,
                                                     pFault->bLocal,
                                                     getListTime(pFault->datetime),
                                                     QString::fromStdString(pFault->strName),
                                                     GetFaultDesc(pFault),
                                                     GetFaultKey(pFault),
                                                     pFault->index.nNumber,
                                                     pFault->index.nChannel
                                                     ));

    return 0;
}

QString CQmlLogic::GetFaultKey(CSysFault* pFault)
{
    QString error_key = QString::fromStdString(pFault->strErrorName);
    return error_key;
}

QString CQmlLogic::GetFaultDesc(CSysFault* pFault)
{
    QString error_key = QString::fromStdString(pFault->strErrorName);
    return GetFaultDesc(error_key,pFault->index.nNumber,pFault->index.nChannel);
}

QString CQmlLogic::GetFaultDesc(QString key,int number,int channel)
{
    QString str = LangStr::GetInstance()->GetLangString(key);

    if(number != -1 && channel != -1)
    {
        return str.arg(number+1).arg(channel+1);
    }
    else if(number != -1)
    {
        return str.arg(number+1);
    }
    return str;
}

int CQmlLogic::updateFault(CSysFault*  pFault)
{
    QVariant index;

    for (int i = 0; i < m_faultModel->rowCount(); i++)
    {
        QModelIndex model_index = m_faultModel->index(i, 0);
       index = m_faultModel->data(model_index, 1);
       int f_index = pFault->nIndex;
       int m_index = index.toInt();
       if (m_index == f_index)
       {
           QString list_time = getListTime(pFault->datetime);
           m_faultModel->setData(model_index, list_time, CQmlFaultModel::faultEnum::e_time);

           m_faultModel->setData(model_index, GetFaultDesc(pFault), CQmlFaultModel::faultEnum::e_desc);
           m_faultModel->setData(model_index, GetFaultKey(pFault), CQmlFaultModel::faultEnum::e_key);
       }
    }

    return 0;
}

int CQmlLogic::delFault(int index)
{
    QVariant tmpIndex;

    for (int i = 0; i < m_faultModel->rowCount(); i++)
    {
       tmpIndex = m_faultModel->data(m_faultModel->index(i, 0), CQmlFaultModel::faultEnum::e_index);
       if (tmpIndex.toInt() == index)
       {
            m_faultModel->removeRow(i);
       }
    }

    return 0;
}

int CQmlLogic::updateFaultLanguage()
{
    QVariant key;
    QVariant number;
    QVariant channel;
    for (int i = 0; i < m_faultModel->rowCount(); i++)
    {
        key =  m_faultModel->data(m_faultModel->index(i, 0), CQmlFaultModel::faultEnum::e_key);
        number = m_faultModel->data(m_faultModel->index(i, 0), CQmlFaultModel::faultEnum::e_num);
        channel = m_faultModel->data(m_faultModel->index(i, 0), CQmlFaultModel::faultEnum::e_ch);
        m_faultModel->setData(m_faultModel->index(i, 0),
                              GetFaultDesc(key.toString(),number.toInt(),channel.toInt()), CQmlFaultModel::faultEnum::e_desc);
    }

    return 0;
}

Models::ListModel *CQmlLogic::getFaultModel()
{
    return m_faultModel;
}

void CQmlLogic::Refresh_FaultList()
{
    CLogic::GetInstance()->Refresh_FaultList();
}

void CQmlLogic::resetError()
{
    CLogic::GetInstance()->ResetError();
}

int CQmlLogic::clearPredefineListModel()
{
    m_predefineModel->removeRows(0, m_predefineModel->rowCount());

    return 0;
}

int CQmlLogic::createPredefineListModel(QString status, QString str)
{
    QList<Models::ListItem *> items;
    std::vector<CPredefine*> tmp;
    CLogic::GetInstance()->GetPredefineList()->GetList(tmp);

    QString tmpName;
    QString tmpId;
    bool isUserIdOK = false;

    unsigned int size = tmp.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (tmp[i]->bAllUser == true)
        {
           if (tmp[i]->eType == PRE_CALL)
           {
               items.insert(i, new CQmlPredefineModel(tmp[i]->nIndex,
                                                      QString::fromStdString(tmp[i]->strName),
                                                      tmp[i]->bSelect,
                                                      tmp[i]->pConnect->eType,
                                                      tmp[i]->pConnect->fVolumedB,
                                                      tmp[i]->bMyConfig,
                                                      tmp[i]->bIsError,
                                                      tmp[i]->eType,
                                                      "#ffffff",
                                                      "",
                                                      1,
                                                      "#c0c0c0",
                                                      0,
                                                      "",
                                                      false,
                                                      false,
                                                      0,
                                                      0));
           }
           else if (tmp[i]->eType == PRE_VOLUME_CONTROL)
           {
               items.insert(i, new CQmlPredefineModel(tmp[i]->nIndex,
                                                      QString::fromStdString(tmp[i]->strName),
                                                      tmp[i]->bSelect,
                                                      3,
                                                      0,
                                                      tmp[i]->bMyConfig,
                                                      tmp[i]->bIsError,
                                                      tmp[i]->eType,
                                                      "#ffffff",
                                                      "",
                                                      1,
                                                      "#c0c0c0",
                                                      0,
                                                      "",
                                                      false,
                                                      false,
                                                      0,
                                                      0));
           }
        }
        else
        {

            tmpId = CQmlUserManage::GetInstance()->getLoginUserId();

            QStringList userIdList;
            userIdList = QString::fromStdString(tmp[i]->strOwnerList).split(',');

            for (int j = 0; j < userIdList.size(); ++j)
            {
                if (userIdList.at(j) == tmpId)
                {
                    isUserIdOK = true;
                }
            }

            if (isUserIdOK == true)
            {     
                if (tmp[i]->eType == PRE_CALL)
                {
                    items.insert(i, new CQmlPredefineModel(tmp[i]->nIndex,
                                                           QString::fromStdString(tmp[i]->strName),
                                                           tmp[i]->bSelect,
                                                           tmp[i]->pConnect->eType,
                                                           tmp[i]->pConnect->fVolumedB,
                                                           tmp[i]->bMyConfig,
                                                           tmp[i]->bIsError,
                                                           tmp[i]->eType,
                                                           "#ffffff",
                                                           "",
                                                           1,
                                                           "#c0c0c0",
                                                           0,
                                                           "",
                                                           false,
                                                           false,
                                                           0,
                                                           0));
                }
                else if (tmp[i]->eType == PRE_VOLUME_CONTROL)
                {
                    items.insert(i, new CQmlPredefineModel(tmp[i]->nIndex,
                                                           QString::fromStdString(tmp[i]->strName),
                                                           tmp[i]->bSelect,
                                                           3,
                                                           0,
                                                           tmp[i]->bMyConfig,
                                                           tmp[i]->bIsError,
                                                           tmp[i]->eType,
                                                           "#ffffff",
                                                           "",
                                                           1,
                                                           "#c0c0c0",
                                                           0,
                                                           "",
                                                           false,
                                                           false,
                                                           0,
                                                           0));

                }
            }

            isUserIdOK = false;
        }
    }

    m_predefineModel->appendRows(items);

    return 0;
}

int CQmlLogic::addPredefineToListModel(CPredefine* tmp)
{
    if (tmp == NULL)
    {
        return -1;
    }

    m_predefineModel->appendRow(new CQmlPredefineModel(tmp->nIndex,
                                                       QString::fromStdString(tmp->strName),
                                                       tmp->bSelect,
                                                       tmp->pConnect->eType,
                                                       tmp->pConnect->fVolumedB,
                                                       tmp->bMyConfig,
                                                       tmp->bIsError,
                                                       tmp->eType,
                                                       "#ffffff",
                                                       "",
                                                       1,
                                                       "#c0c0c0",
                                                       0,
                                                       "",
                                                       false,
                                                       false,
                                                       0,
                                                       0));

    return 0;
}

int CQmlLogic::getPredefineListModelCount()
{
    return m_predefineModel->rowCount();
}

int CQmlLogic::getPredefineListCount()
{
    std::vector<CPredefine*> tmp;
    CLogic::GetInstance()->GetPredefineList()->GetList(tmp);

    return tmp.size();
}

int CQmlLogic::setPredefineSelected(int index, bool select)
{
     CLogic::GetInstance()->GetPredefineList()->Select(index, select);
	 return 0;
}

int CQmlLogic::predefineUnselectAll()
{
    CLogic::GetInstance()->GetPredefineList()->UnSelectAll();

    return 0;
}

Models::ListModel *CQmlLogic::getPredefineModel()
{
    return m_predefineModel;
}

QString CQmlLogic::getPredefineStrValue(int id, QString name)
{
    bool ret;
    int port;
    QString value;
    unsigned int size;

    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetPredefineList()->GetList(vPredefine);

    size = vPredefine.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (vPredefine[i]->nIndex == id)
        {
            if (name == "Name")
            {
                value = QString::fromStdString(vPredefine[i]->strName);
            }
            else if (name == "Time")
            {
                value = getListTime(vPredefine[i]->pConnect->datetime);
            }
            else if (name == "Status")
            {
                switch (vPredefine[i]->pConnect->eConState)
                {
                    case CON_INTERRUPTED:
                        value = WAIT_STATUS;
                        break;
                    case CON_FULL_CONNECT:
                        value = FULL_STATUS;
                        break;
                    case CON_PARTLY_CONNECT:
                        value = PART_STATUS;
                        break;
                    case CON_DIS_CONNECT:
                        value = DIS_STATUS;
                        break;
                    default:
                        value = "";
                        break;
                }
            }
            else if (name == "Gap Time")
            {
                value = QString("%1:%2").arg(vPredefine[i]->pConnect->nLoopInterval/60, 2, 10, QChar('0')).arg(vPredefine[i]->pConnect->nLoopInterval%60, 2, 10, QChar('0'));
            } 
            else if (name == "Pre-signal")
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
            else if (name == "Volume")
            {
                value = QString("%1").arg(vPredefine[i]->pConnect->fVolumedB) + "dB";
            }
            else if (name == "Source")
            {
                if (vPredefine[i]->pConnect->eType == CONNECT_LOCAL)
                {
                    if (vPredefine[i]->pConnect->vIOMapPath.size() > 0)
                    {
                       if (vPredefine[i]->pConnect->bIsUSB)
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
                    value = QString::number(vPredefine[i]->pConnect->nNode);
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
            else if (name == "Zone")
            {
                value = "";
            }
            else if (name == "Description")
            {
                value = QString::fromStdString(vPredefine[i]->strDesc);
            }
            else if (name == "Type")
            {
                if (vPredefine[i]->pConnect->eType == CONNECT_LOCAL)
                {
                    value = "Local";
                }
                else if (vPredefine[i]->pConnect->eType == CONNECT_NETWORK)
                {
                    value = "Network";
                }
                else if (vPredefine[i]->pConnect->eType == CONNECT_LIVE)
                {
                    value = "Live";
                }
            }
            return value;
        }
    }

    return "";
}

int CQmlLogic::getPredefineIntValue(int id, QString name)
{
    unsigned int size;
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetPredefineList()->GetList(vPredefine);

    size = vPredefine.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (vPredefine[i]->nIndex == id)
        {
            if (name == "Cycle")
            {
                return vPredefine[i]->pConnect->nLoopCount;
            }
            else if (name == "Current Cycle")
            {
                return vPredefine[i]->pConnect->nLoopNow;
            }
            else if (name == "Interval")
            {
                return vPredefine[i]->pConnect->nLoopInterval;
            }
            else if (name == "Priority")
            {
                return vPredefine[i]->pConnect->nPriority;
            }
            else if  (name == "nGongChannel")
            {
                return vPredefine[i]->pConnect->nGongChannel;
            }
            else if  (name == "nPreSignal")
            {
                return vPredefine[i]->pConnect->nPreSignal;
            }
            else if (name == "errorNum")
            {
               return vPredefine[i]->nErrorCode;
            }
        }
    }

    return -1;
}

bool CQmlLogic::getPredefineBoolValue(int id, QString name)
{
    unsigned int size;
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetPredefineList()->GetList(vPredefine);

    size = vPredefine.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (vPredefine[i]->nIndex == id)
        {
            if (name == "Reconnect")
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
            else if (name == "Partly Connect")
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
            else if (name == "isLive")
            {
                return vPredefine[i]->pConnect->eType == CONNECT_LIVE;
            }
            else if (name == "MyConfig")
            {
                return vPredefine[i]->bMyConfig;
            }
            else if (name == "isPhysical")
            {
                return vPredefine[i]->pConnect->IsPhysical();
            }
        }
    }
    return false;
}

float CQmlLogic::getPredefineFloatValue(int id, QString name)
{
    unsigned int size;
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetPredefineList()->GetList(vPredefine);

    size = vPredefine.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (vPredefine[i]->nIndex == id)
        {
            if (name == "Volume")
            {
                return vPredefine[i]->pConnect->fVolumedB;
            }
        }
    }

    return 0;
}

int CQmlLogic::getPredefineZoneCount(int id)
{
    unsigned int size;
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetPredefineList()->GetList(vPredefine);

    size = vPredefine.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (vPredefine[i]->nIndex == id)
        {
            return (vPredefine[i]->pConnect->vZone).size();
        }
    }

    return 0;
}

QString CQmlLogic::getPredefineZone(int id, int index)
{
    unsigned int size;
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetPredefineList()->GetList(vPredefine);

    size = vPredefine.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (vPredefine[i]->nIndex == id)
        {
            return QString::fromStdString(vPredefine[i]->pConnect->vZone[index]->strName);
        }
    }

    return "";
}

int CQmlLogic::getPredefineSourceCount(int id)
{
    unsigned int size;
    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetPredefineList()->GetList(vPredefine);

    size = vPredefine.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (vPredefine[i]->nIndex == id)
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

QString CQmlLogic::getPredefineSource(int id, int index)
{
    unsigned int size;
    QString fileName;

    std::vector<CPredefine*> vPredefine;
    CLogic::GetInstance()->GetPredefineList()->GetList(vPredefine);

    size = vPredefine.size();
    for (unsigned int i = 0; i < size; i++)
    {
        if (vPredefine[i]->nIndex == id)
        {
            t_FILEMAP &file = vPredefine[i]->pConnect->vIOMapPath[index];

            if (vPredefine[i]->pConnect->eType == CONNECT_LOCAL)
            {
                if (vPredefine[i]->pConnect->bIsUSB == true)
                {
                    fileName = getFileNameFromPath(QString::fromStdString(file.GetAudioPath()));
                    return fileName;
                }
                else
                {
                    fileName = getFileNameFromPath(QString::fromStdString(file.GetAudioPath()));
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
                        QString::number(vPredefine[i]->pConnect->vIOMapPath[index].nChannel) +
                        "-" +
                        "title" +
                        QString::number(vPredefine[i]->pConnect->vIOMapPath[index].nTitle);
            }
            else if (vPredefine[i]->pConnect->eType == CONNECT_LIVE)
            {

            }
        }
    }

    return "";
}

int CQmlLogic::delPredefineDeep(int index)
{
    delPredefineLogic(index);
    delPredefineModel(index);
    return 0;
}

int CQmlLogic::delPredefineModel(int nID)
{
    QVariant id;

    int count = m_predefineModel->rowCount();
    for (int i = 0; i < count; i++)
    {
       id = m_predefineModel->data(m_predefineModel->index(i, 0), 1);
       if (id.toInt() == nID)
       {
            m_predefineModel->removeRow(i);
       }
    }

    return 0;

}

int CQmlLogic::delPredefineLogic(int nID)
{
    CLogic::GetInstance()->GetPredefineList()->Del(nID);
    return 0;
}

void CQmlLogic::updatePredefineVolume(int index, bool myConfig, float dB)
{
    CLogic::GetInstance()->GetPredefineList()->UpdateVolume(index, myConfig, dB);

    return;
}

bool CQmlLogic::isOverPredefineMaxCount()
{
    long predefineMaxCount = CLogic::GetInstance()->GetInstance()->ReadConfigInit("limit", "predefine_max_count");

    if (getPredefineListCount() >= predefineMaxCount)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int CQmlLogic::analysisResult(const CResultBase* pResult)
{
	if (pResult == NULL)
	{
		return -1;
	}

	if (pResult->strClassName == "CActivateAdd")
	{
        CActivateAdd *p = (CActivateAdd*)pResult;

        activateAddSignal();
        emit syncDetailInfoSignal(p->pConnect->nID, 0, WAIT_STATUS);
	}
	else if (pResult->strClassName == "CActivateDel")
	{
		CActivateDel *p = (CActivateDel*)pResult;
        delActivate(p->nID);

        if (getActivateCount() <= 0)
        {
            emit noActivateSignal();
        }

        bool ret = isActivateImgHide();
        if (ret == true)
        {
            emit removeImgSignal(getLocalImagePath() + "playing_1.png");
        }

        emit signalCallStatus(p->nID, DIS_STATUS);
        emit syncDetailInfoSignal(p->nID, 0, DIS_STATUS);
        emit activateDeleteSignal(p->nID);
	}

	else if (pResult->strClassName == "CActivateUpdate")
	{
		CActivateUpdate* p = (CActivateUpdate*)pResult;
        updateActivate(p->pConnect);

        switch (p->pConnect->eConState)
        {
            case CON_INTERRUPTED:
                emit syncDetailInfoSignal(p->pConnect->nID, p->pConnect->nLoopNow, WAIT_STATUS);
                emit signalCallStatus(p->pConnect->nID, WAIT_STATUS);
                break;
            case CON_FULL_CONNECT:
                emit syncDetailInfoSignal(p->pConnect->nID, p->pConnect->nLoopNow, FULL_STATUS);
                emit signalCallStatus(p->pConnect->nID, FULL_STATUS);
                emit insertImgSignal(getLocalImagePath() + "playing_1.png");
                break;
            case CON_PARTLY_CONNECT:
                emit syncDetailInfoSignal(p->pConnect->nID, p->pConnect->nLoopNow, PART_STATUS);
                emit signalCallStatus(p->pConnect->nID, PART_STATUS);
                emit insertImgSignal(getLocalImagePath() + "playing_1.png");
                break;
            case CON_DIS_CONNECT:
                emit syncDetailInfoSignal(p->pConnect->nID, p->pConnect->nLoopNow, DIS_STATUS);
                emit signalCallStatus(p->pConnect->nID, DIS_STATUS);
                break;
            default:
                break;
        }
	}
	else if (pResult->strClassName == "CFaultAdd")
    {
        CFaultAdd *p = (CFaultAdd*)pResult;
        QString status_bar_text = GetFaultDesc(p->pFault);
        QString status_bar_key = GetFaultKey(p->pFault);

        insertFault(p->pFault);
        setFaultIndex(p->pFault->nIndex);
        setKeyValue(status_bar_key);

        emit setStatusBarMessageSignal(status_bar_text, status_bar_key);
        emit insertImgSignal(getLocalImagePath() + "error.png");
        emit setBeepOffBtnStatusSignal(true);
	}
	else if (pResult->strClassName == "CFaultUpdate")
	{
		CFaultUpdate *p = (CFaultUpdate*)pResult;
        updateFault(p->pFault);

        setFaultIndex(p->pFault->nIndex);
        setKeyValue(GetFaultKey(p->pFault));
        emit setStatusBarMessageSignal(GetFaultDesc(p->pFault), GetFaultKey(p->pFault));
        if (getFaultListModelCount() == 0)
        {
            emit setBeepOffBtnStatusSignal(false);
        }
    }
    else if (pResult->strClassName == "CFaultDelete")
    {
        CFaultDelete *p = (CFaultDelete*)pResult;
        delFault(p->nIndex);

        if (getFaultListModelCount() == 0)
        {
            emit removeImgSignal(getLocalImagePath() + "error.png");
        }

        if (compareFaultIndex(p->nIndex) == 0)
        {
            emit removeStatusBarMessageSignal(getKeyValue());
        }

        if (getFaultListModelCount() == 0)
        {
            emit setBeepOffBtnStatusSignal(false);
        }
    }
    else if (pResult->strClassName == "CZoneStateAll")
    {
        CZoneManage::GetInstance()->updateZoneModelStatus();
        CZoneManage::GetInstance()->updateZoneModelStatusVA();
        emit updateZoneStatusAll();
    }
    else if (pResult->strClassName == "CFileErrorResult")
    {
        QString dir;
        QString name;
        CFileErrorResult *p = (CFileErrorResult*)pResult;
        CQmlSourceManage::GetInstance()->getSourceName(QString::fromStdString(p->strPath), dir, name);
        CQmlSourceManage::GetInstance()->setSourceStatus(dir, name, 1);
    }
    else if (pResult->strClassName == "CMicResult")
    {
        CMicResult *p = (CMicResult*)pResult;
        if (p->bMicOpen == true)
        {
            emit micActivateSignal(p->chProcess);
        }
        else if(p->bMicOpen == false)
        {
            emit micInactiveSignal(p->chProcess);
        }
    }
    else if (pResult->strClassName == "CDKMResult")
    {
        CDKMResult *p = (CDKMResult*)pResult;
        emit updateDkmNumberSignal(QString::number(p->nCount), QString::number(p->nSettingCount));
    }
    else if (pResult->strClassName == "CPresignalResult")
    {
        CPresignalResult *p = (CPresignalResult*)pResult;
        if (p->bPlaying == true)
        {
            emit gongPlayingTrueSignal(p->chProcess);
        }
        else
        {
            emit gongPlayingFalseSignal(p->chProcess);
        }
    }
    else if (pResult->strClassName == "CGPIO")
    {

    }
    else if (pResult->strClassName == "CDataTimeResult")
    {
        CDataTimeResult *p = (CDataTimeResult*)pResult;
        setSystemTime(p->datatime);
    }
    else if (pResult->strClassName == "CNetCheckResult")
    {

    }
    else if (pResult->strClassName == "CMicCheckResult")
    {

    }
    else if (pResult->strClassName == "CPredefineAdd")
    {
        CPredefineAdd *p = (CPredefineAdd*)pResult;
        addPredefineToListModel(p->p);
        emit addPredefineSignal();
    }
    else if (pResult->strClassName == "CScheduleAdd")
    {
        CScheduleAdd *p = (CScheduleAdd*)pResult;
        emit addScheduleSignal(p->p->nIndex);
    }
    else if (pResult->strClassName == "CScheduleUpdate")
    {
        CScheduleUpdate *p = (CScheduleUpdate*)pResult;
        emit updateScheduleSignal(p->p->nIndex, p->p->bOn);
    }
    else if (pResult->strClassName == "CIOMAPResult")
    {
        CIOMAPResult *p = (CIOMAPResult*)pResult;
        if (p->bChanged == true)
        {
            emit audioDatabaseChangedSignal(p->nNode, QString::fromStdString(p->strIOMapPath));
        }
    }
    else if (pResult->strClassName == "CConnectResult")
    {
        //emit updateConnectResultSignal(); // much CConnectResult watchdog timeout
    }
    else if (pResult->strClassName == "CHistoryAdd")
    {
        CHistoryAdd *p = (CHistoryAdd*)pResult;

        emit addHistorySignal(p->p->nIndex);
    }
    else if (pResult->strClassName == "CHistoryDel")
    {
        CHistoryDel *p = (CHistoryDel*)pResult;
        emit  delHistorySignal(p->index);
    }
    else if (pResult->strClassName == "CSourceIndexResult")
    {
        CSourceIndexResult *p = (CSourceIndexResult*)pResult;
        emit updateSourceSignal(p->chProcess, p->nSourceIndex);
    }
    else if (pResult->strClassName == "CDecodeAudioResult")
    {
        CDecodeAudioResult *p = (CDecodeAudioResult*)pResult;
        if (p->bRet == true)
        {
            CQmlSourceManage::GetInstance()->insertUsbSourceListCacheFile(QString::fromStdString(p->strSrc),
                                                                          QString::fromStdString(p->strCache));
            CQmlSourceManage::GetInstance()->setUsbSourceTran(QString::fromStdString(p->strSrc), true);
        }
    }
    else if (pResult->strClassName == "CHardwareVersion")
    {
        CHardwareVersion *p = (CHardwareVersion*)pResult;
        for (int i = 0; i < 4; i++)
        {
            m_hwVersion[i] = p->szVersion[i];
        }
    }
    else if (pResult->strClassName == "CVolResult")
    {
        CVolResult *p = (CVolResult*)pResult;
        CQmlSourceManage::GetInstance()->setNodeVolumeList(*p);
        emit setNodeVolListSignal();
    }
    else if (pResult->strClassName == "CVolumeAjustResult")
    {
        CVolumeAjustResult *p = (CVolumeAjustResult*)pResult;

        emit updateVolumeResultSignal(p->nNode,
                                 p->dev.type,
                                 p->dev.number,
                                 p->dev.channel,
                                 p->nErrorCode);

    }
    else if (pResult->strClassName == "CLanTestResult")
    {
        CLanTestResult *p = (CLanTestResult*)pResult;
        emit netTestSignal(p->eLan, p->nErrorCode);
    }
    else if (pResult->strClassName == "CSystemResult")
    {
        CSystemResult *p = (CSystemResult*)pResult;
        emit systemResultSignal(p->nID, p->nErrorCode);
    }
    else if (pResult->strClassName == "CLocalRecordResult")
    {
        CLocalRecordResult *p = (CLocalRecordResult*)pResult;
        emit localRecordStatusSignal(p->nErrorCode);
    }
    else if (pResult->strClassName == "COpmodeResult")
    {
        COpmodeResult *p = (COpmodeResult*)pResult;
        if (p->eMode == MODE_CONFIG)
        {
            qDebug() << "ETCS enter config mode";
            CLocalConfig::GetInstance()->setIsConfigMode(true);
        }
    }
    else if (pResult->strClassName == "CLampTestResult")
    {
        CLampTestResult *p = (CLampTestResult*)pResult;
        emit selfTestSignal(p->bActivate);
    }
    else if (pResult->strClassName == "CWriteSetting")
    {
        CWriteSetting *p = (CWriteSetting*)pResult;
        writeSettings(QString::fromStdString(p->strFile),
                      QString::fromStdString(p->strSection),
                      QString::fromStdString(p->strKey),
                      QString::fromStdString(p->strValue));
    }
    else
    {
        ;
    }
	return 0;
}

void CQmlLogic::emitData(CResultBase* pResult)
{
	emit transitData(pResult);
}

void CQmlLogic::dealData(CResultBase* pResult)
{
	CResultBase* p = pResult;
	CLogic::GetInstance()->ProtocolCallback(p);
	CLogic::GetInstance()->ReleaseResult(p);
}

void CQmlLogic::VolumeAjust(int id, float vol)
{
    CLogic::GetInstance()->VolumeAjust(id, vol);
}

void CQmlLogic::writeSettings(QString file, QString section, QString key, QString data)
{
    QSettings settings(file, QSettings::IniFormat);
    settings.beginGroup(section);
    settings.setValue(key, data);
    settings.endGroup();

#ifdef WIN32

#else
    sync();
#endif

    return;
}

QString CQmlLogic::readSettings(QString file, QString section, QString key)
{
    QString tmp;
    QSettings settings(file, QSettings::IniFormat);
    tmp = settings.value(section + "/" + key).toString();
    return tmp;
}

QString CQmlLogic::getFileNameFromPath(QString path)
{
    return path.section("/", -1);
}

bool CQmlLogic::isVlanConfigValid(t_VLANConfig &config)
{
    if (config.nID < 0 || 4095 < config.nID)
    {
        return false;
    }

    if (config.nPriority < 0 || 7 < config.nPriority)
    {
        return false;
    }

    if (!isIPAddressValid(QString(config.strIP.c_str()),
                          QString(config.strSubnetMask.c_str())))
    {
        return false;
    }

    return true;
}

QString CQmlLogic::getProjectName()
{
    return PROJECT_NAME;
}

QString CQmlLogic::getVersion()
{
    return VERSION;
}

QString CQmlLogic::getCompileTime()
{
    return COMPILE_TIME;
}

QString CQmlLogic::getFullFontPath(QString lang)
{
    QString uriFontName = "";

#ifdef WIN32
    QDir userFontDir(getCurrentPath() + FONTS_DIR);
    QDir systemFontDir(getCurrentPath() + FONTS_DIR);
#else
    QDir systemFontDir(SYSTEM_FONTS_PATH);
    QDir userFontDir(QString(SD_CARD_PATH) + FONTS_DIR);
#endif

    QList<QFileInfo> fileInfo = systemFontDir.entryInfoList();
    for (int i = 0; i < fileInfo.count(); i++)
    {
        if (fileInfo.at(i).fileName() == lang)
        {
#ifdef WIN32
            uriFontName = getLocalUriHead() + getCurrentPath() + FONTS_DIR + lang;
#else
            uriFontName = getLocalUriHead() + SYSTEM_FONTS_PATH + lang;
#endif
            return uriFontName;
        }
    }

    QList<QFileInfo> fileInfo1 = userFontDir.entryInfoList();
    for (int i = 0; i < fileInfo1.count(); i++)
    {
        if (fileInfo1.at(i).fileName() == lang)
        {

#ifdef WIN32
            uriFontName = getLocalUriHead() + getCurrentPath() + FONTS_DIR + lang;
#else
            uriFontName = getLocalUriHead() + QString(SD_CARD_PATH) + FONTS_DIR + lang;
#endif

           return uriFontName;
        }
    }

    return uriFontName;
}

QString CQmlLogic::getLocalImagePath()
{
    QString tmpPath;
#ifdef WIN32
    tmpPath = getLocalUriHead() + getCurrentPath() + IMAGES_DIR;
#else
    tmpPath = getLocalUriHead() + QString(SD_CARD_PATH) + IMAGES_DIR;
#endif

    return tmpPath;
}

QString CQmlLogic::getLockImagePath()
{
    QString tmpPath;
#ifdef WIN32
    tmpPath = getLocalUriHead() + getCurrentPath() + CONFIG_DIR;
#else
    tmpPath = getLocalUriHead() + QString(SD_CARD_PATH) + CONFIG_DIR;
#endif

    return tmpPath;
}

int CQmlLogic::initRealResolution()
{
    int titleHeight = 80;
    int minHeight= 690;
    int realScreenHeight = 0;

    QDesktopWidget *desktop =  QApplication::desktop();
    QRect screen = desktop->screenGeometry();

    if ((screen.width() == 800) && (screen.height() == 480))
    {
        setRealResolution(480, 800);
    }
    else
    {
        realScreenHeight = screen.height() - titleHeight;
        if (realScreenHeight >= 800)
        {
            setRealResolution(480, 800);
        }
        else if ((realScreenHeight >= minHeight) && (realScreenHeight < 800))
        {
            setRealResolution(480, realScreenHeight);
        }
        else if (realScreenHeight < minHeight)
        {
            setRealResolution(480, minHeight);
        }
    }

    return 0;
}

int CQmlLogic::setRealResolution(int width, int height)
{
    m_realWidth = width;
    m_realHeight = height;
    return 0;
}

int CQmlLogic::getRealResolution(int &width, int &height)
{
    width = m_realWidth;
    height = m_realHeight;
    return 0;
}

int CQmlLogic::getRealWidth()
{
    return m_realWidth;
}

int CQmlLogic::getRealHeight()
{
    return m_realHeight;
}

int CQmlLogic::setIpAddressAndNetmaskToFile(QString ip, QString netmask)
{
    QString tmpNetmask;
    QString ipAndNetmask;

    int count = 0;
    int value;
    int tmp = 0;

    for (int i = 0; i < 4; i++)
    {
        value = netmask.section('.', i, i).toInt();
        tmp = get1Count(value);
        if (tmp >= 0 && tmp <= 8)
        {
            count += tmp;
        }
    }

    tmpNetmask = QString::number(count);
    ipAndNetmask = ip + "/" + tmpNetmask;
    writeSettings("/etc/systemd/network/Management.network", "Network", "Address", ipAndNetmask);
#ifdef WIN32

#else
    sync();
#endif
    return 0;
}

int CQmlLogic::setGatewayToFile(QString gateway)
{
    writeSettings("/etc/systemd/network/Management.network", "Network", "Gateway", gateway);
#ifdef WIN32

#else
    sync();
#endif

    return 0;
}
int CQmlLogic::syncIpAddressAndNetmask(QString ip, QString netmask)
{
    QString cmd;

    cmd = "ifconfig bond1 " + ip + " " + "netmask " + netmask;
    CLogic::GetInstance()->System(cmd.toStdString().c_str());

    return 0;
}

int CQmlLogic::syncIpAddress(QString ip)
{
#ifdef WIN32
    return 0;
#else

   int fd;
   struct ifreq ifr;
   struct sockaddr_in *sock_in;

   fd = socket(AF_INET, SOCK_DGRAM, 0);
   if(fd < 0)
   {
       printf("ip socket erro\n");
       return -1;
   }

   memset(&ifr,0,sizeof(ifr));
   strncpy(ifr.ifr_name, "bond1", sizeof(ifr.ifr_name) - 1);
   ifr.ifr_name[sizeof(ifr.ifr_name) - 1] = '\0';
   sock_in = (struct sockaddr_in*)&ifr.ifr_addr;
   sock_in->sin_family = AF_INET;

   if(inet_aton(ip.toStdString().c_str(), &(sock_in->sin_addr)) < 0)
   {
       printf("ip inet_aton error\n");
       close(fd);
       return -1;
   }

   if(ioctl(fd, SIOCSIFADDR, &ifr) < 0)
   {
       printf("ip ioctl SIOCSIFADDR error\n");
       close(fd);
       return -1;
   }

   close(fd);
   return 0;

#endif
}

int CQmlLogic::syncNetMask(QString netmask)
{
#ifdef WIN32
   return 0;
#else
   int fd;
   struct ifreq ifr;
   struct sockaddr_in *sock_in;

   fd = socket(AF_INET, SOCK_DGRAM, 0);
   if(fd < 0)
   {
       printf("netmask socket error\n");
       return -1;
   }

   memset(&ifr,0,sizeof(ifr));
   strncpy(ifr.ifr_name, "bond1", sizeof(ifr.ifr_name) - 1);
   ifr.ifr_name[sizeof(ifr.ifr_name) - 1] = '\0';
   sock_in = (struct sockaddr_in*)&ifr.ifr_addr;
   sock_in->sin_family = AF_INET;

   if(inet_aton(netmask.toStdString().c_str(), &(sock_in->sin_addr)) < 0)
   {
       printf("netmask inet_pton error\n");
       close(fd);
       return -1;
   }
   if(ioctl(fd, SIOCSIFNETMASK, &ifr) < 0)
   {
       printf("netmask ioctl error\n");
       close(fd);
       return -1;
   }

   close(fd);
   return 0;
#endif
}

int CQmlLogic::syncGateway(QString gateway)
{
#ifdef WIN32
    return 0;
#else
   int fd;
   struct ifreq ifr;
   struct sockaddr_in *sock_in;
   struct rtentry  rt;

   fd = socket(AF_INET, SOCK_DGRAM, 0);
   if(fd < 0)
   {
       printf("gateway socket error\n");
       return -1;
   }

   memset(&ifr,0,sizeof(ifr));
   strncpy(ifr.ifr_name, "bond1", sizeof(ifr.ifr_name) - 1);
   ifr.ifr_name[sizeof(ifr.ifr_name) - 1] = '\0';
   sock_in = (struct sockaddr_in*)&ifr.ifr_addr;
   sock_in->sin_family = AF_INET;

   memset(&rt, 0, sizeof(struct rtentry));
   memset(sock_in, 0, sizeof(struct sockaddr_in));
   sock_in->sin_family = AF_INET;
   sock_in->sin_port = 0;

   if(inet_aton(gateway.toStdString().c_str(), &sock_in->sin_addr)<0)
   {
      printf("gateway inet_aton error\n");
      close(fd);
      return -1;
   }

   memcpy (&rt.rt_gateway, sock_in, sizeof(struct sockaddr_in));
   ((struct sockaddr_in *)&rt.rt_dst)->sin_family=AF_INET;
   ((struct sockaddr_in *)&rt.rt_genmask)->sin_family=AF_INET;
   rt.rt_flags = RTF_GATEWAY;

   if (ioctl(fd, SIOCADDRT, &rt)<0)
   {
       printf("gateway ioctl error\n");
       close(fd);
       return -1;
   }

   close(fd);
   return 0;
#endif
}

QString CQmlLogic::getRealIp()
{
#ifdef WIN32
    return "";
#else

    int sockfd;
    char ip[16] = {0};
    struct ifreq req;
    struct sockaddr_in *host;

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        printf("socket error!!!\n");
        return "";
    }

    bzero(&req, sizeof(struct ifreq));
    strncpy(req.ifr_name, "bond1", sizeof(req.ifr_name) - 1);
    req.ifr_name[sizeof(req.ifr_name) - 1] = '\0';
    if (ioctl(sockfd, SIOCGIFADDR, &req) < 0)
    {
        close(sockfd);
        return "";
    }

    host = (struct sockaddr_in*)&req.ifr_addr;
    strncpy(ip, inet_ntoa(host->sin_addr), sizeof(ip) - 1);
    ip[sizeof(ip) - 1] = '\0';
    close(sockfd);
    return QString(ip);
#endif
}

QString CQmlLogic::getRealNetmask()
{
#ifdef WIN32
    return "";
#else
    int sockfd;
    char buf[16];
    struct ifreq req;
    struct sockaddr_in *host = NULL;

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        printf("socket error!!!\n");
        return "";
    }

    bzero(&req, sizeof(struct ifreq));
    strncpy(req.ifr_name, "bond1", sizeof(req.ifr_name) - 1);
    req.ifr_name[sizeof(req.ifr_name) - 1] = '\0';
    if (ioctl(sockfd, SIOCGIFNETMASK, &req) < 0)
    {
        close(sockfd);
        return "";
    }

    host = (struct sockaddr_in*)&req.ifr_addr;
    strncpy(buf, inet_ntoa(host->sin_addr), sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    close(sockfd);
    return QString(buf);
#endif
}

QString CQmlLogic::getRealGateway()
{
#ifdef WIN32
    return "";
#else
    FILE *f = NULL;
    char line[100] = {0};
    char *p = NULL;
    char *c = NULL;
    char *g = NULL;
    char *saveptr = NULL;

    f = fopen("/proc/net/route" , "r");
    if (f == NULL)
    {
        printf("fopen /proc/net/route error\n");
        return "";
    }

    while(fgets(line , 100 , f))
    {
        //printf("line = %s\n", line);
        p = strtok_r(line , " \t", &saveptr);
        c = strtok_r(NULL , " \t", &saveptr);
        g = strtok_r(NULL , " \t", &saveptr);

        //printf("p = %s\n", p);
        //printf("c = %s\n", c);
        //printf("g = %s\n", g);

        if(p!=NULL && c!=NULL)
        {
            if(strcmp(c , "00000000") == 0)
            {
                if (g)
                {
                    bool ok;
                    int ng = QString(g).toULong(&ok, 16);
                    struct in_addr addr;
                    addr.s_addr=ng;
                    if (f != NULL)
                    {
                         fclose(f);
                         f = NULL;
                    }
#if 0
                    qDebug() << "gateway === " << QString::fromStdString(std::string(inet_ntoa(addr)));
#endif
                    return QString::fromStdString(std::string(inet_ntoa(addr)));
                }
                break;
            }
        }
    }

    if (f != NULL)
    {
        fclose(f);
        f = NULL;
    }

    return "";
#endif
}

QString CQmlLogic::getRealMAC()
{
#ifdef WIN32
    return "";
#else
    int sockfd;
    struct ifreq ifr;
    char *iface = "bond1";
    unsigned char *mac = NULL;

    memset(&ifr, 0, sizeof(ifr));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        printf("socket error!!!\n");
        return "";
    }

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name , iface , IFNAMSIZ-1);

    char szBuf[100] = {0};
    memset(szBuf, 0, sizeof(szBuf));
    if (0 == ioctl(sockfd, SIOCGIFHWADDR, &ifr))
    {
        mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
        snprintf(szBuf, sizeof(szBuf), "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X",
                 mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }
    close(sockfd);
    QString strMAC = QString(szBuf);
    return strMAC;
#endif
}

bool CQmlLogic::isIPAddressValid(QString ip, QString netmask)
{
    bool ret;
    unsigned int intIp;
    unsigned int intNetmask;
    unsigned int ipHostNumber;
    unsigned int baseHostNumber;
    unsigned int baseData = 0xffffff;

    if (ip == "0.0.0.0" || ip == "255.255.255.255")
    {
        return false;
    }

    intIp = getIpFromString(ip);
    intNetmask = getIpFromString(netmask);
    ipHostNumber = getHostNumberFromIp(intIp, intNetmask);
    baseHostNumber = getHostNumberFromIp(baseData, intNetmask);
    if (ipHostNumber == 0)
    {
        return false;
    }
    if (ipHostNumber == baseHostNumber)
    {
        return false;
    }

    QRegExp rx("^(?:(?:2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(?:2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
    ret = rx.exactMatch(ip);
    return ret;
}

bool CQmlLogic::isSubnetMaskIsValid(QString netmask)
{
    bool ret = false;
    if (netmask == "0.0.0.0" || netmask == "255.255.255.255")
    {
        return false;
    }
    QRegExp rx("^(((255\\.){3}(255|254|252|248|240|224|192|128|0+))|((255\\.){2}(255|254|252|248|240|224|192|128|0+)\\.0)|((255\\.)(255|254|252|248|240|224|192|128|0+)(\\.0+){2})|((255|254|252|248|240|224|192|128|0+)(\\.0+){3}))$");
    ret = rx.exactMatch(netmask);
    return ret;
}

bool CQmlLogic::isGatewayIsValid(QString gateway, QString netmask)
{
    bool ret;
    unsigned int intGateway;
    unsigned int intNetmask;
    unsigned int gatewayHostNumber;
    unsigned int baseHostNumber;
    unsigned int baseData = 0xffffff;

    if (gateway == "0.0.0.0" || gateway == "255.255.255.255")
    {
        return false;
    }

    intGateway = getIpFromString(gateway);
    intNetmask = getIpFromString(netmask);
    gatewayHostNumber = getHostNumberFromIp(intGateway, intNetmask);
    baseHostNumber = getHostNumberFromIp(baseData, intNetmask);
    if (gatewayHostNumber == 0)
    {
        return false;
    }
    if (gatewayHostNumber == baseHostNumber)
    {
        return false;
    }

    QRegExp rx("^(?:(?:2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(?:2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
    ret = rx.exactMatch(gateway);
    return ret;
}

int CQmlLogic::deleteGateway(QString gateway)
{
    QString cmd;

    cmd = "route del default gw " + gateway;
    CLogic::GetInstance()->System(cmd.toStdString().c_str());

    return 0;
}
int CQmlLogic::deleteAllDefaultGateway()
{
    QString cmd;

    cmd = "route del default";
    CLogic::GetInstance()->System(cmd.toStdString().c_str());

    return 0;
}

int CQmlLogic::get1Count(int value)
{
    if (value == 255)
    {
        return 8;
    }
    if (value == 254)
    {
        return 7;
    }
    else if (value == 252)
    {
        return 6;
    }
    else if (value == 248)
    {
        return 5;
    }
    else if (value == 240)
    {
        return 4;
    }
    else if (value == 224)
    {
        return 3;
    }
    else if (value == 192)
    {
        return 2;
    }
    else if (value == 128)
    {
        return 1;
    }
    else if (value == 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

unsigned int CQmlLogic::getIpFromString(QString ip)
{
#ifdef WIN32
    return 0;
#else
    in_addr addr;
    unsigned int int_ip = 0;
    if(inet_pton(AF_INET, ip.toStdString().c_str(), &addr))
    {
        int_ip = ntohl(addr.s_addr);
    }
    return int_ip;
#endif
}

unsigned int CQmlLogic::getNetNumberFromIp(unsigned int ip, unsigned int netmask)
{
    return ip & netmask;
}

unsigned int CQmlLogic::getHostNumberFromIp(unsigned int ip, unsigned int netmask)
{
    return ip & (~netmask);
}

void CQmlLogic::gpioTest(bool value)
{
    CLogic::GetInstance()->GPIOTest(value);
}

int CQmlLogic::setSystemTime(t_DateTime &dateTime)
{
#ifdef WIN32
    (void)dateTime;
    return 0;
#else
    struct tm time_tm;
    struct timeval time_tv;
    time_t rawtime;
    time_t timep;
    int ret = 0;

    time(&rawtime);
    localtime_r(&rawtime, &time_tm);
    time_tm.tm_year = dateTime.Year - 1900;
    time_tm.tm_mon = dateTime.Month - 1;
    time_tm.tm_mday = dateTime.Day;
    time_tm.tm_hour = dateTime.Hour;
    time_tm.tm_min = dateTime.Min;
    time_tm.tm_sec = dateTime.Sec;

    if(time_tm.tm_year > 136 || time_tm.tm_year < 118)
    {
        qDebug() << "date time is invalid!";
        return -1;
    }

    timep = mktime(&time_tm);
    time_tv.tv_sec = timep;
    time_tv.tv_usec = 0;
    ret = settimeofday(&time_tv, NULL);
    if(ret != 0)
    {
        fprintf(stderr, "settimeofday failed\n");
        return -1;
    }

#if 1
    int fd;
    fd = open("/dev/rtc0", O_WRONLY);
    if (fd < 0)
    {
        fprintf(stderr, "open /dev/rtc0 failed\n");
        return -1;
    }

    ret = ioctl(fd, RTC_SET_TIME, &time_tm);
    if (ret < 0)
    {
        fprintf(stderr, "ioctl RTC_SET_TIME failed\n");
        return -1;
    }

    close(fd);
#else
    CLogic::GetInstance()->System("hwclock -w");
#endif

    return 0;

#endif
}

int CQmlLogic::setSystemTime(int year, int month, int day,
                  int hour, int min)
{
#ifdef WIN32
    (void)year;
    (void)month;
    (void)day;
    (void)hour;
    (void)min;
    return 0;
#else

    struct tm time_tm;
    struct timeval time_tv;
    time_t timep;
    time_t rawtime;
    int ret = 0;

    time(&rawtime);
    localtime_r(&rawtime, &time_tm);
    time_tm.tm_year = year - 1900;
    time_tm.tm_mon = month - 1;
    time_tm.tm_mday = day;
    time_tm.tm_hour = hour;
    time_tm.tm_min = min;

    timep = mktime(&time_tm);
    time_tv.tv_sec = timep;
    time_tv.tv_usec = 0;
    ret = settimeofday(&time_tv, NULL);
    if(ret != 0)
    {
        fprintf(stderr, "errno: %s\n", strerror(errno));
        fprintf(stderr, "settimeofday failed %d\n", ret);
        return -1;
    }

#if 1
    int fd;
    fd = open("/dev/rtc0", O_WRONLY);
    if (fd < 0)
    {
        fprintf(stderr, "open /dev/rtc0 failed\n");
        return -1;
    }

    ret = ioctl(fd, RTC_SET_TIME, &time_tm);
    if (ret < 0)
    {
        fprintf(stderr, "ioctl RTC_SET_TIME failed\n");
        return -1;
    }

    close(fd);
#else
    CLogic::GetInstance()->System("hwclock -w");
#endif

    UpdateTime();
    return 0;

#endif
}

QString CQmlLogic::getHourAndMinFromSystemTime()
{
#ifdef WIN32
    return "10:45";
#else
    time_t timep;
    struct tm cur_time;
    time(&timep);
    localtime_r(&timep, &cur_time);

    return QString("%1:%2").arg(cur_time.tm_hour, 2, 10, QChar('0')).arg(cur_time.tm_min, 2, 10, QChar('0'));
#endif
}

QString CQmlLogic::getYearAndMonthAndDayFromSystemTime()
{
#ifdef WIN32
    return "18.09.2018";
#else

    QString region;

    time_t timep;
    struct tm cur_time;
    time(&timep);
    localtime_r(&timep, &cur_time);

    return QString("%1.%2.%3").arg(cur_time.tm_mday, 2, 10, QChar('0')).arg(1 + cur_time.tm_mon, 2, 10, QChar('0')).arg(1900 + cur_time.tm_year, 2, 10, QChar('0'));
    /*
    region = CLocalConfig::GetInstance()->readConfig("LocalConfig", "Region");
    if (region == "ETCS_GERMANY")
    {
        return QString("%1.%2.%3").arg(p->tm_mday, 2, 10, QChar('0')).arg(1 + p->tm_mon, 2, 10, QChar('0')).arg(1900 + p->tm_year, 2, 10, QChar('0'));
    }
    else if (region == "ETCS_CHINA")
    {
        return QString::number(1900 + p->tm_year) + "/" +  QString::number(1 + p->tm_mon) + "/" + QString::number(p->tm_mday);
    }
    else
    {
        return QString("%1.%2.%3").arg(p->tm_mday, 2, 10, QChar('0')).arg(1 + p->tm_mon, 2, 10, QChar('0')).arg(1900 + p->tm_year, 2, 10, QChar('0'));
    }
    */

#endif
}

QString CQmlLogic::getDateAndTime(int year, int month, int day, int hour, int min)
{
    return QString("%1/%2/%3 %4:%5").arg(month, 1, 10, QChar('0')).
                                    arg(day, 1, 10, QChar('0')).
                                    arg(year, 1, 10, QChar('0')).
                                     arg(hour, 2, 10, QChar('0')).
                                      arg(min, 2, 10, QChar('0'));
}


int CQmlLogic::getValueFromSystemTime(QString type)
{
#ifdef WIN32
    return 0;
#else

    time_t timep;
    struct tm cur_time;
    time(&timep);
    localtime_r(&timep, &cur_time);

    if (type == "Year")
    {
        return 1900 + cur_time.tm_year;
    }
    else if (type == "Month")
    {
        return 1 + cur_time.tm_mon;
    }
    else if (type == "Day")
    {
        return cur_time.tm_mday;
    }
    else if (type == "Hour")
    {
        return cur_time.tm_hour;
    }
    else if (type == "Min")
    {
        return cur_time.tm_min;
    }
    else
    {
        return 0;
    }
#endif
}

void CQmlLogic::screenshot()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap originalPixmap = screen->grabWindow(0);
    originalPixmap.save("123", "jpg");
}

void CQmlLogic::UpdateTime()
{
#ifdef WIN32
    return;
#else
    t_DateTime temp_date;
    time_t timep;
    struct tm current_time;
    time(&timep);
    localtime_r(&timep, &current_time);

    temp_date.Year = 1900 + current_time.tm_year;
    temp_date.Month = 1 + current_time.tm_mon;
    temp_date.Day = current_time.tm_mday;
    temp_date.Hour = current_time.tm_hour;
    temp_date.Min = current_time.tm_min;
    temp_date.Sec = current_time.tm_sec;
    CLogic::GetInstance()->UpdateTime();

    return;
#endif
}

void CQmlLogic::Sleep()
{
#ifdef WIN32
    return;
#else
    usleep(2000*1000);
    return;
#endif
}

int CQmlLogic::isIpAndGatewaySameNetNumber(QString ip, QString netmask, QString gateway)
{
    unsigned int intIp;
    unsigned int intGateway;
    unsigned int intNetmask;
    unsigned int ipNetNumber1;
    unsigned int ipNetNumber2;

    intIp = getIpFromString(ip);
    intNetmask = getIpFromString(netmask);
    intGateway = getIpFromString(gateway);
    ipNetNumber1 = getNetNumberFromIp(intIp, intNetmask);
    ipNetNumber2 = getNetNumberFromIp(intGateway, intNetmask);
#if _LOGIC_
    qDebug() << "ip ========== " << ip;
    qDebug() << "netmask ========== " << netmask;
    qDebug() << "gateway ========== " << gateway;
    printf("intIp ==========%X\n ", intIp);
    printf("intNetmask ==========%X\n ", intNetmask);
    printf("intGateway ==========%X\n ", intGateway);
    printf("ipNetNumber1 ==========%X\n ", ipNetNumber1);
    printf("ipNetNumber2 ==========%X\n ", ipNetNumber2);
#endif
    if (ipNetNumber1 == ipNetNumber2)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

QString CQmlLogic::getLocalUriHead()
{
#ifdef WIN32
    return "file:///";
#else
    return "file://";
#endif
}

int CQmlLogic::resetFaultIndex()
{
    m_faultIndex = 0xffffffff;

    return 0;
}

int CQmlLogic::setFaultIndex(int index)
{
    m_faultIndex = index;

    return 0;
}

int CQmlLogic::compareFaultIndex(int index)
{
    if (m_faultIndex == index)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}


int CQmlLogic::setKeyValue(QString key)
{
    m_key = key;

    return 0;
}


QString CQmlLogic::getKeyValue()
{
    return m_key;
}

int CQmlLogic::getStringCount(QString text)
{
    return text.count();
}

QString CQmlLogic::getHWVersion()
{
    int ver;

    for (int i = 0; i < 4; i++)
    {
        //m_hwVersion[i] = 1;
        qDebug() << "m_hwVersion ========= " << m_hwVersion[i];
    }

    ver = m_hwVersion[3] * 2 * 2 * 2 + m_hwVersion[2] * 2 * 2 + m_hwVersion[1] *  2 + m_hwVersion[0];

    return QString("HW01.00.%1").arg(ver, 2, 10, QChar('0'));
}

int CQmlLogic::copyLog()
{
#ifdef WIN32
    return 0;
#else
    QDateTime local(QDateTime::currentDateTime());
    QString localTime = local.toString("yyyy.MM.dd-hh.mm.ss");
    QString srcName= QString("Data-") + localTime + ".tar";
    m_srcFullName = QString(SD_CARD_PATH) + srcName;
    m_desFullName = QString(U_DISK_PATH) + srcName;

    QString cmd;
    cmd = QString("/home/root/Bin/dataExport.sh ") + m_srcFullName + " " +  m_desFullName;
    CLogic::GetInstance()->System(cmd.toStdString().c_str(), SYSTEM_CMD_COPY_LOG);

    return 0;
#endif
}

bool CQmlLogic::isDataExist()
{
    QFile file(m_desFullName);
    return file.exists();
}

void CQmlLogic::lanTest()
{
    CLogic::GetInstance()->LanTest();

    return;
}

void CQmlLogic::logicSync()
{
#ifdef WIN32

#else
    sync();
#endif
}

void CQmlLogic::exitETCS()
{
    CFontSetting::Release();
    CImageBrowser::Release();
    CUDisk::Release();
    CKeyboard::Release();
    CHistory::Release();
    CRecord::Release();
    CScheduleManage::Release();
    LangStr::Release();
    CGPIOControl::Release();
    CLocalConfig::Release();
    CQmlPage::Release();
    CQmlUserManage::Release();
    CQmlSourceManage::Release();
    CZoneManage::Release();

    CQmlLogic::GetInstance()->clearPredefineListModel();
    CQmlLogic::GetInstance()->clearFaultListModel();
    CQmlLogic::GetInstance()->clearActivateListModel();
    CQmlLogic::Release();
}

void CQmlLogic::timerUpdate200()
{
    CLogic::GetInstance()->OnTimer200minis();
}

void CQmlLogic::setButtonEnable(QString type, bool bEnable)
{
    e_DKMType eType = DKM_NORMAL;
    if("alarm" == type)
    {
        eType = DKM_ALARM;
    }
    else if("DKM" == type)
    {
        eType = DKM_NORMAL;
    }
    CLogic::GetInstance()->DKMEnable(eType, bEnable);
}

void CQmlLogic::resetConfigPwd()
{
#ifdef WIN32
    return;
#else
    CLogic::GetInstance()->LogOperate("Reset SSH and samba password!");
    QString cmd;
    cmd = QString("cp /usr/local/backup/shadow /etc/");
    system(cmd.toStdString().c_str());
    cmd = QString("cp /usr/local/backup/smbpasswd /usr/local/samba/lib/");
    system(cmd.toStdString().c_str());
    cmd = QString("cp /usr/local/backup/smbpass /run/media/mmcblk0p1/Config/");
    system(cmd.toStdString().c_str());
    cmd = QString("rm -R /usr/local/samba/private/*");
    system(cmd.toStdString().c_str());
    cmd = QString("rm -R /usr/local/samba/var/*");
    system(cmd.toStdString().c_str());
    cmd = QString("sync");
    system(cmd.toStdString().c_str());
    cmd = QString("reboot");
    system(cmd.toStdString().c_str());

    return;
#endif
}

bool CQmlLogic::setVlan(int id, int priority, QString ip, QString mask)
{
    t_VLANConfig default_config;
    default_config.nID = -1;
    default_config.nPriority = 6;
    default_config.strIP = "10.0.0.1";
    default_config.strSubnetMask = "255.255.255.0";

    if (id < 0 || 4095 < id)
    {
        PA::VLANConfig(default_config);
        return true;
    }

    t_VLANConfig config;
    config.nID = id;
    config.nPriority = priority;
    config.strIP = ip.toStdString();
    config.strSubnetMask = mask.toStdString();

    if (!isVlanConfigValid(config))
    {
        PA::VLANConfig(default_config);
        return true;
    }

    PA::VLANConfig(config);
    return true;
}

QString CQmlLogic::getVLANInfo()
{
    QString vlanValid = CLocalConfig::GetInstance()->readConfig("Config", "VlanValid");
    QString vlanID = CLocalConfig::GetInstance()->readConfig("Config", "VlanID");

    if ("true" != vlanValid)
    {
        return LangStr::GetInstance()->GetLangString("ETCS_OFF");
    }

    return vlanID;
}

bool CQmlLogic::isVlanIDValid(QString vlanID)
{
    if (vlanID.isEmpty())
    {
        return false;
    }
    int id = vlanID.toInt();
    if (id < 0 || 4095 < id)
    {
        return false;
    }

    return true;
}

bool CQmlLogic::isVlanpriorityValid(QString vlanPriority)
{
    if (vlanPriority.isEmpty())
    {
        return false;
    }
    int priority = vlanPriority.toInt();
    if (priority < 0 || 7 < priority)
    {
        return false;
    }

    return true;
}

void CQmlLogic::lampTest(bool isActivate)
{
    PA::LampTest(isActivate);
}


bool CQmlLogic::isValidByPlatForm()
{
#ifdef WIN32
    return false;
#else
    return true;
#endif
}

QString CQmlLogic::getActivateTime(t_DateTime &activate_time)
{
    QString tmp;
    tmp = QString("%1:%2:%3").arg(activate_time.Hour, 2, 10, QChar('0')).arg(activate_time.Min, 2, 10, QChar('0')).arg(activate_time.Sec, 2, 10, QChar('0'));
    return tmp;
}

QString CQmlLogic::getActivateStatus(CONSTATE &status)
{
    QString tmp;

    switch (status)
    {
        case CON_FULL_CONNECT:
            tmp = FULL_STATUS; // LangStr::GetInstance()->GetLangString("ETCS_FULL_CONNECT");
            break;
        case CON_PARTLY_CONNECT:
            tmp = PART_STATUS; // LangStr::GetInstance()->GetLangString("ETCS_PARTLY_CONNECT");
            break;
        case CON_INTERRUPTED:
            tmp = WAIT_STATUS; //LangStr::GetInstance()->GetLangString("ETCS_WAIT");
            break;
        case CON_DIS_CONNECT:
            tmp = DIS_STATUS; //LangStr::GetInstance()->GetLangString("ETCS_DISCONNECT");
            break;
		default:
			break;
    }

    return tmp;
}

QString CQmlLogic::getActivateZones(std::vector<CZone*> &vZone)
{
    QString tmp;

    for (unsigned int i = 0; i < vZone.size(); i++)
    {
        if (i < vZone.size() - 1)
        {
            tmp += QString::fromStdString(vZone[i]->strName)+ ";";
        }
        else if (i == vZone.size() - 1)
        {
            tmp += QString::fromStdString(vZone[i]->strName);
        }
    }

    return tmp;
}

int CQmlLogic::getEmergency(bool emergency)
{
    if (emergency == true)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

QString CQmlLogic::getLocalSources(std::vector<std::string> &vSource)
{
    QString tmp;
    QString name;
    QString source;
    int pos;

    for (unsigned int i = 0; i < vSource.size(); i++)
    {
        source = QString::fromStdString(vSource[i]);
        pos = source.lastIndexOf("/");
        if (pos == -1)
        {
            pos = source.lastIndexOf("\\");
        }
        name = source.right(source.length() - pos - 1);
        tmp += name + "\n";
    }

    return tmp;
}

QString CQmlLogic::getNetworkSources(std::vector<t_Audio> &vSource)
{
    QString tmp;
    QString name;
    for (unsigned int i = 0; i < vSource.size(); i++)
    {
        name = "channel" + QString("%1").arg(vSource[i].nChannel) + "-title" +QString("%1").arg(vSource[i].nTitle);
        tmp += name + "\n";
    }

    return tmp;
}

int CQmlLogic::getEmergencyActivateCount()
{
    std::vector<CConnect*> vConnect;
    CLogic::GetInstance()->GetInstance()->GetActivateList()->GetEmergency(vConnect);

    return vConnect.size();
}

QString CQmlLogic::getListTime(t_DateTime &list_time)
{
    QString tmp;
    tmp = QString("%1:%2:%3").arg(list_time.Hour, 2, 10, QChar('0')).arg(list_time.Min, 2, 10, QChar('0')).arg(list_time.Sec, 2, 10, QChar('0'));
    return tmp;
}

QString CQmlLogic::getMinAndSec(int min, int sec)
{
    QString tmp;
    tmp = QString("%1:%2").arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
    return tmp;
}

QString CQmlLogic::getMacAddress()
{
#ifdef WIN32
    return "";
#else
    struct ifreq ifreq;
    int sock = 0;
    char mac[32] = "";
    int i;


    sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0)
    {
        qDebug() << "socket creat error";
        return "";
    }

    strncpy(ifreq.ifr_name, "bond1", sizeof(ifreq.ifr_name) - 1);
    ifreq.ifr_name[sizeof(ifreq.ifr_name) - 1] = '\0';
    if(ioctl(sock,SIOCGIFHWADDR,&ifreq) < 0)
    {
        qDebug() <<"error ioctl";
        close(sock);
        return "";
    }

    for(i = 0; i < 6; i++)
    {
        snprintf(mac+3*i, sizeof(mac)-3*i, "%02X:", (unsigned char)ifreq.ifr_hwaddr.sa_data[i]);
    }

    mac[strlen(mac) - 1] = 0;
    close(sock);

    return QString(mac);
#endif
}
