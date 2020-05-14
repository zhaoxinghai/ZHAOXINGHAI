
#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "predefine.h"
#include "mysocket.h"
#include "msgbase.h"

#define MAXTELLEN      2000

#define MESSAGE_START  0xfe
#define MESSAGE_END    0xfd

class CMsgBase;
class CTSystem;
class CTransport
{
public:
    CTransport(CTSystem* pSystem);
    ~CTransport();

    void Receive(char* pBuf, int len);
    void Clear();

    void Send(CMsgBase* pMsg);
    void SendBroadcast(CMsgBase* pMsg,bool bLifeSignal);

    void GeneralMsgBuffer(const CMsgBase* pMsg, char* pBuf, int &len);

private:

    void UnCompressPacket(std::vector<unsigned char> &vMsg);
    void CompressPacket(std::vector<unsigned char> &vMsg);

    void ReadBuffer();
    void ReadPacket(std::vector<unsigned char> &vMsg);

    int  GetBufferTokenPos(unsigned char ctoken);

    void PushInt(std::vector<unsigned char> &vData,unsigned long c);

    //send buffer
    char*     m_pDataSend;
    int       m_pDataPos;
    CTSystem *m_pSystem;
    std::vector<unsigned char> m_vBuffer;
};

#endif

