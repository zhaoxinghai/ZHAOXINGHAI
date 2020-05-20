
#include <mutex>
#include <queue>
#include "sdk.h"
#include <iostream>
#include "common.h"
#include "mysocket.h"

std::vector<int> g_vID;

static void CallBackLib(const CResult* /*pResult*/)
{
}

std::string GetParameter(std::vector<std::string> &vCmd,std::string str)
{
    std::vector<std::string>::iterator it = vCmd.begin();
	while(it != vCmd.end())
	{
		if(str==(*it))
		{
			if(str == "-h")
			{
				return "-h";
			}
			else if(str == "-help")
			{
				return "-help";
			}
			else
			{
				if((it+1)==vCmd.end())
					return "";

                std::string strRet = *(it+1);
				it = vCmd.erase(it);
				it = vCmd.erase(it);
				return strRet;
			}
		}
		it++;
	}
	return "";
}

void Command_help()
{
    std::cout << "#############################################" << std::endl;
    std::cout << "[local]            ----local announcement" << std::endl;
    std::cout << "[live]             ----live announcement" << std::endl;
    std::cout << "[net]              ----network announcement" << std::endl;
    std::cout << "[stop]             ----stop all announcement" << std::endl;
}

void helpparam()
{
    std::cout << "[-d?]           ----destination port from 1 to 256, '?' is nodeID from 1 to 255"<<std::endl;
    std::cout << "[-g]            ----gong 0,1,2,3"<<std::endl;
    std::cout << "[-volume]       ----volume from 0 to 100"<<std::endl;
    std::cout << "[-reconnect]    ----reconnect 0 or 1"<<std::endl;
    std::cout << "[-partconnect]  ----part connect 0 or 1"<<std::endl;
    std::cout << "[-priority]     ----priority from 1 or 250,1 is highest"<<std::endl;
}

void setparam(std::vector<std::string> &vCmd,CAnnouncement* pAct)
{
    /*
    std::string str = GetParameter(vCmd,"-g");
    if(str.length()>0)
    {
        pAct->nGongChannel = 31;
    	pAct->nGongTitle = atoi(str.c_str());
    }
    else
    {
        pAct->nGongChannel = 31;
    	pAct->nGongTitle = 1;
    }

    str = GetParameter(vCmd,"-reconnect");
	if(str.length()>0 && str=="1")
	{
		SET_RECONNECT(pAct->Flags);
	}

	str = GetParameter(vCmd,"-partconnect");
	if(str.length()>0 && str=="1")
	{
		SET_PARTIAL_CONNECTION(pAct->Flags);
	}
	else
	{
		SET_RECONNECT_REQUEST(pAct->Flags);
	}

	str = GetParameter(vCmd,"-volume");
	if(str.length()>0)
	{
		pAct->Level = atoi(str.c_str());
	}

	str = GetParameter(vCmd,"-priority");
	if(str.length()>0)
	{
		pAct->Priority = atoi(str.c_str());
	}

	//destination
	for(unsigned int i = 0;i<250;i++)
	{
		t_Destination dest;
		dest.Sys = i+1;

        std::string strNode = CCommon::StrFormat("-d%d",i+1);
		str = GetParameter(vCmd,strNode);
		if(str.length()<=0)
			continue;

		while(str.length()>0)
		{
            dest.vOutputPort.push_back(atoi(str.c_str()));
			str = GetParameter(vCmd,strNode);
		}
		pAct->vDest.push_back(dest);
	}
    */
}

void Command_localhelp()
{
    std::cout << std::endl;
    std::cout << "[-s]            ----source file full path"<<std::endl;

    helpparam();
    std::cout << "[-repeat]       ----sources repeat count"<<std::endl;

    std::cout << "example:"<<std::endl;
    std::cout<<"#>local -s /test.adp -d47 41 -g 2";
}

void Command_livehelp()
{
    std::cout << std::endl;
    std::cout << "[-s]            ----source port"<<std::endl;
    helpparam();

    std::cout << "example:"<<std::endl;
    std::cout<<"#>live -s 41 -d47 41 -g 2";
}

void Command_nethelp()
{
    std::cout << std::endl;
    std::cout << "[-n]            ----network node from 1 to 250"<<std::endl;
    std::cout << "[-s]            ----network source channel-title"<<std::endl;
	helpparam();

    std::cout << "example:"<<std::endl;
	std::cout<<"#>net -n 236 -s 11-3 -s 11-2 -d47 41 -g 2";
}

