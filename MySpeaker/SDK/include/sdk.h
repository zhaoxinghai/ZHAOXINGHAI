
#ifndef SDK_H
#define SDK_H

#include "sdkdefine.h"
#include "sdkcallback.h"
#include "sdkerror.h"
#include "sdkconnect.h"
#include "device.h"
#include "version.h"

//this is for function callback
typedef void(*SDKCallback)(const CResult *pResult);

extern "C"
{
    //debug mode can print all the debug info
    void SDK_API EnableDebug();

    //set audio sound card device,example below
    //ch1 will be:1,2,4,5,6
    //ch2 will be:9,10,11,12,13,14
    int SDK_API AddSoundCardDevice(std::string strName, bool bCapture,std::vector<int> &vPortCh1, std::vector<int> &vPortCh2);

    //every 200 minseconds will update,unit dB
    float SDK_API GetLevelmeter(bool bCapture,int nPort);

    //register a global callback function
    bool SDK_API RegisterCallback(SDKCallback callback);

    //Set multicast base address
    void SDK_API SetBaseAddress(int nBaseAddress);

    //Init all system
    bool SDK_API Init(t_Node &curNode, std::vector<t_Node> &vSystem);

    //Init File MAP
    void SDK_API SetFileMAP(std::vector<t_FILEMAP> &vFileMap);

    //get file path from channel-title
    bool SDK_API GetFileMAP(int nChannel,int nTitle,t_FILEMAP &file);

    //Monitor busy state
    void SDK_API MonitorBusyState();

    //Monitor life signal
    void SDK_API MonitorLifeSignal();

    //begin to run the thread
    bool SDK_API Run();
    bool SDK_API Exit(bool bWait);

    //this function update the device state
    void SDK_API UpdateSysState(bool bLocalError);

    //synchronize time and date(datatime master)
    int SDK_API SyncDataTime(t_DateTime mytm);

    //normal announcement(nNode is audio source or scu)
    int  SDK_API NetworkAnnouncement(int nNode, CAnnouncement* pActivate);
  
    //local microphone announcement
    int SDK_API LocalMicrAnnouncement(CAnnouncement *pActivate);

    //local file announcement
    int SDK_API LocalFileAnnouncement(CAnnouncement *pActivate);

    //stop announcement
    int SDK_API StopCall(int chRequest);

    //audio stream record
    int SDK_API LocalRecord(int chRequest,int nPort,std::string strFilePath);
    int SDK_API StopLocalRecord(int chRequest);

    //call volume ajust
    void SDK_API VolumeAjust(int chRequest,float fVolumedB);

    //device volume control
    void SDK_API SetVolume(int nNode,bool bCapture,int nPort,float fVolumedB);
    void SDK_API SetStepVolume(int nNode, bool bCapture, int nPort, float fSetupVolumedB);

    //check audio file
    bool SDK_API CheckAdpFile(std::string strPath);
    bool SDK_API CheckWavFile(std::string strPath);
}

#endif
