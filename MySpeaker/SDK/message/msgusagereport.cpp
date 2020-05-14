
#include "msgbase.h"
#include "msgusagereport.h"
#include "common.h"
#include "callback.h"
#include "sdk.h"
#include "service.h"
#include "routemanager.h"

CMsgUsageReport::CMsgUsageReport(CTSystem* pSys):CMsgBase(pSys)
{
    m_Mid = 0x01;
    m_SubMid = 0;
}

CMsgUsageReport::~CMsgUsageReport()
{

}

void CMsgUsageReport::OnRespond(unsigned char* pBuf, const int len)
{
    /*
    m_SubMid = pBuf[0];
    if (m_Mid == 0x01)
    {
        if (m_SubMid == 0x87)
        {
            OnDestUsageReport10(pBuf,len);
        }
    }
    else if (m_Mid == 0x02)  //destination usage report ext,every 5 seconds
    {
        if (m_SubMid == 0x8e)
        {
            //don't need to use this extension now.
            OnDestUsageReportExtension10(pBuf, len);
        }
    }
    else if (m_Mid == 0x41)
    {
        if (m_SubMid == 0x8D)
        {
            OnLocalSourceUsageReport11(pBuf, len);
        }
        else if (m_SubMid == 0x8E)
        {
            OnRemoteSourceUsageReport11(pBuf, len);
        }
    }
    */
}

void CMsgUsageReport::OnLocalSourceUsageReport11(unsigned char* pBuf, const int len)
{
    /*
    if (len < 7)
    {
        return;
    }

    unsigned char* p = pBuf;

    //submid
    m_SubMid = *p;
    p++;

    //retval
    unsigned char retVal = *p;
    p++;
    if (retVal != 0)
    {
        return;
    }

    //version
    //int version = *p;
    p++;

    //length in bytes of the audio output bitfield
    int AOBFlen = *p;
    p++;

    //start offset in bytes in the audio output bitfield
    int AOBFOffs = *p;
    p++;

    //number of elements
    int NumOfEl = *p;
    p++;

    //length in bytes of each element
    int LenOfEl = *p;
    p++;

    std::vector<t_LocalCall> vReport;
    for (int i = 0; i < NumOfEl; i++)
    {
        unsigned char *pEl = p;
        p += LenOfEl;

        t_LocalCall route;
        route.CS = ReadLONG(pEl);
        pEl += 2;

        route.CSNode = ReadLONG(pEl);
        pEl += 2;

        route.AI = *pEl;
        if(route.AI > 0)
        {
            route.AI += 1;
        }
        pEl++;

        route.Title = *pEl;
        pEl++;

        route.ConState = *pEl;
        pEl++;

        route.Priority = *pEl;
        pEl++;

        route.DstRTP = *pEl;
        pEl++;

        //only the local output is here.
        unsigned char Bitarray[32] = { 0 };
        memcpy(&Bitarray[AOBFOffs], pEl, AOBFlen);
        Change32BytetoVector(Bitarray, route.vOutput);

        vReport.push_back(route);
    }

    m_pSystem->SourceUsageLocal(vReport);
    */
}

void CMsgUsageReport::OnRemoteSourceUsageReport11(unsigned char* pBuf, const int len)
{
    /*
    if (len < 7)
    {
        return;
    }

    unsigned char* p = pBuf;

    //submid
    m_SubMid = *p;
    p++;

    //retval
    unsigned char retVal = *p;
    p++;
    if (retVal != 0)
    {
        return;
    }

    //version
    //int version = *p;
    p++;

    //length in bytes of the audio output bitfield
    int AOBFlen = *p;
    p++;

    //start offset in bytes in the audio output bitfield
    int AOBFOffs = *p;
    p++;

    //number of elements
    int NumOfEl = *p;
    p++;

    //length in bytes of each element
    int LenOfEl = *p;
    p++;

    std::vector<t_RemoteCall> vReport;
    for (int i = 0; i < NumOfEl; i++)
    {
        unsigned char* pEl = p;
        p += LenOfEl;

        t_RemoteCall remote;
        remote.CS = ReadLONG(pEl);
        pEl += 2;

        remote.CSNode = ReadLONG(pEl);
        pEl += 2;

        remote.RTPChannel = *pEl;
        pEl++;

        remote.RTPNode = ReadLONG(pEl);
        pEl += 2;

        remote.flags = *pEl;
        pEl++;

        unsigned char Bitarray[32] = { 0 };
        memcpy(&Bitarray[AOBFOffs], pEl, AOBFlen);
        Change32BytetoVector(Bitarray, remote.vOutput);

        vReport.push_back(remote);
    }
    m_pSystem->SourceUsageRemote(vReport);
    */
}

