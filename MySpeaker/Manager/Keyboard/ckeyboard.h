#ifndef CKEYBOARD_H
#define CKEYBOARD_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QStringList>

class CKeyboard : public QObject
{
    Q_OBJECT
public:
    explicit CKeyboard(QObject *parent = 0);
    static CKeyboard* pKeyboard;
    static CKeyboard* GetInstance();
    static void Release();
    int init();

signals:

public slots:
    int initKeyMap(QString language);
    void printKeyMap();
    int getStringListSize(QString key);
    QString getCharFromKeyMap(QString key, unsigned int pos);
    QString getKeyText(QString key);
    int isTwoPage();

private:
    QMap<QString, QStringList> m_keyMap;
};

#endif // CKEYBOARD_H
