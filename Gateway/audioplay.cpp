
#include "audioplay.h"
#include "common.h"
#include "paerror.h"
#include "public.h"
#include "threadcapture.h"

#ifdef WIN32
extern CThreadCapture g_ThreadCapture;
#endif

CAudioPlay::CAudioPlay(e_PLAY_TYPE type)
{
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
    m_nSilenceFrames = 0;
    m_bSendSilence = false;
    m_nSendSilenceCount = 0;
#ifdef WIN32
    m_pThreadDevice = &g_ThreadCapture;
#endif
    //set adp frame count
    m_adpAudiobuffer.SetSize(ED1AudioAdpFrameCount*2);
    m_adpAudiobufferSG.SetSize(ED1AudioAdpFrameCount * 2);

    m_pcm48kBuffer.SetSize(10*1024);
    m_pcm8kBuffer.SetSize(10*1024);
}

CAudioPlay::~CAudioPlay()
{
}

e_PLAY_TYPE CAudioPlay::GetType()
{
    return m_type;
}

void CAudioPlay::SetRepeatCount(int nRepeat,int nGapTime)
{
    m_nRepeatCount = nRepeat;
    if(m_nRepeatCount == 0)
    {
        m_nRepeatCount = 1;
    }
}

void CAudioPlay::SetAudioPath(std::vector<std::string> &vPath)
{
    //gong
    if (m_strPresignal.length()>0)
    {
        m_vAudioPath.push_back(m_strPresignal);
        m_bHaveGong = true;
    }
    else
    {
        m_bHaveGong = false;
    }

    for (unsigned int i = 0; i < vPath.size(); i++)
    {
        std::string strSrc = vPath[i];
        m_vAudioPath.push_back(strSrc);
    }
}

void CAudioPlay::SetEncodeType(e_SEND_ENCODE eType)
{
    m_eAudioEncoding = eType;
}

