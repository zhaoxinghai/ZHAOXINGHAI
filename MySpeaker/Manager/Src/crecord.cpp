#include <QDir>
#include <QFile>
#include <QDebug>
#include <mutex>
#include <QDateTime>

#include "crecord.h"
#include "Logic.h"
#include "Page.h"
#include "public.h"

static std::mutex m_mutex;

CRecord* CRecord::pRecord = NULL;

CRecord* CRecord::GetInstance()
{
    if (pRecord == NULL)
    {
        m_mutex.lock();
        if (pRecord == NULL)
        {
            pRecord = new CRecord();
        }
        m_mutex.unlock();
    }
    return pRecord;
}

void CRecord::Release()
{
    if (NULL != pRecord)
    {
        m_mutex.lock();
        if (NULL != pRecord)
        {
            delete pRecord;
            pRecord = NULL;
        }
        m_mutex.unlock();
    }
}

CRecord::CRecord()
{

}

void CRecord::init()
{
    QDir dir;
#ifdef WIN32
    dir.mkdir(getCurrentPath() + QString(RECORD_DIR));
    dir.mkdir(getCurrentPath() + QString(RECORD_CACHE_DIR));
#else
    dir.mkdir(QString(SD_CARD_PATH) + RECORD_DIR);
    dir.mkdir(QString(SD_CARD_PATH) + RECORD_CACHE_DIR);
#endif

    setRecordId(0);
    setPrelistenId(0);
    setCachePrelistenId(0);

    clearRecordList();
    getRecordList();

    checkAndInitRecordGroup();
}

int CRecord::checkAndInitRecordGroup()
{
    initRecordsToIOmap();
    return 0;
}

int CRecord::initRecordsToIOmap()
{
    QString baseName;
    int count;

    count = getRecordListCount();
    for (int i = 0; i < count; i++)
    {
        baseName = getItemStrValue(i, "baseName");
        if (RECORD_AUTO_SAVE_NAME != baseName)
        {
            CLogic::GetInstance()->insertRecord(baseName.toStdString());
        }
    }

    return 0;
}

int CRecord::clearRecordList()
{
    m_recordList.clear();

    return 0;
}

int CRecord::getRecordList()
{
#ifdef WIN32
    QString runPath = getCurrentPath() + RECORD_DIR;
#else
    QString runPath = QString(SD_CARD_PATH) + RECORD_DIR;
#endif
    QDir dir(runPath);

    if (!dir.exists())
    {
        qDebug("Cannot find the record directory");
    }

    createAutoSave();

    QStringList filter;
    filter << "*.adp";
    dir.setNameFilters(filter);
    dir.setSorting(QDir::Time);
    QList<QFileInfo> fileInfo = dir.entryInfoList(filter, QDir::Files, QDir::Time);

    moveFileToEndFromQlist(fileInfo, RECORD_AUTO_SAVE_NAME);

    int record_cout = fileInfo.count();
    for (int i = 0; i < record_cout; i++)
    {
        //Time reverse
        int file_index = record_cout - 1 - i;

        QString basename = fileInfo.at(file_index).baseName();
        insertRecordToRecordList(basename);
    }

    return 0;
}



int CRecord::getRecordListCount()
{
    return m_recordList.size();
}

int CRecord::getItemIndex(unsigned int pos)
{
    if ((int)pos > m_recordList.size())
    {
        return -1;
    }

    QMap<int, recordNode_t>::iterator it;
    it = m_recordList.begin() + pos;

    return it.value().index;
}

QString CRecord::getItemStrValue(int index, QString key)
{
    QMap<int, recordNode_t>::iterator it;

    for (it = m_recordList.begin(); it != m_recordList.end(); it++)
    {
        if (index == it.value().index)
        {
            if (key == "name")
            {
                return it.value().fileName;
            }
            else if(key == "baseName")
            {
               QFileInfo fileInfo(it.value().fileName);
               return fileInfo.baseName();
            }
            else if (key == "time")
            {
                return it.value().record_time;
            }
            else if (key == "size")
            {
                unsigned int min;
                unsigned int sec;

                min = it.value().second % 3600 / 60;
                sec =it.value().second % 3600 % 60;

                return QString("%1:%2").arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
            }
        }
    }

    return "";
}

