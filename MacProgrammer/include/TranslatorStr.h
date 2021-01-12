#ifndef TRANSLATORSTR_H
#define TRANSLATORSTR_H

#include <QObject>

class TranslatorStr : public QObject
{
    Q_OBJECT
public:
    explicit TranslatorStr(QObject *parent = 0);
    ~TranslatorStr();

    static QString getProduceStr();
    static QString getProjectStr();
    static QString getEnabledStr();
    static QString getDisabledStr();
    static QString getRepeatStr();
    static QString getLoopStr();
    static QString getShuffleStr();
    static QString getGeneralStr();
    static QString getInvalidStr();
    static QString getUnusedStr();
    static QString getBusyStr();
    static QString getDefectiveStr();
    static QString getUnconnectedStr();
    static QString getConnectedStr();
    static QString getAlarmStr();
    static QString getOnlineStr();
    static QString getOfflineStr();
    static QString getMondayStr();
    static QString getTuesdayStr();
    static QString getWednesdayStr();
    static QString getThursdayStr();
    static QString getFridayStr();
    static QString getSaturdayStr();
    static QString getSundayStr();
    static QString getAS12XXStr();
    static QString getAS1200CSStr();
    static QString getAS1200WSStr();
    static QString getAS1201Str();
    static QString getAS1203Str();
    static QString getAS1204Str();
    static QString getAS1205Str();
    static QString getAS1206Str();
    static QString getAS1207Str();

signals:

public slots:

};

#endif // TRANSLATORSTR_H
