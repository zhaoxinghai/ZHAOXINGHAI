#ifndef CQMLSOURCEMANAGE_H
#define CQMLSOURCEMANAGE_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QDir>
#include <QThread>
#include <vector>
#include <string>

#include "ListModel.h"
#include "Logic.h"

#define FILE_OK           0
#define FILE_NO_EXIST     4
#define FILE_FORMAT_ERROR 5

typedef struct IOMAP_ITEM
{
    QString IndexFst;
    QString IndexSnd;
    QString filePathDir;
    QString Filename;
    QString Description;
    QString fileSize;
    int errorNum;
}IOMapItem;

typedef struct IOMAP_GROUP
{
    QString Group;
    QMap<int, IOMAP_ITEM> ItemList;
}IOMapGroup;

typedef struct IOMAP_FRAG
{
    unsigned int groupIndex;
    unsigned int iomapIndex;
}IOMapFrag;

struct CBaseSource
{
    CBaseSource()
    {
        index = 0;
        select = 0;
    }

    int     index;
    bool    select;
};

struct CUsbSource : public CBaseSource
{
    CUsbSource()
    {
        fileName = "";
    }

    QString simpleFileName;
    QString fileName;
    QString cacheFileName;
};

struct CFileData{
    QString simpleFileName;
    QString fileName;
};

class CQmlSourceManage : public QThread
{
    Q_OBJECT

public:
    CQmlSourceManage();

	static CQmlSourceManage* pQmlSourceManage;
	static CQmlSourceManage* GetInstance();
	static void Release();
    void run();

signals:
    void finishUsbSourceCreateSignal();
    void timeOutSignal();

public slots:

    void init();
    int insertUsbSourceListCacheFile(QString source, QString cacheFile);
	int setUsbSourceSelected(const int index, bool select);
    int setUsbSourceStatus(QString name, int status);
    int setUsbSourceTran(QString fileName, bool tran);
    int clearAllUsbSourceSelected();
    int createUsbSourceList();
    int clearUsbSourceList();
    int fillPathListFromUsbSourceList(std::vector<std::string> &vPath);
    int createUsbSourceModel();
    int clearUsbSourceModel();
    int getUsbSourceModelCount();
    Models::ListModel *getUsbSourceModel();

    int getLocalGroupCount();
    QString getLocalGroupName(int groupIndex);
    QString getLocalGroupValue(int groupIndex, int index, QString attr);
    int getLocalGroupIntValue(int groupIndex, int index, QString attr);
    QString getLocalDescFromFilename(QString name);
    int getLocalIOMapCount(int groupIndex);

    int clearLocalFragment();
    int addLocalFragment(unsigned int group, unsigned int index);
    int getLocalFragmentSize();
    bool isOverLocalSourceMaxCount();
    int fillLocalSource(std::vector<t_FILEMAP> &vPath);
    bool getLocalFullName(int groupIndex, int index,t_FILEMAP &file);

    int getNodeCount();
    int isScu(int index);
    int isDOM(int index);
    int isETCS(int index);
    int getNode(int index);
    int getDeviceType(int index);
    QString getNodeStrValue(int index, QString value);
    int getNodeIntValue(int index, QString value);
    bool getNodeBoolValue(int index, QString value);
    QString loadNetIOMAP(int node);
    int clearNetworkSourceList();
    int getNetworkSourceList(QString file);

    QString getNodeName(int node);
    int getNodeInputCount(int node);
    int getNodeDeviceType(int node);
    int getNodeInputDal(int node, int pos);
    QString getNodeInputStrValue(int node, int dal, QString key);
    int getNodeInputIntValue(int node, int dal, QString key);

    int getGroupCount();
    QString getGroupName(int groupIndex);
    QString getGroupValue(int groupIndex, int index, QString attr);
    int getIOMapCount(int groupIndex);
    int addFragment(unsigned int group, unsigned int index);
    int clearFragment();
    int getFragmentSize();
    bool isOverNetworkSourceMaxCount();
    int fillChannelAndTitle(std::vector<t_Audio> &vAudio);
    t_Audio getNetworkChannelAndTitle(int groupIndex, int index);

    void getSourceName(QString file, QString &dir, QString &name);
    int setSourceStatus(QString dir, QString name, int status);

    void volumeSignalGenerator(float dB);
    void volumeAjust(bool capture, int port, float db);
    void volumeControl(int nNode, unsigned char type, unsigned short number, unsigned char channel, float dB);
    void updateVolumeControl(int node);

    int setNodeVolumeList(CVolResult volList);
    double getVolumeFromNodeDev(int node, unsigned char type, unsigned short number, unsigned char channel);
    double getMinVolumeFromNodeDev(int node,
                                  unsigned char type,
                                  unsigned short number,
                                  unsigned char channel
                                  );
    double getMaxVolumeFromNodeDev(int node,
                                  unsigned char type,
                                  unsigned short number,
                                  unsigned char channel
                                  );

    int setIsRun(bool isRun);
    bool getIsRun();

private:
    int getUDiskFiles(QList<CFileData> &fileList);
    bool isAudioFileExist(QString fullName);

    std::vector<CUsbSource> m_usbSourceList;
    Models::ListModel *m_usbSourceModel;
    QMap<int, IOMapGroup> m_IOMapNetwork;

    QDir m_dir;

    std::vector<IOMapFrag> m_networkFrag;
    std::vector<IOMapFrag> m_localFrag;

    std::vector<QString> m_IOMapFilelist;

    CVolResult m_volResult;

    bool m_isRun;
};

#endif // CQMLSOURCEMANAGE_H
