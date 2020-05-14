
#include "msgbase.h"
#include "service.h"

CMsgBase::CMsgBase(CTSystem* pSys)
{
    //m_ProtocolVersion = CService::GetInstance()->GetProtocolVersion();
    m_pSystem = pSys;
    m_Mid = 0;
    m_SubMid = 0;
}

CMsgBase::~CMsgBase()
{
}

void CMsgBase::SetMid(char mid)
{
    m_Mid = mid;
}

char CMsgBase::GetMid() const
{
    return m_Mid;
}

void CMsgBase::SetSubMid(char submid)
{
    m_SubMid = submid;
}

char CMsgBase::GetSubMid() const
{
    return m_SubMid;
}

int CMsgBase::GetDataLen()
{
    return (int)m_vBuffer.size();
}

void CMsgBase::WriteLONG(unsigned long Value)
{
    unsigned char InChar1 = (unsigned char)(Value >> 24) & 0xff;
    unsigned char InChar2 = (unsigned char)(Value >> 16) & 0xff;
    unsigned char InChar3 = (unsigned char)(Value >> 8) & 0xff;
    unsigned char InChar4 = (unsigned char)(Value >> 0) & 0xff;

    m_vBuffer.push_back(InChar1);
    m_vBuffer.push_back(InChar2);
    m_vBuffer.push_back(InChar3);
    m_vBuffer.push_back(InChar4);
}

int CMsgBase::ReadLONG(unsigned char* pBuf)
{
    unsigned long char1 = (unsigned long)pBuf[0] << 24;
    unsigned long char2 = (unsigned long)pBuf[1] << 16;
    unsigned long char3 = (unsigned long)pBuf[2] << 8;
    unsigned long char4 = (unsigned long)pBuf[3] << 0;
    unsigned long value = char1 | char2 | char3 | char4;
    return (int)value;
}

void CMsgBase::ChangeVectorto32Byte(unsigned char* pDest, std::vector<int> &vOutput)
{
    for (unsigned int i = 0; i < vOutput.size(); i++)
    {
        //get the position
        int nPos = (vOutput[i] - 1) / 8;
        unsigned char &InChar = pDest[nPos];

        //set the value
        int value = vOutput[i] % 8;
        switch (value)
        {
        case 1: InChar |= 0x01; break;
        case 2: InChar |= 0x02; break;
        case 3: InChar |= 0x04; break;
        case 4: InChar |= 0x08; break;
        case 5: InChar |= 0x10; break;
        case 6: InChar |= 0x20; break;
        case 7: InChar |= 0x40; break;
        case 0: InChar |= 0x80; break;
        }
    }
}

void CMsgBase::Change32BytetoVector(unsigned char * pDest, std::vector<int>& vOutput)
{
    for (unsigned int i = 0; i < 32; i++)
    {
        unsigned char InChar = pDest[i];

        if (InChar == 0)
        {
            continue;
        }
        int number = i * 8;
        if (InChar & 0x01)
        {
            vOutput.push_back(number + 1);
        }
        if (InChar & 0x02)
        {
            vOutput.push_back(number + 2);
        }
        if (InChar & 0x04)
        {
            vOutput.push_back(number + 3);
        }
        if (InChar & 0x08)
        {
            vOutput.push_back(number + 4);
        }
        if (InChar & 0x10)
        {
            vOutput.push_back(number + 5);
        }
        if (InChar & 0x20)
        {
            vOutput.push_back(number + 6);
        }
        if (InChar & 0x40)
        {
            vOutput.push_back(number + 7);
        }
        if (InChar & 0x80)
        {
            vOutput.push_back(number + 8);
        }
    }
}