void Command_local(std::vector<std::string> &vCmd)
{
    /*
    std::string str = GetParameter(vCmd,"-h");
    std::string str2 = GetParameter(vCmd,"-help");
	if(str.length()>0 || str2.length()>0)
	{
        Command_localhelp();
		return;
	}

    CAnnouncementFile activate;
    activate.nProcess = SDK_API::GetChProcess();
    g_vID.push_back(activate.nProcess);

    //source
	str = GetParameter(vCmd,"-s");
	if(str.length()<=0)
		return;

	while(str.length()>0)
	{
		activate.vAudioPath.push_back(str);
		str = GetParameter(vCmd,"-s");
	}

	//repeat
	str = GetParameter(vCmd,"-repeat");
	if(str.length()>0)
	{
		activate.nRepeatCount = atoi(str.c_str());
	}

	setparam(vCmd,&activate);
    SDK_API::LocalPlayAnnouncement(&activate);
    */
}

void Command_live(std::vector<std::string> &vCmd)
{
    std::string str = GetParameter(vCmd,"-h");
    std::string str2 = GetParameter(vCmd,"-help");
	if(str.length()>0 || str2.length()>0)
	{
        Command_livehelp();
		return;
	}

    CAnnouncementMicr activate;
    activate.nProcess = SDK_API::GetChProcess();
    g_vID.push_back(activate.nProcess);


    //source
	str = GetParameter(vCmd,"-s");
	if(str.length()<=0)
		return;
	activate.nPort = atoi(str.c_str());

	setparam(vCmd,&activate);
    SDK_API::LocalMicrAnnouncement(&activate);
}

void Command_net(std::vector<std::string> &vCmd)
{
    std::string str = GetParameter(vCmd,"-h");
    std::string str2 = GetParameter(vCmd,"-help");
	if(str.length()>0 || str2.length()>0)
	{
        Command_nethelp();
		return;
	}

    CAnnouncement activate;
    activate.nProcess = SDK_API::GetChProcess();
    g_vID.push_back(activate.nProcess);

    //source
	str = GetParameter(vCmd,"-s");
	if(str.length()<=0)
		return;

	while(str.length()>0)
	{
        std::vector<std::string> vTemp;
		CCommon::Split(vTemp,str,"-");
		if(vTemp.size()==2)
		{
			t_AudioSrc src;
			src.Num = atoi(vTemp[0].c_str())-1;
			src.Title = atoi(vTemp[1].c_str())-1;
			activate.vSrc.push_back(src);
		}
		str = GetParameter(vCmd,"-s");
	}

	setparam(vCmd,&activate);

	str = GetParameter(vCmd,"-n");
	if(str.length()<=0)
		return;
    SDK_API::NormalAnnouncement(atoi(str.c_str()), &activate);
}

#ifndef _MSC_VER
#include <alsa/asoundlib.h>
#endif

void Command_samplerate()
{
#ifndef _MSC_VER
	snd_pcm_t *handle;
    int err;
    snd_pcm_hw_params_t *hwparams;
    snd_pcm_hw_params_alloca(&hwparams);
	int dir = 0;

	//open device
    if ((err = snd_pcm_open(&handle,"default",SND_PCM_STREAM_CAPTURE, 0)) < 0)
	{
		CCommon::Trace("snd_pcm_open");
		return;
	}

	//fill parameters with default values
    err = snd_pcm_hw_params_any(handle, hwparams);
	if (err < 0)
	{
        CCommon::Trace("snd_pcm_hw_params_any");
		return;
	}

	//set the interleaved read/write format
    err = snd_pcm_hw_params_set_access(handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED);
	if (err < 0)
	{
        CCommon::Trace("snd_pcm_hw_params_set_access");
		return;
	}

	//set the sample format 16 bit little-endian
	snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;
    err = snd_pcm_hw_params_set_format(handle, hwparams,format);
	if (err < 0)
	{
        CCommon::Trace("snd_pcm_hw_params_set_format");
		return;
	}

	//set the count of channels
    err = snd_pcm_hw_params_set_channels(handle, hwparams,2);
	if (err < 0)
	{
        CCommon::Trace("snd_pcm_hw_params_set_channels");
		return;
	}

	//set the sampling rate
    unsigned int rate = 48000;
    err = snd_pcm_hw_params_set_rate_near(handle, hwparams, &rate, &dir);
	if (err < 0)
	{
        CCommon::Trace("snd_pcm_hw_params_set_rate_near");
		return;
	}

	//set the period size
    snd_pcm_uframes_t period = 418;
    err = snd_pcm_hw_params_set_period_size_near(handle, hwparams, &period, &dir);
	if (err < 0)
	{
        CCommon::Trace("snd_pcm_hw_params_set_period_size_near");
		return;
	}

	//set buffer size
    snd_pcm_uframes_t bufsize = 418;
    err = snd_pcm_hw_params_set_buffer_size_near(handle, hwparams, &bufsize);
	if (err < 0)
	{
        CCommon::Trace("snd_pcm_hw_params_set_buffer_size_near");
		return;
	}

	//write the parameters to device
    err = snd_pcm_hw_params(handle, hwparams);
	if (err < 0)
	{
        CCommon::Trace("snd_pcm_hw_params");
		return;
	}

	//get one period
	unsigned long peroidFrames;
    snd_pcm_hw_params_get_period_size(hwparams, &peroidFrames,&dir);
	CCommon::Trace("one period frames:%d",peroidFrames);

	//get buffer size
	unsigned long buffersize;
    snd_pcm_hw_params_get_buffer_size(hwparams, &buffersize);
	CCommon::Trace("buffer size:%d", buffersize);

	//sample rate
	unsigned int rrate;
    snd_pcm_hw_params_get_rate(hwparams,&rrate,&dir);
	CCommon::Trace("really sample rate:%d", rrate);

	//snd_pcm_hw_params_free(params);
	if((err = snd_pcm_prepare(handle))<0)
	{
		CCommon::Trace("%s", "Cannot prepare audio interface for use");
		return;
	}

	//start
	snd_pcm_start(handle);

    //loop capture
    char Data[1024 * 10];
    int nTestCount = 0;
    long nTestframe = 2300;
    long long total = nTestframe*418;
    std::chrono::time_point<std::chrono::high_resolution_clock> begin = std::chrono::high_resolution_clock::now();\

    while (true)
    {
        snd_pcm_sframes_t frames = snd_pcm_readi(handle, &Data[0], 418);
        if (frames == -EPIPE)
        {
            //EPIPE means overrun
            snd_pcm_prepare(handle);
            CCommon::Trace("snd_pcm_readi error %d", frames);
        }
        else if (frames < 0)
        {
            CCommon::Trace("snd_pcm_readi error %d", frames);
        }
        else
        {
            nTestCount++;
            if(nTestCount == nTestframe)
            {
                nTestCount = 0;
                std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
                long long time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
                begin = end;

                long nRate = total*1000/time;
                std::string str = CCommon::StrFormat("sample rate:%ld",nRate);
                CCommon::Trace("%s",str.c_str());
            }
        }
    }
    snd_pcm_close(handle);
#endif
}

