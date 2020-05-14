
#include "audiojob.h"
#include "common.h"
#include "service.h"
#include "public.h"

const int g_nWriteErrorCount = 114*5;

CAudioJob::CAudioJob()
{
    m_chProcess = 0;
    m_nRecordID = 0;
    m_bRunning = false;

    m_bPause = false;
	m_bStop = false;
    m_bManuStop = false;

    m_rtpChannel = -1;
    m_rtpNode = 0;
    m_error = 0;
    m_className = "CAudioJob";
    m_nWriteErrorCount = 0;

    m_fVolumeScale = 1.0f;
    m_fVolumeScaleSG = 1.0f;
    m_nBlockSize = 2;

    m_bEnableVLAN = false;
    m_VLANSock = 0;
    m_bResetVLANNet = false;

    m_bAudioSignal = false;
    m_bPreSignal = false;
    m_bEndSignal = false;
}

CAudioJob::~CAudioJob()
{
}

void CAudioJob::Start()
{
    if(m_bStop)
    	return;

    m_bPause = false;
}

void CAudioJob::Stop()
{
	if(m_bStop)
		return;

    m_bStop = true;
    m_bManuStop = true;
}

void CAudioJob::Pause()
{
	if(m_bStop)
		return;

    m_bPause = true;
}

void CAudioJob::SetChProcess(int nProcess)
{
    m_chProcess = nProcess;
}

int CAudioJob::GetChProcess()
{
    return m_chProcess;
}

void CAudioJob::SetRecordID(int nRecordID)
{
    m_nRecordID = nRecordID;
}

int CAudioJob::GetRecordID()
{
    return m_nRecordID;
}

void CAudioJob::SetRtpNode(int nNode)
{
    m_rtpNode = nNode;
}

void CAudioJob::SetRtpChannel(int nChannel)
{
    m_rtpChannel = nChannel;
}

int CAudioJob::GetRtpChannel()
{
    return m_rtpChannel;
}

int CAudioJob::GetRtpNode()
{
    return m_rtpNode;
}

int CAudioJob::GetError()
{
    return m_error;
}

std::string CAudioJob::GetUdpAddress()
{
    std::string strFormat;
    if(m_rtpNode >= 256)
    {
        strFormat = CCommon::StrFormat("225.%d.1.%d",m_rtpNode-256,m_rtpChannel);
    }
    else
    {
        strFormat = CCommon::StrFormat("225.%d.0.%d",m_rtpNode,m_rtpChannel);
    }
    return strFormat;
}

void CAudioJob::WaitMiniSeconds(int nMiniSecond)
{
    if (m_error != RESULT_VAL_OK)
    {
        return;
    }

    int count = 0;
    while (!m_bManuStop)
    {
        CCommon::SleepMinisecond(100);
        count += 100;
        if (count >= nMiniSecond)
        {
            break;
        }
    }
}

void CAudioJob::Finish()
{
    SendPreSignal(false);
    SendEndSignal(false);

    if (m_error != 0)
    {
        LOG_ERROR("Audiojob Error: %d", m_error);
    }
    auto pMsg = std::make_shared<CMsg>();
    pMsg->type = MSG_AUDIO_FINISH;
    pMsg->pointer = this;
    CService::GetInstance()->Push(pMsg);
};

void CAudioJob::FinishPause()
{
    SendPreSignal(false);
    SendEndSignal(false);

    if (m_error != 0)
    {
        LOG_ERROR("Audiojob Error: %d", m_error);
    }
    if (!CService::GetInstance()->IsExit())
    {
        auto pMsg = std::make_shared<CMsg>();
        pMsg->type = MSG_AUDIO_FINISH_PAUSE;
        pMsg->pointer = this;
        CService::GetInstance()->Push(pMsg);
    }
}

void CAudioJob::VolumeAjust(unsigned char *adpData, bool bSG)
{
    float scale = bSG ? (m_fVolumeScaleSG*m_fVolumeScale):m_fVolumeScale;
    if(scale > 0.99 && scale < 1.01)
    {
       return;
    }

    //append checksum
    CPublic::AppendCheckSum(adpData, ED1AudioAdpFrameCount);

    //decode to pcm
    TAudioSample sample[ED1AudioFrameSampleCount];
    memset(&sample[0], 0, sizeof(sample));

    if (m_audioDecoder.DecodeOk(sample, adpData))
    {
        if(scale < CCommon::dB2FloatScale(-19.5f))
        {
            memset(sample,0,sizeof(sample));
        }
        else
        {
            for (unsigned i = 0; i < ED1AudioFrameSampleCount; i++)
            {
                double temp = sample[i] * scale;
                sample[i] = CAudioFile::GetSampleF(temp);
            }
        }
        m_audioEncode.Encode(adpData, sample);
    }
    else
    {
        m_bStop = true;
        m_error = ERROR_DECODE_ADP;
        LOG_ERROR("Audiojob Decode Error,ErrorCode:%d", m_error);
    }
}

