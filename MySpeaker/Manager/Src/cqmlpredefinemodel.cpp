#include "cqmlpredefinemodel.h"

CQmlPredefineModel::CQmlPredefineModel(QObject *parent) : Models::ListItem(parent)
{

}

CQmlPredefineModel::CQmlPredefineModel(const int &index,
                                       const QString &name,
                                       const int &selected,
                                       const int &type,
                                       const float &volume,
                                       const int &myConfig,
                                       const bool &isError,
                                       const int &predefineType,
                                       const QString &color,
                                       const QString &text,
                                       const int &borderWidth,
                                       const QString &borderColor,
                                       const int &callId,
                                       const QString &callStatus,
                                       const bool &haveGong,
                                       const bool &timerTmp,
                                       const int &timerCount,
                                       const int &timerMode,
                                       QObject *parent)
          : Models::ListItem(parent),
          m_index(index),
          m_name(name),
          m_selected(selected),
          m_type(type),
          m_volume(volume),
          m_myConfig(myConfig),
          m_isError(isError),
          m_predefineType(predefineType),
          m_color(color),
          m_text(text),
          m_borderWidth(borderWidth),
          m_borderColor(borderColor),
          m_callId(callId),
          m_callStatus(callStatus),
          m_haveGong(haveGong),
        m_timerTmp(timerTmp),
        m_timerCount(timerCount),
        m_timerMode(timerMode)
{

}

QVariant CQmlPredefineModel::data(int role) const
{
    switch(role)
    {
    case e_index:
        return this->m_index;
    case e_name:
        return this->m_name;
    case e_selected:
        return this->m_selected;
    case e_type:
        return this->m_type;
    case e_volume:
        return this->m_volume;
    case e_myConfig:
        return this->m_myConfig;
    case e_isError:
        return this->m_isError;
    case e_predefineType:
        return this->m_predefineType;
    case e_color:
        return this->m_color;
    case e_text:
        return this->m_text;
    case e_borderWidth:
        return this->m_borderWidth;
    case e_borderColor:
        return this->m_borderColor;
    case e_callId:
        return this->m_callId;
    case e_callStatus:
        return this->m_callStatus;
    case e_haveGong:
        return this->m_haveGong;
    case e_timerTmp:
        return this->m_timerTmp;
    case e_timerCount:
        return this->m_timerCount;
    case e_timerMode:
        return this->m_timerMode;
    default :
        return QVariant();
    }
}

bool CQmlPredefineModel::setData(int role, const QVariant &value)
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
    case e_selected:
        this->m_selected = value.toInt();
        this->triggerItemUpdate();
        return true;
    case e_type:
        this->m_type = value.toInt();
        this->triggerItemUpdate();
        return true;
    case e_volume:
        this->m_volume = value.toFloat();
        this->triggerItemUpdate();
        return true;
    case e_myConfig:
        this->m_myConfig = value.toBool();
        this->triggerItemUpdate();
        return true;
    case e_isError:
        this->m_isError = value.toBool();
        this->triggerItemUpdate();
        return true;
    case e_predefineType:
        this->m_predefineType = value.toInt();
        this->triggerItemUpdate();
        return true;
    case e_color:
        this->m_color = value.toString();
        this->triggerItemUpdate();
        return true;
    case e_text:
        this->m_text = value.toString();
        this->triggerItemUpdate();
        return true;
    case e_borderWidth:
        this->m_borderWidth = value.toInt();
        this->triggerItemUpdate();
        return true;
    case e_borderColor:
        this->m_borderColor = value.toString();
        this->triggerItemUpdate();
        return true;
    case e_callId:
        this->m_callId = value.toInt();
        this->triggerItemUpdate();
        return true;
    case e_callStatus:
        this->m_callStatus = value.toString();
        this->triggerItemUpdate();
        return true;
    case e_haveGong:
        this->m_haveGong = value.toBool();
        this->triggerItemUpdate();
        return true;
    case e_timerTmp:
        this->m_timerTmp = value.toBool();
        this->triggerItemUpdate();
        return true;
    case e_timerCount:
        this->m_timerCount = value.toInt();
        this->triggerItemUpdate();
        return true;
    case e_timerMode:
        this->m_timerMode = value.toInt();
        this->triggerItemUpdate();
        return true;
    default :
        return false;
    }
}

QHash<int, QByteArray> CQmlPredefineModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[e_index] = "e_index";
    roles[e_name] = "e_name";
    roles[e_selected] = "e_selected";
    roles[e_type] = "e_type";
    roles[e_volume] = "e_volume";
    roles[e_myConfig] = "e_myConfig";
    roles[e_isError] = "e_isError";
    roles[e_predefineType] = "e_predefineType";
    roles[e_color] = "e_color";
    roles[e_text] = "e_text";
    roles[e_borderWidth] = "e_borderWidth";
    roles[e_borderColor] = "e_borderColor";
    roles[e_callId] = "e_callId";
    roles[e_callStatus] = "e_callStatus";
    roles[e_haveGong] = "e_haveGong";
    roles[e_timerTmp] = "e_timerTmp";
    roles[e_timerCount] = "e_timerCount";
    roles[e_timerMode] = "e_timerMode";
    return roles;
}
