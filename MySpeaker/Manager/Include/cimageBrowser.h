#ifndef IMAGE_BROWSER_H
#define IMAGE_BROWSER_H

#include <QObject>
#include <QString>
#include <QThread>
#include <vector>

typedef struct MY_IMAGE_ITEM
{
    QString fileName;
    QString filePath;
    QString uriFileName;
}IMAGE_ITEM;

class CImageBrowser : public QObject
{
    Q_OBJECT

public:
    CImageBrowser();
    static CImageBrowser* pImageBrowser;
    static CImageBrowser* GetInstance();
    static void Release();

public slots:
    void init();
    int clearImageList();
    int createImageList();
    int getImageListCount();
    QString getImageUriFileName(int pos);

signals:

private:

    std::vector<IMAGE_ITEM> m_imageList;
};

#endif
