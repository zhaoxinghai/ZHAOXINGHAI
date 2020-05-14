#include <mutex>
#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <string>

#ifdef WIN32

#else
#include <unistd.h>
#endif

#include "clocalconfig.h"
#include "tinyxml2.h"
#include "Logic.h"
#include "cqmlusermanage.h"
#include "public.h"

static std::mutex m_mutex;

CLocalConfig* CLocalConfig::pLocalConfig = NULL;

CLocalConfig* CLocalConfig::GetInstance()
{
    if (pLocalConfig == NULL)
    {
        m_mutex.lock();
        if (pLocalConfig == NULL)
        {
            pLocalConfig = new CLocalConfig();
        }
        m_mutex.unlock();
    }
    return pLocalConfig;
}

void CLocalConfig::Release()
{
    if (NULL != pLocalConfig)
    {
        m_mutex.lock();
        if (NULL != pLocalConfig)
        {
            delete pLocalConfig;
            pLocalConfig = NULL;
        }
        m_mutex.unlock();
    }
}

CLocalConfig::CLocalConfig(QObject *parent) : QObject(parent)
{

}

void CLocalConfig::init()
{
#ifdef WIN32
   m_localConfigFile = getCurrentPath() + CONFIG_DIR + LOCAL_CONFIG_FILE;
   m_localConfigBackupFile = getCurrentPath() + CONFIG_DIR + LOCAL_CONFIG_BACKUP_FILE;
#else
   m_localConfigFile =  QString(SD_CARD_PATH) + CONFIG_DIR + LOCAL_CONFIG_FILE;
   m_localConfigBackupFile = QString(SD_CARD_PATH) + CONFIG_DIR + LOCAL_CONFIG_BACKUP_FILE;
#endif

    setIsConfigMode(false);
    initLocalConfigFile();
}

int CLocalConfig::writeConfig(QString configType, QString name, QString value)
{
    if (getIsConfigMode() == true)
    {
        return -1;
    }

    QString tmpName;

    tinyxml2::XMLDocument xml;

    if (tinyxml2::XMLError::XML_SUCCESS != xml.LoadFile(m_localConfigFile.toStdString().c_str()))
    {
        qDebug() << "write load file error:" << m_localConfigFile;
        return -1;
    }

    tinyxml2::XMLElement* root = xml.FirstChildElement(LOGIC_CONFIG);
    if (NULL == root)
    {
        return -1;
    }

    tinyxml2::XMLElement* localConfig = root->FirstChildElement(configType.toStdString().c_str());
    if (localConfig == NULL)
    {
        return -1;
    }

    tinyxml2::XMLElement* item = localConfig->FirstChildElement(LOCAL);
    while (NULL != item)
    {
        tmpName = item->Attribute(NAME);
        if (name == tmpName)
        {
            item->SetAttribute(VALUE, value.toStdString().c_str());
            break;
        }

        item = item->NextSiblingElement(LOCAL);
    }

    if (tinyxml2::XML_SUCCESS != xml.SaveFile(m_localConfigFile.toStdString().c_str()))
    {
        return -1;
    }

#ifdef WIN32

#else
    sync();
#endif

    return 0;
}

int CLocalConfig::insertConfig(QString name, QString value)
{
    if (getIsConfigMode() == true)
    {
        return -1;
    }

    tinyxml2::XMLDocument xml;
    tinyxml2::XMLElement* item = NULL;
    tinyxml2::XMLElement* config = NULL;

    if (tinyxml2::XMLError::XML_SUCCESS != xml.LoadFile(m_localConfigFile.toStdString().c_str()))
    {
        qDebug() << "insertConfig load file error:" << m_localConfigFile;
        return -1;
    }

    tinyxml2::XMLElement* root = xml.FirstChildElement(LOGIC_CONFIG);
    if (NULL == root)
    {
        return -1;
    }

    config = root->FirstChildElement(CONFIG);
    if (config == NULL)
    {
         item = xml.NewElement(CONFIG);
         root->InsertEndChild(item);
    }

    config = root->FirstChildElement(CONFIG);
    item = xml.NewElement(LOCAL);

    item->SetAttribute(NAME, name.toStdString().c_str());
    item->SetAttribute(VALUE, value.toStdString().c_str());
    config->InsertEndChild(item);

    if (tinyxml2::XML_SUCCESS != xml.SaveFile(m_localConfigFile.toStdString().c_str()))
    {
        return -1;
    }

#ifdef WIN32

#else
    sync();
#endif

    return 0;
}

