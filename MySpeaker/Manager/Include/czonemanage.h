#ifndef CZONEMANAGE_H
#define CZONEMANAGE_H

#include <QObject>
#include "ListModel.h"
#include "Logic.h"

class CZoneManage : public QObject
{
    Q_OBJECT
public:
    explicit CZoneManage(QObject *parent = 0);
    static CZoneManage* pZoneManage;
    static CZoneManage* GetInstance();
    static void Release();

public slots:

    int init();
    int getZoneListCount();
    int getValidUserZoneListCount();
    int getZoneIndex(int pos);
    bool isZoneNameValid(int index, QString str);
    bool isZoneUserIdValid(int index);
    QString getZoneStrValue(int index, QString key);
    int getZoneIntValue(int index, QString key);
    bool getZoneBoolValue(int index, QString key);
    int setZoneSelected(int index, bool select);
    int selectAllFromUserPermission();
    int unselectAllFromUserPermission();
    int unSelectAll();
    bool isOneZoneSelected();
    int getSelectCount();
    bool isAllZoneChecked();

    int clearZoneModel();
    int createZoneModel();
    int getZoneModelCount();
    int zoneModelSelectAll();
    int zoneModelUnselectAll();
    int getZoneModelSelectCount();
    bool isZoneModelMoreThanOneZoneSelected();
    Models::ListModel *getZoneModel();
    int updateZoneModelStatus();
    int printAllSelectZones();


    int clearZoneModelVA();
    int createZoneModelVA();
    int getZoneModelCountVA();
    int zoneModelSelectAllVA();
    int zoneModelUnselectAllVA();
    int getZoneModelSelectCountVA();
    bool isZoneModelMoreThanOneZoneSelectedVA();
    Models::ListModel *getZoneModelVA();
    int updateZoneModelStatusVA();
    int printAllSelectZonesVA();
    int setZoneSelectedVA(int index, bool select);
    int unSelectAllVA();
    int selectAllFromUserPermissionVA();
    int unselectAllFromUserPermissionVA();
    bool isOneZoneSelectedVA();
    int getSelectCountVA();
    void ResetStateVA();
    bool isVAZoneUserIdValid(int index, QString name);

signals:

public slots:

private:

    QString getColor(int status);
    QString getColorVA(int status);

    Models::ListModel *m_zoneModel;
    Models::ListModel *m_zoneModelVA;
};

#endif // CZONEMANAGE_H