void CMsgUsageReport::SendLocalSourceUsageReport11()
{
    m_vBuffer.clear();
    m_Mid = 0x41;
    m_vBuffer.push_back(0x8D);   //sub mid
    m_vBuffer.push_back(0x00);   //retval
    m_vBuffer.push_back(0x01);   //version

    //get the playing local routes
    CRouteManager *pData = &CService::GetInstance()->m_routeManager;
    std::vector<CRouteLocal*> vLocal;
    pData->GetLocalUsageRoute(vLocal);
    
    if(vLocal.size()==0)
    {
        m_vBuffer.push_back(0x00);   //AOBFlen
        m_vBuffer.push_back(0x00);   //AOBFOffs
        m_vBuffer.push_back(0x00);   //NumOfEl
        m_vBuffer.push_back(0x00);   //LenOfEl
        m_pSystem->m_pTransport->SendBroadcast(this,false);
        return;
    }
    
    //busy local
    char NumOfEl = (char)vLocal.size();
    m_vBuffer.push_back(0x00);           //AOBFlen
    m_vBuffer.push_back(0x00);           //AOBFOffs
    m_vBuffer.push_back(NumOfEl);        //NumOfEl
    m_vBuffer.push_back(0x09);           //LenOfEl 9 bytes

    CService* pService = CService::GetInstance();
    for (int i = 0; i < NumOfEl; i++)
    {
        CRouteLocal*p = vLocal[i];
        CActivateMicr* pMic = (CActivateMicr*)p->m_pActivate;

        WriteLONG(0x00);                                    //CS call number
        WriteLONG(CService::GetInstance()->GetNode());      //CSNode

        int nRtpChannel = pMic->nRtpChannel;
        if (pService->IsGongPlaying(nRtpChannel))
        {
            m_vBuffer.push_back(30);                         //AI 31
        }
        else
        {
            if(pMic->type == ACTIVATE_PLAY)
            {
                m_vBuffer.push_back(249);                    //AI 250,OK
            }
            else
            {
                m_vBuffer.push_back(p->m_nPort-1);           //AI,OK
            }
        }
        m_vBuffer.push_back(0);                              //Title
        m_vBuffer.push_back(p->m_RouteResult.e_ConState);    //Con.State
        m_vBuffer.push_back((unsigned char)pMic->Priority);  //Priority
        m_vBuffer.push_back(pMic->nRtpChannel);              //DstRtp
    }

    m_pSystem->m_pTransport->SendBroadcast(this,false);
}

void CMsgUsageReport::SendRemoteSourceUsageReport11()
{
    m_vBuffer.clear();
    m_Mid = 0x41;
    m_vBuffer.push_back(0x8E);   //sub mid
    m_vBuffer.push_back(0x00);   //retval
    m_vBuffer.push_back(0x01);   //version

    //get the playing remote routes
    CRouteManager *pData = &CService::GetInstance()->m_routeManager;
    std::vector<CRouteRemote*> vRemote;
    pData->GetRemoteUsageRoute(vRemote);

    if (vRemote.size()==0)
    {
        m_vBuffer.push_back(0x00);   //AOBFlen
        m_vBuffer.push_back(0x00);   //AOBFOffs
        m_vBuffer.push_back(0x00);   //NumOfEl
        m_vBuffer.push_back(0x00);   //LenOfEl

        m_pSystem->m_pTransport->SendBroadcast(this,false);
        return;
    }

    //busy remote
    char NumOfEl = (char)vRemote.size();
    m_vBuffer.push_back(0x20);           //AOBFlen 256/8 = 32
    m_vBuffer.push_back(0x00);           //AOBFOffs  00 begin
    m_vBuffer.push_back(NumOfEl);        //NumOfEl
    m_vBuffer.push_back(0x28);           //LenOfEl 8+32 = 40 bytes

    for (unsigned int i = 0; i < vRemote.size(); i++)
    {
        CRouteRemote*p = vRemote[i];
        CActivate* pAct = p->m_pActivate;

        //WriteLONG(pAct->nOriCsNum);                              //CS call number
        //WriteLONG(pAct->nOriNodeNum);                            //CSNode
        m_vBuffer.push_back((unsigned char)pAct->vSrc[0].Title);  //RtpChannel
        WriteLONG(pAct->vSrc[0].Num);                            //RtpNode
        m_vBuffer.push_back(0x00);                                //flags(reserved)

        //local output                               
        unsigned char szDest[32] = { 0 };
        memset(szDest, 0, sizeof(szDest));
        std::vector<int> vOutput;
        for (unsigned int j = 0; j < p->m_vOutput.size(); j++)
        {
            if(p->m_vOutput[j].bConnect)
            {
                int nPort = p->m_vOutput[j].nPort;  //OK
                vOutput.push_back(nPort);
            }
        }
        ChangeVectorto32Byte(szDest, vOutput);
        
        //AOBF Array
        for (int k = 0; k < 32; k++)
        {
            m_vBuffer.push_back(szDest[k]);
        }
    }
 
    m_pSystem->m_pTransport->SendBroadcast(this,false);
}
