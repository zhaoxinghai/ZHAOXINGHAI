
#ifndef _PUBLIC_H
#define _PUBLIC_H

#include "sdkdefine.h"
#include "mylog.h"
#include "audiodevice.h"
#include "define.h"
#include "sdkcallback.h"

struct t_Wav_header
{
    char riff[4];
    unsigned int riff_len;

    char wave[4];
    char fmt[4];
    unsigned int   fmt_size;
    unsigned short compression_code;
    unsigned short nr_channels;
    unsigned int   sample_rate;
    unsigned int   bytes_per_second;
    unsigned short block_align;
    unsigned short bits_per_sample;

    char data[4];
    unsigned int data_size;
};

class CPublic
{
public:
    //get channel audio buffer from 2 channel
    static void GetChannelBuffer(unsigned char* Left,
                                 unsigned char* Right,
                                 unsigned char* Data,
                                 int nFrames,
                                 int nBlock);

    static void SetChannelBuffer(unsigned char* Left,
                                 unsigned char* Right,
                                 unsigned char* Data,
                                 int nFrames,
                                 int nBlock);
  
    //id
    static int GetChRequest();
    static int GetChProcess();

    //is the port exist
    static bool IsPortExist(std::vector<int> &vSrc, int nPort);

    static void GetDateTime(t_DateTime &datetime);
    static bool DateTimeCompare(struct tm &t1,struct tm &t2);

    //check adp file format
    static bool CheckAdpFile(std::string strPath);
    static bool CheckWavFile(std::string strPath,int &nChannel,int &nDeep,int &nRate);

    //append 4 byte's check at end(len+4)
    static void AppendCheckSum(unsigned char *pBuffer, int len);

    static bool IsAllAdpFormat(std::vector<t_FILEMAP> &vPath);
};

#endif
