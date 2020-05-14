#include <mutex>
#include <QString>
#include <QStringList>
#include <QDebug>

#include "czonemanage.h"
#include "clocalconfig.h"
#include "cqmlusermanage.h"
#include "zoneModel.h"

static std::mutex m_mutex1;

CZoneManage* CZoneManage::pZoneManage = NULL;

CZoneManage * CZoneManage::GetInstance()
{
    if (pZoneManage == NULL)
    {
        m_mutex1.lock();
        if (pZoneManage == NULL)
        {
            pZoneManage = new CZoneManage();
        }
        m_mutex1.unlock();
    }

    return pZoneManage;
}

void CZoneManage::Release()
{
    if (NULL != pZoneManage)
    {
        m_mutex1.lock();
        if (NULL != pZoneManage)
        {
            delete pZoneManage;
            pZoneManage = NULL;
        }
        m_mutex1.unlock();
    }
}

CZoneManage::CZoneManage(QObject *parent) : QObject(parent),
    m_zoneModel(new Models::ListModel(new CZoneModel())),
    m_zoneModelVA(new Models::ListModel(new CZoneModel()))
{

}

int CZoneManage::init()
{
    return 0;
}

int CZoneManage::getZoneListCount()
{
    std::vector<CZone *> tmp;
    CLogic::GetInstance()->GetZoneList()->GetList(tmp);

    return tmp.size();
}

int CZoneManage::getValidUserZoneListCount()
{
    std::vector<CZone *> tmp;
    CLogic::GetInstance()->GetZoneList()->GetList(tmp);
    int count = 0;
    bool ret = false;

    for (unsigned int i = 0; i < tmp.size(); i++)
    {
        if (tmp[i]->bAllUser == true)
        {
            count++;
        }
        else
        {
            ret = isZoneUserIdValid(tmp[i]->nIndex);
            if (ret == true)
            {
                count++;
            }
        }
    }

    return count;
}

int CZoneManage::getZoneIndex(int pos)
{
    std::vector<CZone *> tmp;
    CLogic::GetInstance()->GetZoneList()->GetList(tmp);

    if ((unsigned int)pos > tmp.size() - 1)
    {
        return -1;
    }

    return tmp[pos]->nIndex;
}

bool CZoneManage::isZoneNameValid(int index, QString str)
{
    QString name;

    name = getZoneStrValue(index, "name");

    return name.contains(str.trimmed(), Qt::CaseInsensitive);
}

bool CZoneManage::isZoneUserIdValid(int index)
{
    QString ownerList;
    QString tmpName;
    QString tmpId;
    bool ret;

    ret = getZoneBoolValue(index, "allUser");
    if (ret == true)
    {
        return true;
    }

    ownerList = getZoneStrValue(index, "ownerList");

    tmpId = CQmlUserManage::GetInstance()->getLoginUserId();

    QStringList userIdList;
    userIdList = ownerList.split(',');

    for (int j = 0; j < userIdList.size(); ++j)
    {
        if (userIdList.at(j) == tmpId)
        {
            return true;
        }
    }

    return false;
}

QString CZoneManage::getZoneStrValue(int index, QString key)
{
    std::vector<CZone *> tmp;
    CLogic::GetInstance()->GetZoneList()->GetList(tmp);

    for (unsigned int i = 0; i < tmp.size(); i++)
    {
        if (tmp[i]->nIndex == index)
        {
            if (key == "name")
            {
                return QString::fromStdString(tmp[i]->strName);
            }
            else if (key == "ownerList")
            {
                return QString::fromStdString(tmp[i]->strOwnerList);
            }
        }
    }

    return "";
}

int CZoneManage::getZoneIntValue(int index,  QString key)
{
    std::vector<CZone *> tmp;
    CLogic::GetInstance()->GetZoneList()->GetList(tmp);

    for (unsigned int i = 0; i < tmp.size(); i++)
    {
        if (tmp[i]->nIndex == index)
        {
            if (key == "status")
            {
                 return (int)(tmp[i]->eState);
            }
            else if (key == "statusVA")
            {
                return (int)(tmp[i]->eStateVA);
            }
        }
    }

    return -1;
}

