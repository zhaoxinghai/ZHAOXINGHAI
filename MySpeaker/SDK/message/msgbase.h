
#ifndef MESSAGE_BASE_H
#define MESSAGE_BASE_H

#include "predefine.h"
#include "callback.h"
#include "d1system.h"

enum e_MIDMSG
{
    PAMSG_MID_LIFE_SIGNAL,
    PAMSG_MID_BUSY_STATE,
    PAMSG_MID_CONNECT
};

enum e_PAMsgLevelmeterControl
{
    PAMSG_SMID_LMSTART = 0x10,            // 10 Start Levelmeter
    PAMSG_SMID_LMSTOP = 0x11,             // 11 Stop Levelmeter
    PAMSG_SMID_LMREFRESH = 0x12,          // 12 Refresh Levelmeters
    PAMSG_SMID_LMDATA = 0x13,             // 13 Levelmeter status
    PAMSG_SMID_AUDIO_CONTROL = 0x01,      // 01 Audio control
    PAMSG_SMID_DEVICE_STATE = 0x00,        // 00 Device state
    PAMSG_SMID_AUDIO_CONTROL_REPLY = 0x81
};

class CTSystem;
class CMsgBase
{
public:
    CMsgBase(CTSystem* pSys);
    virtual ~CMsgBase();

    //do first,this buffer begin at mid
    virtual void OnRespond(unsigned char* pBuf,const int len) = 0;
    virtual void SetMid(char mid);

    char GetMid() const;

    void SetSubMid(char submid);
    char GetSubMid() const;
    int GetDataLen();

    static void ChangeVectorto32Byte(unsigned char* pDest, std::vector<int> &vOutput);
    static void Change32BytetoVector(unsigned char* pDest, std::vector<int> &vOutput);

    std::vector<unsigned char>  m_vBuffer;

protected:
    void WriteLONG(unsigned long Value);
    int ReadLONG(unsigned char* pBuf);

    int         m_Mid;            // Message ID only for send
    int         m_SubMid;         // Message SubID
    CTSystem*  m_pSystem;
};

#endif

