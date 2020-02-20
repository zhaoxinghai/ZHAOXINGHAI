
#include "audiojob.h"
#include "common.h"
#include "paerror.h"
#include "public.h"

#ifndef WIN32
#include "libresample.h"
#endif

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
    m_bPlayGong = false;
    m_nBlockSize = 2;

    m_pcm48kBuffer.SetSize(10*1024);
    m_pcm8kBuffer.SetSize(10*1024);
    m_pcm16kBuffer.SetSize(10*1024);

    m_pcmG711a.SetSize(10*1024);
    m_pcmG711u.SetSize(10*1024);
    m_pcmG722.SetSize(10*1024);
}

CAudioJob::~CAudioJob()
{
}

std::string CAudioJob::GetClassName()
{
    return m_className;
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

void CAudioJob::SetPresignal(std::string strPath)
{
    m_strPresignal = strPath;
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
    if (true)//CService::GetInstance()->GetProtocolVersion() == PROTOCOL_V11)
    {
        if(m_rtpNode >= 256)
        {
            strFormat = CCommon::StrFormat("225.%d.1.%d",m_rtpNode-256,m_rtpChannel);
        }
        else
        {
            strFormat = CCommon::StrFormat("225.%d.0.%d",m_rtpNode,m_rtpChannel);
        }
    }
    else
    {
        strFormat = CCommon::StrFormat("224.1.%d.%d",m_rtpNode,m_rtpChannel);
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
    //GongSignal(false);

    if (m_error != 0)
    {
        printf("Audiojob Error: %d", m_error);
    }
    /*
    auto pMsg = std::make_shared<CMsg>();
    pMsg->type = MSG_AUDIO_FINISH;
    pMsg->pointer = this;
    CService::GetInstance()->Push(pMsg);
    */
};

void CAudioJob::FinishPause()
{
    //GongSignal(false);

    if (m_error != 0)
    {
        printf("Audiojob Error: %d", m_error);
    }
    /*
    if (!CService::GetInstance()->IsExit())
    {
        auto pMsg = std::make_shared<CMsg>();
        pMsg->type = MSG_AUDIO_FINISH_PAUSE;
        pMsg->pointer = this;
        CService::GetInstance()->Push(pMsg);
    }
    */
}

bool CAudioJob::IsNeed2Start()
{
    if (m_bStop || m_bPause)
    {
        return false;
    }
    return true;
}

bool CAudioJob::Write(const char *strWrite,int nWrite)
{
    int write = 0;

#ifdef _MSC_VER
    write = sendto(m_Socket.GetSocket(),strWrite, nWrite, 0,
                (sockaddr*)&m_Socket.GetUDPAddress(),sizeof(sockaddr_in));
#else
    write = sendto(m_Socket.GetSocket(),strWrite, nWrite, 0,
                (sockaddr*)&m_Socket.GetUDPAddress(),sizeof(sockaddr_in));
#endif
    return write == nWrite;
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
    m_pcm48kBuffer.write((unsigned char*)sample,ED1AudioFrameSampleCount*sizeof(float));
}

//#define EASY_RESAMPLE

void CAudioJob::ResampleFrom48k()
{
    //48k buffer
    float inBuffer[2*ED1AudioFrameSampleCount];
    int inBufferLen = m_pcm48kBuffer.getUsedSize();
    int inBufferFrame = inBufferLen/sizeof(float);
    float   outBuffer[ED1AudioFrameSampleCount];
    int     outFrame = 0;

#ifdef EASY_RESAMPLE
    if(m_eAudioEncoding == ENCODE_G722)
    {
        int inFrame16k = (inBufferFrame/3)*3;
        m_pcm48kBuffer.read((unsigned char*)inBuffer,inFrame16k*sizeof(float));
        for(int i = 0;i<inFrame16k;i += 3)
        {
            outBuffer[outFrame] = inBuffer[i];
            outFrame++;
        }
    }
    else
    {
        int inFrame8k = (inBufferFrame/6)*6;
        m_pcm48kBuffer.read((unsigned char*)inBuffer,inFrame8k*sizeof(float));
        for(int i = 0;i<inFrame8k;i += 6)
        {
            outBuffer[outFrame] = inBuffer[i];
            outFrame++;
        }
    }

#else
    int     lastFlag = 0;
    int     inBufferUsed;
    m_pcm48kBuffer.read((unsigned char*)inBuffer,inBufferLen);
    outFrame = resample_process(m_resamplehandle,
                              m_resamplefactor,
                              inBuffer,
                              inBufferFrame,
                              lastFlag,
                              &inBufferUsed,
                              outBuffer,
                              ED1AudioFrameSampleCount);
#endif

    //transfer float to short
    for (int i = 0; i < outFrame; i++)
    {
        long SampleValue = long(outBuffer[i] * 32768.0f);
        short sampleShort = CAudioFile::GetSampleS(SampleValue);

        if(m_eAudioEncoding == ENCODE_G722)
        {
            m_pcm16kBuffer.write((unsigned char*)&sampleShort,2);
        }
        else
        {
            m_pcm8kBuffer.write((unsigned char*)&sampleShort,2);
        }
    }
}

void CAudioJob::Resampleto48k(e_SEND_ENCODE eType)
{
    float inBuffer[2*ED1AudioFrameSampleCount];
    int inBufferLen = 0;
    int inBufferFrame = 0;
    float   outBuffer[10*ED1AudioFrameSampleCount];
    int     outFrame = 0;

    if(eType == ENCODE_G711a || eType == ENCODE_G711u)
    {
        inBufferLen = m_pcm8kBuffer.getUsedSize();
        inBufferFrame = inBufferLen/sizeof(float);
        m_pcm8kBuffer.read((unsigned char*)inBuffer,inBufferLen);
    }
    else if(eType == ENCODE_G722)
    {
        inBufferLen = m_pcm16kBuffer.getUsedSize();
        inBufferFrame = inBufferLen/sizeof(float);
        m_pcm16kBuffer.read((unsigned char*)inBuffer,inBufferLen);
    }
    int     lastFlag = 0;
    int     inBufferUsed;

    outFrame = resample_process(m_resamplehandle,
                              m_resamplefactor,
                              inBuffer,
                              inBufferFrame,
                              lastFlag,
                              &inBufferUsed,
                              outBuffer,
                              10*ED1AudioFrameSampleCount);


    m_pcm48kBuffer.write((unsigned char*)outBuffer,outFrame*sizeof(float));

    static int test = 0;
    if(++test < 10)
    {
        printf("48k intFrame:%d outFrame:%d\n",inBufferFrame,outFrame);
    }
}

void CAudioJob::Encoding(e_SEND_ENCODE eType)
{
    if(eType==ENCODE_G711a)
    {
        pcmToG711(true);
    }
    else if(eType==ENCODE_G711u)
    {
        pcmToG711(false);
    }
    else if(eType==ENCODE_G722)
    {
        pcmToG722();
    }
}

void CAudioJob::Decoding(unsigned char* pBuf,int nFrame,e_SEND_ENCODE eType)
{
    if(eType==ENCODE_G711a)
    {
        G711topcm(pBuf,nFrame,true);
    }
    else if(eType==ENCODE_G711u)
    {
        G711topcm(pBuf,nFrame,false);
    }
    else if(eType==ENCODE_G722)
    {
        G722topcm(pBuf,nFrame);
    }
}

void CAudioJob::pcmToG711(bool bALaw)
{
    int16_t buf[1024];
    int iFrame = m_pcm8kBuffer.getUsedSize()/2;

    if(iFrame>1024)
    {
        printf("iFrame Error.");
    }

    m_pcm8kBuffer.read((unsigned char*)&buf[0],iFrame*2);

    if(bALaw)
    {
        static int tt = 0;
        if(++tt < 10)
        {
            printf("711a-iFrame:%d\n",iFrame);
        }

        for (int i = 0; i < iFrame; i++)
        {
            int8_t sample = ALaw_Encode(buf[i]);
            m_pcmG711a.write((unsigned char*)&sample,1);
        }
    }
    else
    {
        static int tt = 0;
        if(++tt < 10)
        {
            printf("711u-iFrame:%d\n",iFrame);
        }

        for (int i = 0; i < iFrame; i++)
        {
            int8_t sample = MuLaw_Encode(buf[i]);
            m_pcmG711u.write((unsigned char*)&sample,1);
        }
    }
}

void CAudioJob::pcmToG722()
{
    int16_t buf[1024];
    int iFrame = m_pcm16kBuffer.getUsedSize()/2;

    if(iFrame>1024)
    {
        printf("iFrame Error.");
    }

    m_pcm16kBuffer.read((unsigned char*)&buf[0],iFrame*2);

    uint8_t outbuf[1024];
    int outlen = g722_encode(&m_g722EncodeState, outbuf, buf, iFrame);
    m_pcmG722.write((unsigned char*)outbuf,outlen);
}

void CAudioJob::G711topcm(unsigned char * pBuf,int iFrame,bool bALaw)
{
    short rawsample[1024];
    if(bALaw)
    {
        for (int i = 0; i < iFrame; i++)
        {
            rawsample[i] = ALaw_Decode(pBuf[i]);
        }
    }
    else
    {
        for (int i = 0; i < iFrame; i++)
        {
            rawsample[i] = MuLaw_Decode(pBuf[i]);
        }
    }
    float sampleFloat[1024];
    CAudioFile::S16Bit2Float((unsigned char*)rawsample, sampleFloat, iFrame);

    m_pcm8kBuffer.write((unsigned char*)sampleFloat,iFrame*sizeof(float));
}

void CAudioJob::G722topcm(unsigned char * pBuf,int iFrame)
{
    short rawsample[1024];
    int outlen = g722_decode(&m_g722DecodeState, rawsample, pBuf, iFrame);

    float sampleFloat[1024];
    CAudioFile::S16Bit2Float((unsigned char*)rawsample, sampleFloat, iFrame);

    m_pcm16kBuffer.write((unsigned char*)sampleFloat,iFrame*sizeof(float));
}

