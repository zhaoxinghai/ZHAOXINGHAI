
#include "transport.h"
#include "topsystem.h"
#include "threadsend.h"
#include "service.h"
#include "common.h"
#include "mylog.h"
#include "msgfactory.h"

CTransport::CTransport(CTSystem* pSystem)
{
    m_pDataSend = NULL;
    m_pDataPos = 0;
    m_pSystem = pSystem;
}

CTransport::~CTransport()
{
}

void CTransport::Receive(char* pBuf,int len)
{
    if (len > 0)
    {
        //put receive to the buffer
        for (int i = 0;i < len;i++)
        {
            m_vBuffer.push_back(pBuf[i]);
        }
        ReadBuffer();
    }
}

void CTransport::Clear()
{
    m_vBuffer.clear();
}

void CTransport::ReadBuffer()
{
    while (true)
    {
        int endPos = GetBufferTokenPos((unsigned char)MESSAGE_END);
        if (endPos < 0)
           return;

        int startPos = GetBufferTokenPos((unsigned char)MESSAGE_START);
        if (endPos > startPos)
        {
            //read packet from start to end
            int len = endPos - startPos + 1;
            std::vector<unsigned char> vPacket;
            for (int i = startPos; i < len; i++)
            {
                vPacket.push_back(m_vBuffer[i]);
            }
            //remove from buffer
            m_vBuffer.erase(m_vBuffer.begin(), m_vBuffer.begin() + endPos + 1);

            ReadPacket(vPacket);
        }
        else
        {
            //remove from buffer
            m_vBuffer.erase(m_vBuffer.begin(), m_vBuffer.begin() + endPos + 1);
        }
    }
}

void CTransport::ReadPacket(std::vector<unsigned char> &vMsg)
{
    if (vMsg.size() < 14)
    {
        return;
    }

    UnCompressPacket(vMsg);

    //create message object
    unsigned char mid = vMsg[9];
    unsigned char smid = vMsg[12];
    auto pMsg = CMsgFactory::CreateMsgClass(mid,smid,m_pSystem);
    if (pMsg == NULL)
    {
        return;
    }

    //get length
    int mdl = vMsg[10] * 256 + vMsg[11];
    int reallen = (int)vMsg.size() - 14;
    if (mdl != reallen)
    {
        return;
    }

    //pMsg->SetDest(&vMsg[1]);

    //source
    //pMsg->SetSource(&vMsg[5]);

    //excute data message
    int contentlen = (int)vMsg.size() - 14;
    if ( (contentlen + 12) > PACKET_BUFLEN)
    {
        return;
    }

    unsigned char* Buf = new unsigned char[contentlen];        //checked

    for(int i = 0; i < contentlen;i++)
    {
        Buf[i] = vMsg[i+12];
    }
    pMsg->SetMid(mid);
    pMsg->OnRespond(&Buf[0], contentlen);

    delete []Buf;
}



void CTransport::UnCompressPacket(std::vector<unsigned char> &vMsg)
{
    bool bTransfer = false;
    std::vector<unsigned char> vNewMsg;
    for (unsigned int i = 0;i < vMsg.size();i++)
    {
        if(bTransfer)
        {
            bTransfer = false;
            vNewMsg.push_back(0xFF - vMsg[i]);
            continue;
        }

        unsigned char Hex = vMsg[i];
        switch (Hex)
        {
        case 0xFF:
            bTransfer = true;
            break;
        default:
            vNewMsg.push_back(Hex);
            break;
        }
    }
    vMsg.clear();
    vMsg = vNewMsg;
}

void CTransport::CompressPacket(std::vector<unsigned char> &vMsg)
{
    std::vector<unsigned char> vNewMsg;
    for (unsigned int i = 0;i < vMsg.size();i++)
    {
        unsigned char Hex = vMsg[i];
        switch (Hex)
        {
        case 0xFF:
            vNewMsg.push_back(0xFF);
            vNewMsg.push_back(0x00);
            break;
        case 0xFE:
            vNewMsg.push_back(0xFF);
            vNewMsg.push_back(0x01);
            break;
        case 0xFD:
            vNewMsg.push_back(0xFF);
            vNewMsg.push_back(0x02);
            break;
        default:
            vNewMsg.push_back(Hex);
            break;
        }
    }
    vMsg = vNewMsg;
}

int CTransport::GetBufferTokenPos(unsigned char ctoken)
{
    for (unsigned int i = 0;i < m_vBuffer.size();i++)
    {
        if (m_vBuffer[i] == ctoken)
        {
            return i;
        }
    }
    return -1;
}

void CTransport::SendBroadcast(CMsgBase* pMsg,bool bLifeSignal)
{
    std::shared_ptr<t_SendMsg> Msg = std::make_shared<t_SendMsg>();
    Msg->bBroadcast = true;

    std::string strAddress;
    if(bLifeSignal)
    {
        strAddress = CCommon::StrFormat("%d.255.255.255",g_SDKServer.m_nBaseAddress);
    }
    else
    {
        strAddress = CCommon::StrFormat("%d.255.255.254", g_SDKServer.m_nBaseAddress);
    }

    strncpy(Msg->szIP, strAddress.c_str(),sizeof(Msg->szIP));
    GeneralMsgBuffer(pMsg, Msg->szData, Msg->len);
    g_SDKServer.SendMsg(Msg);
}

void CTransport::Send(CMsgBase* pMsg)
{
    std::shared_ptr<t_SendMsg> Msg = std::make_shared<t_SendMsg>();
    Msg->bBroadcast = false;

    GeneralMsgBuffer(pMsg, Msg->szData, Msg->len);

    Msg->sock = m_pSystem->m_sock;
    Msg->bTCP = m_pSystem->m_IsTCP;
    strncpy(Msg->szIP, m_pSystem->m_Node.strIP.c_str(),sizeof(Msg->szIP)-1);
    g_SDKServer.SendMsg(Msg);
}

void CTransport::GeneralMsgBuffer(const CMsgBase* pMsg, char* pBuf, int &len)
{
    int Mdl = (int)pMsg->m_vBuffer.size();

    unsigned char CheckSum = 0;
    std::vector<unsigned char> vData;

    //node
    PushInt(vData,g_SDKServer.GetNode());

    //mid
    PushInt(vData, pMsg->GetMid());

    //mdl
    PushInt(vData,Mdl);

    //data
    for (unsigned int i = 0; i < pMsg->m_vBuffer.size(); i++)
    {
        vData.push_back(pMsg->m_vBuffer[i]);
    }

    //Compress Packet
    CompressPacket(vData);

    //set buffer
    len = vData.size()+2;
    pBuf[0] = (unsigned char)MESSAGE_START;
    for (int i = 1; i < len - 1; i++)
    {
        pBuf[i] = vData[i - 1];
    }
    pBuf[len - 1] = (unsigned char)MESSAGE_END;
    pBuf[len] = '\0';
}

void CTransport::PushInt(std::vector<unsigned char> &vData,unsigned long c)
{
    unsigned char InChar1 = (unsigned char)(c >> 24) & 0xff;
    unsigned char InChar2 = (unsigned char)(c >> 16) & 0xff;
    unsigned char InChar3 = (unsigned char)(c >> 8) & 0xff;
    unsigned char InChar4 = (unsigned char)(c >> 0) & 0xff;

    vData.push_back(InChar1);
    vData.push_back(InChar2);
    vData.push_back(InChar3);
    vData.push_back(InChar4);
}

