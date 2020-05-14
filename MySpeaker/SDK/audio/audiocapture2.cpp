
#include "audiocapture2.h"
#include "common.h"
#include "public.h"
#include "mylog.h"
#include "define.h"
#include "service.h"
#include "routemanager.h"
#include "callback.h"

CAudioCapture2::CAudioCapture2(int nPort)
{
    m_nPort = nPort;
    m_bMicopen = false;

    m_className = "CAudioCapture2";
    m_nPreSignalRemain = 0;
    m_pThreadDevice = CService::GetInstance()->GetThreadDevice(true, m_nPort);
    m_Pcmbuffer.SetSize(418*100);
    m_PcmBufferOut.SetSize(418*100);

    m_nSrcIndex = -1;
    m_bVARecord = false;
}

CAudioCapture2::~CAudioCapture2()
{
    MicroSignl(false);
    CService::GetInstance()->m_routeManager.RemoveChannel(m_rtpChannel);
}

void CAudioCapture2::Run()
{
    MicrAnnouncement();

    MicroSignl(false);

    if (m_bStop)
    {
        m_hFilePreSignal.close();
        Finish();
    }
    else
    {
        FinishPause();
    }
}

void CAudioCapture2::Pause()
{
    CAudioJob::Pause();
    MicroSignl(false);
}

void CAudioCapture2::Stop()
{
	CAudioJob::Stop();
	MicroSignl(false);
}

void CAudioCapture2::Loop()
{
    /*
    while (true)
    {
        if (m_bStop || m_bPause)
        {
            break;
        }

        //timeout,continue
        if (m_pThreadDevice->WaitSignal())
        {
            if (m_type == CAPTURE_NET_ANNOUNCEMENT)
            {
                SendRtpFrame();
            }
            else if (m_type == CAPTURE_RECORD)
            {
                OnRecord();
            }
        }
    }
    */
}

void CAudioCapture2::MicrAnnouncement()
{
    /*
	if(!InitNet())
	{
        m_bStop = true;
		return;
	}

    if(m_bVARecord)
    {
        StartVARecord();
    }
    //start loop
    Loop();

    ExitNet();

    if(m_bVARecord)
    {
        StopVARecord();
    }
    */
}

bool CAudioCapture2::SendRtpFrame()
{
    /*
    bool bContinue = false;
    unsigned char rtpFrame[ED1RtpFrameSizeInBytes];

    //need to play presignal first
    if (GetGongAdpFrame(&rtpFrame[ED1RtpHeaderSizeInBytes]))
    {
        //playing gong
    }
    else
    {
        bContinue = GetCaptureAdpFrame(&rtpFrame[ED1RtpHeaderSizeInBytes]);
        if(m_bVARecord)
        {
            VARecord(&rtpFrame[ED1RtpHeaderSizeInBytes]);
        }
    }

    //append checksum
    CPublic::AppendCheckSum(&rtpFrame[ED1RtpHeaderSizeInBytes], ED1AudioAdpFrameCount);

    //set rtp header
    m_rtpFrameCreator.CreateRtpHeader(rtpFrame);

    //send to net
    if (!Write((char*)&rtpFrame[0], ED1RtpFrameSizeInBytes))
    {  
        ++m_nWriteErrorCount;
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
    return bContinue;
    */
    return true;
}

void CAudioCapture2::OpenGongAudio()
{
    /*
    if (m_vPreSignalPath.size() > 0)
    {
        m_hFilePreSignal.open(m_vPreSignalPath[0], std::ios::binary);
        if (!m_hFilePreSignal.is_open())
        {
            LOG_ERROR("%s", "PreSignal File open error");
            m_nPreSignalRemain = 0;
            return;
        }
        //get file size
        m_hFilePreSignal.seekg(0, std::ios::end);
        m_nPreSignalRemain = (unsigned int)m_hFilePreSignal.tellg();
        m_hFilePreSignal.seekg(0, std::ios::beg);
    }
    */
}

