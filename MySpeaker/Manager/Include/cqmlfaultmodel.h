#ifndef CQMLFAULTMODEL_H
#define CQMLFAULTMODEL_H

#include "ListItem.h"
#include "SysFaultList.h"

class CQmlFaultModel : public Models::ListItem
{
    Q_OBJECT

public:
    explicit CQmlFaultModel(QObject *parent = NULL);
    explicit CQmlFaultModel(const int &index,
                               bool blocal,
                               const QString &activate_time,
                               const QString &name,
                               const QString &desc,
                               const QString &key,
                               const int &number,
                               const int &channel,
                               QObject *parent = NULL);

    enum faultEnum
    {
        e_index = 1,
        e_local,
        e_time,
        e_name,
        e_desc,
        e_key,
        e_num,
        e_ch
    };


public:
    QVariant data(int role) const;
    bool setData(int role, const QVariant &value);
    QHash<int, QByteArray> roleNames() const;

private:

    int m_index;
    bool    m_blocal;
    QString m_time;
    QString m_name;
    QString m_desc;
    QString m_key;
    int     m_number;
    int     m_channel;
};

//Q_DECLARE_METATYPE(t_FaultIndex);

#endif // CQMLFAULTMODEL_H
