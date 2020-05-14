#include <QDebug>
#include <QFile>
#include <string>
#include <mutex>

#ifdef WIN32

#else
#include <unistd.h>
#endif

#include "cqmlusermanage.h"
#include "Logic.h"
#include "tinyxml2.h"
#include "public.h"
#include "picosha2.h"

static std::mutex m_mutex;

CQmlUserManage* CQmlUserManage::pQmlUserConfig = NULL;

CQmlUserManage* CQmlUserManage::GetInstance()
{
    if (pQmlUserConfig == NULL)
    {
        m_mutex.lock();
        if (pQmlUserConfig == NULL)
        {
            pQmlUserConfig = new CQmlUserManage();
        }
        m_mutex.unlock();
    }
    return pQmlUserConfig;
}

void CQmlUserManage::Release()
{
    if (NULL != pQmlUserConfig)
    {
        m_mutex.lock();
        if (NULL != pQmlUserConfig)
        {
            delete pQmlUserConfig;
            pQmlUserConfig = NULL;
        }
        m_mutex.unlock();
    }
}

CQmlUserManage::CQmlUserManage(QObject *parent) : QObject(parent)
{

}

void CQmlUserManage::init()
{   
    //printUserList();
}

int CQmlUserManage::getUserListCount()
{
    std::vector<t_User> userList;
    userList = CLogic::GetInstance()->GetUserList();

    return userList.size();
}

QString CQmlUserManage::getUserId(int pos)
{
    int count;
    std::vector<t_User> userList;
    userList = CLogic::GetInstance()->GetUserList();
    count = userList.size();

    if (pos > count - 1)
    {
        return "";
    }

    return QString::fromStdString(userList[pos].strID);
}

QString CQmlUserManage::getUserListStrValue(QString id, QString key)
{
    int count;
    std::vector<t_User> userList;
    userList = CLogic::GetInstance()->GetUserList();
    count = userList.size();

    for (int i = 0; i < count; i++)
    {
        if (QString::fromStdString(userList[i].strID) == id)
        {
            if (key == "name")
            {
                return QString::fromStdString(userList[i].strName);
            }
            else if (key == "passwd")
            {
                return QString::fromStdString(userList[i].strPass);
            }
            else if (key == "permission")
            {
                return QString::fromStdString(userList[i].strPermission);
            }
            else
            {
                return "";
            }
        }
    }

    return "";
}

int CQmlUserManage::setUserListIntValue(QString id, int value)
{
    int count;
    std::vector<t_User> userList;
    userList = CLogic::GetInstance()->GetUserList();
    count = userList.size();

    for (int i = 0; i < count; i++)
    {
        if (QString::fromStdString(userList[i].strID) == id)
        {
            userList[i].nLockTime = value;
            CLogic::GetInstance()->UpdateUser(userList[i]);

            return 0;
        }
    }

    return -1;
}


int CQmlUserManage::getUserListIntValue(QString id, QString key)
{
    int count;
    std::vector<t_User> userList;
    userList = CLogic::GetInstance()->GetUserList();
    count = userList.size();

    for (int i = 0; i < count; i++)
    {
        if (QString::fromStdString(userList[i].strID) == id)
        {
            if (key == "lockTime")
            {
                return userList[i].nLockTime;
            }
        }
    }

    return -1;
}

int CQmlUserManage::setLoginUserId(QString id)
{
    m_loginUserId = id;

    return 0;
}

QString CQmlUserManage::getLoginUserId()
{
    return m_loginUserId;
}

int CQmlUserManage::setVALoginUserId(QString id)
{
    m_VALoginUserId = id;

    return 0;
}

QString CQmlUserManage::getVALoginUserId()
{
    return m_VALoginUserId;
}

int CQmlUserManage::setUser(QString id)
{
    CLogic::GetInstance()->SetUser(id.toStdString());

    return 0;
}

QString CQmlUserManage::encryptPassword(QString password)
{
    std::string newPassword;
    newPassword = picosha2::hash256_hex_string<std::string>(password.toStdString());

    return QString::fromStdString(newPassword);
}

int CQmlUserManage::checkConfigFileOK()
{
#ifdef WIN32
    QString configFile = getCurrentPath() + CONFIG_DIR + CONFIG_FILE;
#else
    QString configFile = QString(SD_CARD_PATH) + CONFIG_DIR + CONFIG_FILE;
#endif

    tinyxml2::XMLDocument xml;

    if (tinyxml2::XMLError::XML_SUCCESS != xml.LoadFile(configFile.toStdString().c_str()))
    {
        return -1;
    }

    tinyxml2::XMLElement* root = xml.FirstChildElement(CONFIG_ROOT_ITEM);
    if (root == NULL)
    {
        return -1;
    }

    return 0;
}

void CQmlUserManage::printUserList()
{
    int count;
    std::vector<t_User> userList;

    userList = CLogic::GetInstance()->GetUserList();

    count = userList.size();
    for (int i = 0; i < count; i++)
    {
        qDebug() << "user list id = " << QString::fromStdString(userList.at(i).strID);
        qDebug() << "user list name = " << QString::fromStdString(userList.at(i).strName);
        qDebug() << "user list permission = " << QString::fromStdString(userList.at(i).strPermission);
        qDebug() << "user list lock time = " << userList.at(i).nLockTime;
    }
}

bool CQmlUserManage::isNeedResetAccount(QString user, QString pwd)
{
    if (RESET_ACCOUNT_USER == user \
        && RESET_PASSWD_NUM == pwd)
    {
        return true;
    }

    return false;
}