int CRecord::getItemIntValue(int index, QString key)
{
    (void)index;
    (void)key;

    return 0;
}

bool CRecord::getItemBoolValue(int index, QString key)
{
    QMap<int, recordNode_t>::iterator it;

    for (it = m_recordList.begin(); it != m_recordList.end(); it++)
    {
        if (index == it.value().index)
        {
            if (key == "match")
            {
                return it.value().match;
            }
            else if (key == "select")
            {
                return it.value().select;
            }
        }
    }

    return true;
}

int CRecord::startRecord()
{
    QString filePath;
    setRecordId(CLogic::GetInstance()->GetCallID());

#ifdef WIN32
    filePath = getCurrentPath() + RECORD_CACHE_DIR + "tmpRecord.adp";
#else
    filePath = QString(SD_CARD_PATH) + RECORD_CACHE_DIR + "tmpRecord.adp";
#endif

    return PA::LocalRecord(getRecordId(), 41, filePath.toStdString());
}

int CRecord::stopRecord()
{
    PA::StopLocalRecord(getRecordId());

    return 0;
}

int CRecord::prelistenCacheRecord()
{
    t_LocalOption opt;
    std::vector<std::string> vPath;
    CUSBPage local;

#ifdef WIN32
    vPath.push_back((getCurrentPath() + RECORD_CACHE_DIR + "tmpRecord.adp").toStdString());
#else
    vPath.push_back((QString(SD_CARD_PATH) + RECORD_CACHE_DIR + "tmpRecord.adp").toStdString());
#endif

    setCachePrelistenId(CLogic::GetInstance()->GetCallID());
    return local.PreListen(getCachePrelistenId(), vPath, opt);
}

int CRecord::stopPrelistenCacheRecord()
{
    CLocalPage local;
    local.StopPreListen(getCachePrelistenId());

    return 0;
}

int CRecord::prelistenRecord(int index)
{
    t_LocalOption opt;
    std::vector<std::string> vPath;
    CUSBPage local;

#ifdef WIN32
    vPath.push_back((getCurrentPath() + RECORD_DIR + getItemStrValue(index, "name")).toStdString());
#else
    vPath.push_back((QString(SD_CARD_PATH) + RECORD_DIR + getItemStrValue(index, "name")).toStdString());
#endif

    setPrelistenId(CLogic::GetInstance()->GetCallID());
    return local.PreListen(getPrelistenId(), vPath, opt);
}

int CRecord::stopPrelistenRecord()
{
    CLocalPage local;
    local.StopPreListen(getPrelistenId());

    return 0;
}

int CRecord::saveRecord(QString baseName)
{
#ifdef WIN32
    QString tmpPath = getCurrentPath() + RECORD_CACHE_DIR + "tmpRecord.adp";
    QString path = getCurrentPath() + RECORD_DIR + baseName +".adp";
#else
    QString tmpPath = QString(SD_CARD_PATH) + RECORD_CACHE_DIR + "tmpRecord.adp";
    QString path = QString(SD_CARD_PATH) + RECORD_DIR + baseName + ".adp";
#endif

    QFile::rename(tmpPath, path);

#ifndef WIN32
    sync();
#endif

    return 0;
}

int CRecord::insertRecordToIOmap(QString baseName)
{
    CLogic::GetInstance()->insertRecord(baseName.toStdString());
    return 0;
}

int CRecord::updateRecord(QString oldBaseName, QString newBaseName)
{
#ifdef WIN32
    QString oldPath = getCurrentPath() + RECORD_DIR + oldBaseName +".adp";
    QString newPath = getCurrentPath() + RECORD_DIR + newBaseName +".adp";
#else
    QString oldPath = QString(SD_CARD_PATH) + RECORD_DIR + oldBaseName + ".adp";
    QString newPath = QString(SD_CARD_PATH) + RECORD_DIR + newBaseName + ".adp";
#endif

    QFile::rename(oldPath, newPath);

    return 0;
}

int CRecord::updateRecordToIOmap(QString oldBaseName, QString newBaseName)
{
    CLogic::GetInstance()->updateRecord(oldBaseName.toStdString(),newBaseName.toStdString());
    return 0;
}

