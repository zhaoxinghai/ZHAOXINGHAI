#include <QDir>
#include <QDebug>
#include <QFile>
#include <QCoreApplication>
#include <mutex>
#include <string>
#include <vector>
#include <algorithm>

#ifdef WIN32

#else
#include<sys/time.h>
#include<unistd.h>
#endif

#include "cqmlsourcemanage.h"
#include "cqmlsourcemodel.h"
#include "cqmlzonemodel.h"
#include "tinyxml2.h"
#include "interface.h"
#include "public.h"
#include "ConfigManage.h"
#include "ConfigAPI.h"
#include "cudisk.h"
#include "Logic.h"

static std::mutex m_mutex1;

CQmlSourceManage* CQmlSourceManage::pQmlSourceManage = NULL;

CQmlSourceManage * CQmlSourceManage::GetInstance()
{
	if (pQmlSourceManage == NULL)
	{
		m_mutex1.lock();
		if (pQmlSourceManage == NULL)
		{
			pQmlSourceManage = new CQmlSourceManage();
		}
		m_mutex1.unlock();
	}
	
	return pQmlSourceManage;
}

void CQmlSourceManage::Release()
{
	if (NULL != pQmlSourceManage)
	{
		m_mutex1.lock();
		if (NULL != pQmlSourceManage)
		{
			delete pQmlSourceManage;
			pQmlSourceManage = NULL;
		}
		m_mutex1.unlock();
	}
}

CQmlSourceManage::CQmlSourceManage() :
    m_usbSourceModel(new Models::ListModel(new CQmlSourceModel()))
{

}

void CQmlSourceManage::init()
{
    m_dir = QDir(U_DISK_PATH);

    setIsRun(false);
    this->start();
}

void CQmlSourceManage::run()
{
    bool ret;
    int count = 0;

    while(1)
    {
        if (getIsRun() == false)
        {
            sleep(2);
            continue;
        }
        else
        {
            sleep(2);
            if (CUDisk::GetInstance()->checkUDiskStatus() != HAVE_UDISK)
            {
                setIsRun(false);
                count = 0;
                continue;
            }

            if (CUDisk::GetInstance()->isUDiskMounted() == true)
            {
                CUDisk::GetInstance()->selfUmountUDisk();
            }

            ret = CUDisk::GetInstance()->isUDiskMountPathExist();
            if (ret == false)
            {
                CUDisk::GetInstance()->createUDiskMountPath();
            }

            CUDisk::GetInstance()->selfMountUDisk();
            ret = CUDisk::GetInstance()->isUDiskMounted();
            if (ret == true)
            {
                setIsRun(false);
                emit finishUsbSourceCreateSignal();
                count = 0;
                continue;
            }

            count++;
            if (count >= 30)
            {
                setIsRun(false);
                emit timeOutSignal();
                count = 0;
            }
        }
    }
}

int CQmlSourceManage::insertUsbSourceListCacheFile(QString source, QString cacheFile)
{
    for (unsigned int i = 0; i < m_usbSourceList.size(); i++)
    {
        if (m_usbSourceList[i].fileName == source)
        {
            m_usbSourceList[i].cacheFileName = cacheFile;
        }
    }

    return 0;
}

int CQmlSourceManage::createUsbSourceModel()
{
    bool tmp;
    int errorNum = FILE_OK;
    bool isAdp;
    bool isNeedDecode = true;

    for (unsigned int i = 0; i < m_usbSourceList.size(); i++)
    {
        if(m_usbSourceList[i].fileName.section(".", -1) == "adp")
        {
            isAdp = true;
        }
        else
        {
            isAdp = false;
        }

        if (isAdp == true)
        {
            tmp = PA::CheckAdpFile(m_usbSourceList[i].fileName.toStdString());
            if (tmp == true)
            {
                errorNum = FILE_OK;
            }
            else
            {
                errorNum = FILE_FORMAT_ERROR;
            }
        }

        m_usbSourceModel->appendRow(new CQmlSourceModel(m_usbSourceList[i].index,
                                                    m_usbSourceList[i].select,
                                                    m_usbSourceList[i].simpleFileName,
                                                    errorNum,
                                                    isNeedDecode));
#if 0
        if (isAdp == true)
        {
            m_usbSourceModel->appendRow(new CQmlSourceModel(m_usbSourceList[i].index,
                                                        m_usbSourceList[i].select,
                                                        m_usbSourceList[i].simpleFileName,
                                                        errorNum,
                                                        isNeedDecode));
        }
        else
        {
            m_usbSourceModel->appendRow(new CQmlSourceModel(m_usbSourceList[i].index,
                                                        m_usbSourceList[i].select,
                                                        m_usbSourceList[i].simpleFileName,
                                                        errorNum,
                                                        isNeedDecode));
        }
#endif

        errorNum = FILE_OK;

	}

    return 0;
}