void CAudioJob::VolumeAjust(float fVolumeScale)
{
    m_fVolumeScale = fVolumeScale;
}

void CAudioJob::VolumeAjustSG(float fVolumeScale)
{
    m_fVolumeScaleSG = fVolumeScale;
}

void CAudioJob::VolumeAjustEG(float fVolumeScale)
{
    m_fVolumeScaleEG = fVolumeScale;
}

void CAudioJob::AllocatePort(bool bCapture, int nPort)
{
    std::unique_lock < std::mutex > lck(m_mutexBuf);

    m_vPort.push_back(nPort);
    
    CAudioBuffer* p = CAudioData::GetAudioBuffer(bCapture, nPort);

    bool bExist = false;
    for (unsigned int i = 0; i < m_vAudioBuffer.size(); i++)
    {
        if (p == m_vAudioBuffer[i])
        {
            bExist = true;
            break;
        }
    }
    if (!bExist)
    {
        p->reset();
        m_vAudioBuffer.push_back(p);
    }
}

void CAudioJob::FreePort(bool bCapture, int nPort)
{
    std::unique_lock < std::mutex > lck(m_mutexBuf);

    auto iter = m_vPort.begin();
    while (iter != m_vPort.end())
    {
        if ((*iter) == nPort)
        {
            m_vPort.erase(iter);
            break;
        }
        iter++;
    }
    std::vector<CAudioBuffer*> vTemp = m_vAudioBuffer;
    CAudioData::GetAudioBuffer(bCapture, m_vPort, m_vAudioBuffer);
    for (unsigned int i = 0; i < vTemp.size(); i++)
    {
        bool bExist = false;
        for (unsigned int k = 0; k < m_vAudioBuffer.size(); k++)
        {
            if (vTemp[i] == m_vAudioBuffer[k])
            {
                bExist = true;
                break;
            }
        }
        if (!bExist)
        {
            vTemp[i]->reset();
        }
    }
}

void CAudioJob::Adp2PcmBuffer(unsigned char* pAdp, int nFrames)
{
    //decode adp to pcm
    TAudioSample sample[ED1AudioFrameSampleCount];
    memset(&sample[0], 0, sizeof(sample));
    if (!m_audioDecoder.DecodeOk(sample, pAdp))
    {
        memset(&sample[0], 0, sizeof(sample));
    }

    //transfer sample
    short SampleOut[ED1AudioFrameSampleCount*4];
    for (int i = 0; i < nFrames; i++)
    {
        long SampleValue = long(sample[i] * 32768.0f);
        SampleOut[i] = CAudioFile::GetSampleS(SampleValue);
    }

    //write the buffer
    {
        std::unique_lock < std::mutex > lck(m_mutexBuf);
        for (unsigned int i = 0; i < m_vAudioBuffer.size(); i++)
        {
            m_vAudioBuffer[i]->write((unsigned char*)&SampleOut[0], m_nBlockSize*nFrames);
        }
    }
}

void CAudioJob::Adp2PcmBufferListening(unsigned char* pAdp, int nFrames,e_BUFFER_TYPE eType)
{
    //decode adp to pcm
    TAudioSample sample[ED1AudioFrameSampleCount];
    memset(&sample[0], 0, sizeof(sample));
    if (!m_audioDecoder.DecodeOk(sample, pAdp))
    {
        memset(&sample[0], 0, sizeof(sample));
    }

    //volume ajust
    float scale = m_fVolumeScale;
    if(eType == BUF_PRE_SIGNAL)
    {
        scale = m_fVolumeScaleSG*m_fVolumeScale;
    }
    else if(eType == BUF_END_SIGNAL)
    {
        //to do
    }
    short SampleOut[ED1AudioFrameSampleCount];

    if(scale < CCommon::dB2FloatScale(-19.5f))
    {
        memset(SampleOut,0,sizeof(SampleOut));
    }
    else
    {
        for (int i = 0; i < nFrames; i++)
        {
            float fSample = CAudioFile::GetSampleF(sample[i]*scale);
            long SampleValue = long(fSample * 32768.0f);
            SampleOut[i] = CAudioFile::GetSampleS(SampleValue);
        }
    }

    //write the buffer
    {
        std::unique_lock < std::mutex > lck(m_mutexBuf);
        for (unsigned int i = 0; i < m_vAudioBuffer.size(); i++)
        {
            m_vAudioBuffer[i]->write((unsigned char*)&SampleOut[0], m_nBlockSize*nFrames);
        }
    }
}