bool CZoneManage::getZoneBoolValue(int index, QString key)
{
    std::vector<CZone *> tmp;
    CLogic::GetInstance()->GetZoneList()->GetList(tmp);

    for (unsigned int i = 0; i < tmp.size(); i++)
    {
        if (tmp[i]->nIndex == index)
        {
            if (key == "allUser")
            {
                 return tmp[i]->bAllUser;
            }
            else if (key == "selectVA")
            {
                return tmp[i]->bSelectVA;
            }
            else if (key == "select")
            {
                return tmp[i]->bSelect;
            }
        }
    }

    return false;
}

int CZoneManage::setZoneSelected(int index, bool select)
{
    CLogic::GetInstance()->GetZoneList()->Select(index, select);

    return 0;
}

int CZoneManage::selectAllFromUserPermission()
{
    QVariant index;

    int count = m_zoneModel->rowCount();
    for (int i = 0; i < count; i++)
    {
        index = m_zoneModel->data(m_zoneModel->index(i, 0), 1);
        setZoneSelected(index.toInt(), true);
    }

    return 0;
}

int CZoneManage::unselectAllFromUserPermission()
{
    QVariant index;

    int count = m_zoneModel->rowCount();
    for (int i = 0; i < count; i++)
    {
        index = m_zoneModel->data(m_zoneModel->index(i, 0), 1);
        setZoneSelected(index.toInt(), false);
    }

    return 0;
}

int CZoneManage::unSelectAll()
{
    CLogic::GetInstance()->GetZoneList()->UnSelectAll();

    return 0;
}

bool CZoneManage::isOneZoneSelected()
{
    std::vector<CZone *> tmp;
    CLogic::GetInstance()->GetZoneList()->GetList(tmp);

    for (unsigned int i = 0; i < tmp.size(); i++)
    {
        if (tmp[i]->bSelect == true)
        {
            return true;
        }
    }

    return false;
}

int CZoneManage::getSelectCount()
{
    int count = 0;
    std::vector<CZone *> tmp;
    CLogic::GetInstance()->GetZoneList()->GetList(tmp);

    for (unsigned int i = 0; i < tmp.size(); i++)
    {
        if (tmp[i]->bSelect == true)
        {
            count++;
        }
    }

    return count;
}

bool CZoneManage::isAllZoneChecked()
{
    std::vector<CZone *> tmp;
    CLogic::GetInstance()->GetZoneList()->GetList(tmp);

    for (unsigned int i = 0; i < tmp.size(); i++)
    {
        if (tmp[i]->eState == ZONE_NO_NETWORK)
        {
            return false;
        }
    }

    return true;
}

int CZoneManage::clearZoneModel()
{
    m_zoneModel->clear();

    return 0;
}

int CZoneManage::createZoneModel()
{
    int ret;
    unsigned int size;
    std::vector<CZone*> vZone;
    CLogic::GetInstance()->GetZoneList()->GetList(vZone);

    size = vZone.size();
    for (unsigned int i = 0; i < size; i++)
    {
        ret = isZoneUserIdValid(vZone[i]->nIndex);
        if (ret == true)
        {
            m_zoneModel->appendRow(new CZoneModel(vZone[i]->nIndex,
                                                  QString::fromStdString(vZone[i]->strName),
                                                  getColor(getZoneIntValue(vZone[i]->nIndex, "status")),
                                                  false,
                                                  getColorVA(getZoneIntValue(vZone[i]->nIndex, "statusVA")),
                                                  false));
        }
    }
    return 0;
}

int CZoneManage::getZoneModelCount()
{
    return m_zoneModel->rowCount();
}

int CZoneManage::zoneModelSelectAll()
{
    int count = m_zoneModel->rowCount();
    for (int i = 0; i < count; i++)
    {
        m_zoneModel->setData(m_zoneModel->index(i, 0), true, 4);
    }

    return 0;
}

int CZoneManage::zoneModelUnselectAll()
{
    int count = m_zoneModel->rowCount();
    for (int i = 0; i < count; i++)
    {
        m_zoneModel->setData(m_zoneModel->index(i, 0), false, 4);
    }

    return 0;
}

int CZoneManage::getZoneModelSelectCount()
{
    int selectCount = 0;
    QVariant select = false;

    int count = m_zoneModel->rowCount();
    for (int i = 0; i < count; i++)
    {
        select = m_zoneModel->data(m_zoneModel->index(i, 0), 4);
        if (select.toBool() == true)
        {
            selectCount++;
        }
    }

    return selectCount;
}