int CQmlSourceManage::clearUsbSourceModel()
{
    m_usbSourceModel->removeRows(0, m_usbSourceModel->rowCount());

    return 0;
}

int CQmlSourceManage::getUsbSourceModelCount()
{
    return m_usbSourceModel->rowCount();
}

int CQmlSourceManage::setUsbSourceSelected(const int index, bool select)
{
	for (unsigned int i = 0; i < m_usbSourceList.size(); i++)
	{
        if (index == m_usbSourceList[i].index)
		{
            m_usbSourceList[i].select = select;
		}
	}

	return 0;
}

int CQmlSourceManage::setUsbSourceStatus(QString name, int status)
{
    for (unsigned int i = 0; i < m_usbSourceList.size(); i++)
    {
        if (name == m_usbSourceList[i].simpleFileName)
        {
             m_usbSourceModel->setData(m_usbSourceModel->index(i, 0), status, 4);
        }
    }

    return 0;
}

int CQmlSourceManage::setUsbSourceTran(QString fileName, bool tran)
{
    for (unsigned int i = 0; i < m_usbSourceList.size(); i++)
    {
        if (fileName == m_usbSourceList[i].fileName)
        {
             m_usbSourceModel->setData(m_usbSourceModel->index(i, 0), tran, 5);
        }
    }

    return 0;

}

int CQmlSourceManage::clearAllUsbSourceSelected()
{
    for (unsigned int i = 0; i < m_usbSourceList.size(); i++)
    {
        m_usbSourceList[i].select = false;
    }

    return 0;
}

int CQmlSourceManage::createUsbSourceList()
{
    QList<CFileData> fileList;

    getUDiskFiles(fileList);

    for (int i = 0; i < fileList.count(); i++)
    {
        CUsbSource tmp;
        tmp.index = i;
        tmp.select = 0;
        tmp.fileName = fileList.at(i).fileName;
        tmp.simpleFileName = fileList.at(i).simpleFileName;
        m_usbSourceList.push_back(tmp);
    }

    return 0;
}

int CQmlSourceManage::clearUsbSourceList()
{
    m_usbSourceList.clear();

    return 0;
}

int CQmlSourceManage::fillPathListFromUsbSourceList(std::vector<std::string> &vPath)
{
	for (unsigned int i = 0; i < m_usbSourceList.size(); i++)
	{
        if (true == m_usbSourceList[i].select)
		{
            if ((m_usbSourceList[i].fileName.section(".", -1) == "adp") ||
                (m_usbSourceList[i].fileName.section(".", -1) == "wav") ||
                (m_usbSourceList[i].fileName.section(".", -1) == "WAV"))
            {
                vPath.push_back(m_usbSourceList[i].fileName.toStdString());
            }
            else
            {
                vPath.push_back(m_usbSourceList[i].cacheFileName.toStdString());
            }
		}
	}

	return 0;
}

Models::ListModel* CQmlSourceManage::getUsbSourceModel()
{
    return m_usbSourceModel;
}

int CQmlSourceManage::getLocalGroupCount()
{
    std::vector<t_AudioGroup>&List = CLogic::GetInstance()->GetGroupList();
    return List.size();
}

QString CQmlSourceManage::getLocalGroupName(int groupIndex)
{
    std::vector<t_AudioGroup>&List = CLogic::GetInstance()->GetGroupList();
    if((int)List.size()>groupIndex)
    {
        return QString::fromStdString(List[groupIndex].strName);
    }
    return "";
}

QString CQmlSourceManage::getLocalGroupValue(int groupIndex, int index, QString attr)
{
    std::vector<t_AudioGroup>&List = CLogic::GetInstance()->GetGroupList();
    if((int)List.size()>groupIndex)
    {
        if((int)List[groupIndex].vPlay.size()>index)
        {
            t_Play &play = List[groupIndex].vPlay[index];

            if (attr == "Desc")
            {
               return QString::fromStdString(play.strDesc);
            }
        }
    }
    return "";
}