int CRecord::removeItemFromRecordGroup(QString baseName)
{
    CLogic::GetInstance()->removeRecord(baseName.toStdString());
    return 0;
}

int CRecord::removeAllCacheRecord()
{
    QString cmd;

#ifdef WIN32
    QString file= getCurrentPath() + RECORD_CACHE_DIR + "tmpRecord.adp";
    QFile::remove(file);
#else
    QString file= QString(SD_CARD_PATH) + RECORD_CACHE_DIR + "tmpRecord.adp";
    QFile::remove(file);
    //cmd = QString("rm ") + SD_CARD_PATH + RECORD_CACHE_DIR + "/* -rf";
    //CLogic::GetInstance()->System(cmd.toStdString().c_str());
#endif

    return 0;
}

int CRecord::stopPrelistenFromId(int id)
{
    CLocalPage local;
    local.StopPreListen(id);

    return 0;
}

int CRecord::setRecordId(int id)
{
    m_recordId = id;

    return 0;
}

int CRecord::getRecordId()
{
    return m_recordId;
}

int CRecord::setPrelistenId(int id)
{
    m_prelistenId = id;

    return 0;
}

int CRecord::getPrelistenId()
{
    return m_prelistenId;
}

int CRecord::setCachePrelistenId(int id)
{
    m_cachePrelistenId = id;

    return 0;
}

int CRecord::getCachePrelistenId()
{
    return m_cachePrelistenId;
}

int CRecord::insertRecordToRecordList(QString baseName)
{
    QString fileName = baseName + ".adp";

#ifdef WIN32
    QFile file(getCurrentPath() + RECORD_DIR + fileName);
    QFileInfo fileInfo(getCurrentPath() + RECORD_DIR+ fileName);
#else
    QFile file(QString(SD_CARD_PATH) + RECORD_DIR + fileName);
    QFileInfo fileInfo(QString(SD_CARD_PATH) + RECORD_DIR + fileName);
#endif

    recordNode_t tmpNode;
    if (m_recordList.count() == 0)
    {
        tmpNode.index = 0;
    }
    else
    {
        QMap<int, recordNode_t>::iterator it;
        it = m_recordList.end() - 1;
        tmpNode.index = it.value().index + 1;
    }

    tmpNode.fileName = fileName;
    tmpNode.second = getFileSize(file.size());
    QTime tmpTime = fileInfo.created().time();
    QDate tmpDate = fileInfo.created().date();
    tmpNode.record_time = GetRecordTime(tmpDate,tmpTime);
    tmpNode.select = false;
    tmpNode.match = PA::CheckAdpFile(fileInfo.absoluteFilePath().toStdString());
    m_recordList.insert(tmpNode.index, tmpNode);


    return tmpNode.index;
}

QString CRecord::GetRecordTime(QDate &data,QTime &tm)
{
    QString record_time = QString("%1:%2:%3  %4/%5/%6") \
            .arg(tm.hour(), 2, 10, QChar('0')) \
            .arg(tm.minute(), 2, 10, QChar('0')) \
            .arg(tm.second(), 2, 10, QChar('0')) \
            .arg(data.month(), 2, 10, QChar('0')) \
            .arg(data.day(), 2, 10, QChar('0')) \
            .arg(data.year(), 2, 10, QChar('0'));
    return record_time;
}

int CRecord::updateRecordToRecordList(QString oldBaseName, QString newBaseName)
{
    QString oldFileName = oldBaseName +".adp";
    QString newFileName = newBaseName +".adp";

    QMap<int, recordNode_t>::iterator it;
    for (it = m_recordList.begin(); it != m_recordList.end(); it++)
    {
        if (oldFileName == it.value().fileName)
        {
            it.value().fileName = newFileName;
            return 0;
        }
    }

    return -1;
}

int CRecord::removeRecordFromRecordList(int index)
{
#ifdef WIN32
     QString file = getCurrentPath() + RECORD_DIR+ getItemStrValue(index, "name");
#else
     QString file = QString(SD_CARD_PATH) + RECORD_DIR + getItemStrValue(index, "name");
#endif

    QFile::remove(file);

    QMap<int, recordNode_t>::iterator it;

    for (it = m_recordList.begin(); it != m_recordList.end(); it++)
    {
        if (index == it.value().index)
        {
            m_recordList.erase(it);
            break;
        }
    }

    return 0;
}

