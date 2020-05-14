#include <QDir>
#include <QDebug>
#include <qdom.h>
#include <QDomDocument>
#include <QFile>
#include <mutex>
#include "langstr.h"
#include "public.h"
#include "clocalconfig.h"

static std::mutex m_mutex;

LangStr* LangStr::pLangStr = NULL;

LangStr* LangStr::GetInstance()
{
    if (pLangStr == NULL)
    {
        m_mutex.lock();
        if (pLangStr == NULL)
        {
            pLangStr = new LangStr(NULL);
        }
        m_mutex.unlock();
    }
    return pLangStr;
}

void LangStr::Release()
{
    if (NULL != pLangStr)
    {
        m_mutex.lock();
        if (NULL != pLangStr)
        {
            delete pLangStr;
            pLangStr = NULL;
        }
        m_mutex.unlock();
    }
}

LangStr::LangStr(QObject *parent): QObject(parent)
{

}

void LangStr::init()
{
    m_langMap.clear();
    m_langList.clear();
    readLangMap();
    //printLangMap();
    readLangConfigTable(getMapIndex(CLocalConfig::GetInstance()->readConfig("LocalConfig", "Language")));
    //printConfigTable();
}

LangStr::~LangStr()
{

}

int LangStr::readLangMap()
{
    QDomNode node;
    QDomNode fisrtChildnode;
    QDomNode secondChildnode;
    QDomNode thirdChildnode;
    QDomNode fourthChildnode;
    QDomAttr nodeAttr;
    int index;
    int ret;

    QDomDocument domDocument;
    QString errorStr;
    int errorLine;
    int errorColumn;

#ifdef WIN32
    QFile file(getCurrentPath() + CONFIG_DIR + LANGUAGE_XML_FILE);
#else
    QFile file(QString(SD_CARD_PATH) + CONFIG_DIR + LANGUAGE_XML_FILE);
#endif
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return -1;
    }

    if (!domDocument.setContent(&file, &errorStr, &errorLine, &errorColumn))
    {
        file.close();
        return -1;
    }
    file.close();

    m_langMap.clear();
    QDomElement root = domDocument.documentElement();
    node = root.firstChild();
    while(!node.isNull())
    {
        if(node.isElement() && node.nodeName().toLower() == "worksheet")
        {
            nodeAttr = node.toElement().attributeNode(tr("ss:Name"));
            if(nodeAttr.value() == "ETCS")
            {
                QDomElement firstChildElement = node.toElement();
                fisrtChildnode = firstChildElement.firstChild();
                while (!fisrtChildnode.isNull())
                {
                    if(fisrtChildnode.isElement() && fisrtChildnode.nodeName().toLower() == "table")
                    {
                        QDomElement secondChildElement = fisrtChildnode.toElement();
                        secondChildnode = secondChildElement.firstChild();
                        index = 0;
                        while(!secondChildnode.isNull())
                        {
                            if(secondChildnode.isElement() && secondChildnode.nodeName().toLower() == "row")
                            {
                                QDomElement thirdChildElement = secondChildnode.toElement();
                                thirdChildnode = thirdChildElement.firstChild();
                                while(!thirdChildnode.isNull())
                                {
                                    if(thirdChildnode.isElement() && thirdChildnode.nodeName().toLower() == "cell")
                                    {
                                        QDomElement fourthChildElement = thirdChildnode.toElement();
                                        fourthChildnode = fourthChildElement.firstChild();
                                        ret = languageIsValid(fourthChildnode.toElement().text());
                                        if (ret == 0)
                                        {
                                            m_langMap.insert(fourthChildnode.toElement().text(), index);
                                        }
                                        index++;
                                    }
                                    thirdChildnode = thirdChildnode.nextSibling();
                                }
                                break;
                            }
                            secondChildnode = secondChildnode.nextSibling();
                        }
                    }
                    fisrtChildnode = fisrtChildnode.nextSibling();
                }
            }
        }
        node = node.nextSibling();
    }

    return 0;
}

void LangStr::printLangMap()
{
    QMap<QString, int>::iterator pos;
    for (pos = m_langMap.begin(); pos != m_langMap.end(); pos++)
    {
        qDebug() << "language = " << pos.key();
        qDebug() << "index = " << pos.value();
    }
}

int LangStr::getMapIndex(QString lang)
{
    QMap<QString, int>::iterator pos;
    for (pos = m_langMap.begin(); pos != m_langMap.end(); pos++)
    {
        if (lang == pos.key())
        {
            return pos.value();
        }
    }

    return -1;
}

QString LangStr::getMapLanguage(int index)
{
    QMap<QString, int>::iterator pos;
    for (pos = m_langMap.begin(); pos != m_langMap.end(); pos++)
    {
        if (index == pos.value())
        {
            return pos.key();
        }
    }
    return "";
}

