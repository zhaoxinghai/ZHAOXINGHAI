#ifndef FONT_SETTING_H
#define FONT_SETTING_H

#include <QObject>
#include <QString>
#include <vector>

typedef struct UI_LANG_FONT_ITEM
{
    QString lang;
    QString font;
}LANG_FONT_ITEM;

class CFontSetting : public QObject
{
    Q_OBJECT

public:
    CFontSetting();
    static CFontSetting* pFontSetting;
    static CFontSetting* GetInstance();
    static void Release();

public slots:
    void init();
    int createFontList();
    int clearFontList();
    int printFontList();
    QString getFontFromLanguage(QString lang);

signals:

private:
    std::vector<LANG_FONT_ITEM> m_fontList;

};

#endif
