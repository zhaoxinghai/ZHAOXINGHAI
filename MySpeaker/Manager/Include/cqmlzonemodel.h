#ifndef CQMLZONEMODEL_H
#define CQMLZONEMODEL_H

#include "ListItem.h"

class CQmlZoneModel : public Models::ListItem
{
    Q_OBJECT

public:
    explicit CQmlZoneModel(QObject *parent = NULL);
    explicit CQmlZoneModel(const int &index, const int &status, const int &selected, const QString &strName, QObject *parent = NULL);

    enum zoneEnum
    {
        e_index = 1,
        e_status,
        e_selected,
        e_strName
    };


public:
    QVariant data(int role) const;
    bool setData(int role, const QVariant &value);
    QHash<int, QByteArray> roleNames() const;

private:
    int m_index;
    int m_status;
    int m_selected;
    QString m_strName;
};

#endif // CQMLZONEMODEL_H
