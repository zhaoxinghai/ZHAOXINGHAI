
#ifndef SDK_H
#define SDK_H

#include "predefine.h"
#include "callback.h"
#include "paerror.h"
#include "connection.h"
#include "device.h"
#include "detectmemoryleak.h"
#include "version.h"

//this is for function callback
typedef void(*PACallback)(const CResultBase *pResult);

namespace PA
{
    //Mode config/normal
    void DLL_API SwitchMode(e_Mode eMode);

    //Enable accept remote connect
    void DLL_API EnableAccept(bool bEnable);

    //Enable security
    void DLL_API EnableSecurity(bool bEnable);

    //is audio job overrun
    bool DLL_API IsAudioJobOverRun(int &nRunningCount);

    //set audio sound card device,example below
    //ch1 will be:1,2,4,5,6
    //ch2 will be:9,10,11,12,13,14
    int DLL_API AddSoundCardDevice(std::string strName, bool bCapture,std::vector<int> &vPortCh1, std::vector<int> &vPortCh2);
    void DLL_API InitVolume(float fSG,float fMic,float fLineIn,float fSpeaker,float fLineOut);

    //every 200 minseconds will update,unit dB
    float DLL_API GetLevelmeter(bool bCapture,int nPort);

    //register a global callback function
    bool DLL_API RegisterCallback(PACallback callback);

    //Init input parameter
    void DLL_API InitArg(int count,char** arg);

    //Set multicast base address
    void DLL_API SetBaseAddress(int nBaseAddress);

    //Init all system
    bool DLL_API Init(t_Node &curNode, std::vector<t_Node> &vSystem);

    //Init File MAP
    void DLL_API SetFileMAP(std::vector<t_FILEMAP> &vFileMap);

    //get file path from channel-title
    bool DLL_API GetFileMAP(int nChannel,int nTitle,t_FILEMAP &file);

    //Init Configuration
    void DLL_API Init(t_Configuration &c);

    //Monitor busy state
    void DLL_API MonitorBusyState();

    //Monitor lifesignal
    void DLL_API MonitorLifeSignal();

    //get a unique id
    unsigned short DLL_API GetChProcess();

    //begin to run the thread
    bool DLL_API Run();
    bool DLL_API Exit(bool bWait);
    
    //this function update the device state
    void DLL_API UpdateSysState(bool bLocalError);

    //synchronize time and date(datatime master)
    int DLL_API SyncDataTime(t_DateTime mytm);

    //Get device state
    void DLL_API GetDevicesState(int nNode, t_VDevice dev);

    //loger
    void DLL_API LogDebug(const char *szFormat,...);
    void DLL_API LogError(const char *szFormat, ...);

    //normal announcement(nNode is audio source or scu)
    int  DLL_API NormalAnnouncement(int nNode,CActivate* pActivate);
  
    //local microphone announcement
    int DLL_API LocalMicrAnnouncement(CActivateMicr *pActivate);

    //from local disk or USB to net(source is local path)
    int DLL_API LocalPlayAnnouncement(CActivatePlay *pActivate);

    //stop announcement
    int DLL_API StopAnnouncement(int chProcess);

    //audio stream record
    int DLL_API LocalRecord(int chRequest,int nPort,std::string strFilePath);
    int DLL_API StopLocalRecord(int chRequest);

    //Volume ajust
    void DLL_API VolumeAjust(int chProcess,float fVolumedB);
    void DLL_API SetVolume(bool bCapture,int nPort,float fVolumedB);

    //check audio file
    bool DLL_API CheckAdpFile(std::string strPath);
    bool DLL_API CheckWavFile(std::string strPath);

    //transport line
    void DLL_API TransportLine(int nNode,bool bAction,std::vector<t_TL> vTL);

    //Telnet command
    std::string DLL_API TelnetCommand(std::string strCmd);
    void DLL_API TextOutput(MYSOCK tcpSock,std::string strResult);

    //volume control
    void DLL_API AudioControl(int nNode,const t_VDevice &de,float fVolume);

    //Audio control result
    void DLL_API ReplyAudioControl(bool IsTCP,int nNode,MYSOCK tcpSock,t_VolReply &reply);

    //Enable CCCF function
    void DLL_API EnableCCCF(bool bOpen);

    //LampTest for CCCF
    void DLL_API LampTest(bool bActivate);
}

#endif
