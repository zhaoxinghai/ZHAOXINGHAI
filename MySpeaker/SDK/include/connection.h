
/*
* Date:     2016-8-4
* Author :  xinghai,zhao
* Content : struct define for parameters
*/

#ifndef CONNECTION_H
#define CONNECTION_H

#include "predefine.h"
#include "device.h"
#include "callback.h"

//the activate flags,total 32 bit
#define FLAGS_PARTIAL_CONNECT      0x00000001
#define FLAGS_RECONNECT            0x00000002
#define FLAGS_REPORT_STATE_CHANGE  0x00000004
#define FLAGS_TEMPORAL_STORAGE     0x00000008

#define FLAGS_RECONNECT_REQUEST    0x00000020

//the activate flags
#define SET_PARTIAL_CONNECTION(x)         (x |= FLAGS_PARTIAL_CONNECT)
#define IS_PARTIAL_CONNECTION(x)          (x & FLAGS_PARTIAL_CONNECT)

#define SET_RECONNECT(x)                  (x |= FLAGS_RECONNECT)
#define IS_RECONNECT(x)                   (x & FLAGS_RECONNECT)

#define SET_REPORT_STATE_CHANGE(x)        (x |= FLAGS_REPORT_STATE_CHANGE)
#define IS_REPORT_STATE_CHANGE(x)         (x & FLAGS_REPORT_STATE_CHANGE)

#define SET_TEMPORAL_STORAGE(x)           (x |= FLAGS_TEMPORAL_STORAGE)
#define IS_TEMPORAL_STORAGE(x)            (x & FLAGS_TEMPORAL_STORAGE)

#define SET_RECONNECT_REQUEST(x)          (x |= FLAGS_RECONNECT_REQUEST)
#define IS_RECONNECT_REQUEST(x)           (x & FLAGS_RECONNECT_REQUEST)

//the activate type
enum e_ACTIVATE_TYPE
{
    ACTIVATE_NORMAL,
    ACTIVATE_MICR,
    ACTIVATE_PLAY
};

//activate source
struct t_AudioSrc
{
	t_AudioSrc()
	{
		Loop = 0;
		Nextldx = 255;
        Type = AS_TYPE_AI;

		Num = 0;
		Title = 0;
	}
    unsigned char   Loop;            //0-250, 255 loop counter (255 means forever)
    unsigned char   Nextldx;         //index of the next source �C 255 means always next source
    unsigned short  Num;             //source number of type AS_TYPE_AI (0 means AI 1)
    unsigned char   Type;            //AS_TYPE_AD,ASTYPE_AN,AS_TYPE_CS
    unsigned short  Title;           //0 to 249	source title (0 means title 1)
};

//activate destination
struct t_AudioDest
{
	t_AudioDest()
	{
		Sys = 0;
		Type = AS_TYPE_AO;
	}
    t_AudioDest(const t_AudioDest &c)
    {
        this->Sys = c.Sys;
        this->Type = c.Type;

        for (unsigned int i = 0;i < c.vOutputPort.size();i++)
        {
            vOutputPort.push_back(c.vOutputPort[i]);
        }
        for (unsigned int i = 0;i < c.vOuputName.size();i++)
        {
            vOuputName.push_back(c.vOuputName[i]);
        }
    }

    virtual ~t_AudioDest()
    {
        vOutputPort.clear();
        vOuputName.clear();
    }
    unsigned short  Sys;             //0 DstnEl, local system
    unsigned char   Type;            //AS_TYPE_AO bitarray refers to Audio Outputs
                                     //AS_TYPE_DD bitarray refers to Destination Definitions         
    std::vector<int>     vOutputPort;
    std::vector<std::string> vOuputName;

};

class CActivate
{
public:
    CActivate()
	{
        type = ACTIVATE_NORMAL;
        chProcess = 0;
    	chRequest = 0;           
    	Priority = 250;
    	TimeOut = 0;
    	Level = 0;   
    	Flags = 0;
        bPreListen = false;
        nRepeatCount = 1;

        nTriggleNode = 0;
        nTriggleType = 0;
        nTriggleNumber = 0;
    };
    CActivate(const CActivate *pA)
    {
        type = pA->type;
        chProcess = pA->chProcess;

        chRequest = pA->chRequest;
        Priority = pA->Priority;
        TimeOut = pA->TimeOut;
        Level = pA->Level;
        Flags = pA->Flags;
        bPreListen = pA->bPreListen;

        nTriggleNode = pA->nTriggleNode;
        nTriggleType = pA->nTriggleType;
        nTriggleNumber = pA->nTriggleNumber;

        for (unsigned int i = 0;i < pA->vSrc.size();i++)
        {
            vSrc.push_back(pA->vSrc[i]);
        }
        for (unsigned int i = 0;i < pA->vDest.size();i++)
        {
            vDest.push_back(pA->vDest[i]);
        }
    }
    virtual ~CActivate()
    {
        vSrc.clear();
        vDest.clear();
    }

    e_ACTIVATE_TYPE type;
    int  chRequest;
    int  chProcess;   //this is a unique id from function PA::GetChProcess()
    bool bPreListen;
    int  Priority;
    int  TimeOut;     //seconds
    int  Level;       //-200 to 200
    int  Flags;
    int  nRepeatCount;

    int  nTriggleNode;
    int  nTriggleType;
    int  nTriggleNumber;

    std::vector<t_AudioSrc>   vSrc;
    std::vector<t_AudioDest>  vDest;
};

class CActivateMicr : public CActivate
{
public:
    CActivateMicr()
    {
        type = ACTIVATE_MICR;
        nPort = 0;
        nRtpNode = 0;
        nRtpChannel = 0;
    }
    CActivateMicr(const CActivateMicr* pA):CActivate(pA)
    {
        nPort = pA->nPort;
        nRtpNode = pA->nRtpNode;
        nRtpChannel = pA->nRtpChannel;  
    }
    virtual ~CActivateMicr()
    {

    }
    int  nPort;
    int  nRtpNode;
    int  nRtpChannel;
};

class CActivatePlay: public CActivateMicr
{
public:
    CActivatePlay()
    {
        type = ACTIVATE_PLAY;
        nPort = 0;
        bIsUSB = false;
    }
    CActivatePlay(const CActivatePlay *pA):CActivateMicr(pA)
    {
        for (unsigned int i = 0;i < pA->vIOMapPath.size();i++)
        {
            this->vIOMapPath.push_back(pA->vIOMapPath[i]);
        }
        nRepeatCount = pA->nRepeatCount;
        bIsUSB = pA->bIsUSB;
    }
    virtual ~CActivatePlay()
    {
        this->vIOMapPath.clear();
    }
    bool                      bIsUSB;
    std::vector<std::string>  vLocalPath;
    std::vector<t_FILEMAP>    vIOMapPath;
};

class CReActivate
{
public:
    CReActivate()
	{
    	chRequest = 0;
    	chProcess = 0;
    	nRtpNode = 0;
    	nRtpChannel = 0;
	};
    virtual ~CReActivate() {};

    int  chRequest;
    int  chProcess;
    int  nRtpNode;
    int  nRtpChannel;
};

class CDeActivate
{
public:
    CDeActivate()
	{
    	chRequest = 0;
    	chProcess = 0;
    };
    virtual ~CDeActivate() {};

    int chRequest;
    int chProcess;
};

#endif
