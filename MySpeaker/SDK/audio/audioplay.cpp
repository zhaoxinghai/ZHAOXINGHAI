
#include "audioplay.h"
#include "common.h"
#include "mylog.h"
#include "threadtimer.h"
#include "sdkerror.h"
#include "service.h"
#include "routemanager.h"
#include "sdkcallback.h"
#include "public.h"

CAudioPlay::CAudioPlay(e_PLAY_TYPE type)
{
#if 0
    float sampleFloat[ED1AudioFrameSampleCount];
    memset(sampleFloat,0,sizeof(sampleFloat));
    m_audioEncode.Encode(m_SilenceADP, sampleFloat);

    std::ofstream file;
    file.open("C:\\Doc\\silence100.adp",std::ios::binary|std::ios::app);
    for(int i = 0;i<11483;i++)
    {
        file.write((const char*)&sampleFloat[0],ED1AudioAdpFrameCount);
    }
    file.close();
#endif

    m_type = type;
    m_className = "CAudioPlay";
    m_nRepeatCount = 1;
    m_nCurrentRepeat = 1;
    m_sequenceNo = 0;
    m_bHaveGong = false;
    m_bAudioSignal = false;
    m_bDecodingSignal = false;
    m_nCurPlayIndex = 0;
    m_bPlayFinish = false;

    m_pThreadDevice = g_SDKServer.m_vThreadCapture[0].get();

    //set adp frame count
    m_adpAudiobuffer.SetSize(ED1AudioAdpFrameCount*2);
    m_adpAudiobufferSG.SetSize(ED1AudioAdpFrameCount * 2);
    m_adpAudiobufferEG.SetSize(ED1AudioAdpFrameCount * 2);
}

CAudioPlay::~CAudioPlay()
{
    g_SDKServer.m_routeManager.RemoveChannel(m_rtpChannel);
}

e_PLAY_TYPE CAudioPlay::GetType()
{
    return m_type;
}

void CAudioPlay::SetRepeatCount(int nRepeat)
{
    m_nRepeatCount = nRepeat;
    if(m_nRepeatCount == 0)
    {
        m_nRepeatCount = 1;
    }
}

void CAudioPlay::SetAudioPath(std::vector<t_FILEMAP> &vPath)
{
    //gong
    if (m_PreSignalFile.vPath.size()>0)
    {
        m_vAudioPath.push_back(m_PreSignalFile.vPath[0]);
        m_bHaveGong = true;
    }
    else
    {
        m_bHaveGong = false;
    }

    for (unsigned int i = 0; i < vPath.size(); i++)
    {
        t_FILEMAP &file = vPath[i];
        if(file.vPath.size()>0)
        {
            m_vAudioPath.push_back(file.vPath[0]);
        }
    }
}

void CAudioPlay::Run()
{
    g_SDKServer.PushRtpJob();

    if ( m_type == LOCAL_ADP2NET)
    {
        if(InitNet())
        {
            Loop();
        }
        else
        {
            LOG_ERROR("%s","CAudioPlay InitNet Error");
            m_bStop = true;
        }
        ExitNet();
    }
    else if( m_type == LOCAL_ADP2LISTEN)
    {
        Loop();
    }

    SendAudioSignal(false);
    g_SDKServer.PopRtpJob();

    if (m_bStop)
    {
        Finish();
    }
    else
    {
        FinishPause();  
    }

}

void CAudioPlay::Loop()
{
    //loop the job
    while (true)
    {
        if (m_bStop || m_bPause)
        {
            break;
        }
        //timeout,continue
        if (m_pThreadDevice->WaitSignal())
        {
            SendRtpFrame();
        }
    }
    CCommon::WaitMinisecond(200, &m_bManuStop);
}

void CAudioPlay::SendRtpFrame()
{
    //decode data to the buffer
    DecodeFile2Buffer();

    //read adp buffer
    unsigned char rtpFrameSend[ED1RtpFrameSizeInBytes];
    memset(rtpFrameSend, 0, sizeof(rtpFrameSend));
    unsigned char *adpData = &rtpFrameSend[ED1RtpHeaderSizeInBytes];
    
    int used = m_adpAudiobuffer.getUsedSize();
    int usedSG = m_adpAudiobufferSG.getUsedSize();
    int usedEG = m_adpAudiobufferEG.getUsedSize();

    e_BUFFER_TYPE eType = BUF_PRE_SIGNAL;

    if (usedSG >= ED1AudioAdpFrameCount)
    {
        eType = BUF_PRE_SIGNAL;
        m_adpAudiobufferSG.read((unsigned char*)adpData, ED1AudioAdpFrameCount);
        if(m_type == LOCAL_ADP2NET)
        {
            VolumeAjust(adpData,true);
        }
        SendPreSignal(true);
        SendAudioSignal(false);
    }
    else if(used >= ED1AudioAdpFrameCount)
    {
        eType = BUF_AUDIO;
        m_adpAudiobuffer.read((unsigned char*)adpData, ED1AudioAdpFrameCount);
        if(m_type == LOCAL_ADP2NET)
        {
            VolumeAjust(adpData,false);
        }
        SendPreSignal(false);
        SendAudioSignal(true);
    }
    else if(usedEG >= ED1AudioAdpFrameCount)
    {
        eType = BUF_END_SIGNAL;
        m_adpAudiobufferEG.read((unsigned char*)adpData, ED1AudioAdpFrameCount);
        if(m_type == LOCAL_ADP2NET)
        {
            VolumeAjust(adpData,false);
        }
        SendPreSignal(false);
        SendAudioSignal(false);
        SendEndSignal(true);
    }
    else
    {
        m_bStop = true;
        return;
    }

    if(m_bStop || m_bManuStop)
    {
        return;
    }

    //append checksum
    CPublic::AppendCheckSum(&adpData[0], ED1AudioAdpFrameCount);

    if(m_type == LOCAL_ADP2NET)
    {
        //set rtp header
        m_rtpFrameCreator.CreateRtpHeader((unsigned char*)&rtpFrameSend[0]);

        //send to net
        if (!Write((const char*)&rtpFrameSend[0], ED1RtpFrameSizeInBytes))
        {
            const int SecondCount = 114;
            if (!m_bManuStop && m_nWriteErrorCount > g_nWriteErrorCount)
            {
                if(m_bEnableVLAN)
                {
                    LOG_ERROR("%d %s", m_chProcess, "!m_Socket.Write(VLAN)");
                }
                else
                {
                    LOG_ERROR("%d %s", m_chProcess, "!m_Socket.Write");
                }
                m_error = ERROR_WRITE_UDP;
                m_bStop = true;
            }
        }
    }
    else if(m_type == LOCAL_ADP2LISTEN)
    {
        Adp2PcmBufferListening(&adpData[0],ED1AudioFrameSampleCount,eType);
    }
}

