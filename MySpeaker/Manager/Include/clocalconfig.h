#ifndef CLOCALCONFIG_H
#define CLOCALCONFIG_H

#include <QString>
#include <QObject>

class CLocalConfig : public QObject
{
    Q_OBJECT
public:
    explicit CLocalConfig(QObject *parent = 0);
    static CLocalConfig* pLocalConfig;
    static CLocalConfig* GetInstance();
    static void Release();

public slots:
    void init();
    int initLocalConfigFile();
    int writeConfig(QString configType, QString name, QString value);
    int insertConfig(QString name, QString value);
    QString readConfig(QString configType, QString name);

    QString getProtocolVersion();
    QString getUploadMaster();
    QString getStationName();
    int getLocalNode();
    int getUsedStorage(int type);
    int getTotalStorage(int type);
    QString getLockImg();

    int setIsConfigMode(bool mode);
    bool getIsConfigMode();

private:
    int isLocalConfigFileInit();
    int createLocalConfigFile();

    QString m_localConfigFile;
    QString m_localConfigBackupFile;

    bool m_isConfigMode;
};

#endif // CLOCALCONFIG_H
