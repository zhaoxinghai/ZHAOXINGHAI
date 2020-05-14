#include "cqmlzonemodel.h"

#include <QDebug>

CQmlZoneModel::CQmlZoneModel(QObject *parent) : Models::ListItem(parent)
{

}

CQmlZoneModel::CQmlZoneModel(const int &index, const int &status, const int &selected, const QString &strName, QObject *parent)
        : Models::ListItem(parent),
        m_index(index),
        m_status(status),
        m_selected(selected),
        m_strName(strName)
{

}

QVariant CQmlZoneModel::data(int role) const
{
    switch(role)
    {
    case e_index:
        return this->m_index;
    case e_status:
        return this->m_status;
    case e_selected:
        return this->m_selected;
    case e_strName:
        return this->m_strName;
    default :
        return QVariant();
    }
}

bool CQmlZoneModel::setData(int role, const QVariant &value)
{
    switch(role)
    {
    case e_index:
        this->m_index = value.toInt();
        this->triggerItemUpdate();
        return true;
    case e_status:
        this->m_status = value.toInt();
        this->triggerItemUpdate();
        return true;
    case e_selected:
        this->m_selected = value.toInt();
        this->triggerItemUpdate();
        return true;
    case e_strName:
        this->m_strName = value.toString();
        this->triggerItemUpdate();
        return true;
    default :
        return false;
    }
}

QHash<int, QByteArray> CQmlZoneModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[e_index] = "e_index";
    roles[e_status] = "e_status";
    roles[e_selected] = "e_selected";
    roles[e_strName] = "e_strName";
    return roles;
}
