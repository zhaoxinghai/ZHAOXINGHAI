#include "cqmlactivatemodel.h"

#include <QDebug>

CQmlActivateModel::CQmlActivateModel(QObject *parent) : Models::ListItem(parent)
{

}

CQmlActivateModel::CQmlActivateModel(const int &id,
                                 const QString &activate_time,
                                 const QString &name,
                                 const QString &status,
                                 const QString &zones,
                                 const int &emergency,
                                 QObject *parent)
          : Models::ListItem(parent),
          m_id(id),
          m_time(activate_time),
          m_name(name),
          m_status(status),
          m_zones(zones),
          m_emergency(emergency)
{

}

QVariant CQmlActivateModel::data(int role) const
{
    switch(role)
    {
    case e_id:
        return this->m_id;
    case e_time:
        return this->m_time;
    case e_name:
        return this->m_name;
    case e_status:
        return this->m_status;
    case e_zones:
        return this->m_zones;
    case e_emergency:
        return this->m_emergency;
    default :
        return QVariant();
    }
}

bool CQmlActivateModel::setData(int role, const QVariant &value)
{
    switch(role)
    {
    case e_id:
        this->m_id = value.toInt();
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
    case e_status:
        this->m_status = value.toString();
        this->triggerItemUpdate();
        return true;
    case e_zones:
        this->m_zones = value.toString();
        this->triggerItemUpdate();
        return true;
    case e_emergency:
        this->m_zones = value.toInt();
        this->triggerItemUpdate();
        return true;
    default :
        return false;
    }
}

QHash<int, QByteArray> CQmlActivateModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[e_id] = "e_id";
    roles[e_time] = "e_time";
    roles[e_name] = "e_name";
    roles[e_status] = "e_status";
    roles[e_zones] = "e_zones";
    roles[e_emergency] = "e_emergency";
    return roles;
}
