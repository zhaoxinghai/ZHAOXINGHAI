#include "zoneModel.h"

CZoneModel::CZoneModel(QObject *parent) : Models::ListItem(parent)
{

}

CZoneModel::CZoneModel(const int &index,
                       const QString &name,
                       const QString &color,
                       const bool &select,
                       const QString &VAColor,
                       const bool &VASelect,
                       QObject *parent)
          : Models::ListItem(parent),
          m_index(index),
          m_name(name),
          m_color(color),
          m_select(select),
          m_VAColor(VAColor),
          m_VASelect(VASelect)
{

}

QVariant CZoneModel::data(int role) const
{
    switch(role)
    {
    case e_index:
        return this->m_index;
    case e_name:
        return this->m_name;
    case e_color:
        return this->m_color;
    case e_select:
        return this->m_select;
    case e_VAColor:
        return this->m_VAColor;
    case e_VASelect:
        return this->m_VASelect;
    default :
        return QVariant();
    }
}

bool CZoneModel::setData(int role, const QVariant &value)
{
    switch(role)
    {
    case e_index:
        this->m_index = value.toInt();
        this->triggerItemUpdate();
        return true;
    case e_name:
        this->m_name = value.toString();
        this->triggerItemUpdate();
        return true;
    case e_color:
        this->m_color = value.toString();
        this->triggerItemUpdate();
        return true;
    case e_select:
        this->m_select = value.toBool();
        this->triggerItemUpdate();
        return true;
    case e_VAColor:
        this->m_VAColor = value.toString();
        this->triggerItemUpdate();
        return true;
    case e_VASelect:
        this->m_VASelect = value.toBool();
        this->triggerItemUpdate();
        return true;
    default :
        return false;
    }
}

QHash<int, QByteArray> CZoneModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[e_index] = "e_index";
    roles[e_name] = "e_name";
    roles[e_color] = "e_color";
    roles[e_select] = "e_select";
    roles[e_VAColor] = "e_VAColor";
    roles[e_VASelect] = "e_VASelect";

    return roles;
}