int CRecord::printRecordList()
{
    QMap<int, recordNode_t>::iterator it;

    for (it = m_recordList.begin(); it != m_recordList.end(); it++)
    {
        qDebug() << "record index = " << it.value().index;
        qDebug() << "record fileName = " << it.value().fileName;
        qDebug() << "record second = " << it.value().second;
        qDebug() << "record match = " << it.value().match;
        qDebug() << "record select = " << it.value().select;
    }

    return 0;
}

QString CRecord::getFormatTime(int count)
{
    int min;
    int sec;

    min = count / 60;
    sec = count % 60;

    return QString("%1:%2").arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
}

int CRecord::getRecordMaxTime()
{
    long recordMaxTime;
    recordMaxTime = CLogic::GetInstance()->GetInstance()->ReadConfigInit("limit", "recorder_max_time");
    if (recordMaxTime <= 0)
    {
        return 30;
    }
    else
    {
        return recordMaxTime;
    }
}

bool CRecord::isOverRecordMaxCount()
{
    long recordMaxCount = CLogic::GetInstance()->GetInstance()->ReadConfigInit("limit", "recorder_max_count");

    if (getRecordListCount() >= recordMaxCount)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CRecord::isOverRecordStorage()
{
    int type = DIR_RECORD;

    int usedStorage = CLogic::GetInstance()->GetDirUsedSize((e_FOLDER)type);
    int totalStorage = CLogic::GetInstance()->GetDirTotalSize((e_FOLDER)type);

    if (usedStorage >= totalStorage)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int CRecord::getRemainRecordSecond()
{
    int type = DIR_RECORD;
    int usedStorage = CLogic::GetInstance()->GetDirUsedSize((e_FOLDER)type);
    int totalStorage = CLogic::GetInstance()->GetDirTotalSize((e_FOLDER)type);

    int remainStorage = totalStorage - usedStorage;

    if (remainStorage <= 1)
    {
        return -1;
    }
    int kbPerSecond = 54;
    int remainSecond = remainStorage * 1024 / kbPerSecond;


    int maxTime = getRecordMaxTime();
    int maxSecond = maxTime * 60;

    if (remainSecond > maxSecond)
    {
        return maxSecond;
    }

    return remainSecond;
}


int CRecord::selectRecordItem(int index)
{
    QMap<int, recordNode_t>::iterator it;

    for (it = m_recordList.begin(); it != m_recordList.end(); it++)
    {
        if (index == it.value().index)
        {
            it.value().select = true;
        }
    }

    return 0;
}

bool CRecord::getRecordItemSelect(int index)
{
    QMap<int, recordNode_t>::iterator it;

    for (it = m_recordList.begin(); it != m_recordList.end(); it++)
    {
        if (index == it.value().index)
        {
            return it.value().select;
        }
    }

    return true;
}

int CRecord::unselectRecordItem(int index)
{
    QMap<int, recordNode_t>::iterator it;

    for (it = m_recordList.begin(); it != m_recordList.end(); it++)
    {
        if (index == it.value().index)
        {
            it.value().select = false;
        }
    }

    return 0;
}

int CRecord::selectAllRecordItem()
{
    QMap<int, recordNode_t>::iterator it;

    for (it = m_recordList.begin(); it != m_recordList.end(); it++)
    {
        it.value().select = true;
    }

    return 0;
}

int CRecord::unselectAllRecordItem()
{
    QMap<int, recordNode_t>::iterator it;

    for (it = m_recordList.begin(); it != m_recordList.end(); it++)
    {
        it.value().select = false;
    }

    return 0;

}

int CRecord::fillRecordSource(std::vector<std::string> &vPath)
{
    QMap<int, recordNode_t>::iterator it;

    for (it = m_recordList.begin(); it != m_recordList.end(); it++)
    {
        if (true == it.value().select)
        {
#ifdef WIN32
            QString file = getCurrentPath() + RECORD_DIR + it.value().fileName;
#else
            QString file = QString(SD_CARD_PATH) + RECORD_DIR + it.value().fileName;
#endif
             vPath.push_back(file.toStdString());
        }
    }

    return 0;
}

QString CRecord::getAutoName()
{
    QDateTime current = QDateTime::currentDateTime();
    QTime tmpTime = current.time();
    QDate tmpDate = current.date();
    QString name = QString("%1%2%3_%4%5%6")\
            .arg(tmpDate.year(), 2, 10, QChar('0')) \
            .arg(tmpDate.month(), 2, 10, QChar('0')) \
            .arg(tmpDate.day(), 2, 10, QChar('0'))\
            .arg(tmpTime.hour(), 2, 10, QChar('0')) \
            .arg(tmpTime.minute(), 2, 10, QChar('0')) \
            .arg(tmpTime.second(), 2, 10, QChar('0')) ;

    return name;
}

bool CRecord::doubleCheck(QString name)
{
#ifdef WIN32
    QString runPath = getCurrentPath() + QString(RECORD_DIR);
#else
    QString runPath = QString(SD_CARD_PATH) + RECORD_DIR;
#endif
    QDir dir(runPath);

    if (!dir.exists())
    {
        qDebug("Cannot find the record directory");
    }

    QStringList filter;
    filter << "*.adp";
    dir.setNameFilters(filter);
    dir.setSorting(QDir::Time);
    QList<QFileInfo> fileInfo = dir.entryInfoList(filter);

    for (int i = 0; i < fileInfo.count(); i++)
    {
        if (fileInfo.at(i).baseName() == name)
        {
            return true;
        }
    }

    return false;
}

bool CRecord::isReservedName(QString name)
{
    if (RECORD_AUTO_SAVE_NAME == name)
    {
        return true;
    }

    return false;
}

int CRecord::getFileSize(unsigned int size)
{
    unsigned int tmp;
    unsigned int tmp1;
    int second;

    //second = (size * 418) / (48000 * 480)

    tmp = size * 0.1045;
    tmp1 = 480 * 12;
    second = tmp / tmp1;

    return second;
}

bool CRecord::removeFileFromQlist(QList<QFileInfo> &fileInfo, QString filename)
{
    QMutableListIterator<QFileInfo> itr(fileInfo);
    while (itr.hasNext())
    {
        if (filename == itr.next().baseName())
        {
            itr.remove();
            return true;
        }
    }
    return false;
}

void CRecord::moveFileToEndFromQlist(QList<QFileInfo> &fileInfo, QString filename)
{
    QMutableListIterator<QFileInfo> itr(fileInfo);
    QFileInfo autosave;
    bool isAutosaveExist = false;
    while (itr.hasNext())
    {
        if (filename == itr.next().baseName())
        {
            autosave = itr.value();
            itr.remove();
            isAutosaveExist = true;
        }
    }
    if (isAutosaveExist)
    {
        fileInfo.append(autosave);
    }
}

void CRecord::updateAutosaveToRecordList()
{
#ifdef WIN32
    QString runPath = getCurrentPath() + QString(RECORD_DIR);
#else
    QString runPath = QString(SD_CARD_PATH) + RECORD_DIR;
#endif

    QString fileName = QString(RECORD_AUTO_SAVE_NAME) + ".adp";
    QFile file(runPath + fileName);
    QFileInfo fileInfo(runPath + fileName);
    if (!file.exists())
    {
        return;
    }

    QMap<int, recordNode_t>::iterator it;
    for (it = m_recordList.begin(); it != m_recordList.end(); it++)
    {
        if (fileName == it.value().fileName)
        {
            it.value().second = getFileSize(file.size());

            QTime tmpTime = fileInfo.created().time();
            QDate tmpDate = fileInfo.created().date();
            it.value().record_time = GetRecordTime(tmpDate,tmpTime);
        }
    }

}

bool CRecord::isCCCFVersion()
{
    return CLogic::GetInstance()->IsCCCFVersion();
}

QString CRecord::getAutoSaveName()
{
    return QString(RECORD_AUTO_SAVE_NAME);
}

void CRecord::createAutoSave()
{
#ifdef WIN32
    QString runPath = getCurrentPath() + QString(RECORD_DIR);
#else
    QString runPath = QString(SD_CARD_PATH) + RECORD_DIR;
#endif

    QFile file(runPath + QString(RECORD_AUTO_SAVE_NAME) + ".adp");
    if (!file.exists())
    {
        file.open(QIODevice::WriteOnly);
    }
}
