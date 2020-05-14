#ifndef CQMLACTIVATEMODEL_H
#define CQMLACTIVATEMODEL_H

#include "ListItem.h"

class CQmlActivateModel : public Models::ListItem
{
    Q_OBJECT

public:
    explicit CQmlActivateModel(QObject *parent = NULL);
    explicit CQmlActivateModel(const int &id,                               
                               const QString &activate_time,
                               const QString &name,
                               const QString &status,
                               const QString &zones,
                               const int &emergency,
                               QObject *parent = NULL);

    enum zoneEnum
    {
        e_id = 1,
        e_time,
        e_name,
        e_status,
        e_zones,
        e_emergency
    };


public:
    QVariant data(int role) const;
    bool setData(int role, const QVariant &value);
    QHash<int, QByteArray> roleNames() const;

private:
    int m_id;
    QString m_time;
    QString m_name;
    QString m_status;
    QString m_zones;
    int m_emergency;
};

#endif // CQMLACTIVATEMODEL_H