int CQmlSourceManage::getLocalGroupIntValue(int groupIndex, int index, QString attr)
{
    std::vector<t_AudioGroup>&List = CLogic::GetInstance()->GetGroupList();
    if((int)List.size()>groupIndex)
    {
        if((int)List[groupIndex].vPlay.size()>index)
        {
            t_Play &play = List[groupIndex].vPlay[index];

            if (attr == "errorNum")
            {
                if(play.bFileNotExist)
                {
                    return FILE_NO_EXIST;
                }
                else if(play.bFileFormatError)
                {
                    return FILE_FORMAT_ERROR;
                }
                else
                {
                    return FILE_OK;
                }
            }
        }
    }
    return -1;
}

QString CQmlSourceManage::getLocalDescFromFilename(QString name)
{/*
    QMap<int, IOMapGroup>::iterator i;
    QMap<int, IOMAP_ITEM>::iterator j;

    for (i = m_IOMapLocal.begin(); i != m_IOMapLocal.end(); i++)
    {
        for (j = i.value().ItemList.begin(); j != i.value().ItemList.end(); j++)
        {
            if (j.value().Filename == name)
            {
                return j.value().Description;
            }
        }
    }
*/
    return "";
}

int CQmlSourceManage::getLocalIOMapCount(int groupIndex)
{
    std::vector<t_AudioGroup>&List = CLogic::GetInstance()->GetGroupList();
    if((int)List.size()>groupIndex)
    {
        return List[groupIndex].vPlay.size();
    }
    return -1;
}

int CQmlSourceManage::addLocalFragment(unsigned int group, unsigned int index)
{
    IOMapFrag tmp;
    tmp.groupIndex = group;
    tmp.iomapIndex = index;

    m_localFrag.push_back(tmp);

    return 0;
}

int CQmlSourceManage::clearLocalFragment()
{
    m_localFrag.clear();

    return 0;
}

int CQmlSourceManage::getLocalFragmentSize()
{
    return m_localFrag.size();
}

