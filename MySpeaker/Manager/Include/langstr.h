#ifndef LANGSTR_H
#define LANGSTR_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>

typedef struct {
    QString m_resId;
    QString m_langStr;
}MULTI_LANG;

typedef QList<MULTI_LANG> LanguageList;

class LangStr : public QObject
{
    Q_OBJECT

public:

    explicit LangStr(QObject *parent = 0);
    ~LangStr();
    static LangStr* pLangStr;
    static LangStr* GetInstance();
    static void Release();

public slots:
    void init();
    int readLangMap();
    void printLangMap();
    int getMapIndex(QString lang);
    QString getMapLanguage(int index);
    int getMapSize();
    bool readLangConfigTable(int index);
    void printConfigTable();
    QString GetLangString(QString resId);
    QString GetLangString(QString resId,int number,int channel);
private:
    int languageIsValid(QString str);
    LanguageList m_langList;
    QMap<QString, int> m_langMap;
};

#endif // LANGSTR_H