bool CAudioCapture2::GetPreSignalAdpFrame(unsigned char* pAdpData)
{
    /*
    if (m_nPreSignalRemain < ED1AudioAdpFrameCount)
    {
        SendPreSignal(false);
        return false;
    }
    //read one frame from adp file
    m_hFilePreSignal.read((char*)pAdpData, ED1AudioAdpFrameCount);

    VolumeAjust(pAdpData,true);

    m_nPreSignalRemain -= ED1AudioAdpFrameCount;

    SendPreSignal(true);

    //current play index
    if(m_nSrcIndex != 0)
    {
        m_nSrcIndex = 0;
        auto pMsg = std::make_shared<CMsg>();
        pMsg->type = MSG_PLAY_INDEX;
        pMsg->nInt1 = m_chProcess;
        pMsg->nInt2 = m_nSrcIndex;
        CService::GetInstance()->Push(pMsg);
    }
    */
    return true;
}

bool CAudioCapture2::GetCaptureAdpFrame(unsigned char* pAdpData)
{
    bool bContinue = false;
    MicroSignl(true);

    //read capture buffer
    unsigned char rawsample[ED1AudioFrameSampleCount*4];
    memset(rawsample, 0, sizeof(rawsample));
    {
        std::unique_lock < std::mutex > lck(m_mutexBuf);
        if (m_vAudioBuffer.size() > 0)
        {
            m_vAudioBuffer[0]->read((unsigned char*)&rawsample[0],
                    ED1AudioFrameSampleCount*m_nBlockSize);
        }
        if((int)m_vAudioBuffer.size() >= ED1AudioFrameSampleCount*m_nBlockSize)
        {
            bContinue = true;
        }
    }

    float sampleFloat[ED1AudioFrameSampleCount];
    CAudioFile::S16Bit2Float(rawsample, sampleFloat, ED1AudioFrameSampleCount);

    //volume ajust
    if(m_fVolumeScale < CCommon::dB2FloatScale(-19.5f))
    {
        memset(sampleFloat,0,sizeof(sampleFloat));
    }
    else
    {
        for (int i = 0; i < ED1AudioFrameSampleCount; i++)
        {
            sampleFloat[i] = CAudioFile::GetSampleF(sampleFloat[i] * m_fVolumeScale);
        }
    }

    //encode
    m_audioEncode.Encode(pAdpData, sampleFloat);

    return bContinue;
}

int CAudioCapture2::GetPort()
{
    return m_nPort;
}

void CAudioCapture2::MicroSignl(bool bMic)
{
    /*
    if (bMic != m_bMicopen)
    {
        m_bMicopen = bMic;
        CMicResult ret;
        ret.chProcess = m_chProcess;
        ret.bMicOpen = bMic;
        ret.nPort = m_nPort;
        CService::GetInstance()->ExcuteCallback(&ret);

        //current play index
        if (bMic)
        {
            auto pMsg = std::make_shared<CMsg>();
            pMsg->type = MSG_PLAY_INDEX;
            pMsg->nInt1 = m_chProcess;
            //pMsg->nInt2 = m_vPreSignalPath.size()>0?1:0;
            CService::GetInstance()->Push(pMsg);
        }
    }
    */
}

void CAudioCapture2::SetVARecord(bool bRecord)
{
    m_bVARecord = bRecord;
}

void CAudioCapture2::StartVARecord()
{
    m_nVARecordFrames = 0;

#ifdef _MSC_VER
    m_strFilePath = CCommon::GetModulePath() + "Audio\\201907290250.temp";
#else
    m_strFilePath = std::string(TF_PATH) + "Record/201907290250.temp";
#endif

    remove(m_strFilePath.c_str());

    //create record files
    m_hFile.open(m_strFilePath.c_str(), std::ios::binary|std::ios::app);
    if (!m_hFile.is_open())
    {
        LOG_ERROR("VA Record File Open error:%s", m_strFilePath.c_str());
    }
}

void CAudioCapture2::VARecord(unsigned char* pAdpData)
{
    if(m_hFile.is_open())
    {
        //write to file
        m_hFile.write((char*)pAdpData, ED1AudioAdpFrameCount);

        ++m_nVARecordFrames;

        //max record time is 30 minis
        static const int maxFrames = 115*60*30;
        if(m_nVARecordFrames > maxFrames)
        {
            StopVARecord();
        }
    }
}

void CAudioCapture2::StopVARecord()
{
    if (!m_hFile.is_open())
    {
        return;
    }
    m_hFile.close();

#ifdef _MSC_VER
    std::string strPath = CCommon::GetModulePath() + "Record\\autoRecord.adp";
#else
    std::string strPath = std::string(ARECORD);
#endif

    remove(strPath.c_str());
    rename(m_strFilePath.c_str(),strPath.c_str());

#ifndef WIN32
    sync();
#endif
}