bool CZoneManage::isZoneModelMoreThanOneZoneSelected()
{
    QVariant select = false;

    int count = m_zoneModel->rowCount();
    for (int i = 0; i < count; i++)
    {
        select = m_zoneModel->data(m_zoneModel->index(i, 0), 4);
        if (select.toBool() == true)
        {
            return true;
        }
    }

    return false;
}

Models::ListModel *CZoneManage::getZoneModel()
{
    return m_zoneModel;
}

int CZoneManage::updateZoneModelStatus()
{
    QVariant index;

    int count = m_zoneModel->rowCount();
    for (int i = 0; i < count; i++)
    {
        index = m_zoneModel->data(m_zoneModel->index(i, 0), 1);
        m_zoneModel->setData(m_zoneModel->index(i, 0), getColor(getZoneIntValue(index.toInt(), "status")), 3);
    }

    return 0;
}

int CZoneManage::printAllSelectZones()
{
    int count = 0;
    std::vector<CZone *> tmp;
    CLogic::GetInstance()->GetZoneList()->GetList(tmp);

    for (unsigned int i = 0; i < tmp.size(); i++)
    {
        if (tmp[i]->bSelect == true)
        {
            qDebug() << "select zone index = " << tmp[i]->nIndex;
            qDebug() << "select zone name = " << QString::fromStdString(tmp[i]->strName);
            count++;
        }
    }

    qDebug() << "select count = " << count;
    return count;
}

int CZoneManage::clearZoneModelVA()
{
    m_zoneModelVA->clear();

    return 0;
}

int CZoneManage::createZoneModelVA()
{
    int ret;
    unsigned int size;
    std::vector<CZone*> vZone;
    CLogic::GetInstance()->GetZoneList()->GetList(vZone);

    size = vZone.size();
    for (unsigned int i = 0; i < size; i++)
    {
        ret = isVAZoneUserIdValid(vZone[i]->nIndex, CQmlUserManage::GetInstance()->getVALoginUserId());
        if (ret == true)
        {
            m_zoneModelVA->appendRow(new CZoneModel(vZone[i]->nIndex,
                                                  QString::fromStdString(vZone[i]->strName),
                                                  getColor(getZoneIntValue(vZone[i]->nIndex, "status")),
                                                  false,
                                                  getColorVA(getZoneIntValue(vZone[i]->nIndex, "statusVA")),
                                                  false));
        }
    }
    return 0;
}

int CZoneManage::getZoneModelCountVA()
{
    return m_zoneModelVA->rowCount();
}

int CZoneManage::zoneModelSelectAllVA()
{
    int count = m_zoneModelVA->rowCount();
    for (int i = 0; i < count; i++)
    {
        m_zoneModelVA->setData(m_zoneModelVA->index(i, 0), true, 6);
    }

    return 0;
}

int CZoneManage::zoneModelUnselectAllVA()
{
    int count = m_zoneModelVA->rowCount();
    for (int i = 0; i < count; i++)
    {
        m_zoneModelVA->setData(m_zoneModelVA->index(i, 0), false, 6);
    }

    return 0;
}

int CZoneManage::getZoneModelSelectCountVA()
{
    int selectCount = 0;
    QVariant select = false;

    int count = m_zoneModelVA->rowCount();
    for (int i = 0; i < count; i++)
    {
        select = m_zoneModelVA->data(m_zoneModelVA->index(i, 0), 6);
        if (select.toBool() == true)
        {
            selectCount++;
        }
    }

    return selectCount;
}

bool CZoneManage::isZoneModelMoreThanOneZoneSelectedVA()
{
    QVariant select = false;

    int count = m_zoneModelVA->rowCount();
    for (int i = 0; i < count; i++)
    {
        select = m_zoneModelVA->data(m_zoneModelVA->index(i, 0), 6);
        if (select.toBool() == true)
        {
            return true;
        }
    }

    return false;
}

Models::ListModel *CZoneManage::getZoneModelVA()
{
    return m_zoneModelVA;
}

