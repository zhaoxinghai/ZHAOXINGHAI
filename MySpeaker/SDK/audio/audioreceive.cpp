
#include "audioreceive.h"
#include "common.h"
#include "mylog.h"
#include "threadtimer.h"
#include "sdkerror.h"
#include "service.h"
#include "routemanager.h"
#include "sdkcallback.h"
#include "d1rtpframelayer.h"

#ifndef _MSC_VER
#include "cpsw_vlan.h"
#endif

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
    m_bResetVLANNet = false;

#ifndef _MSC_VER
    m_pcap_handle = NULL;
#endif
}

CAudioReceive::~CAudioReceive()
{
}

#ifdef _MSC_VER
void CAudioReceive::Run()
{
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
        bRead = m_Socket.Read(&Buf[0], ED1RtpFrameSizeInBytes);

        if (bRead)
        {
            RemoteData(&Buf[0]);
        }
        else if (!m_bStop)
        {
            CCommon::SleepMinisecond(1);
        }
    }
    ExitNet(false);

    if (m_bStop)
    {
        Finish();
    }
    else
    {
        FinishPause();
    }
}
#else
void CAudioReceive::Run()
{
    if(m_bEnableVLAN)
    {
        InitNetPcap();
        m_bResetVLANNet = false;
    }
    else
    {
        if (!InitNet())
        {
            m_bStop = true;
            Finish();
            return;
        }
    }
    unsigned char Buf[1024 * 4];
    bool bRead = false;
    while (true)
    {
        if (m_bStop || m_bPause)
        {
            break;
        }
        if(m_bEnableVLAN)
        {
            bRead = ReadPcap(&Buf[0]);
        }
        else
        {
            bRead = m_Socket.Read(&Buf[0], ED1RtpFrameSizeInBytes);
        }
        if (bRead)
        {
            RemoteData(&Buf[0]);
        }
        else if (!m_bStop)
        {
            CCommon::SleepMinisecond(1);
        }
    }
    ExitNet(m_bEnableVLAN);

    if (m_bStop)
    {
        Finish();
    }
    else
    {
        FinishPause();
    }
}
#endif

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
        LOG_ERROR("%d Socket create: %d", m_chProcess, m_Socket.GetLastError());
        m_error = ERROR_SOCKET_CREATE;
        return false;
    }

    if(!m_Socket.SetReuseAddr())
    {
        LOG_ERROR("%s","m_Socket.SetReuseAddr");
        m_error = ERROR_SOCKET_SET_REUSE;
        return false;
    }

    std::string strDest = GetUdpAddress();
    unsigned short port = TRP_CHANNEL_START + this->m_rtpChannel;

#ifdef _MSC_VER
    if (!m_Socket.Bind(port))
#else
    if (!m_Socket.Bind(strDest.c_str(),port))
#endif
    {
        LOG_ERROR("%d Socket bind port error:%d", m_chProcess, port);
        m_error = ERROR_SOCKET_BIND;
        return false;
    }

    if (!m_Socket.SetTimeout(100,100))
    {
        LOG_ERROR("%s", "Socket SetTimeout error.");
        m_error = ERROR_SOCKET_SET_TIMEOUT;
        return false;
    }

    //set udp address
    if (!m_Socket.SetUDPAddress(strDest.c_str(), port))
    {
        LOG_ERROR("%d %s", m_chProcess, "!m_Socket.SetMCastAddress");
        m_error = ERROR_MULTIPLE_ADDRESS;
        return false;
    }
	if (!m_Socket.JoinGroup(strDest.c_str()))
	{
        LOG_ERROR("Socket joingroup IP:%s,Port:%d", strDest.c_str(),port);
		m_error = ERROR_JOIN_UDP_GROUP;
		return false;
	}
    return true;
}

bool CAudioReceive::ExitNet(bool bPacap)
{
    if(bPacap)
    {
#ifndef _MSC_VER
        if(m_pcap_handle != NULL)
        {
            ::pcap_close(m_pcap_handle);
            m_pcap_handle = NULL;
        }
#endif
    }
    else
    {
        std::string strIP = GetUdpAddress();
        m_Socket.LeaveGroup(strIP.c_str());
        m_Socket.Close();
    }
    return true;
}

bool CAudioReceive::InitNetPcap()
{
    LOG_DEBUG("%d InitNetPcap NET:%s",m_chProcess,m_strVLANNet.c_str());

#ifndef _MSC_VER
    //open
    char error_content[BUFSIZE] = "";
    m_pcap_handle = ::pcap_open_live(m_strVLANNet.c_str(),BUFSIZE,1,50,error_content);
    if(m_pcap_handle == NULL)
    {
        LOG_ERROR("m_pcap_handle open Error,net:%s",m_strVLANNet.c_str());
        m_error = ERROR_NETWORK_ERROR;
        return false;
    }
    //filter
    struct bpf_program filter;
    std::string strDest = GetUdpAddress();
    unsigned short port = TRP_CHANNEL_START + this->m_rtpChannel;
    std::string strFilter = CCommon::StrFormat("dst host %s and dst port %d",strDest.c_str(),port);

    if(::pcap_compile(m_pcap_handle, &filter, strFilter.c_str(), 1, 0) == -1)
    {
        LOG_ERROR("%s","pcap_compile Error");
        m_error = ERROR_NETWORK_ERROR;
        return false;
    }
    if(::pcap_setfilter(m_pcap_handle, &filter) == -1)
    {
        LOG_ERROR("%s","pcap_setfilter Error");
        m_error = ERROR_NETWORK_ERROR;
        ::pcap_close(m_pcap_handle);
        return false;
    }
#endif

    return true;
}

bool CAudioReceive::ReadPcap(unsigned char *strRead)
{
    std::unique_lock<std::mutex> lck(m_mutexVLAN);

#ifdef _MSC_VER
    UNREFERENCED_PARAMETER(strRead);
    return false;
#else
    //network switch, retset the pacap
    if(m_bResetVLANNet)
    {
        ExitNet(true);
        if(InitNetPcap())
        {
            m_bResetVLANNet = false;
        }
        else
        {
            return false;
        }
    }
    if(m_pcap_handle == NULL)
    {
        m_bResetVLANNet = true;
        return false;
    }

    struct my_Ethernet_header
    {
        unsigned char ether_dhost[6];
        unsigned char ether_shost[6];
        unsigned short ether_type;
    };

    struct pcap_pkthdr protocol_header;
    my_Ethernet_header *ethernet_protocol;
    const unsigned char *p_packet_content = NULL;
    p_packet_content = ::pcap_next(m_pcap_handle,&protocol_header);
    if(p_packet_content == NULL)
    {
        return false;
    }

    ethernet_protocol = (my_Ethernet_header*)p_packet_content;

    unsigned short ethernet_type = ntohs(ethernet_protocol->ether_type);
    if(ethernet_type == PRO_IP)
    {
        if(protocol_header.len == UDP_NORMAL_LEN)
        {
            memcpy(strRead,&p_packet_content[UDP_NORMAL_HEADER],ED1RtpFrameSizeInBytes);
            return true;
        }
    }
    else if(ethernet_type == PRO_VLAN)
    {
        if(protocol_header.len == UDP_VLAN_LEN)
        {
            memcpy(strRead,&p_packet_content[UDP_VLAN_HEADER],ED1RtpFrameSizeInBytes);
            return true;
        }
    }
    return false;

#endif 
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
        LOG_DEBUG("%s", "receive Frames error");
        return;
    }
    m_sequenceNo = sequenceNo;

    Adp2PcmBuffer(pBuf + 12, ED1AudioFrameSampleCount);
}
