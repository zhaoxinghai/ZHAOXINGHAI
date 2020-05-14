#ifndef CRECORD_H
#define CRECORD_H

#include <QObject>
#include <QMap>
#include <vector>
#include <QFileInfo>

typedef struct AUDIO_RECORD_NODE
{
    int index;
    QString fileName;
    QString record_time;
    unsigned int second;
    bool select;
    bool match;
}recordNode_t;

class CRecord : public QObject
{
    Q_OBJECT
public:
    CRecord();
    static CRecord* pRecord;
    static CRecord* GetInstance();
    static void Release();

public slots:

    void init();
    int checkAndInitRecordGroup();
    int initRecordsToIOmap();
    int clearRecordList();
    int getRecordList();
    int getRecordListCount();
    int getItemIndex(unsigned int pos);
    QString getItemStrValue(int index, QString key);
    int getItemIntValue(int index, QString key);
    bool getItemBoolValue(int index, QString key);
    int insertRecordToRecordList(QString baseName);
    int updateRecordToRecordList(QString oldBaseName, QString newBaseName);
    int removeRecordFromRecordList(int index);
    int printRecordList();
    int saveRecord(QString baseName);
    int updateRecord(QString oldBaseName, QString newBaseName);
    int insertRecordToIOmap(QString baseName);
    int updateRecordToIOmap(QString oldBaseName, QString newBaseName);
    int removeItemFromRecordGroup(QString baseName);

    int removeAllCacheRecord();

    int startRecord();
    int stopRecord();
    int prelistenCacheRecord();
    int stopPrelistenCacheRecord();
    int prelistenRecord(int index);
    int stopPrelistenRecord();
    int stopPrelistenFromId(int id);
    int setRecordId(int id);
    int getRecordId();
    int setPrelistenId(int id);
    int getPrelistenId();
    int setCachePrelistenId(int id);
    int getCachePrelistenId();

    int selectRecordItem(int index);
    bool getRecordItemSelect(int index);
    int unselectRecordItem(int index);
    int selectAllRecordItem();
    int unselectAllRecordItem();
    int fillRecordSource(std::vector<std::string> &vPath);

    QString getAutoName();
    bool doubleCheck(QString name);
    bool isReservedName(QString name);
    QString getFormatTime(int count);

    int getRecordMaxTime(); //Min
    bool isOverRecordMaxCount();
    bool isOverRecordStorage();
    int getRemainRecordSecond();

    void updateAutosaveToRecordList();

    bool isCCCFVersion();

    QString getAutoSaveName();

private:
    int getFileSize(unsigned int size);

    bool removeFileFromQlist(QList<QFileInfo> &fileInfo, QString filename);

    void moveFileToEndFromQlist(QList<QFileInfo> &fileInfo, QString filename);

    void createAutoSave();
    QString GetRecordTime(QDate &data,QTime &tm);

    int m_cachePrelistenId;
    int m_prelistenId;
    int m_recordId;
    QMap<int, recordNode_t> m_recordList;
};

#endif // CRECORD_H
