
#include "audiobuffer.h"
#include "audiodevice.h"
#include "device.h"
#include "audiojob.h"
#include "common.h"
#include "mylog.h"

#define FRAME418  ED1AudioFrameSampleCount

CAudioBuffer::CAudioBuffer(std::vector<int> &vPort)
{
    m_vPort = vPort;
}

CAudioBuffer::~CAudioBuffer()
{
}

int CAudioBuffer::write(unsigned char* buf, int len)
{
    std::unique_lock < std::mutex > lck(m_mutex);

    if(m_Audiobuffer.getFreeSize() < len)
    {
        unsigned char tmp[2*FRAME418];
        m_Audiobuffer.read(tmp, 2*FRAME418);
    }

    int nWrite = m_Audiobuffer.write(buf, len);
    return nWrite;
}

bool CAudioBuffer::IsPortExit(int nPort)
{
    for (unsigned int i = 0; i < m_vPort.size(); i++)
    {
        if (m_vPort[i] == nPort)
        {
            return true;
        }
    }
    return false;
}

void CAudioBuffer::setframe(int nframe)
{
    std::unique_lock < std::mutex > lck(m_mutex);
    m_Audiobuffer.SetSize(FRAME418 * sizeof(short) * nframe);
}

int CAudioBuffer::read(unsigned char* buf, int len)
{
    std::unique_lock < std::mutex > lck(m_mutex);

    if (m_Audiobuffer.getUsedSize() < len)
    {
        memset(buf, 0, len);
        return 0;
    }
    int nRead = m_Audiobuffer.read(buf, len);
    return nRead;
}

void CAudioBuffer::reset()
{
    std::unique_lock < std::mutex > lck(m_mutex);

    m_Audiobuffer.setEmpty();
}

int CAudioBuffer::freesize()
{
    std::unique_lock < std::mutex > lck(m_mutex);

    int len = m_Audiobuffer.getFreeSize();
    return len;
}

int CAudioBuffer::usedsize()
{
    std::unique_lock < std::mutex > lck(m_mutex);

    int len = m_Audiobuffer.getUsedSize();
    return len;
}

///////////////////////////////////////////////////////////

std::vector<std::shared_ptr<CAudioBuffer>> CAudioData::m_vCaptureBuf;
std::vector<std::shared_ptr<CAudioBuffer>> CAudioData::m_vPlayBuf;

CAudioBuffer* CAudioData::CreateBuf(bool bCapture, std::vector<int> &vPort)
{
    auto p = std::make_shared<CAudioBuffer>(vPort);
    if (bCapture)
    {
        m_vCaptureBuf.push_back(p);
        int nFrame = 2;
        p->m_Audiobuffer.SetSize(FRAME418 * sizeof(short) * nFrame);
    }
    else
    {
        m_vPlayBuf.push_back(p);
        int nFrame = 12;
        p->m_Audiobuffer.SetSize(FRAME418 * sizeof(short) * nFrame);
    }
    return p.get();
}

CAudioBuffer* CAudioData::GetAudioBuffer(bool bCapture, int nPort)
{
    if (bCapture)
    {
        for (unsigned int i = 0; i < m_vCaptureBuf.size(); i++)
        {
            if (m_vCaptureBuf[i]->IsPortExit(nPort))
            {
                return m_vCaptureBuf[i].get();
            }
        }
    }
    else
    {
        for (unsigned int i = 0; i < m_vPlayBuf.size(); i++)
        {
            if (m_vPlayBuf[i]->IsPortExit(nPort))
            {
                return m_vPlayBuf[i].get();
            }
        }
    }
    return NULL;
}

void CAudioData::GetAudioBuffer(bool bCapture, std::vector<int> &vPort, std::vector<CAudioBuffer*> &vAudioBuf)
{
    vAudioBuf.clear();
    for (unsigned int i = 0; i < vPort.size(); i++)
    {
        CAudioBuffer* p = GetAudioBuffer(bCapture, vPort[i]);
        if(p == NULL)
        {
            LOG_ERROR("%s","CAudioData::GetAudioBuffer,p == NULL");
        }

        //exist
        bool bExist = false;
        for (unsigned int k = 0; k < vAudioBuf.size(); k++)
        {
            if (p == vAudioBuf[k])
            {
                bExist = true;
                break;
            }
        }
        //push back
        if (!bExist)
        {
            vAudioBuf.push_back(p);
        }
    }
}

int CAudioData::write(bool bCapture, int nPort, unsigned char* buf, int len)
{
    CAudioBuffer *p = GetAudioBuffer(bCapture, nPort);
    return p->write(buf,len);
}

int CAudioData::read(bool bCapture, int nPort, unsigned char* buf, int len)
{
    CAudioBuffer *p = GetAudioBuffer(bCapture, nPort);
    return p->read(buf, len);
}

void CAudioData::release()
{
    m_vCaptureBuf.clear();
    m_vPlayBuf.clear();
}
