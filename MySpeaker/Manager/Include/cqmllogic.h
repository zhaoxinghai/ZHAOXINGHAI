#ifndef CQMLLOGIC_H
#define CQMLLOGIC_H

#include <QObject>
#include <QSettings>

#include "ListModel.h"
#include "cqmlzonemodel.h"
#include "cqmlactivatemodel.h"
#include "cqmlfaultmodel.h"
#include "Logic.h"
#include "UIcallback.h"
#include "langstr.h"

class CQmlLogic : public QObject
{
    Q_OBJECT
public:
    explicit CQmlLogic(QObject *parent = 0);
	static CQmlLogic* pQmlLogic;
	static CQmlLogic* GetInstance();
	static void Release();

public slots:

    void init();
    void initProtocl();
    void initVlan();
    void runLogic();

    int createActivateListModel();
    int clearActivateListModel();
    int getActivateCount();
    int updateActivate(CConnect *pConnect);
    int DeleteActivate(int nID);
    int delActivate(int nID);
    int clearAllActivate();
    int clearAllUsbActivate();
    bool isActivateImgHide();
    Models::ListModel *getActivateModel();
    QString getActivateStrValue(int id, QString name);
    bool getActivateBoolValue(int id, QString name);
    int getActivateIntValue(int id, QString name);
    int getActivateZoneCount(int id);
    QString getActivateZone(int id, int index);
    int getActivateSourceCount(int id);
    QString getActivateSource(int id, int index);

    int clearFaultListModel();
    int createFaultListModel();
    int getFaultListModelCount();
    int insertFault(CSysFault*  pFault);
    int updateFault(CSysFault*  pFault);
    int delFault(int index);
    int updateFaultLanguage();
    QString GetFaultKey(CSysFault* pFault);
    QString GetFaultDesc(CSysFault* pFault);
    QString GetFaultDesc(QString key,int number,int channel);
    Models::ListModel *getFaultModel();
    void Refresh_FaultList();
    void resetError();

    int clearPredefineListModel();
    int createPredefineListModel(QString status, QString str);
    int addPredefineToListModel(CPredefine *tmp);
    int getPredefineListModelCount();
    int getPredefineListCount();
    int setPredefineSelected(int index, bool select);
    int predefineUnselectAll();
    Models::ListModel *getPredefineModel();
    QString getPredefineStrValue(int id, QString name);
    int getPredefineIntValue(int id, QString name);
    bool getPredefineBoolValue(int id, QString name);
    float getPredefineFloatValue(int id, QString name);
    int getPredefineZoneCount(int id);
    QString getPredefineZone(int id, int index);
    int getPredefineSourceCount(int id);
    QString getPredefineSource(int id, int index);
    int delPredefineDeep(int index);
    int delPredefineModel(int nID);
    int delPredefineLogic(int nID);
    void updatePredefineVolume(int index, bool Myconfig, float dB);
    bool isOverPredefineMaxCount();

	int analysisResult(const CResultBase* pResult);
	void emitData(CResultBase* pResult);
	void dealData(CResultBase* pResult);
    void VolumeAjust(int id, float vol);

    QString getProjectName();
    QString getVersion();
    QString getCompileTime();
    QString getMinAndSec(int min, int sec);
    QString getMacAddress();
    QString getFullFontPath(QString lang);
    QString getLocalImagePath();
    QString getLockImagePath();
    int initRealResolution();
    int setRealResolution(int width, int height);
    int getRealResolution(int &width, int &height);
    int getRealWidth();
    int getRealHeight();

    int setIpAddressAndNetmaskToFile(QString ip, QString netmask);
    int setGatewayToFile(QString gateway);
    int syncIpAddressAndNetmask(QString ip, QString netmask);
    int syncIpAddress(QString ip);
    int syncNetMask(QString netmask);
    int syncGateway(QString gateway);
    QString getRealIp();
    QString getRealNetmask();
    QString getRealGateway();
    QString getRealMAC();
    bool isIPAddressValid(QString ip, QString netmask);
    bool isSubnetMaskIsValid(QString netmask);
    bool isGatewayIsValid(QString gateway, QString netmask);
    int deleteGateway(QString gateway);
    int deleteAllDefaultGateway();