void command(std::vector<std::string> &vCmd)
{
    if (vCmd.size() == 0)
        return;

    if (vCmd[0] == "help")
    {
        Command_help();
    }
    else if (vCmd[0] == "local")
    {
        Command_local(vCmd);
    }
    else if (vCmd[0] == "net")
    {
        Command_net(vCmd);
    }
    else if (vCmd[0] == "live")
    {
        Command_live(vCmd);
    }
    else if (vCmd[0] == "stop")
    {
        for(unsigned int i = 0;i<g_vID.size();i++)
        {
            SDK_API::StopAnnouncement(g_vID[i]);
        }
        g_vID.clear();
    }
}

#define TEST_RATE 0
#define TEST_M_UDP 0

int main(int count,char** arg)
{
#if TEST_RATE
    Command_samplerate();
    return 0;
#endif

    if(CCommon::IsMatch(count,arg,"-debug"))
    {
        SDK_API::EnableDebug();
    }

	if(count < 2)
	{
		CCommon::Trace("Argements error,need ip address argement.");
		return 0;
	}

    std::string strIP = std::string(arg[1]);

    //SDK_API::SetVersion(PROTOCOL_V10);
    SDK_API::RegisterCallback(CallBackLib);
    SDK_API::MonitorLifeSignal();
    SDK_API::MonitorBusyState();

    //sound card
    std::vector<int> vPortCh1;
    std::vector<int> vPortCh2;
    vPortCh1.push_back(41);
    vPortCh2.push_back(42);
    SDK_API::AddSoundCardDevice("default", true, vPortCh1, vPortCh2);
    SDK_API::AddSoundCardDevice("default", false, vPortCh1, vPortCh2);

    t_Node curNode;
    //curNode.eType = DEVICE_ETCS;
    curNode.strIP = strIP;
    size_t pos = curNode.strIP.rfind('.');
    std::string strPre = curNode.strIP.substr(0, pos);
    std::string strNext = curNode.strIP.substr(pos+1, curNode.strIP.length() - pos-1);
    curNode.nNode = atoi(strNext.c_str());

    std::vector<t_Node> vNode;
    for (int i = 2; i <= 250; i++)
    {
        if (i == curNode.nNode)
            continue;

        t_Node Node;

        //Node.eType = DEVICE_4_24;
        Node.nNode = i;
        Node.strIP = CCommon::StrFormat("%s.%d", strPre.c_str(),Node.nNode);
        vNode.push_back(Node);
    }
    SDK_API::Init(curNode, vNode);
    SDK_API::Run();

    Command_help();
   
    //loop
    char cmd[1024];
    while (true)
    {
        std::cout << std::endl << "#>";

        memset(cmd, 0, sizeof(cmd));

        std::cin.getline(cmd, 1024);
        std::string str(cmd);
        std::vector<std::string> vCmd;
        CCommon::Split(vCmd, str, " ");

        if(vCmd.size()==0)
        	continue;

        if (vCmd[0] == "exit")
        {
            SDK_API::Exit(true);
            break;
        }
        command(vCmd);
    }
    return 0;
}