void CAudioPlay::Run()
{
    if(m_eAudioEncoding != ENCODE_ADP)
    {
        if(m_eAudioEncoding == ENCODE_G722)
        {
            m_resamplefactor = 0.333333333333f;
        }
        else
        {
            m_resamplefactor = 0.166666666666f;
        }
        m_resamplehandle = resample_open(0, m_resamplefactor, m_resamplefactor);
        m_resamplefwidth = resample_get_filter_width(m_resamplehandle);
    }
    if(m_eAudioEncoding == ENCODE_G722)
    {
        g722_encode_init(&m_g722EncodeState, G722_BIT_RATE, 0);
    }
#ifdef WIN32
    g_ThreadCapture.PushRtpJob();
#endif

    //init silence adp
    float sampleFloat[ED1AudioFrameSampleCount];
    memset(sampleFloat,0,sizeof(sampleFloat));
    m_audioEncode.Encode(m_SilenceADP, sampleFloat);

    if ( m_type == LOCAL_ADP2NET)
    {
        if(InitNet())
        {
            Loop();
        }
        else
        {
            printf("%s","CAudioPlay InitNet Error");
            m_bStop = true;
        }
        ExitNet();
    }
    else if( m_type == LOCAL_ADP2LISTEN)
    {
        Loop();
    }

    //AudioSignal(false);
#ifdef WIN32
    g_ThreadCapture.PopRtpJob();
#endif

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

void CAudioPlay::Start()
{
    CAudioJob::Start();
}

void CAudioPlay::Stop()
{
    CAudioJob::Stop();
}

void CAudioPlay::Pause()
{
    CAudioJob::Pause();
}

bool CAudioPlay::InitNet()
{
    //create socket
    if (!m_Socket.Create(SOCK_DGRAM))
    {
        printf("%d Socket create: %d", m_chProcess, m_Socket.GetLastError());
        m_error = ERROR_SOCKET_CREATE;
        return false;
    }

    if (!m_Socket.SetBuffer(2*496,496))
    {
        printf("%d SetBUffer:%d", m_chProcess, m_Socket.GetLastError());
        m_error = ERROR_SOCKET_SET_BUFFER;
        return false;
    }

    //set udp address
    std::string strDest = GetUdpAddress();
    unsigned short port = TRP_CHANNEL_START + this->m_rtpChannel;

    if (!m_Socket.SetUDPAddress(strDest.c_str(),port))
    {
        printf("%d %s", m_chProcess, "!m_Socket.SetMCastAddress");
        m_error = ERROR_MULTIPLE_ADDRESS;
        m_Socket.Close();
        return false;
    }
    m_Socket.SetTTL();
    m_nWriteErrorCount = 0;
    return true;
}

bool CAudioPlay::ExitNet()
{
    m_Socket.Close();
    return true;
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
    bool bSG = false;

    if (usedSG >= ED1AudioAdpFrameCount)
    {
        bSG = true;
        m_adpAudiobufferSG.read((unsigned char*)adpData, ED1AudioAdpFrameCount);
    }
    else if(used >= ED1AudioAdpFrameCount)
    {
        bSG = false;
        m_adpAudiobuffer.read((unsigned char*)adpData, ED1AudioAdpFrameCount);
    }
    else if(m_bSendSilence)
    {
        if(m_nSendSilenceCount<m_nSilenceFrames)
        {
            m_nSendSilenceCount++;
        }
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

    if(m_eAudioEncoding==ENCODE_ADP)
    {
        //append checksum
        CPublic::AppendCheckSum(&adpData[0], ED1AudioAdpFrameCount);

        //set rtp header
        m_rtpFrameCreator.CreateRtpHeader((unsigned char*)&rtpFrameSend[0]);

        //send to net
        if (!Write((const char*)&rtpFrameSend[0], ED1RtpFrameSizeInBytes))
        {
            printf("%d %s", m_chProcess, "!m_Socket.Write");
            m_error = ERROR_WRITE_UDP;
            m_bStop = true;
        }
    }
    else
    {
        //adp to pcm
        Adp2PcmBuffer(adpData, ED1AudioFrameSampleCount);

        //resample pcm float sample from 48k to 8k/16k
        ResampleFrom48k();

        //encoding
        Encoding(m_eAudioEncoding);

        int readlen = 160;
        unsigned char buf[1024];

        if(m_eAudioEncoding==ENCODE_G711a)
        {
            readlen = G711_FRAME_LEN;
            if(m_pcmG711a.getUsedSize()>=readlen)
            {
                m_pcmG711a.read(buf,readlen);
            }
            else
            {
                return;
            }
        }
        else if(m_eAudioEncoding==ENCODE_G711u)
        {
            readlen = G711_FRAME_LEN;
            if(m_pcmG711u.getUsedSize()>=readlen)
            {
                m_pcmG711u.read(buf,readlen);
            }
            else
            {
                return;
            }
        }
        else if(m_eAudioEncoding==ENCODE_G722)
        {
            readlen = G722_FRAME_LEN;
            if(m_pcmG722.getUsedSize()>=readlen)
            {
                m_pcmG722.read(buf,readlen);
            }
            else
            {
                return;
            }
        }
        //send to net
        if (!Write((const char*)&buf[0], readlen))
        {
            printf("%d %s", m_chProcess, "!m_Socket.Write");
            m_error = ERROR_WRITE_UDP;
            m_bStop = true;
        }
    }
}

void CAudioPlay::DecodeFile2Buffer()
{
    DecodeAdpFile2AdpBuffer();
}

void CAudioPlay::DecodeAdpFile2AdpBuffer()
{
    while (m_audioFile.IsAdpEnough()
           || (m_bSendSilence && m_nSendSilenceCount<m_nSilenceFrames))
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
            if(m_bSendSilence)
            {
                m_nSendSilenceCount++;
                m_adpAudiobuffer.write(m_SilenceADP, ED1AudioAdpFrameCount);
            }
            else
            {
                m_audioFile.ReadAdpFrame(&adpData[0]);
                m_adpAudiobuffer.write(((unsigned char*)&adpData[0]), ED1AudioAdpFrameCount);
            }
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
        if (m_nRepeatCount == 0
                || m_nRepeatCount == 255
                || m_nCurrentRepeat < m_nRepeatCount)
        {
            //go to silence
            if(!m_bSendSilence && m_nSilenceFrames > 0)
            {
                m_bSendSilence = true;
                m_nSendSilenceCount = 0;
                return true;
            }

            //go to next repeat
            m_nCurrentRepeat++;
            m_nCurPlayIndex = 0;

            //CCurrentRepeat ret;
            //ret.chProcess = m_chProcess;
            //ret.nCurrentRepeat = m_nCurrentRepeat;
            //CService::GetInstance()->ExcuteCallback(&ret);
        }
        else
        {
            m_bPlayFinish = true;
            return false;
        }
    }
    m_bSendSilence = false;

    //open new file
    std::string strPath = m_vAudioPath[m_nCurPlayIndex];
    if(!m_audioFile.Open(strPath))
    {
        printf("open file error,path: %s", strPath.c_str());
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
    m_nCurPlayIndex++;
    return true;
}

void CAudioPlay::GetListeningPort(std::vector<int> &vPort)
{
    vPort.clear();
    for(unsigned int i = 0;i<m_vPort.size();i++)
    {
        vPort.push_back(m_vPort[i]);
    }
}


#define APPLICATION OPUS_APPLICATION_AUDIO
#define MAX_PACKET_SIZE (3*1276)

void CAudioPlay::adpcm2opus()
{
    std::ofstream hFile;
    std::string strPath = "/test.adp";
    std::string strRecordPath = "/test.opus";

    hFile.open(strRecordPath.c_str(), std::ios::binary|std::ios::app);
    if (!hFile.is_open())
    {
        printf("open file error,path:%s", strRecordPath.c_str());
        return;
    }

    if(!m_audioFile.Open(strPath))
    {
        printf("open file error,path: %s", strPath.c_str());
        return;
    }

    int err = 0;
    OpusEncoder *encoder = opus_encoder_create(48000, 1, APPLICATION, &err);
    if (err<0)
    {
        fprintf(stderr, "failed to create an encoder: %s\n", opus_strerror(err));
        return;
    }
    while (m_audioFile.IsAdpEnough())
    {
        unsigned char adpData[ED1AudioAdpFrameCount + 4];
        m_audioFile.ReadAdpFrame(&adpData[0]);

        //decode adp to pcm
        TAudioSample sample[ED1AudioFrameSampleCount];
        memset(&sample[0], 0, sizeof(sample));
        if (!m_audioDecoder.DecodeOk(sample, adpData))
        {
            printf("adpcm Decode Error,path: %s", strPath.c_str());
            memset(&sample[0], 0, sizeof(sample));
        }

        unsigned char cbits[MAX_PACKET_SIZE];
        int nbBytes = opus_encode_float(m_Opus_encoder, sample, ED1AudioFrameSampleCount, cbits, MAX_PACKET_SIZE);
        if (nbBytes<0)
        {
            fprintf(stderr, "encode failed: %s\n", opus_strerror(nbBytes));
        }
        else
        {
            hFile.write((char*)&cbits[0], nbBytes);
            printf("pcmToOPUS nbBytes: %d", nbBytes);
        }
    }
    hFile.close();
    if(encoder)
    {
        opus_encoder_destroy(m_Opus_encoder);
    }
}
