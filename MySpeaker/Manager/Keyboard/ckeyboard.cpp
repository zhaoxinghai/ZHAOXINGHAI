#include <QFile>
#include <QCoreApplication>
#include <QDebug>
#include <QDomDocument>
#include <mutex>

#include "ckeyboard.h"
#include "conf.h"
#include "public.h"

static std::mutex m_mutex;

CKeyboard* CKeyboard::pKeyboard = NULL;

CKeyboard* CKeyboard::GetInstance()
{
    if (pKeyboard == NULL)
    {
        m_mutex.lock();
        if (pKeyboard == NULL)
        {
            pKeyboard = new CKeyboard();
        }
        m_mutex.unlock();
    }
    return pKeyboard;
}

void CKeyboard::Release()
{
    if (NULL != pKeyboard)
    {
        m_mutex.lock();
        if (NULL != pKeyboard)
        {
            delete pKeyboard;
            pKeyboard = NULL;
        }
        m_mutex.unlock();
    }
}

CKeyboard::CKeyboard(QObject *parent) : QObject(parent)
{

}

int CKeyboard::init()
{
    return 0;
}

int CKeyboard::initKeyMap(QString language)
{
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument domProject;

#ifdef WIN32
    QFile file(getCurrentPath() + CONFIG_DIR + KEYBOARD_CONFIG_FILE);
#else
    QFile file(QString(KEYBOARD_CONFIG_PATH) + KEYBOARD_CONFIG_FILE);
#endif
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "open keyboard config file error";
        return -1;
    }

    if (!domProject.setContent(&file, &errorStr, &errorLine,&errorColumn))
    {
        qDebug() << "domProject setContent error";
        file.close();
        return -1;
    }
    file.close();

    QDomNode node;
    QDomNode fisrtChildnode;
    QDomNode secondChildnode;
    QDomNode thirdChildnode;
    QDomNode fourthChildnode;
    QDomAttr nodeAttr;

    QDomElement root = domProject.documentElement();
    node = root.firstChild();
    while(!node.isNull())
    {
        if(node.isElement() && node.nodeName().toLower() == "worksheet")
        {
            nodeAttr = node.toElement().attributeNode("ss:Name");
            if(nodeAttr.value() == language)
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
                                unsigned int first = 0;
                                QString key;
                                QStringList tmpList;
                                while(!thirdChildnode.isNull())
                                {
                                    if(thirdChildnode.isElement() && thirdChildnode.nodeName().toLower() == "cell")
                                    {
                                        QDomElement fourthChildElement = thirdChildnode.toElement();
                                        fourthChildnode = fourthChildElement.firstChild();

                                        if (first == 0)
                                        {
                                            if (fourthChildnode.toElement().text().contains("KEY_"))
                                            {
                                                key = fourthChildnode.toElement().text();
                                                first = 1;
                                            }
                                        }
                                        else
                                        {
                                            tmpList << fourthChildnode.toElement().text();
                                        }

                                    }
                                    thirdChildnode = thirdChildnode.nextSibling();
                                }
                                m_keyMap.insert(key, tmpList);
                                //break;
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

void CKeyboard::printKeyMap()
{
    QMap<QString, QStringList>::iterator it;

    for (it = m_keyMap.begin(); it != m_keyMap.end(); it++)
    {
        qDebug() << "keyboard key ---------------------> " << it.key();
        for (int j = 0; j < it.value().size(); j++)
        {
            qDebug() << "keyboard char = " << it.value().at(j);
        }
    }
}

int CKeyboard::getStringListSize(QString key)
{
    QMap<QString, QStringList>::iterator it;

    for (it = m_keyMap.begin(); it != m_keyMap.end(); it++)
    {
        if (it.key() == key)
        {
            return it.value().size();
        }
    }

    return -1;
}

QString CKeyboard::getCharFromKeyMap(QString key, unsigned int pos)
{
    QMap<QString, QStringList>::iterator it;

    for (it = m_keyMap.begin(); it != m_keyMap.end(); it++)
    {
        if (it.key() == key && (int)pos < it.value().size())
        {
            return it.value().at(pos);
        }
    }

    return key;
}

QString CKeyboard::getKeyText(QString key)
{
    QString text;
    QMap<QString, QStringList>::iterator it;

    for (it = m_keyMap.begin(); it != m_keyMap.end(); it++)
    {
        if (it.key() == key)
        {
            for (int j = 0; j < it.value().size(); j++)
            {
                text += it.value().at(j);
            }
        }
    }

    return text;
}

int CKeyboard::isTwoPage()
{
    QMap<QString, QStringList>::iterator it;

    for (it = m_keyMap.begin(); it != m_keyMap.end(); it++)
    {
        if (it.key().right(1).toInt() == 2)
        {
            if (it.value().size() > 0)
            {
                return 0;
            }
        }
    }

    return -1;
}
