#ifndef CSCHEDULEMANAGE_H
#define CSCHEDULEMANAGE_H

#include <QObject>
#include <Logic.h>

class CScheduleManage : public QObject
{
    Q_OBJECT
public:
    explicit CScheduleManage(QObject *parent = 0);
    static CScheduleManage* pScheduleManage;
    static CScheduleManage* GetInstance();
    static void Release();

    void init();

signals:

public slots:
    int getScheduleListCount();
    int getScheduleIndex(unsigned int pos);
    bool isIndexValid(int index);
    int getScheduleValue(int index, QString key);
    QString getScheduleStrValue(int index, QString key);
    int getScheduleIntValue(int index, QString key);
    bool getScheduleBoolValue(int index, QString key);
    bool scheduleIsLive(int index);
    bool getDay(int index, int day);
    int setMday(int day, bool value);
    int setScheduleParam(int hour, int min, QString name, bool onlyOnce, int cycle, int gapTime);
    int updateSchedule(int index, bool myConfig);
    int delSchedule(int index);
    int openSchedule(int index, bool myConfig);
    int closeSchedule(int index, bool myConfig);
    int clearAllData();
    int AllScheduleOn(bool value);
    bool isAllScheduleOff();
    bool isAllScheduleError();
    QString getStrTime(int one, int two);
    bool isOverScheduleMaxCount();

private:

    int clearMday();
    int clearscheduleParam();

    bool m_day[7];
    t_ScheduleParam m_param;

};

#endif // CSCHEDULEMANAGE_H