QString CLocalConfig::readConfig(QString configType, QString name)
{
    QString tmpName;
    tinyxml2::XMLDocument xml;

    if (tinyxml2::XMLError::XML_SUCCESS != xml.LoadFile(m_localConfigFile.toStdString().c_str()))
    {
        qDebug() << "read load file error:" << m_localConfigFile;
        return "";
    }

    tinyxml2::XMLElement* root = xml.FirstChildElement(LOGIC_CONFIG);
    if (root == NULL)
    {
        return "";
    }
    tinyxml2::XMLElement* localConfig = root->FirstChildElement(configType.toStdString().c_str());
    if (localConfig == NULL)
    {
        return "";
    }
    tinyxml2::XMLElement* item = localConfig->FirstChildElement(LOCAL);

    while (NULL != item)
    {
        tmpName = item->Attribute(NAME);
        if (name == tmpName)
        {
            return item->Attribute(VALUE);
        }

        item = item->NextSiblingElement(LOCAL);
    }

    return "";
}

QString CLocalConfig::getProtocolVersion()
{
    return QString("%1").arg(CLogic::GetInstance()->GetProtocolVersion());
}

QString CLocalConfig::getUploadMaster()
{
    return QString::fromStdString(CLogic::GetInstance()->GetUploadMaster());
}

QString CLocalConfig::getStationName()
{
    return QString::fromStdString(CLogic::GetInstance()->GetStationName());
}

int CLocalConfig::getLocalNode()
{
    return CLogic::GetInstance()->GetNode();
}

int CLocalConfig::getUsedStorage(int type)
{
    return CLogic::GetInstance()->GetDirUsedSize((e_FOLDER)type);
}

int CLocalConfig::getTotalStorage(int type)
{
    return CLogic::GetInstance()->GetDirTotalSize((e_FOLDER)type);
}

QString CLocalConfig::getLockImg()
{

#ifdef WIN32
    QFile lockImgeFile(getCurrentPath() + CONFIG_DIR + LOCK_IMAGE_FILE);
    QFile defaultLockImgeFile(getCurrentPath() + CONFIG_DIR + DEFAULT_LOCK_IMAGE_FILE);
#else
    QFile lockImgeFile(QString(SD_CARD_PATH) + CONFIG_DIR + LOCK_IMAGE_FILE);
    QFile defaultLockImgeFile(QString(SD_CARD_PATH) + CONFIG_DIR + DEFAULT_LOCK_IMAGE_FILE);
#endif

    if (lockImgeFile.exists() == true)
    {
        return getLocalUriHead() + lockImgeFile.fileName();
    }
    else if (defaultLockImgeFile.exists()  == true)
    {
        return getLocalUriHead() + defaultLockImgeFile.fileName();
    }
    else
    {
        return "";
    }
}

int CLocalConfig::setIsConfigMode(bool mode)
{
    m_isConfigMode = mode;

    return 0;
}

bool CLocalConfig::getIsConfigMode()
{
    return m_isConfigMode;
}

int CLocalConfig::initLocalConfigFile()
{
    int ret;

    ret = isLocalConfigFileInit();
    if (ret < 0)
    {
        createLocalConfigFile();
    }

    return 0;
}

int CLocalConfig::isLocalConfigFileInit()
{
    tinyxml2::XMLDocument xml;

    if (tinyxml2::XMLError::XML_SUCCESS != xml.LoadFile(m_localConfigFile.toStdString().c_str()))
    {
        return -1;
    }

    tinyxml2::XMLElement* root = xml.FirstChildElement(LOGIC_CONFIG);
    if (root == NULL)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int CLocalConfig::createLocalConfigFile()
{
    if (getIsConfigMode() == true)
    {
        return -1;
    }

    FILE *op = NULL;
    FILE *inp = NULL;


    op = fopen(m_localConfigBackupFile.toStdString().c_str(), "rb");
    if (op == NULL)
    {
        return -1;
    }

    inp = fopen(m_localConfigFile.toStdString().c_str(), "wb");
    if (inp == NULL)
    {
        fclose(op);
        return -1;
    }

    char c;
    while(feof(op) == 0)
    {
        fread(&c,1,1,op);
        fwrite(&c,1,1,inp);
    }

    if (op != NULL)
    {
       fclose(op);
       op = NULL;
    }

    if (inp != NULL)
    {
        fclose(inp);
        inp = NULL;
    }

#ifdef WIN32

#else
    sync();
#endif

    return 0;
}
