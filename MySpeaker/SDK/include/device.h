
#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <vector>

enum e_ASTYPE
{
    AS_TYPE_PR = 0,	    //VD1	pre - amplifier
    AS_TYPE_MA = 1,	    //unused	mixing amplifier
    AS_TYPE_PA = 2,	    //VD1	power amplifier
    AS_TYPE_LR = 3,	    //VD1	line relais
    AS_TYPE_SG = 4,	    //VD1	signal generator
    AS_TYPE_RU = 5,	    //VD1	speech storage
    AS_TYPE_IO = 6,	    //VD1	signaling input - output device
    AS_TYPE_RR = 7,	    //VD1	replacement relais
    AS_TYPE_MX = 8,	    //unused	matrix device
    AS_TYPE_NI = 9,	    //unused	network interface
    AS_TYPE_CI = 10,	//unused	callstation interface
    AS_TYPE_MU = 11,	//unused	multiplexer(I2C)
    AS_TYPE_IC = 12,	//unused	PSCD callstation
    AS_TYPE_AM = 13,	//unused	analog measurement device
    AS_TYPE_CS = 14,	//VD1	callstation
    AS_TYPE_PS = 15,	//VD1	preselect station
    AS_TYPE_DD = 16,	//VD1	destination definition
    AS_TYPE_SS = 17,	//unused	source sequence
    AS_TYPE_TL = 18,	//VD1	transport line
    AS_TYPE_IB = 19,	   //unused	input buffer - PSCD callstation
    AS_TYPE_NONE = 20,	    //VD1	no type
    AS_TYPE_INVALID = 21,	//VD1	invalid type
    AS_TYPE_ALL = 22,		//all types
    AS_TYPE_AI = 23,	//VD1	virtual audio input
    AS_TYPE_AO = 24,	//VD1	virtual audio output
    AS_TYPE_SF = 25,	//VD1	special function
    AS_TYPE_SC = 26,	//VD1	system control device
    AS_TYPE_SX = 27,	//Unused	interface box(SBOX)
    AS_TYPE_HO = 28,	//VD1	controller computer(HOST)
    AS_TYPE_RH = 29,	//unused	remote controller computer(remote HOST)
    AS_TYPE_AL = 30,	//unused	audio line
    AS_TYPE_AN = 31,	//VD1	audio net �C a audio connection between systems
    AS_TYPE_LS = 32,	//unused	loudspeaker survey
    AS_TYPE_LT = 33,	//Unused	line survey
    AS_TYPE_AV = 34,	//VD1	automatic volume control
    AS_TYPE_MI = 35,	//unused	matrix input
    AS_TYPE_MO = 36,	//unused	matrix output
    AS_TYPE_DI = 37,	//unused	DIM4 digital callstation interface
    AS_TYPE_DS = 38,	//VD1	DCS digital callstation
    AS_TYPE_VC = 39,	//VD1	volume control input
    AS_TYPE_RTP = 40,	//VD1	audio stream
    AS_TYPE_PEQ = 41,	//VD1	parametric equalizer
    AS_TYPE_GEQ = 42,	//VD1	graphic equalizer
    AS_TYPE_DELAY = 43,	//VD1	audio delay
    AS_TYPE_ADPCM = 44,	//VD1	encoder type
    AS_TYPE_MPEG = 45,	//VD1	encoder type
    AS_TYPE_LP = 48     //VD1   Audio loop
};

enum e_Mode
{
    MODE_CONFIG,
    MODE_NORMAL
};

//device's type
enum e_DEVICE_TYPE
{
    DEVICE_IP_SPEAKER_A20 = 0,
    DEVICE_UNKNOW,
};

//the mininum unit
struct t_VDevice
{
    t_VDevice& operator = (const t_VDevice& c)
    {
        if(this != &c)
        {
            this->type = c.type;
            this->number = c.number;
            this->channel = c.channel;
        }
        return (*this);
    }
    bool operator == (const t_VDevice& c)
    {
        if (type == c.type && number == c.number && channel == c.channel)
        {
            return true;
        }
        return false;
    }
    bool operator != (const t_VDevice& c)
    {
        if (type == c.type && number == c.number && channel == c.channel)
        {
            return false;
        }
        return true;
    }
    unsigned char  type;	 //type code defined at ASTYPE definition
    unsigned short number;
    unsigned char  channel;  //0 - 250 ......... channel of the device
};

//config
struct t_Node
{
    t_Node()
    {
        eType = DEVICE_UNKNOW;
    }
    e_DEVICE_TYPE    eType;
    int              nNode;
    std::string      strIP;      //host ip
    std::string      strName;    //device name
};

struct t_FILEMAP
{
    t_FILEMAP()
    {
        nNumber1 = 0;
        nNumber2 = 0;
    }
    t_FILEMAP(int num1,int num2,std::string sName,std::string strPath)
    {
        nNumber1 = num1;
        nNumber2 = num2;
        strName = sName;
        vPath.push_back(strPath);
    }
    t_FILEMAP& operator = (const t_FILEMAP& c)
    {
        if(this != &c)
        {
            this->nNumber1 = c.nNumber1;
            this->nNumber2 = c.nNumber2;
            this->strName = c.strName;
            this->vPath = c.vPath;
        }
        return (*this);
    }
    void AddAudioFile(std::string strPath)
    {
        vPath.push_back(strPath);
    }

    //this function is for USB
    std::string GetAudioPath()
    {
        if(vPath.size()>0)
        {
            return vPath[0];
        }
        return "";
    }

    int          nNumber1;
    int          nNumber2;
    std::string  strName;
    std::vector<std::string> vPath;
};

//transport line
struct t_TL
{
    int nNode;
    int nType;  // 0 ...................I / O transport line
                // 1 to 30 ..........PSCD callstation
    int nNum;   // line's number
};

enum e_ETCS_DEVICE_TYPE
{
    DEVICE_MICOPHONE,
    DEVICE_DKM,
    DEVICE_MAIN_LAN,
    DEVICE_BACKUP_LAN,
    DEVICE_VA,
    DEVICE_CPU
};

struct t_DeviceStatus
{
    t_DeviceStatus()
    {
        type = DEVICE_MICOPHONE;
        IsError = false;
    }

    t_DeviceStatus& operator = (const t_DeviceStatus& c)
    {
        if(this != &c)
        {
            this->type = c.type;
            this->IsError = c.IsError;
            this->strDesc = c.strDesc;
        }
        return (*this);
    }
    e_ETCS_DEVICE_TYPE  type;
    bool        IsError;
    std::string strDesc;
};

struct t_Configuration
{
    //dB
    float    fMicrophone_ADC;
    int      nMaxLogDebug;
    int      nMaxBgQueueCount;
    int      nMaxPlayingCount;
};

struct t_VLANConfig
{
    int         nID;
    int         nPriority;
    std::string strIP;
    std::string strSubnetMask;
};

struct t_VolReply
{
    t_VDevice device;
    float fVolume;
    float fMin;
    float fMax;
};

#endif
