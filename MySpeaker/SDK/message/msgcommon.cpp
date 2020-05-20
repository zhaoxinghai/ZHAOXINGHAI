
#include "msgbase.h"
#include "msgcommon.h"
#include "common.h"
#include "sdkcallback.h"
#include "sdk.h"
#include "service.h"
#include "routemanager.h"

CMsgCommon::CMsgCommon(CTSystem* pSys):CMsgBase(pSys)
{
    //m_Mid = PAMSG_MID_VER;
    m_SubMid = 0;
}

CMsgCommon::~CMsgCommon()
{

}

void CMsgCommon::OnRespond(unsigned char* pBuf, const int len)
{
    /*
    switch (m_Mid)
    {
    case PAMSG_MID_VER2:
        OnVersion11(pBuf, len);
        break;
    case PAMSG_MID_LOGONINFO:
        OnLogin(pBuf, len);
        break;
    case PAMSG_MID_CTRLTL:
        OnTransportLine(pBuf, len);
        break;
    case PAMSG_MID_TEXTOUT:
        LOG_DEBUG("%s","PAMSG_MID_TEXTOUT");
        break;
    case PAMSG_MID_TEXTIN:
        OnTextIn(pBuf, len);
        break;
    case PAMSG_MID_MMS:
        LOG_DEBUG("%s","PAMSG_MID_MMS");
        break;
    case PAMSG_MID_SPECIAL:
        OnSpecialFunction(pBuf,len);
        break;
    default:
        LOG_ERROR("CMsgCommon: unknow mid = %d\n",m_Mid);
        break;
    }
    */
}

void CMsgCommon::OnVersion10(unsigned char* /*pBuf*/, const int /*len*/)
{
    /*
    if (pBuf[0] != 0x80)
        return;

    unsigned char *p = pBuf + 2;

    char szAppVer[100] = {0};
    memcpy(szAppVer, p, 65);

    p += 65;
    char szAppData[100] = { 0 };
    memcpy(szAppData, p, 21);

    p += 21;
    char szAppCompiler[100] = { 0 };
    memcpy(szAppCompiler, p, 65);

    p += 65;
    unsigned char ProtVerAct = p[0];
    unsigned char ProtVerMax = p[1];

    std::string str = szAppData;
    CVersonResult ver;
    ver.strClassName = "CVersonResult";
    ver.nErrorCode = 0;
    ver.strErrorDesc = "OK";
    ver.strVer = szAppVer;
    ver.strDate = str.substr(0, 11);
    ver.strTime = str.substr(12);
    ver.strCompiler = szAppCompiler;
    ver.nProtVerAct = ProtVerAct;
    ver.nPortVerMax = ProtVerMax;

    g_SDKServer.ExcuteCallback(&ver);
    */
}

void CMsgCommon::OnVersion11(unsigned char* /*pBuf*/, const int /*len*/)
{
    /*
    if (pBuf[0] != 0x80)
        return;

    unsigned char *p = pBuf + 2;

    char szAppVer[100] = {0};
    memcpy(szAppVer, p, 65);

    p += 65;
    char szAppData[100] = { 0 };
    memcpy(szAppData, p, 21);

    p += 21;
    char szAppCompiler[100] = { 0 };
    memcpy(szAppCompiler, p, 65);

    p += 65;
    unsigned char ProtVerAct = p[0];
    unsigned char ProtVerMax = p[1];

    std::string str = szAppData;
    CVersonResult ver;
    ver.strClassName = "CVersonResult";
    ver.nErrorCode = 0;
    ver.strErrorDesc = "OK";
    ver.strVer = szAppVer;
    ver.strDate = str.substr(0, 11);
    ver.strTime = str.substr(12);
    ver.strCompiler = szAppCompiler;
    ver.nProtVerAct = ProtVerAct;
    ver.nPortVerMax = ProtVerMax;

    g_SDKServer.ExcuteCallback(&ver);
    */
}

void CMsgCommon::AskVersion()
{
    //m_Mid = PAMSG_MID_VER;
    m_vBuffer.push_back(0);
    m_pSystem->OnSend(this);
}

void CMsgCommon::AskLogin()
{
    //m_Mid = PAMSG_MID_LOGONINFO;
    m_pSystem->OnSend(this);
}

void CMsgCommon::OnLogin(unsigned char* /*pBuf*/, const int /*len*/)
{
    /*
    m_vBuffer.clear();

    //RetVal byte,0	ok,1 error
    m_vBuffer.push_back(0);

    //Enable byte 0....................deactivated, remote access is forbidden
    //            1....................activated, remote access is allowed
    m_vBuffer.push_back(1);

    //SysNameLen	byte	0 to 40	length of the system name std::string
    CTSystem* pSys = g_SDKServer.GetSelfD1System();
    m_vBuffer.push_back((unsigned char)pSys->m_Node.strName.length());

    //SysName	array[]	system name
    for(unsigned i = 0;i<pSys->m_Node.strName.size();i++)
    {
        m_vBuffer.push_back(pSys->m_Node.strName[i]);
    }

    //PwLen	byte	0 to 40	length of the password std::string
    std::string strPass;
    m_vBuffer.push_back((unsigned char)strPass.length());

    //SysPw	array[]	password std::string
    for(unsigned i = 0;i<strPass.size();i++)
    {
        m_vBuffer.push_back(strPass[i]);
    }

    m_pSystem->OnSend(this);
    */
}

