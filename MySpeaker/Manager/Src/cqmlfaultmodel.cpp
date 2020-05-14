#include "cqmlfaultmodel.h"

CQmlFaultModel::CQmlFaultModel(QObject *parent) : Models::ListItem(parent)
{

}

CQmlFaultModel::CQmlFaultModel(const int &index,
                                 bool blocal,
                                 const QString &activate_time,
                                 const QString &name,
                                 const QString &desc,
                                 const QString &key,
                                 const int &number,
                                 const int &channel,
                                 QObject *parent)
          : Models::ListItem(parent),
          m_index(index),
          m_blocal(blocal),
          m_time(activate_time),
          m_name(name),
          m_desc(desc),
          m_key(key),
          m_number(number),
          m_channel(channel)
{

}

QVariant CQmlFaultModel::data(int role) const
{
    switch(role)
    {
    case e_index:
        return m_index;
    case e_local:
        return m_blocal;
    case e_time:
        return this->m_time;
    case e_name:
        return this->m_name;
    case e_desc:
        return this->m_desc;
    case e_key:
        return this->m_key;
    case e_num:
        return this->m_number;
    case e_ch:
        return this->m_channel;
    default :
        return QVariant();
    }
}

bool CQmlFaultModel::setData(int role, const QVariant &value)
{
    switch(role)
    {
    case e_index:
        this->m_index = value.toInt();
        this->triggerItemUpdate();
        return true;
    case e_local:
        this->m_blocal = value.toInt();
        this->triggerItemUpdate();
        return true;
    case e_time:
        this->m_time = value.toString();
        this->triggerItemUpdate();
        return true;
    case e_name:
        this->m_name = value.toString();
        this->triggerItemUpdate();
        return true;
    case e_desc:
        this->m_desc = value.toString();
        this->triggerItemUpdate();
        return true;
    case e_key:
        this->m_key = value.toString();
        this->triggerItemUpdate();
        return true;
    case e_num:
        this->m_number = value.toInt();
        this->triggerItemUpdate();
        return true;
    case e_ch:
        this->m_channel = value.toInt();
        this->triggerItemUpdate();
        return true;
    default :
        return false;
    }
}

QHash<int, QByteArray> CQmlFaultModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[e_index] = "e_index";
    roles[e_local] = "e_local";
    roles[e_time] = "e_time";
    roles[e_name] = "e_name";
    roles[e_desc] = "e_desc";
    roles[e_key] = "e_key";
    roles[e_num] = "e_num";
    roles[e_ch] = "e_ch";
    return roles;
}
