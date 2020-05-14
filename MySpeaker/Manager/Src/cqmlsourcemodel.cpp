#include "cqmlsourcemodel.h"

CQmlSourceModel::CQmlSourceModel(QObject *parent) : Models::ListItem(parent)
{

}

CQmlSourceModel::CQmlSourceModel(const int &index,
                                 const int &selected,
                                 const QString &sourceName,
                                 const int &errorNum,
                                 const bool &tran,
                                 QObject *parent)
        : Models::ListItem(parent),
          m_index(index),
          m_selected(selected),
          m_sourceName(sourceName),
          m_errorNum(errorNum),
          m_tran(tran)

{

}

QVariant CQmlSourceModel::data(int role) const
{
    switch(role)
    {
    case e_index:
        return this->m_index;
    case e_sourceName:
        return this->m_sourceName;
    case e_selected:
        return this->m_selected;
    case e_errorNum:
        return this->m_errorNum;
    case e_tran:
        return this->m_tran;
    default :
        return QVariant();
    }
}

bool CQmlSourceModel::setData(int role, const QVariant &value)
{
    switch(role)
    {
    case e_index:
        this->m_index = value.toInt();
        this->triggerItemUpdate();
        return true;
    case e_selected:
        this->m_selected = value.toInt();
        this->triggerItemUpdate();
        return true;
    case e_sourceName:
        this->m_sourceName = value.toString();
        this->triggerItemUpdate();
        return true;
    case e_errorNum:
        this->m_errorNum = value.toInt();
        this->triggerItemUpdate();
        return true;
    case e_tran:
        this->m_tran = value.toBool();
        this->triggerItemUpdate();
        return true;
    default :
        return false;
    }
}

QHash<int, QByteArray> CQmlSourceModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[e_index] = "e_index";
    roles[e_selected] = "e_selected";
    roles[e_sourceName] = "e_sourceName";
    roles[e_errorNum] = "e_errorNum";
    roles[e_tran] = "e_tran";
    return roles;
}