int LangStr::getMapSize()
{
    return m_langMap.size();
}

bool LangStr::readLangConfigTable(int index)
{
    MULTI_LANG tempLangIndex;
    int rowIndex = 0;

    QDomNode node;
    QDomNode fisrtChildnode;
    QDomNode secondChildnode;
    QDomNode thirdChildnode;
    QDomNode fourthChildnode;
    QDomAttr nodeAttr;

    QDomDocument domDocument;
    QString errorStr;
    int errorLine;
    int errorColumn;

#ifdef WIN32
    QFile file(getCurrentPath() + CONFIG_DIR + LANGUAGE_XML_FILE);
#else
    QFile file(QString(SD_CARD_PATH) + CONFIG_DIR + LANGUAGE_XML_FILE);
#endif
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return false;
    }

    if (!domDocument.setContent(&file, &errorStr, &errorLine, &errorColumn))
    {
        file.close();
        return false;
    }
    file.close();

    m_langList.clear();

    QDomElement root = domDocument.documentElement();
    node = root.firstChild();
    while(!node.isNull())
    {
        if(node.isElement() && node.nodeName().toLower() == "worksheet")
        {
            nodeAttr = node.toElement().attributeNode(tr("ss:Name"));
            if(nodeAttr.value() == "ETCS")
            {
                QDomElement firstChildElement = node.toElement();
                fisrtChildnode = firstChildElement.firstChild();
                while (!fisrtChildnode.isNull())
                {
                    if(fisrtChildnode.isElement() && fisrtChildnode.nodeName().toLower() == "table")
                    {
                        QDomElement secondChildElement = fisrtChildnode.toElement();
                        secondChildnode = secondChildElement.firstChild();
                        while(!secondChildnode.isNull())
                        {
                            if(secondChildnode.isElement() && secondChildnode.nodeName().toLower() == "row")
                            {
                                QDomElement thirdChildElement = secondChildnode.toElement();
                                thirdChildnode = thirdChildElement.firstChild();
                                rowIndex = 0;
                                tempLangIndex.m_resId = "";
                                tempLangIndex.m_langStr = "";
                                while(!thirdChildnode.isNull())
                                {
                                    if (rowIndex == 0)
                                    {
                                        if(thirdChildnode.isElement() && thirdChildnode.nodeName().toLower() == "cell")
                                        {
                                            QDomElement fourthChildElement = thirdChildnode.toElement();
                                            fourthChildnode = fourthChildElement.firstChild();
                                            if(fourthChildnode.isElement() && fourthChildnode.nodeName().toLower() == "data")
                                            {
                                                tempLangIndex.m_resId = fourthChildnode.toElement().text();
                                            } 
                                        }                       
                                    }
                                    else if (rowIndex == index)
                                    {
                                        if(thirdChildnode.isElement() && thirdChildnode.nodeName().toLower() == "cell")
                                        {
                                            QDomElement fourthChildElement = thirdChildnode.toElement();
                                            fourthChildnode = fourthChildElement.firstChild();
                                            if(fourthChildnode.isElement() && fourthChildnode.nodeName().toLower() == "data")
                                            {
                                                tempLangIndex.m_langStr = fourthChildnode.toElement().text();
                                                break;
                                            }
                                        }
                                    }

                                    thirdChildnode = thirdChildnode.nextSibling();
                                    rowIndex++;
                                }

                                m_langList.push_back(tempLangIndex);
                            }
                            secondChildnode = secondChildnode.nextSibling();
                        }
                    }
                    fisrtChildnode = fisrtChildnode.nextSibling();
                }
            }
        }
        node = node.nextSibling();
    }

    return true;
}

void LangStr::printConfigTable()
{
    QList<MULTI_LANG>::iterator pos;
    for (pos = m_langList.begin(); pos != m_langList.end(); pos++)
    {
        qDebug() << "m_resId = " <<pos->m_resId;
        qDebug() << "element = " <<pos->m_langStr;
    }
}

QString LangStr::GetLangString(QString resId)
{
    QList<MULTI_LANG>::iterator pos;

    for (pos = m_langList.begin(); pos != m_langList.end(); pos++)
    {
        if(resId == pos->m_resId)
        {
           return pos->m_langStr;
        }
    }
    return resId;
}

QString LangStr::GetLangString(QString resId,int number,int channel)
{
    QString str = GetLangString(resId);
    if(number != -1 && channel != -1)
    {
        return str.arg(number+1).arg(channel+1);
    }
    else if(number != -1)
    {
        return str.arg(number+1);
    }
    else
        return str;
}

int LangStr::languageIsValid(QString str)
{
    bool ret;
    ret = str.contains("<");
    if (ret == true)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
