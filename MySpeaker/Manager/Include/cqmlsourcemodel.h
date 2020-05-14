#ifndef CQMLLOCALLSOURCEMODEL_H
#define CQMLLOCALLSOURCEMODEL_H

#include "ListItem.h"

class CQmlSourceModel : public Models::ListItem
{
    Q_OBJECT

public:
    explicit CQmlSourceModel(QObject *parent = NULL);
    explicit CQmlSourceModel(const int &index,
                             const int &selected,
                             const QString &sourceName,
                             const int &errorNum,
                             const bool &tran,
                             QObject *parent = NULL);

    enum localSourceEnum
    {
        e_index = 1,
        e_selected,
        e_sourceName,
        e_errorNum,
        e_tran
    };

public:
    QVariant data(int role) const;
    bool setData(int role, const QVariant &value);
    QHash<int, QByteArray> roleNames() const;

private:
    int m_index;
    int m_selected;
    QString m_sourceName;
    int m_errorNum;
    bool m_tran;
};

#endif // CQMLLOCALLSOURCEMODEL_H
