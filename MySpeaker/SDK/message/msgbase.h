
#ifndef MESSAGE_BASE_H
#define MESSAGE_BASE_H

#include "sdkdefine.h"
#include "sdkcallback.h"
#include "topsystem.h"

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

