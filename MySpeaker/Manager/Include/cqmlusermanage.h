#ifndef CQMLUSERMANAGE_H
#define CQMLUSERMANAGE_H

#include <QObject>
#include <QString>

#include "ListModel.h"
#include "constant.h"

class CQmlUserManage : public QObject
{
    Q_OBJECT
public:
    CQmlUserManage(QObject *parent = 0);
    static CQmlUserManage* pQmlUserConfig;
    static CQmlUserManage* GetInstance();
    static void Release();

signals:

public slots:
    void init();
    int getUserListCount();
    QString getUserId(int pos);
    QString getUserListStrValue(QString id, QString key);
    int setUserListIntValue(QString id, int value);
    int getUserListIntValue(QString id, QString key);
    int setLoginUserId(QString id);
    QString getLoginUserId();
    int setVALoginUserId(QString id);
    QString getVALoginUserId();
    int setUser(QString id);

    QString encryptPassword(QString password);
    int checkConfigFileOK();
    void printUserList();

    bool isNeedResetAccount(QString user, QString pwd);

private:

    QString m_loginUserName;
    QString m_loginUserId;
    QString m_VALoginUserId;
};

#endif // CQMLUSERMANAGE_H
