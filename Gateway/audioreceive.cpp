
#include "audioreceive.h"
#include "common.h"
#include "paerror.h"
#include "libresample.h"

#define BUFSIZE 1514
#define UDP_NORMAL_HEADER   42
#define UDP_NORMAL_LEN      538
#define UDP_VLAN_HEADER     46
#define UDP_VLAN_LEN        542

#define TEST_SAMPLERATE 0

enum eETPROTOCOL
{
    PRO_IP = 0x0800,
    PRO_VLAN = 0x8100
};

CAudioReceive::CAudioReceive()
{
    m_className = "CAudioReceive";
    m_sequenceNo = 0;

    m_resamplehandle = NULL;
    m_resamplefwidth = 0;
    m_resamplefactor = 0;
    m_Opus_encoder = NULL;
}

CAudioReceive::~CAudioReceive()
{
    if(m_Opus_encoder)
    {
        opus_encoder_destroy(m_Opus_encoder);
    }
}
#define APPLICATION OPUS_APPLICATION_AUDIO
#define BITRATE 64000

void CAudioReceive::Run()
{
    if(m_eFromCode == ENCODE_ADP)
    {
        if(m_eToCode == ENCODE_OPUS)
        {
            int err = 0;
            m_Opus_encoder = opus_encoder_create(48000, 1, APPLICATION, &err);
            if (err<0)
            {
                fprintf(stderr, "failed to create an encoder: %s\n", opus_strerror(err));
                return;
            }

            err = opus_encoder_ctl(m_Opus_encoder, OPUS_SET_BITRATE(BITRATE));
            if (err<0)
            {
                fprintf(stderr, "failed to set bitrate: %s\n", opus_strerror(err));
                return;
            }
        }
        else if(m_eToCode == ENCODE_G722)
        {
            m_resamplefactor = 0.333333333333f;
        }
        else
        {
            m_resamplefactor = 0.166666666666f;
        }
    }
    else if(m_eFromCode == ENCODE_G722)
    {
        m_resamplefactor = 3;
        g722_decode_init(&m_g722DecodeState, G722_BIT_RATE, 0);
    }
    else
    {
        m_resamplefactor = 6;
    }
    m_resamplehandle = resample_open(0, m_resamplefactor, m_resamplefactor);
    m_resamplefwidth = resample_get_filter_width(m_resamplehandle);

    if (!InitNet())
    {
        m_bStop = true;
        Finish();
        return;
    }

    unsigned char Buf[1024 * 4];
    bool bRead = false;
    while (true)
    {
        if (m_bStop || m_bPause)
        {
            break;
        }

        if(m_eFromCode == ENCODE_ADP)
        {
            bRead = m_Socket.Read(&Buf[0], ED1RtpFrameSizeInBytes);
            if (bRead)
            {
                RemoteData(&Buf[0]);
            }
            else if (!m_bStop)
            {
                printf("Receive Error!!!");
            }
        }
        else
        {
            if(m_eFromCode == ENCODE_G711a)
            {
                bRead = m_Socket.Read(&Buf[0], G711_FRAME_LEN);
            }
            else if(m_eFromCode == ENCODE_G711u)
            {
                bRead = m_Socket.Read(&Buf[0], G711_FRAME_LEN);
            }
            else if(m_eFromCode == ENCODE_G722)
            {
                bRead = m_Socket.Read(&Buf[0], G722_FRAME_LEN);
            }
            //decoding
            Decoding(&Buf[0],G711_FRAME_LEN,m_eFromCode);

            //resample to 48k
            Resampleto48k(m_eFromCode);
        }
    }
    ExitNet();

    if (m_bStop)
    {
        Finish();
    }
    else
    {
        FinishPause();
    }
    resample_close(m_resamplehandle);
}

void CAudioReceive::Pause()
{
    CAudioJob::Pause();
}

void CAudioReceive::Start()
{
    CAudioJob::Start();
}

void CAudioReceive::Stop()
{
    CAudioJob::Stop();
}

