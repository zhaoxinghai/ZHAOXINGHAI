#include <mutex>
#include <qdom.h>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include "cfontSetting.h"
#include "public.h"

static std::mutex m_mutex;

CFontSetting* CFontSetting::pFontSetting = NULL;

CFontSetting* CFontSetting::GetInstance()
{
    if (pFontSetting == NULL)
    {
        m_mutex.lock();
        if (pFontSetting == NULL)
        {
            pFontSetting = new CFontSetting();
        }
        m_mutex.unlock();
    }
    return pFontSetting;
}

void CFontSetting::Release()
{
    if (NULL != pFontSetting)
    {
        m_mutex.lock();
        if (NULL != pFontSetting)
        {
            delete pFontSetting;
            pFontSetting = NULL;
        }
        m_mutex.unlock();
    }
}

CFontSetting::CFontSetting()
{

}

void CFontSetting::init()
{
    createFontList();
    //printFontList();
}

int CFontSetting::createFontList()
{
    QDomNode node;
    QDomNode fisrtChildnode;
    QDomNode secondChildnode;
    QDomNode thirdChildnode;
    QDomNode fourthChildnode;
    QDomAttr nodeAttr;

    LANG_FONT_ITEM tmpItem;
    int pos = 0;
    bool isFirstCell = true;
     QDomDocument domDocument;
     QString errorStr;
     int errorLine;
     int errorColumn;

#ifdef WIN32
    QFile file(getCurrentPath() + CONFIG_DIR + FONT_FILENAME);
#else
    QFile file(QString(SD_CARD_PATH) + CONFIG_DIR + FONT_FILENAME);
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

    QDomElement root = domDocument.documentElement();
    node = root.firstChild();
    while(!node.isNull())
    {
        if(node.isElement() && node.nodeName().toLower() == "worksheet")
        {
            nodeAttr = node.toElement().attributeNode(tr("ss:Name"));
            if(nodeAttr.value() == "Fonts")
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
                                if (true == isFirstCell)
                                {
                                    isFirstCell = false;
                                    secondChildnode = secondChildnode.nextSibling();
                                    continue;
                                }

                                QDomElement thirdChildElement = secondChildnode.toElement();
                                thirdChildnode = thirdChildElement.firstChild();
                                tmpItem.lang = "";
                                tmpItem.font = "";
                                pos = 0;
                                while(!thirdChildnode.isNull())
                                {
                                    if(thirdChildnode.isElement() && thirdChildnode.nodeName().toLower() == "cell")
                                    {
                                        QDomElement fourthChildElement = thirdChildnode.toElement();
                                        fourthChildnode = fourthChildElement.firstChild();

                                        if (0 == pos)
                                        {
                                            tmpItem.lang = fourthChildnode.toElement().text();
                                            pos++;
                                        }
                                        else
                                        {
                                            tmpItem.font = fourthChildnode.toElement().text();
                                            break;
                                        }
                                    }
                                    thirdChildnode = thirdChildnode.nextSibling();
                                }

                                m_fontList.push_back(tmpItem);
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

int CFontSetting::clearFontList()
{
    m_fontList.clear();

    return 0;
}

int CFontSetting::printFontList()
{
    std::vector<LANG_FONT_ITEM>::iterator i;

    for (i = m_fontList.begin(); i != m_fontList.end(); i++)
    {
        qDebug() << "Font list language == " << (*i).lang;
        qDebug() << "Font list language == " << (*i).font;
    }

    return 0;
}

QString CFontSetting::getFontFromLanguage(QString lang)
{
    std::vector<LANG_FONT_ITEM>::iterator i;

    for (i = m_fontList.begin(); i != m_fontList.end(); i++)
    {
        if ((*i).lang == lang)
        {
            return (*i).font;
        }
    }

    return "";
}