int CZoneManage::updateZoneModelStatusVA()
{
    int count = m_zoneModelVA->rowCount();
    for (int i = 0; i < count; i++)
    {
        QModelIndex mode_index = m_zoneModelVA->index(i, 0);
        QVariant zone_index = m_zoneModelVA->data(mode_index, 1);

        int status = getZoneIntValue(zone_index.toInt(), "status");
        QString color = getColor(status);
        m_zoneModelVA->setData(mode_index, color, 3);

        int va_status = getZoneIntValue(zone_index.toInt(), "statusVA");
        QString va_color = getColorVA(va_status);
        m_zoneModelVA->setData(mode_index, va_color, 5);
    }

    return 0;
}

int CZoneManage::printAllSelectZonesVA()
{
    int count = 0;
    std::vector<CZone *> tmp;
    CLogic::GetInstance()->GetZoneList()->GetList(tmp);

    for (unsigned int i = 0; i < tmp.size(); i++)
    {
        if (tmp[i]->bSelectVA == true)
        {
            qDebug() << "VA select zone index = " << tmp[i]->nIndex;
            qDebug() << "VA select zone name = " << QString::fromStdString(tmp[i]->strName);
            count++;
        }
    }

    qDebug() << "VA select count = " << count;
    return count;
}

int CZoneManage::setZoneSelectedVA(int index, bool select)
{
    CLogic::GetInstance()->GetZoneList()->SelectVA(index, select);

    return 0;
}

int CZoneManage::unSelectAllVA()
{
    CLogic::GetInstance()->GetZoneList()->UnSelectAllVA();

    return 0;
}

int CZoneManage::selectAllFromUserPermissionVA()
{
    QVariant index;

    int count = m_zoneModelVA->rowCount();
    for (int i = 0; i < count; i++)
    {
        index = m_zoneModelVA->data(m_zoneModelVA->index(i, 0), 1);
        setZoneSelectedVA(index.toInt(), true);
    }

    return 0;
}

int CZoneManage::unselectAllFromUserPermissionVA()
{
    QVariant index;

    int count = m_zoneModelVA->rowCount();
    for (int i = 0; i < count; i++)
    {
        index = m_zoneModelVA->data(m_zoneModelVA->index(i, 0), 1);
        setZoneSelectedVA(index.toInt(), false);
    }

    return 0;
}

bool CZoneManage::isOneZoneSelectedVA()
{
    std::vector<CZone *> tmp;
    CLogic::GetInstance()->GetZoneList()->GetList(tmp);

    for (unsigned int i = 0; i < tmp.size(); i++)
    {
        if (tmp[i]->bSelectVA == true)
        {
            return true;
        }
    }

    return false;
}

int CZoneManage::getSelectCountVA()
{
    int count = 0;
    std::vector<CZone *> tmp;
    CLogic::GetInstance()->GetZoneList()->GetList(tmp);

    for (unsigned int i = 0; i < tmp.size(); i++)
    {
        if (tmp[i]->bSelectVA == true)
        {
            count++;
        }
    }

    return count;
}

void CZoneManage::ResetStateVA()
{
    CLogic::GetInstance()->GetZoneList()->ResetStateVA();

    return;
}

bool CZoneManage::isVAZoneUserIdValid(int index, QString tmpId)
{
    QString ownerList;
    QString tmpName;
    bool ret;

    ret = getZoneBoolValue(index, "allUser");
    if (ret == true)
    {
        return true;
    }

    ownerList = getZoneStrValue(index, "ownerList");

    QStringList userIdList;
    userIdList = ownerList.split(',');

    for (int j = 0; j < userIdList.size(); ++j)
    {
        if (userIdList.at(j) == tmpId)
        {
            return true;
        }
    }

    return false;
}

QString CZoneManage::getColor(int status)
{
    QString tmpColor;
    switch(status)
    {
        case 0:
            tmpColor = "#bebebe";
            break;
        case 1:
            tmpColor = "#00c767";
            break;
        case 2:
            tmpColor = "#ffffff";
            break;
        case 3:
            tmpColor = "#f7b529";
            break;
        default:
            tmpColor = "#bebebe";
            break;
    }

    return tmpColor;
}

QString CZoneManage::getColorVA(int status)
{
    QString tmpColor;
    switch(status)
    {
        case 0:
            tmpColor = "#e0e0e0";
            break;
        case 1:
            tmpColor = "#47b063";
            break;
        case 2:
            tmpColor = "#e53a26";
            break;
        case 3:
            tmpColor = "#f87719";
            break;
        default:
            tmpColor = "#e0e0e0";
            break;
    }

    return tmpColor;
}
