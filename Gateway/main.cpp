
#include <mutex>
#include <queue>
#include <iostream>
#include "common.h"
#include "threadpool.h"
#include "audioreceive.h"
#include "audioplay.h"

#ifdef WIN32
#include "threadcapture.h"
CThreadCapture  g_ThreadCapture;
#endif

ThreadPool      g_ThreadPool;
int             g_nProcess;

e_SEND_ENCODE GetType(std::string strType)
{
    if(strType == "adpcm")
        return ENCODE_ADP;
    else if(strType == "g711a")
        return ENCODE_G711a;
    else if(strType == "g711u")
        return ENCODE_G711u;
    else if(strType == "g722")
        return ENCODE_G722;
    else
        return ENCODE_UNKNOW;
}

void Command_help()
{
#ifndef WIN32
    std::cout << "play num thread audio to network" << std::endl;
    std::cout << "[adpcm] [num]" << std::endl;
    std::cout << "[g711a] [num]" << std::endl;
    std::cout << "[g711u] [num]" << std::endl;
    std::cout << "[g722]  [num]" << std::endl;
#else
    std::cout << "recv num thread audio from network" << std::endl;
    std::cout << "[adpcm] [g711a] [num]   //recv adpcm to g711a" << std::endl;
    std::cout << "[adpcm] [g711u] [num]   //recv adpcm to g711u" << std::endl;
    std::cout << "[adpcm] [g722]  [num]   //recv adpcm to g722" << std::endl;
    std::cout << std::endl;
    std::cout << "[g711a] [adpcm] [num]   //recv g711a to adpcm" << std::endl;
    std::cout << "[g711u] [adpcm] [num]   //recv g711u to adpcm" << std::endl;
    std::cout << "[g722]  [adpcm] [num]   //recv g722 to adpcm" << std::endl;
#endif
}

void Command_play(int nThread,e_SEND_ENCODE eType)
{
#ifdef WIN32
    g_ThreadCapture.BeginThread();

    for(int i = 1;i<=nThread;i++)
    {
        CAudioPlay *Job = new CAudioPlay(LOCAL_ADP2NET);
        Job->SetRepeatCount(255,0);
        Job->SetChProcess(g_nProcess++);

        std::string strFile = CCommon::StrFormat("%stest.adp",
                                    CCommon::GetModulePath().c_str());
        std::vector<std::string> vPath;
        vPath.push_back(strFile);
        Job->SetAudioPath(vPath);
        Job->SetRtpNode(218);
        Job->SetRtpChannel(i+eType*8);
        Job->m_eAudioEncoding = eType;

        g_ThreadPool.push(Job);
        Job->Start();
    }
#endif
}

void Command_recv(int nThread,e_SEND_ENCODE eFrom,e_SEND_ENCODE eTo)
{
    for(int i = 1;i<=nThread;i++)
    {
        CAudioReceive *Job = new CAudioReceive();
        Job->SetChProcess(g_nProcess++);
        Job->SetRtpNode(218);
        Job->SetRtpChannel(i+eFrom*8);
        Job->m_eFromCode = eFrom;
        Job->m_eToCode = eTo;
        Job->m_eAudioEncoding = eTo;
        g_ThreadPool.push(Job);
        Job->Start();
    }
}

bool command(std::vector<std::string> &vCmd)
{
    if(vCmd.size()==2)
    {
        int nThread = atoi(vCmd[1].c_str());
        e_SEND_ENCODE type = GetType(vCmd[0]);
        if(type == ENCODE_UNKNOW)
        {
            printf("parameter Error!\n");
            return false;
        }
        Command_play(nThread,type);
    }
    else if(vCmd.size()==3)
    {
        int nThread = atoi(vCmd[2].c_str());
        e_SEND_ENCODE eFrom = GetType(vCmd[0]);
        e_SEND_ENCODE eTo = GetType(vCmd[1]);
        if(eFrom==ENCODE_UNKNOW || eTo==ENCODE_UNKNOW)
        {
            printf("parameter Error!\n");
            return false;
        }
        Command_recv(nThread,eFrom,eTo);
    }
    else
    {
        return false;
    }
    return true;
}

int main(int count,char** arg)
{
    //start audio thread
    g_ThreadPool.start(8);
    g_nProcess = 1;

    //get command
    std::vector<std::string> vCmd;
    for(int i = 1;i<count;i++)
    {
        std::string str(arg[i]);
        vCmd.push_back(str);
    }
    if(vCmd.size()==0)
    {
        printf("Error:vCmd.size()==0\n");
    }

    if(command(vCmd))
    {
        for(int i = 0;i<10000;i++)
        {
            CCommon::SleepMinisecond(1000);
        }
    }
    return 0;
}
