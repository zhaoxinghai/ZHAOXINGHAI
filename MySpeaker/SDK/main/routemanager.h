
#ifndef ROUTE_MANAGER_H
#define ROUTE_MANAGER_H

#include "sdkdefine.h"
#include "sdkconnect.h"
#include "audioplay.h"
#include "audiocapture.h"
#include "audioreceive.h"
#include "topsystem.h"
#include "threadrecv.h"
#include "threadsend.h"
#include "threadaccept.h"
#include "threadtimer.h"
#include "define.h"
#include "routelocal.h"
#include "routeremote.h"
#include "routenetwork.h"

class CRoute;
class CRouteLocal;
class CRouteRemote;

class CRouteManager
{
    struct t_AudioDevice
    {
        bool bCapture;
        int  nPort;
        std::string strDeviceName;
    };

public:
    CRouteManager();
    virtual ~CRouteManager();

    void Init();

    //the audio running job
    CAudioJob* GetAudio(int chProcess);
    void AddAudio(CAudioJob* pJob);
    void StartAudio(int chProcess);
    void PauseAudio(int chProcess);
    void StopAudio(int chProcess);
    int StopRecord(int nRecordID);
    void RemoveAudio(int chProcess);
    void StopAllAudio();
    int GetCapturePlayAudio();
    std::string Constat();

    void VolumeAjustAudio(int chRequest,float fVolumeScale);
    void VolumeAjustAudioSG(float fVolumeScale);
    void AllocatePort(int chRequest,bool bCapture,int nPort,int nPriority,int nRTPChannel);
    void FreePort(int chRequest, bool bCapture, int nPort);

    //by the route's process id
    CRoute* GetRouteByProcess(int chProcess);
    CRoute* GetRouteByProcessRequest(int chProcess,int chRequest);
    
    CRouteRemote* GetRemoteRoute(int nNode,int chRequest);

    //by the result
    CRoute* GetRouteByRequest(int chRequest);
   
    void AddRoute(CRoute* pRoute);
    void CleanRoute();

    //for usagereport(V11)
    void GetLocalUsageRoute(std::vector<CRouteLocal*> &vLocal);
    void GetRemoteUsageRoute(std::vector<CRouteRemote*> &vRemote);

    //interrupt and restore
    void InterruptInput(CRouteLocal* pNewRoute);
    void InterruptOutput(CRouteRemote* pNewRoute);
    void InterruptAllOutput(int nPort);
    void InterruptInput(int nPort);

    void RestoreInput();
    void RestoreOutput();
   
    //every second timer
    void EverySecond();

    //get rtpchannel
    int GetChannel();
    void RemoveChannel(int nChannel);

    //the device port
    void SetDevice(bool bCapture, int nPort, std::string strDeviceName);
    std::string GetDevice(bool bCapture, int nPort);

    void AudioJobFinish(CAudioJob* pJob);
    void AudioJobFinishPause(CAudioJob* pJob);
    void CurrentPlayIndex(int chProcess, int nIndex);

    int  m_nRemoteMaxCount;
    int  m_nPlayingMaxCount;

protected: 
    void InterruptOutput(CRouteRemote* pRoute, CRouteRemote* pNewRoute);

    void GetConflictPort(
        std::vector<t_Output> &vSrc, 
        std::vector<t_Output> &vDest,
        std::vector<int> &vConflict);

    //announcement list
    std::vector<CRoute*>    m_vRoute;
    std::mutex              m_mutexRoute;

    //on thread audio list(local audio not in the route)
    std::vector<CAudioJob*> m_vAudioRun;
    std::mutex              m_mutexAudio;

    //rtp to net play channel
    bool                    m_RtpPort[128];
    int                     m_nChannelLast;

    //device dsp name
    std::vector<t_AudioDevice>   m_vAudioDevice;

    //restore output
    int                     m_nRestoreSec;
    bool                    m_bRestoreOutput;
    bool                    m_bRestoreInput;

    //speaker amplifer
    FILE*                   m_AmpHandle;
};

#endif