void CAudioPlay::DecodeFile2Buffer()
{
    if(m_bEnableSecurity)
    {
        DecodeAdpFile2AdpBuffer2();
    }
    else
    {
        DecodeAdpFile2AdpBuffer();
    }
}

void CAudioPlay::DecodeAdpFile2AdpBuffer()
{
    while (m_audioFile.IsAdpEnough())
    {
        unsigned char adpData[ED1AudioAdpFrameCount + 4];

        if (m_bDecodingSignal)
        {
            if (m_adpAudiobufferSG.getFreeSize() < ED1AudioAdpFrameCount)
            {
                return; //rtp buffer is full
            }
            //read one frame from adp file
            m_audioFile.ReadAdpFrame(&adpData[0]);
            m_adpAudiobufferSG.write(((unsigned char*)&adpData[0]), ED1AudioAdpFrameCount);
        }
        else
        {
            if (m_adpAudiobuffer.getFreeSize() < ED1AudioAdpFrameCount)
            {
                return; //rtp buffer is full
            }
            m_audioFile.ReadAdpFrame(&adpData[0]);
            m_adpAudiobuffer.write(((unsigned char*)&adpData[0]), ED1AudioAdpFrameCount);
        }
    }

    if (!OpenNewFile())
    {
        m_bStop = true;
        return;
    }
    else
    {
        DecodeAdpFile2AdpBuffer();
    }
}

bool CAudioPlay::OpenNewFile()
{
    if (m_bPlayFinish)
    {
        return false;
    }

    //no other file
    if (m_nCurPlayIndex >= (int)m_vAudioPath.size())
    {
        if (m_nRepeatCount == 0 || m_nCurrentRepeat < m_nRepeatCount)
        {
            //go to next repeat
            m_nCurrentRepeat++;
            m_nCurPlayIndex = 0;
        }
        else
        {
            m_bPlayFinish = true;
            return false;
        }
    }

    //open new file
    std::string strPath = m_vAudioPath[m_nCurPlayIndex];
    if(!m_audioFile.Open(strPath))
    {
        LOG_ERROR("open file error,path: %s", strPath.c_str());
        m_error = ERROR_FILE_OPEN;
        m_bPlayFinish = true;
        return false;
    }

    //gong result
    if (m_bHaveGong && m_nCurPlayIndex == 0)
    {
        m_bDecodingSignal = true;
    }
    else
    {
        m_bDecodingSignal = false;
    }

    //current play index
    auto pMsg = std::make_shared<CMsg>();
    pMsg->type = MSG_PLAY_INDEX;
    pMsg->nInt1 = m_chProcess;
    pMsg->nInt2 = m_nCurPlayIndex;
    g_SDKServer.Push(pMsg);

    m_nCurPlayIndex++;
    return true;
}

void CAudioPlay::DecodeAdpFile2AdpBuffer2()
{
    while (m_audioFile.IsAdpEnough())
    {
        unsigned char adpData[ED1AudioAdpFrameCount + 4];

        if (m_bDecodingSignal)
        {
            if (m_adpAudiobufferSG.getFreeSize() < ED1AudioAdpFrameCount)
            {
                return; //rtp buffer is full
            }
            //read one frame from adp file
            m_audioFile.ReadAdpFrame(&adpData[0]);
            m_adpAudiobufferSG.write(((unsigned char*)&adpData[0]), ED1AudioAdpFrameCount);
        }
        else
        {
            if (m_adpAudiobuffer.getFreeSize() < ED1AudioAdpFrameCount)
            {
                return; //rtp buffer is full
            }
            m_audioFile.ReadAdpFrame(&adpData[0]);
            m_adpAudiobuffer.write(((unsigned char*)&adpData[0]), ED1AudioAdpFrameCount);
        }
    }

    if (!OpenNewFile())
    {
        m_bStop = true;
        return;
    }
    else
    {
        DecodeAdpFile2AdpBuffer();
    }
}

void CAudioPlay::GetListeningPort(std::vector<int> &vPort)
{
    vPort.clear();
    for(unsigned int i = 0;i<m_vPort.size();i++)
    {
        vPort.push_back(m_vPort[i]);
    }
}