bool CAudioJob::IsNeed2Start()
{
    if (m_bStop || m_bPause)
    {
        return false;
    }
    return true;
}

void CAudioJob::EnableVLAN(bool bEnable)
{
    m_bEnableVLAN = bEnable;
}

void CAudioJob::EnableSecurity(bool bEnable)
{
    m_bEnableSecurity = bEnable;
}

void CAudioJob::SwitchVLANNet(MYSOCK socket,std::string strNet)
{
    std::unique_lock<std::mutex> lck(m_mutexVLAN);

    if(m_bEnableVLAN)
    {
        if(m_VLANSock != socket || m_strVLANNet != strNet)
        {
            m_VLANSock = socket;
            m_strVLANNet = strNet;
            m_bResetVLANNet = true;
        }
    }
}

bool CAudioJob::Write(const char *strWrite,int nWrite)
{
    int write = 0;

#ifdef _MSC_VER
    write = sendto(m_Socket.GetSocket(),strWrite, nWrite, 0,
                (sockaddr*)&m_Socket.GetUDPAddress(),sizeof(sockaddr_in));
#else

    if(m_bEnableVLAN)
    {
        write = sendto(m_VLANSock,strWrite, nWrite, 0,
                    (sockaddr*)&m_Socket.GetUDPAddress(),sizeof(sockaddr_in));
    }
    else
    {
        write = sendto(m_Socket.GetSocket(),strWrite, nWrite, 0,
                    (sockaddr*)&m_Socket.GetUDPAddress(),sizeof(sockaddr_in));
    }
#endif
    return write == nWrite;
}

void CAudioJob::Run()
{

}

bool CAudioJob::IsStop()
{
    return m_bStop;
}

bool CAudioJob::IsManuStop()
{
    return m_bManuStop;
}

bool CAudioJob::IsRunning()
{
    return m_bRunning;
}

void CAudioJob::SetRunning(bool bRunning)
{
    m_bRunning = bRunning;
}

bool CAudioJob::InitNet()
{
    if (!m_Socket.Create(SOCK_DGRAM))
    {
        LOG_ERROR("%d Socket create:%d", m_chProcess, m_Socket.GetLastError());
        m_error = ERROR_SOCKET_CREATE;
        return false;
    }

    if (!m_Socket.SetBuffer(2*496,496))
    {
        LOG_ERROR("%d SetBUffer:%d", m_chProcess, m_Socket.GetLastError());
        m_error = ERROR_SOCKET_SET_BUFFER;
        return false;
    }

    std::string strDest = GetUdpAddress();
    int port = TRP_CHANNEL_START + this->m_rtpChannel;
    if (!m_Socket.SetUDPAddress(strDest.c_str(), (unsigned short)port))
    {
        LOG_ERROR("%d Socket set udp address:%d", m_chProcess, m_Socket.GetLastError());
        m_error = ERROR_SOCKET_SET_UDP;
        return false;
    }
    m_Socket.SetTTL();
    m_nWriteErrorCount = 0;
    return true;
}

bool CAudioJob::ExitNet()
{
    m_Socket.Close();
    return true;
}

void CAudioJob::SetPreSignalFile(t_FILEMAP &file)
{
    m_PreSignalFile = file;
}

void CAudioJob::SetEndSignalFile(t_FILEMAP &file)
{
    m_EndSignalFile = file;
}

void CAudioJob::SendPreSignal(bool bPlay)
{
    if (bPlay != m_bPreSignal)
    {
        m_bPreSignal = bPlay;

        //audio input channel 250 is busy
        auto pMsg = std::make_shared<CMsg>();
        pMsg->type = MSG_GONG_SIGNAL;
        pMsg->nInt1 = m_chProcess;
        pMsg->usedpriority.bUsed = bPlay;
        pMsg->usedpriority.nRtpChannel = m_rtpChannel;
        CService::GetInstance()->Push(pMsg);
    }
}

void CAudioJob::SendEndSignal(bool bPlay)
{

}

void CAudioJob::SendAudioSignal(bool bPlay)
{
    if (bPlay != m_bAudioSignal)
    {
        //message
        m_bAudioSignal = bPlay;
        auto pMsg = std::make_shared<CMsg>();
        pMsg->type = MSG_AUDIO_SIGNAL;
        pMsg->nInt1 = m_chProcess;
        pMsg->usedpriority.bUsed = bPlay;
        pMsg->usedpriority.nRtpChannel = m_rtpChannel;
        CService::GetInstance()->Push(pMsg);
    }
}

