#ifndef CHISTORY_H
#define CHISTORY_H

#include <QObject>
#include <vector>
#include <string>

#include "Logic.h"

class CHistory : public QObject
{
    Q_OBJECT
public:
    explicit CHistory(QObject *parent = 0);
    static CHistory* pHistory;
    static CHistory* GetInstance();
    static void Release();

signals:

public slots:
    void init();
    int getHistoryListCount();
    int getHistoryIndex(unsigned int  pos);
    bool isHistoryValid(unsigned int pos);
    QString getHistoryStrValue(int index, QString key);
    int getHistoryIntValue(int index, QString key);
    bool getHistoryBoolValue(int index, QString key);
    int getHistoryZoneCount(int index);
    QString getHistoryZone(int index, int pos);
    int getHistorySourceCount(int index);
    QString getHistorySource(int index, int pos);

    int localCall(int index);
    int networkCall(int index);
    int liveCall(int index);

    int commonUSBCall(t_LocalOption &opt, std::vector<std::string> &vPath);
    int commonLocalCall(t_LocalOption &opt, std::vector<t_FILEMAP> &vPath);
    int commonNetworkCall(t_NetworkOption &opt, std::vector<t_Audio> &vAudio);
    int commonLiveCall(t_LiveOption &opt);
    int commonStopLocalCall(int id);
    int commonStopNetworkCall(int id);
    int commonStopLiveCall(int id);

    int historyCall(int index);

    void GetHistoryLocalPath(int index,std::vector<std::string> &vPath);
    void GetHistoryIOMapPath(int index,std::vector<t_FILEMAP> &vPath);
    void getHistoryNetworkSourcePath(int index,std::vector<t_Audio> &vAudio);
    int getHistoryLivePort(int index);

    t_LocalOption getHistoryLocalCallParam(int index);
    t_NetworkOption getHistoryNetworkCallParam(int index);
    t_LiveOption getHistoryLiveCallParam(int index);

    void CleanRecent();

private:
    int m_callID;
};

#endif // CHISTORY_H
