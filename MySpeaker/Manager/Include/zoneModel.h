#ifndef ZONE_MODLE_H
#define ZONE_MODLE_H

#include "ListItem.h"

class CZoneModel : public Models::ListItem
{
    Q_OBJECT

public:
    explicit CZoneModel(QObject *parent = NULL);
    explicit CZoneModel(const int &index,
                        const QString &name,
                        const QString &color,
                        const bool &select,
                        const QString &VAColor,
                        const bool &VASelect,
                        QObject *parent = NULL);

    enum zoneEnum
    {
        e_index = 1,
        e_name,
        e_color,
        e_select,
        e_VAColor,
        e_VASelect,
    };


public:
    QVariant data(int role) const;
    bool setData(int role, const QVariant &value);
    QHash<int, QByteArray> roleNames() const;

private:
    int m_index;
    QString m_name;
    QString m_color;
    bool m_select;
    QString m_VAColor;
    bool m_VASelect;
};

#endif
