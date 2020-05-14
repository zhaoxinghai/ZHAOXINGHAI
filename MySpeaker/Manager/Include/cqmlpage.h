#ifndef CQMLPAGE_H
#define CQMLPAGE_H

#include <QObject>
#include "Logic.h"
#include "Page.h"

class CQmlPage : public QObject
{
    Q_OBJECT

public:
    explicit CQmlPage(QObject *parent = 0);

    static CQmlPage* pQmlPage;
    static CQmlPage* GetInstance();
    static void Release();

public slots:

    void init();
    int livePageStartCall(int iPri,
                          float S,
                          int iSingal,
                          bool bReconnect,
                          bool bPartly,
                          int port);
    int livePageStopCall(int id);
    int liveMakePredefine(QString name,
                          int iPri,
                          float iVolume,
                          int iSingal,
                          bool bReconnect,
                          bool bPartly,
                          int port);
    int liveMakeSchedule(int iPri,
                         float iVolume,
                         int iSingal,
                         bool bReconnect,
                         bool bPartly,
                         int port);

    int networkPageStartCall(QString callType, int node,
                            int iPri,
                            float iVolume,
                            int iSingal,
                            bool bReconnect,
                            bool bPartly,
                            int repeat,
                            int interval,
                            bool gapTime);
    int networkPageStartLiveCall(QString callType,
                                 int node,
                                 int channel,
                                 int title,
                            int iPri,
                            float iVolume,
                            int iSingal,
                            bool bReconnect,
                            bool bPartly,
                            int repeat,
                            int interval,
                            bool gapTime);
    int networkPageStopCallFromId(QString callType, int id);
    int networkMakePredefine(int node,
                             QString name,
                             int iPri,
                             float iVolume,
                             int iSingal,
                             bool bReconnect,
                             bool bPartly,
                             int repeat,
                             int interval,
                             bool gapTime);
    int networkLiveMakePredefine(int node, int channel, int title,
                             QString name,
                             int iPri,
                             float iVolume,
                             int iSingal,
                             bool bReconnect,
                             bool bPartly,
                             int repeat,
                             int interval,
                             bool gapTime);
    int networkMakeSchedule(int node, int iPri,
                            float iVolume,
                            int iSingal,
                            bool bReconnect,
                            bool bPartly,
                            int repeat,
                            int interval,
                            bool gapTime);
    int networkLiveMakeSchedule(int node, int channel, int title, int iPri,
                            float iVolume,
                            int iSingal,
                            bool bReconnect,
                            bool bPartly,
                            int repeat,
                            int interval,
                            bool gapTime);

    int baseStartCall(QString SrcType,
                      QString callType,
                      int iPri,
                      float iVolume,
                      int iPreSingal,
                      int iEndSignal,
                      bool bReconnect,
                      bool bPartly,
                      int repeat,
                      int interval,
                      bool gapTime);
    int baseStopCallFromId(QString callType, int id);
    int baseMakePredefine(QString SrcType, QString name,
                          int iPri,
                          float iVolume,
                          int iSingal,
                          bool bReconnect,
                          bool bPartly,
                          int repeat,
                          int interval,
                          bool gapTime);

    int baseMakeSchedule(QString SrcType,
                         int iPri,
                         float iVolume,
                         int iSingal,
                         bool bReconnect,
                         bool bPartly,
                         int repeat,
                         int interval,
                         bool gapTime);

    int predefinePreListen(float vol);
    int predefineStopPreListen(int id);
    int predefineCall(float vol);
    int predefineStopCallFromId(int id);
    int predefineVolumeControl();
    int predefineMakeSchedule(float vol);

    int vaLiveStartCall();
    int vaLiveStopCall(int id);
    int live();
    int evacuation();
    int alert();
    bool isEVAExist();
    bool isAlertExist();
    int GetEVAErrorCode();
    int GetAlertErrorCode();

    int getCallId();
    int setScheduleParam(int hour, int min, QString name, bool onlyOnce, int cycle, int gapTime);
    int setMday(int day, bool value);
    int clearscheduleParam();
    int clearMday();

    int getLocalIntOption(QString SrcType,QString optType);
    bool getLocalBoolOption(QString SrcType,QString optType);
    float getLocalFloatOption(QString SrcType, QString optType);
    int setLocalOption(QString SrcType,
                       int iPri,
                       float iVolume,
                       int iSingal,
                       bool bReconnect,
                       bool bPartly,
                       int repeat,
                       int interval);
    int setLocalVolume(QString SrcType, float iVolume);

    int getLiveIntOption(QString optType);
    bool getLiveBoolOption(QString optType);
    float getLiveFloatOption(QString optType);
    int setLiveOption(int iPri,
                       float iVolume,
                       int iSingal,
                       bool bReconnect,
                       bool bPartly,
                       int port);

    int setLiveVolume(float iVolume);

    int getNetworkIntOption(QString optType);
    bool getNetworkBoolOption(QString optType);
    float getNetworkFloatOption(QString optType);
    int setNetworkOption(int node, int iPri,
                       float iVolume,
                       int iSingal,
                       bool bReconnect,
                       bool bPartly,
                       int repeat,
                       int interval);
    int setNetworkVolume(float iVolume);

    int printAllOptions();

private:
    int m_callID;
    t_ScheduleParam m_param;
    bool m_day[7];

    int m_audioJobCount;
    bool m_isAudioJobCountValid;
};
#endif // CQMLPAGE_H
