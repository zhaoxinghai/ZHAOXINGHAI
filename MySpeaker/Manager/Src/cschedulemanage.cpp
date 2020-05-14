#include <QDebug>
#include <QString>
#include <QStringList>
#include <mutex>

#include "cschedulemanage.h"
#include "Logic.h"
#include "clocalconfig.h"
#include "cqmlusermanage.h"

static std::mutex m_mutex1;

CScheduleManage* CScheduleManage::pScheduleManage = NULL;

CScheduleManage * CScheduleManage::GetInstance()
{
    if (pScheduleManage == NULL)
    {
        m_mutex1.lock();
        if (pScheduleManage == NULL)
        {
            pScheduleManage = new CScheduleManage();
        }
        m_mutex1.unlock();
    }

    return pScheduleManage;
}

void CScheduleManage::Release()
{
    if (NULL != pScheduleManage)
    {
        m_mutex1.lock();
        if (NULL != pScheduleManage)
        {
            delete pScheduleManage;
            pScheduleManage = NULL;
        }
        m_mutex1.unlock();
    }
}

CScheduleManage::CScheduleManage(QObject *parent) : QObject(parent)
{

}

void CScheduleManage::init()
{

}

int CScheduleManage::getScheduleListCount()
{
    std::vector<CScheduleTask*> vTask;
    CLogic::GetInstance()->GetScheduleList()->GetList(vTask);
    return vTask.size();
}

int CScheduleManage::getScheduleIndex(unsigned int pos)
{
    std::vector<CScheduleTask*> vTask;

    CLogic::GetInstance()->GetScheduleList()->GetList(vTask);
    if (pos < vTask.size())
    {
        return vTask[pos]->nIndex;
    }

    return -1;
}