void CMsgCommon::OnTransportLine(unsigned char* pBuf, const int /*len*/)
{
    /*
    //m_Mid = PAMSG_MID_CTRLTL;
    
    unsigned char* p = pBuf;
    m_SubMid = *p;
    p++;

    CTransportLineResult ret;

    unsigned char action = *p;
    p++;
    ret.bAction = (action == 1) ? true:false;

    unsigned short NumofEl = ReadLONG(p);
    p += 2;

    for (int i = 0; i < NumofEl; i++)
    {
        t_TL tl;
        tl.nNode = *p;
        p++;

        tl.nType = *p;
        p++;

        tl.nNum = ReadLONG(p);

        if (tl.nNode == g_SDKServer.GetNode())
        {
            ret.vTL.push_back(tl);
        }
    }

    g_SDKServer.ExcuteCallback(&ret);*/
}

void CMsgCommon::TransportLine(bool bAction, std::vector<t_TL>& vTL)
{
    /*
    //m_Mid = PAMSG_MID_CTRLTL;

    //subMid
    m_vBuffer.push_back(0x00);

    //action
    m_vBuffer.push_back(bAction ? 0x01 : 0x00);

    //NumofEL
    m_vBuffer.push_back((unsigned char)vTL.size());

    for (unsigned int i = 0; i < vTL.size(); i++)
    {
        m_vBuffer.push_back(vTL[i].nNode);
        m_vBuffer.push_back(vTL[i].nType);
        WriteLONG(vTL[i].nNum);
    }
    m_pSystem->OnSend(this);*/
}

void CMsgCommon::OnTextIn(unsigned char* pBuf, const int len)
{
    /*
    int nLen = pBuf[0];
    CTextInResult ret;
    ret.tcpSocket = m_pSystem->m_sock;
    ret.nNode = m_pSystem->m_Node.nNode;

    if(nLen == 0)
    {
        g_SDKServer.ExcuteCallback(&ret);
    }
    else if(nLen<400)
    {
        char sysdata[500];
        memcpy(sysdata,pBuf+1,nLen);
        sysdata[nLen] = '\0';
        std::string str(sysdata);
        ret.strCmd = str;
        g_SDKServer.ExcuteCallback(&ret);
    }*/
}

void CMsgCommon::TextOutput(std::string strResult)
{
    /*
    //m_Mid = PAMSG_MID_TEXTOUT;
    m_vBuffer.clear();

    //Dstn
    m_vBuffer.push_back(0x00);

    //len
    unsigned char len = strResult.length();
    m_vBuffer.push_back(len);

    for(unsigned int i = 0;i<strResult.size();i++)
    {
        m_vBuffer.push_back(strResult[i]);
    }
    m_pSystem->OnSend(this);*/
}

void CMsgCommon::SpecialFunction(unsigned short number,bool bActivate)
{
    /*
    //m_Mid = PAMSG_MID_SPECIAL;
    m_vBuffer.clear();

    //subMid
    m_vBuffer.push_back(0x00);

    //version
    m_vBuffer.push_back(0x01);

    //special number
    WriteLONG(number);

    //action
    if(bActivate)
    {
        m_vBuffer.push_back(0x01);
    }
    else
    {
        m_vBuffer.push_back(0x00);
    }

    m_pSystem->m_pTransport->SendBroadcast(this,true);
    */
}

void CMsgCommon::OnSpecialFunction(unsigned char* pBuf,const int len)
{
    /*
    if(len < 5)
    {
        return;
    }
    CService* pSer = CService::GetInstance();
    if(!pSer->m_bCCCF)
    {
        return;
    }
    if(m_pSystem == pSer->GetSelfD1System())
    {
        //not deal the self system for lamptest
        return;
    }

    //subMid
    m_SubMid = pBuf[0];

    //version
    //int version = pBuf[1];

    //special number
    unsigned short number = ReadLONG(&pBuf[2]);

    //action
    int action = pBuf[4];

    int LampTest = 502;
    int SpeakerMute = 500;

    if(number == LampTest)
    {
        CLampTestResult ret;
        ret.nNode = m_pSystem->GetNode();
        ret.bActivate = (action == 1);
        pSer->ExcuteCallback(&ret);
    }
    else if(number == SpeakerMute)
    {
        pSer->MuteSpeaker(m_pSystem->GetNode(),action == 1);
    }
    */
}