bool CQmlSourceManage::isOverLocalSourceMaxCount()
{
    long localSourceMaxCount = CLogic::GetInstance()->GetInstance()->ReadConfigInit("limit", "local_source_max_count");

    if (getLocalFragmentSize() >= localSourceMaxCount)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int CQmlSourceManage::fillLocalSource(std::vector<t_FILEMAP> &vPath)
{
    QString fullName;
    std::vector<IOMapFrag>::iterator i;
    for (i = m_localFrag.begin(); i != m_localFrag.end(); i++)
    {
        t_FILEMAP file;
        if(getLocalFullName((*i).groupIndex, (*i).iomapIndex,file))
        {
            vPath.push_back(file);
        }
    }

    return 0;
}

bool CQmlSourceManage::getLocalFullName(int groupIndex, int index,t_FILEMAP &file)
{
    std::vector<t_AudioGroup>&List = CLogic::GetInstance()->GetGroupList();
    if((int)List.size()>groupIndex)
    {
        if((int)List[groupIndex].vPlay.size()>index)
        {
            t_Play &play = List[groupIndex].vPlay[index];
            file.nChannel = play.iChannel;
            file.nTitle = play.iTitle;
            file.strName = play.strDesc;
            if(CLogic::GetInstance()->IsSecurity())
            {
                file.vPath = play.vAudioList;
            }
            else
            {
                file.vPath.push_back(play.strPath);
            }
            return true;
        }
    }
    return false;
}

int CQmlSourceManage::getNodeCount()
{
    std::vector<CSysState> &vList = CLogic::GetInstance()->GetNetworkList();

    return vList.size();
}

int CQmlSourceManage::isScu(int index)
{
    std::vector<CSysState> &vList = CLogic::GetInstance()->GetNetworkList();

    if ((unsigned int)index > vList.size() - 1)
    {
        return -1;
    }

    if (vList[index].eType == DEVICE_SCU)
    {
        return 0;
    }

    return -1;
}

int CQmlSourceManage::isDOM(int index)
{
    std::vector<CSysState> &vList = CLogic::GetInstance()->GetNetworkList();

    if ((unsigned int)index > vList.size() - 1)
    {
        return -1;
    }

    if ((vList[index].eType == DEVICE_4_8) || (vList[index].eType == DEVICE_4_24))
    {
        return 0;
    }

    return -1;
}

int CQmlSourceManage::isETCS(int index)
{
    std::vector<CSysState> &vList = CLogic::GetInstance()->GetNetworkList();

    if ((unsigned int)index > vList.size() - 1)
    {
        return -1;
    }

    if (vList[index].eType == DEVICE_ETCS)
    {
        return 0;
    }

    return -1;
}

int CQmlSourceManage::getNode(int index)
{
    std::vector<CSysState> &vList = CLogic::GetInstance()->GetNetworkList();

    if ((unsigned int)index > vList.size() - 1)
    {
        return -1;
    }

    return vList[index].nNode;
}

int CQmlSourceManage::getDeviceType(int index)
{
    std::vector<CSysState> &vList = CLogic::GetInstance()->GetNetworkList();

    if ((unsigned int)index > vList.size() - 1)
    {
        return -1;
    }

    return (int)vList[index].eType;
}

QString CQmlSourceManage::getNodeStrValue(int index, QString value)
{
    std::vector<CSysState> &vList = CLogic::GetInstance()->GetNetworkList();

    if ((unsigned int)index > vList.size() - 1)
    {
        return "";
    }

    if (value == "name")
    {
        return QString::fromStdString(vList[index].strName);
    }
    else if (value == "ip")
    {
        return QString::fromStdString(vList[index].strIP);
    }

    return "";
}

int CQmlSourceManage::getNodeIntValue(int index, QString value)
{
    std::vector<CSysState> &vList = CLogic::GetInstance()->GetNetworkList();

    if ((unsigned int)index > vList.size() - 1)
    {
        return -1;
    }

    if (value == "node")
    {
         return vList[index].nNode;
    }

    return -1;
}

bool CQmlSourceManage::getNodeBoolValue(int index, QString value)
{
    std::vector<CSysState> &vList = CLogic::GetInstance()->GetNetworkList();

    if ((unsigned int)index > vList.size() - 1)
    {
        return false;
    }

    if (value == "enable")
    {
        return vList[index].IsConnect;
    }
    else if (value == "sourceVisible")
    {
        return vList[index].IsSourceVisible;
    }
    else
    {
        ;
    }

    return true;
}

QString CQmlSourceManage::loadNetIOMAP (int node)
{
    return QString::fromStdString(CLogic::GetInstance()->LoadNetIOMAP(node));
}

int CQmlSourceManage::clearNetworkSourceList()
{
    m_IOMapNetwork.clear();

    return 0;
}

int CQmlSourceManage::getNetworkSourceList(QString file)
{
    tinyxml2::XMLDocument xml;
    IOMapGroup iomapGroup;
    IOMAP_ITEM iomapItem;
    unsigned int groupCount = 0;
    unsigned int itemCount = 0;
    QString tmpFile;

    tmpFile = file;
    if (file.contains(TXT_SUFFIX) == true)
    {
        LIB_CONFIGM::LoadSCUDatabase(file.toStdString().c_str());
        tmpFile = file.replace(QString(TXT_SUFFIX), QString(XML_SUFFIX));
        LIB_CONFIGM::SaveConfigFile(tmpFile.toStdString().c_str());
    }

    if (tinyxml2::XMLError::XML_SUCCESS != xml.LoadFile(tmpFile.toStdString().c_str()))
    {
        qDebug() << "get network source load tmpFile error:" << tmpFile;
        return -1;
    }

    tinyxml2::XMLElement* root = xml.FirstChildElement(CONFIG_IOMAP);
    if (NULL == root)
    {
        return -1;
    }
    tinyxml2::XMLElement* group = root->FirstChildElement(COMMON_GROUP);

    groupCount = 0;
    while (NULL != group)
    {
        iomapGroup.Group = group->Attribute(GROUP_NAME_ATTR);

        tinyxml2::XMLElement* iomap = group->FirstChildElement(IOMAP_ITEM_NAME);
        itemCount = 0;
        while (NULL != iomap)
        {
            iomapItem.IndexFst = iomap->Attribute(INDEX_FST_ATTR);
            iomapItem.IndexSnd = iomap->Attribute(INDEX_SND_ATTR);
            iomapItem.Filename = iomap->Attribute(FILENAME_ATTR);
            iomapItem.Description = iomap->Attribute(DESCRIPTION_ATTR);
            iomapGroup.ItemList.insert(itemCount, iomapItem);
            itemCount++;
            iomap = iomap->NextSiblingElement(IOMAP_ITEM_NAME);
        }

        m_IOMapNetwork.insert(groupCount, iomapGroup);
        iomapGroup.Group = "";
        iomapGroup.ItemList.clear();
        groupCount++;
        group = group->NextSiblingElement(COMMON_GROUP);
    }

#if defined(_SOURCE_MANAGE) && (_SOURCE_MANAGE > 0)
    for (int i = 0; i < m_IOMapNetwork.size(); i++)
    {
        for (int j = 0; j < m_IOMapNetwork[i].ItemList.size(); j++)
        {
            qDebug() << "network iomap IndexFst " << m_IOMapNetwork[i].ItemList[j].IndexFst;
            qDebug() << "network iomap IndexSnd " << m_IOMapNetwork[i].ItemList[j].IndexSnd;
            qDebug() << "network iomap Filename " << m_IOMapNetwork[i].ItemList[j].Filename;
            qDebug() << "network iomap Description " << m_IOMapNetwork[i].ItemList[j].Description;
        }
    }
#endif

    return 0;
}

int CQmlSourceManage::getNodeInputCount(int node)
{
    std::vector<CSysState> &vList = CLogic::GetInstance()->GetNetworkList();

    int count = 0;
    count = vList.size();
    for (int i = 0; i < count; i++)
    {
        if (vList[i].nNode == node)
        {
            return vList[i].vDALDevice.size();
        }
    }

    return -1;
}

QString CQmlSourceManage::getNodeName(int node)
{
    std::vector<CSysState> &vList = CLogic::GetInstance()->GetNetworkList();

    int count = 0;
    count = vList.size();
    for (int i = 0; i < count; i++)
    {
        if (vList[i].nNode == node)
        {
            return QString::fromStdString(vList[i].strName);
        }
    }

    return "";
}

int CQmlSourceManage::getNodeDeviceType(int node)
{
    std::vector<CSysState> &vList = CLogic::GetInstance()->GetNetworkList();

    int count = 0;
    count = vList.size();
    for (int i = 0; i < count; i++)
    {
        if (vList[i].nNode == node)
        {
            return (int)vList[i].eType;
        }
    }

    return -1;
}

int CQmlSourceManage::getNodeInputDal(int node, int pos)
{
    std::vector<CSysState> &vList = CLogic::GetInstance()->GetNetworkList();

    int nodeCount = vList.size();
    for (int i = 0; i < nodeCount; i++)
    {
        if (vList[i].nNode == node)
        {
            if ((unsigned int)pos > vList[i].vDALDevice.size() - 1)
            {
                return -1;
            }

            return vList[i].vDALDevice[pos].nDal;
        }
    }

    return -1;
}

QString CQmlSourceManage::getNodeInputStrValue(int node, int dal, QString key)
{
    QString value = "";
    std::vector<CSysState> &vList = CLogic::GetInstance()->GetNetworkList();

    int nodeCount = 0;
    int dalCount = 0;
    nodeCount = vList.size();
    for (int i = 0; i < nodeCount; i++)
    {
        if (vList[i].nNode == node)
        {
            dalCount = vList[i].vDALDevice.size();
            for (int j = 0; j < dalCount; j++)
            {
                if (dal == vList[i].vDALDevice[j].nDal)
                {
                    if (key == "name")
                    {
                        value = QString::fromStdString(vList[i].vDALDevice[j].strName);
                        return value;
                    }
                    else if (key == "type")
                    {
                        value = QString::fromStdString(vList[i].vDALDevice[j].strType);
                        return value;
                    }
                }
            }
        }
    }

    return value;
}

int CQmlSourceManage::getNodeInputIntValue(int node, int dal, QString key)
{
    std::vector<CSysState> &vList = CLogic::GetInstance()->GetNetworkList();

    int nodeCount = 0;
    int dalCount = 0;
    nodeCount = vList.size();
    for (int i = 0; i < nodeCount; i++)
    {
        if (vList[i].nNode == node)
        {
            dalCount = vList[i].vDALDevice.size();
            for (int j = 0; j < dalCount; j++)
            {
                if (dal == vList[i].vDALDevice[j].nDal)
                {
                    if(key == "micPort")
                    {
                        return vList[i].vDALDevice[j].nMicPort;

                    }
                    else if (key == "lineinPort")
                    {
                        return vList[i].vDALDevice[j].nLineInPort;
                    }
                }
            }
        }
    }

    return -1;
}

int CQmlSourceManage::getGroupCount()
{
   return m_IOMapNetwork.size();
}

QString CQmlSourceManage::getGroupName(int groupIndex)
{
    QMap<int, IOMapGroup>::iterator i;

    for (i = m_IOMapNetwork.begin(); i != m_IOMapNetwork.end(); i++)
    {
        if (i.key() == groupIndex)
        {
            return i.value().Group;
        }
    }

    return "";
}

QString CQmlSourceManage::getGroupValue(int groupIndex, int index, QString attr)
{
    QMap<int, IOMapGroup>::iterator i;
    QMap<int, IOMAP_ITEM>::iterator j;

    for (i = m_IOMapNetwork.begin(); i != m_IOMapNetwork.end(); i++)
    {
        if (i.key() == groupIndex)
        {
            for (j = i.value().ItemList.begin(); j != i.value().ItemList.end(); j++)
            {
                if (j.key() == index)
                {
                    if (attr == "channel")
                    {
                       return j.value().IndexFst;
                    }
                    if (attr == "title")
                    {
                       return j.value().IndexSnd;
                    }
                    if (attr == "file")
                    {
                       return j.value().Filename;
                    }
                    if (attr == "Desc")
                    {
                       return j.value().Description;
                    }
                }
            }
        }
    }

    return "";
}

int CQmlSourceManage::getIOMapCount(int groupIndex)
{
    QMap<int, IOMapGroup>::iterator i;

    for (i = m_IOMapNetwork.begin(); i != m_IOMapNetwork.end(); i++)
    {
        if (i.key() == groupIndex)
        {
            return i.value().ItemList.size();
        }
    }

    return -1;
}

int CQmlSourceManage::addFragment(unsigned int group, unsigned int index)
{
    IOMapFrag tmp;
    tmp.groupIndex = group;
    tmp.iomapIndex = index;

    m_networkFrag.push_back(tmp);

    return 0;
}

int CQmlSourceManage::clearFragment()
{
    m_networkFrag.clear();

    return 0;
}

int CQmlSourceManage::getFragmentSize()
{
    return m_networkFrag.size();
}

bool CQmlSourceManage::isOverNetworkSourceMaxCount()
{
    long networkSourceMaxCount = CLogic::GetInstance()->GetInstance()->ReadConfigInit("limit", "network_source_max_count_v11");

    if (getFragmentSize() >= networkSourceMaxCount)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int CQmlSourceManage::fillChannelAndTitle(std::vector<t_Audio> &vAudio)
{
    t_Audio tmp;
    std::vector<IOMapFrag>::iterator i;
    for (i = m_networkFrag.begin(); i != m_networkFrag.end(); i++)
    {
        tmp = getNetworkChannelAndTitle((*i).groupIndex, (*i).iomapIndex);
        vAudio.push_back(tmp);
    }

    return 0;
}

t_Audio CQmlSourceManage::getNetworkChannelAndTitle(int groupIndex, int index)
{
    t_Audio tmp;
    QMap<int, IOMapGroup>::iterator i;
    QMap<int, IOMAP_ITEM>::iterator j;

    for (i = m_IOMapNetwork.begin(); i != m_IOMapNetwork.end(); i++)
    {
        if (i.key() == groupIndex)
        {
            for (j = i.value().ItemList.begin(); j != i.value().ItemList.end(); j++)
            {
                if (j.key() == index)
                {
                    tmp.nChannel = j.value().IndexFst.toInt();
                    tmp.nTitle = j.value().IndexSnd.toInt();
                    return tmp;
                }
            }
        }
    }

    return tmp;
}

void CQmlSourceManage::getSourceName(QString file, QString &dir, QString &name)
{
    QString tmp;

    int pos;
    pos = file.lastIndexOf("/");
    name = file.right(file.length() - pos - 1);
    tmp = file.left(pos);
    pos = tmp.lastIndexOf("/");
    dir = tmp.right(tmp.length() - pos - 1);

    return;
}

int CQmlSourceManage::setSourceStatus(QString dir, QString name, int status)
{
    if (dir == "usbAudio")
    {
        setUsbSourceStatus(name, status);
    }
    return 0;
}

void CQmlSourceManage::volumeSignalGenerator(float dB)
{
    CLogic::GetInstance()->SetVolumeSignalGenerator(dB);

    return;
}

void CQmlSourceManage::volumeAjust(bool capture, int port, float dB)
{
    CLogic::GetInstance()->SetVolume(capture, port, dB);
    return;
}

void CQmlSourceManage::volumeControl(int node,  unsigned char type, unsigned short number, unsigned char channel, float dB)
{
    t_VDevice dev;

    dev.type = (e_ASTYPE) type;
    dev.channel = channel;
    dev.number = number;

    CLogic::GetInstance()->VolumeControl(node, dev, dB);

    return ;
}

void CQmlSourceManage::updateVolumeControl(int node)
{
    CLogic::GetInstance()->UpdateVolumeControl(node);
}

int CQmlSourceManage::setNodeVolumeList(CVolResult volList)
{
    m_volResult = volList;

    return 0;
}

double CQmlSourceManage::getVolumeFromNodeDev(int node, unsigned char type, unsigned short number, unsigned char channel)
{
    if (node != m_volResult.nNode)
    {
        return -1000;
    }

    for (unsigned int i = 0; i < m_volResult.vDeviceVol.size(); i++)
    {
        if ((type == m_volResult.vDeviceVol[i].dev.type) && (number == m_volResult.vDeviceVol[i].dev.number) && (channel == m_volResult.vDeviceVol[i].dev.channel))
        {
            return m_volResult.vDeviceVol[i].fdB;
        }
    }

    return -1000;
}

double CQmlSourceManage::getMinVolumeFromNodeDev(int node,
                                              unsigned char type,
                                              unsigned short number,
                                              unsigned char channel
                                              )
{
    if (node != m_volResult.nNode)
    {
        return -1000;
    }

    for (unsigned int i = 0; i < m_volResult.vDeviceVol.size(); i++)
    {
        if ((type == m_volResult.vDeviceVol[i].dev.type) && (number == m_volResult.vDeviceVol[i].dev.number) && (channel == m_volResult.vDeviceVol[i].dev.channel))
        {
            return m_volResult.vDeviceVol[i].fMindB;
        }
    }

    return -1000;
}

double CQmlSourceManage::getMaxVolumeFromNodeDev(int node,
                                              unsigned char type,
                                              unsigned short number,
                                              unsigned char channel
                                              )
{
    if (node != m_volResult.nNode)
    {
        return -1000;
    }

    for (unsigned int i = 0; i < m_volResult.vDeviceVol.size(); i++)
    {
        if ((type == m_volResult.vDeviceVol[i].dev.type) && (number == m_volResult.vDeviceVol[i].dev.number) && (channel == m_volResult.vDeviceVol[i].dev.channel))
        {
            return m_volResult.vDeviceVol[i].fMaxdB;
        }
    }

    return -1000;
}

int CQmlSourceManage::setIsRun(bool isRun)
{
    m_isRun = isRun;
    PA::LogDebug("%s Line %d : %s : %d\n", __FILE__, __LINE__, "setIsRun", isRun);
    return 0;
}

bool CQmlSourceManage::getIsRun()
{
    return m_isRun;
}

int CQmlSourceManage::getUDiskFiles(QList<CFileData> &fileList)
{
     QStringList filter;
     //filter << "*.adp" << "*.mp3" << "*.wav";
     filter << "*.adp" << "*.wav" << "*.WAV";
     m_dir.setNameFilters(filter);
     m_dir.setSorting(QDir::Name);

     int count = 0;
     long usbMaxCount = CLogic::GetInstance()->GetInstance()->ReadConfigInit("limit", "usb_source_max_count");
     QList<QFileInfo> fileInfo = m_dir.entryInfoList(filter);

     if (usbMaxCount > fileInfo.count())
     {
        count = fileInfo.count();
     }
     else
     {
        count = usbMaxCount;
     }

     for (int i = 0; i < count; i++)
     {
         CFileData tmpFd;
         tmpFd.fileName = m_dir.path() + "/" + fileInfo.at(i).fileName();
         tmpFd.simpleFileName = fileInfo.at(i).fileName();

#if defined(_SOURCE_MANAGE) && (_SOURCE_MANAGE > 0)
         qDebug() << "**********************************";
         QByteArray ba;
         qDebug() << tmpFd.simpleFileName;
         qDebug() << ba.append(tmpFd.simpleFileName).toHex() << endl;
         qDebug() << "**********************************";
#endif

         fileList.append(tmpFd);
     }

     return 0;
}

bool CQmlSourceManage::isAudioFileExist(QString fullName)
{
    QFileInfo fileInfo(fullName);
    return fileInfo.exists();
}