bool CScheduleManage::isIndexValid(int index)
{
    std::vector<CScheduleTask*> vTask;
    QString ownerList;
    QString tmpName;
    QString tmpId;

    CLogic::GetInstance()->GetScheduleList()->GetList(vTask);

    for (unsigned int i = 0; i < vTask.size(); i++)
    {
        if (index == vTask[i]->nIndex)
        {
            if (vTask[i]->bAllUser == true)
            {
                return true;
            }
            else
            {

                tmpId = CQmlUserManage::GetInstance()->getLoginUserId();

                ownerList = QString::fromStdString(vTask[i]->strOwnerList);
                QStringList userIdList;
                userIdList = ownerList.split(',');

                for (int j = 0; j < userIdList.size(); ++j)
                {
                    if (userIdList.at(j) == tmpId)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int CScheduleManage::getScheduleValue(int index, QString key)
{
    std::vector<CScheduleTask*> vTask;

    CLogic::GetInstance()->GetScheduleList()->GetList(vTask);

    for (unsigned int i = 0; i < vTask.size(); i++)
    {
        if (index == vTask[i]->nIndex)
        {
            if (key == "editable")
            {
                return vTask[i]->bMyConfig == true ? 1 : 0;
            }
            else if (key == "switchStatus")
            {
                return vTask[i]->bOn == true ? 1 : 0;
            }
            else if (key == "startHour")
            {
                return vTask[i]->param.startDateTime.Hour;
            }
            else if (key == "endHour")
            {
                return vTask[i]->param.endDateTime.Hour;
            }
            else if (key == "startMin")
            {
                return vTask[i]->param.startDateTime.Min;
            }
            else if (key == "endMin")
            {
                return vTask[i]->param.endDateTime.Min;
            }
            else if (key == "once")
            {
                return vTask[i]->param.bOnlyOnce == true ? 1 : 0;
            }
            else if (key == "cycle")
            {
                return vTask[i]->param.nLoopCount;
            }
            else if (key == "gapTime")
            {
                return vTask[i]->param.nLoopInterval;
            }
            else
            {
                return -1;
            }
        }
    }

    return -1;
}

QString CScheduleManage::getScheduleStrValue(int index, QString key)
{
    std::vector<CScheduleTask*> vTask;

    CLogic::GetInstance()->GetScheduleList()->GetList(vTask);

    for (unsigned int i = 0; i < vTask.size(); i++)
    {
        if (index == vTask[i]->nIndex)
        {
            if (key == "name")
            {
                return QString::fromStdString(vTask[i]->param.strName);
            }
            else
            {
                return "";
            }
        }
    }

    return "";
}

int CScheduleManage::getScheduleIntValue(int index, QString key)
{
    std::vector<CScheduleTask*> vTask;

    CLogic::GetInstance()->GetScheduleList()->GetList(vTask);

    for (unsigned int i = 0; i < vTask.size(); i++)
    {
        if (index == vTask[i]->nIndex)
        {
            if (key == "errorNum")
            {
                return vTask[i]->nErrorCode;
            }

        }
    }

    return -1;
}

bool CScheduleManage::getScheduleBoolValue(int index, QString key)
{
    std::vector<CScheduleTask*> vTask;

    CLogic::GetInstance()->GetScheduleList()->GetList(vTask);

    for (unsigned int i = 0; i < vTask.size(); i++)
    {
        if (index == vTask[i]->nIndex)
        {
            if (key == "MyConfig")
            {
                return vTask[i]->bMyConfig;
            }
            if (key == "AllUser")
            {
                return vTask[i]->bAllUser;
            }
            if (key == "isError")
            {
                return vTask[i]->bIsError;
            }
            if (key == "switchStatus")
            {
                return vTask[i]->bOn;
            }
        }
    }

    return false;
}

bool CScheduleManage::scheduleIsLive(int index)
{
    std::vector<CScheduleTask*> vTask;

    CLogic::GetInstance()->GetScheduleList()->GetList(vTask);

    for (unsigned int i = 0; i < vTask.size(); i++)
    {
        if (index == vTask[i]->nIndex)
        {
            return vTask[i]->pConnect->IsPhysical();
        }
    }

    return false;
}

bool CScheduleManage::getDay(int index, int day)
{
    if (day > 6 || day < 0)
    {
        return false;
    }

    std::vector<CScheduleTask*> vTask;

    CLogic::GetInstance()->GetScheduleList()->GetList(vTask);

    for (unsigned int i = 0; i < vTask.size(); i++)
    {
        if (index == vTask[i]->nIndex)
        {
            return vTask[i]->param.szDay[day];
        }
    }

    return false;
}

int CScheduleManage::setMday(int day, bool value)
{
    if (day > 6 || day < 0)
    {
        return -1;
    }

    m_day[day] = value;

    return 0;
}

int CScheduleManage::setScheduleParam(int hour,
                               int min,
                               QString name,
                               bool onlyOnce,
                               int cycle,
                               int gapTime)
{
    m_param.startDateTime.Hour = hour;
    m_param.startDateTime.Min = min;
    m_param.strName = name.toStdString();
    m_param.bOnlyOnce = onlyOnce;
    m_param.nLoopCount = cycle;
    m_param.nLoopInterval = gapTime;

#if defined(_SCHEDULE_MANAGE_) && (_SCHEDULE_MANAGE_ > 0)
    qDebug() << "Schedule Manage m_param.startDateTime.Hour" << m_param.startDateTime.Hour;
    qDebug() << "Schedule Manage m_param.startDateTime.Min" <<  m_param.startDateTime.Min;
    qDebug() << "Schedule Manage m_param.strName" << QString::fromStdString(m_param.strName);
    qDebug() << "Schedule Manage m_param.bOnlyOnce" << m_param.bOnlyOnce;
    qDebug() << "Schedule Manage m_param.nLoopCount" << m_param.nLoopCount;
    qDebug() << "Schedule Manage m_param.nLoopInterval" << m_param.nLoopInterval;
#endif

    for (unsigned int i = 0; i < 7; i++)
    {
        m_param.szDay[i] = m_day[i];
#if defined(_SCHEDULE_MANAGE_) && (_SCHEDULE_MANAGE_ > 0)
        qDebug() << "Schedule Manage m_param.szDay" << i <<  m_param.szDay[i];
#endif
    }

    return 0;
}

int CScheduleManage::updateSchedule(int index, bool myConfig)
{
#if defined(_SCHEDULE_MANAGE_) && (_SCHEDULE_MANAGE_ > 0)
    qDebug() << "updateSchedule m_param.startDateTime.Hour" << m_param.startDateTime.Hour;
    qDebug() << "updateSchedule m_param.startDateTime.Min" <<  m_param.startDateTime.Min;
    qDebug() << "updateSchedule m_param.strName" << QString::fromStdString(m_param.strName);
    qDebug() << "updateSchedule m_param.bOnlyOnce" << m_param.bOnlyOnce;
    qDebug() << "updateSchedule m_param.nLoopCount" << m_param.nLoopCount;
    qDebug() << "updateSchedule m_param.nLoopInterval" << m_param.nLoopInterval;
#endif
    CLogic::GetInstance()->GetScheduleList()->Update(index, myConfig, m_param);

    return 0;
}

int CScheduleManage::delSchedule(int index)
{
    CLogic::GetInstance()->GetScheduleList()->Del(index);

    return 0;
}

int CScheduleManage::openSchedule(int index, bool myConfig)
{
    CLogic::GetInstance()->GetScheduleList()->TurnOn(index, myConfig, true);
    return 0;
}

int CScheduleManage::closeSchedule(int index, bool myConfig)
{
    CLogic::GetInstance()->GetScheduleList()->TurnOn(index, myConfig, false);
    return 0;
}

int CScheduleManage::clearAllData()
{
    clearMday();
    clearscheduleParam();

    return 0;
}

QString CScheduleManage::getStrTime(int one, int two)
{
    QString tmp;
    tmp = QString("%1:%2").arg(one, 2, 10, QChar('0')).arg(two, 2, 10, QChar('0'));
    return tmp;
}

int CScheduleManage::AllScheduleOn(bool value)
{
    return CLogic::GetInstance()->AllScheduleOn(value);
}

bool CScheduleManage::isAllScheduleOff()
{
    std::vector<CScheduleTask*> vTask;

    CLogic::GetInstance()->GetScheduleList()->GetList(vTask);

    for (unsigned int i = 0; i < vTask.size(); i++)
    {
        if (vTask[i]->bOn == true)
        {
            return false;
        }
    }
    return true;
}

bool CScheduleManage::isAllScheduleError()
{
    std::vector<CScheduleTask*> vTask;
    CLogic::GetInstance()->GetScheduleList()->GetList(vTask);

    for (unsigned int i = 0; i < vTask.size(); i++)
    {
        if (vTask[i]->bIsError == false)
        {
            return false;
        }
    }
    return true;
}

bool CScheduleManage::isOverScheduleMaxCount()
{
    long scheduleMaxCount = CLogic::GetInstance()->GetInstance()->ReadConfigInit("limit", "schedule_max_count");

    if (getScheduleListCount() >= scheduleMaxCount)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int CScheduleManage::clearMday()
{
    for (unsigned int i = 0; i < 7; i++)
    {
        m_day[i] = false;
    }

    return 0;
}


int CScheduleManage::clearscheduleParam()
{
    m_param.startDateTime.Hour = 0;
    m_param.startDateTime.Min = 0;
    m_param.strName = "";
    m_param.bOnlyOnce = false;
    m_param.nLoopCount = 0;
    m_param.nLoopInterval = 0;

    for (unsigned int i = 0; i < 7; i++)
    {
        m_param.szDay[i] = false;
    }

    return 0;
}