bool CAudioReceive::InitNet()
{
    //create socket
    if (!m_Socket.Create(SOCK_DGRAM))
    {
        printf("%d Socket create: %d", m_chProcess, m_Socket.GetLastError());
        m_error = ERROR_SOCKET_CREATE;
        return false;
    }

    if(!m_Socket.SetReuseAddr())
    {
        printf("%s","m_Socket.SetReuseAddr");
        m_error = ERROR_SOCKET_SET_REUSE;
        return false;
    }

    std::string strDest = GetUdpAddress();
    unsigned short port = TRP_CHANNEL_START + this->m_rtpChannel;

#ifdef WIN32
    if (!m_Socket.Bind(port))
#else
    if (!m_Socket.Bind(strDest.c_str(),port))
#endif
    {
        printf("%d Socket bind port error:%d", m_chProcess, port);
        m_error = ERROR_SOCKET_BIND;
        return false;
    }

    /*
    if (!m_Socket.SetTimeout(10000,10000))
    {
        printf("%s", "Socket SetTimeout error.");
        m_error = ERROR_SOCKET_SET_TIMEOUT;
        return false;
    }
    */

    //set udp address
    if (!m_Socket.SetUDPAddress(strDest.c_str(), port))
    {
        printf("%d %s", m_chProcess, "!m_Socket.SetMCastAddress");
        m_error = ERROR_MULTIPLE_ADDRESS;
        return false;
    }
	if (!m_Socket.JoinGroup(strDest.c_str()))
	{
        printf("Socket joingroup IP:%s,Port:%d", strDest.c_str(),port);
		m_error = ERROR_JOIN_UDP_GROUP;
		return false;
	}
    return true;
}

bool CAudioReceive::ExitNet()
{
    std::string strIP = GetUdpAddress();
    m_Socket.LeaveGroup(strIP.c_str());
    m_Socket.Close();
    return true;
}

void CAudioReceive::RemoteData(unsigned char * pBuf)
{
    // parse RTP header
    unsigned char* p = pBuf;

    //web://de.wikipedia.org/wiki/Real-Time_Transport_Protocol
    // get information of 1st byte
    unsigned int data = p[0];
    p++;

    //unsigned int version = data & ((1 << 2) - 1);
    data >>= 2;

    // the dom and the scu software never uses padding
    bool padding = (data & 1) != 0;
    data >>= 1;
    data >>= 1;

    // get information of 2nd byte
    data = p[0];
    p++;

    //bool marker = (data & 1) != 0;
    data >>= 1;
    //unsigned int payloadType = data & ((1 << 7) - 1);

    unsigned short char1 = (unsigned short)((unsigned short)p[0] << 8);
    unsigned short char2 = (unsigned short)((unsigned short)p[1] << 0);
    unsigned short sequenceNo = char1 | char2;
    p += 2;

    //unsigned long timeStamp;
    //memcpy(&timeStamp, p, 4);
    p += 4;

    //unsigned long syncSrcId;
    //memcpy(&syncSrcId, p, 4);
    p += 4;

    size_t payloadSize = p - pBuf;
    if (padding)
    {
        size_t paddingBytes = pBuf[payloadSize - 1];
        if (paddingBytes < payloadSize)
        {
            payloadSize -= paddingBytes;
        }
        else
        {
            payloadSize = 0;
        }
    }
    if (payloadSize != ED1RtpHeaderSizeInBytes)
    {
        printf("%s", "receive Frames error");
        return;
    }
    m_sequenceNo = sequenceNo;

    //adp to pcm
    Adp2PcmBuffer(pBuf + 12, ED1AudioFrameSampleCount);

    //resample float sample from 48k to 8k
    if(m_eToCode == ENCODE_OPUS)
    {

    }
    else
    {
        ResampleFrom48k();
    }

    //encoding
    Encoding(m_eToCode);
}

void CAudioReceive::pcm2adp(e_SEND_ENCODE eType)
{
    int needLen = ED1AudioFrameSampleCount*sizeof(float);
    if(m_pcm48kBuffer.getUsedSize()>=needLen)
    {
        float sampleFloat[ED1AudioFrameSampleCount];
        m_pcm48kBuffer.read((unsigned char*)sampleFloat,needLen);

        unsigned char AdpData[1024];
        m_audioEncode.Encode(AdpData, sampleFloat);
    }
}
