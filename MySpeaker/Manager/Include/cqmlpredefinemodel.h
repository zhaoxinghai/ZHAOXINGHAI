#ifndef CQMLPREDEFINEMODEL_H
#define CQMLPREDEFINEMODEL_H

#include "ListItem.h"

class CQmlPredefineModel : public Models::ListItem
{
    Q_OBJECT

public:
    explicit CQmlPredefineModel(QObject *parent = NULL);
    explicit CQmlPredefineModel(const int &index,
                               const QString &name,
                                const int &selected,
                                const int &type,
                                const float &volume,
                                const int &myConfig,
                                const bool &isError,
                                const int &predefineType,
                                const QString &color,
                                const QString &text,
                                const int &borderWidth,
                                const QString &borderColor,
                                const int &callId,
                                const QString &callStatus,
                                const bool &haveGong,
                                const bool &timerTmp,
                                const int &timerCount,
                                const int &timerMode,
                               QObject *parent = NULL);

    enum zoneEnum
    {
        e_index = 1,
        e_name,
        e_selected,
        e_type,
        e_volume,
        e_myConfig,
        e_isError,
        e_predefineType,
        e_color,
        e_text,
        e_borderWidth,
        e_borderColor,
        e_callId,
        e_callStatus,
        e_haveGong,
        e_timerTmp,
        e_timerCount,
        e_timerMode,
    };


public:
    QVariant data(int role) const;
    bool setData(int role, const QVariant &value);
    QHash<int, QByteArray> roleNames() const;

private:
    int m_index;
    QString m_name;
    int m_selected;
    int m_type;
    float m_volume;
    bool m_myConfig;
    bool m_isError;
    int m_predefineType;
    QString m_color;
    QString m_text;
    int m_borderWidth;
    QString m_borderColor;
    int m_callId;
    QString m_callStatus;
    bool m_haveGong;
    bool m_timerTmp;
    int m_timerCount;
    int m_timerMode;
};

#endif // CQMLPREDEFINEMODEL_H