    void gpioTest(bool value);
    int setSystemTime(t_DateTime &dateTime);
    int setSystemTime(int year, int month, int day, int hour, int min);
    QString getHourAndMinFromSystemTime();
    QString getYearAndMonthAndDayFromSystemTime();
    QString getDateAndTime(int year, int month, int day, int hour, int min);
    int getValueFromSystemTime(QString type);
    void screenshot();
    void UpdateTime();
    void Sleep();
    int isIpAndGatewaySameNetNumber(QString ip, QString netmask, QString gateway);
    QString getLocalUriHead();

    int resetFaultIndex();
    int setFaultIndex(int index);
    int compareFaultIndex(int index);
    int setKeyValue(QString key);
    QString getKeyValue();

    int getStringCount(QString text);

    QString getHWVersion();
    int copyLog();
    bool isDataExist();

    void lanTest();
    void logicSync();
    void exitETCS();
    void timerUpdate200();

    void setButtonEnable(QString type, bool bEnable);
    void resetConfigPwd();

    bool setVlan(int id, int priority, QString ip, QString mask);
    QString getVLANInfo();
    bool isVlanIDValid(QString vlanID);
    bool isVlanpriorityValid(QString vlanPriority);

    void lampTest(bool isActivate);

signals:
    void transitData(CResultBase* pResult);
    void signalCallStatus(int id, QString text);
    void setStatusBarMessageSignal(QString text, QString key);
    void removeStatusBarMessageSignal(QString key);
    void insertImgSignal(QString img);
    void removeImgSignal(QString img);
    void waitMaskHide();
    void updateDkmNumberSignal(QString num, QString total);
    void micActivateSignal(int id);
    void micInactiveSignal(int id);
    void gongPlayingTrueSignal(int id);
    void gongPlayingFalseSignal(int id);
    void syncDetailInfoSignal(int id, int current, QString status);
    void activateAddSignal();
    void activateDeleteSignal(int id);
    void noActivateSignal();
    void addPredefineSignal();
    void addScheduleSignal(int index);
    void updateScheduleSignal(int index);
    void updateScheduleSignal(int index, bool value);
    void updateConnectResultSignal();
    void addHistorySignal(int index);
    void delHistorySignal(int index);
    void updateSourceSignal(int id, int index);
    void updateZoneStatusSignal(int index, int status);
    void updateZoneStatusAll();
    void setNodeVolListSignal();
    void updateVolumeResultSignal(int node,
                                  unsigned char type,
                                  unsigned short number,
                                  unsigned char channel,
                                  int errorCode);
    void netTestSignal(int port, int errorCode);
    void systemResultSignal(int id, int ret);
    void audioDatabaseChangedSignal(int node,  QString path);
    void setBeepOffBtnStatusSignal(bool value);
    void localRecordStatusSignal(int errorNum);
    void selfTestSignal(bool isActived);

private:
    QString getActivateTime(t_DateTime &activate_time);
    QString getActivateStatus(CONSTATE &status);
    QString getActivateZones(std::vector<CZone*> &vZone);
    int getEmergency(bool emergency);
    QString getLocalSources(std::vector<std::string> &vSource);
    QString getNetworkSources(std::vector<t_Audio> &vSource);
    int getEmergencyActivateCount();
    QString getListTime(t_DateTime &list_time);
    int get1Count(int value);
    unsigned int getIpFromString(QString ip);
    unsigned int getNetNumberFromIp(unsigned int ip, unsigned int netmask);
    unsigned int getHostNumberFromIp(unsigned int ip, unsigned int netmask);

    bool isValidByPlatForm();
    void writeSettings(QString file, QString section, QString key, QString data);
    QString readSettings(QString file, QString section, QString key);
    QString getFileNameFromPath(QString path);

    bool isVlanConfigValid(t_VLANConfig &config);

    Models::ListModel *m_activateModel;
    Models::ListModel *m_faultModel;
    Models::ListModel *m_predefineModel;

    int m_eth0ConnectStatus;
    int m_eth1ConnectStatus;
    int m_standalone;
    int m_realWidth;
    int m_realHeight;

    int m_faultIndex;
    QString m_key;
    e_MicState m_micState;
    int m_hwVersion[4];

    QString m_srcFullName;
    QString m_desFullName;

};

#endif // CQMLLOGIC_H
