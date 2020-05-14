
#include "public.h"
#include "common.h"

void CPublic::GetChannelBuffer(unsigned char* Left,
                               unsigned char* Right,
                               unsigned char* Data,
                               int nFrames,
                               int nBlock)
{
    for (int i = 0; i < nFrames; ++i)
    {
        memcpy(Left,Data,nBlock);
        Left += nBlock;
        Data += nBlock;

        memcpy(Right,Data,nBlock);
        Right += nBlock;
        Data += nBlock;
    }
}

void CPublic::SetChannelBuffer(unsigned char* Left,
                               unsigned char* Right,
                               unsigned char* Data,
                               int nFrames,
                               int nBlock)
{
    for (int i = 0; i < nFrames; ++i)
    {
        memcpy(Data,Left,nBlock);
        Left += nBlock;
        Data += nBlock;

        memcpy(Data,Right,nBlock);
        Right += nBlock;
        Data += nBlock;
    }
}

unsigned short CPublic::GetChRequest()
{
    static unsigned short request = 0;
    request++;
    if (request >= 65535)
    {
        request = 1;
    }
    return request;
}

unsigned short CPublic::GetChProcess()
{
    static unsigned short process = 0;
    process++;
    if (process >= 65535)
    {
        process = 1;
    }
    return process;
}

bool CPublic::IsPortExist(std::vector<int> &vSrc, int nPort)
{
    for (unsigned int i = 0; i < vSrc.size(); i++)
    {
        if (vSrc[i] == nPort)
        {
            return true;
        }
    }
    return false;
}

void CPublic::GetDateTime(t_DateTime &datetime)
{
    struct tm now_time;
    time_t time_seconds = time(0);

#ifdef _MSC_VER
    localtime_s(&now_time, &time_seconds);
#else
    localtime_r(&time_seconds, &now_time);
#endif

    datetime.Year = now_time.tm_year + 1900;
    datetime.Month = now_time.tm_mon + 1;
    datetime.Day = now_time.tm_mday;
    datetime.Hour = now_time.tm_hour;
    datetime.Min = now_time.tm_min;
    datetime.Sec = now_time.tm_sec;
}

bool CPublic::CheckAdpFile(std::string strPath)
{
    //file type
    size_t pos = strPath.rfind(".");
    if (pos == std::string::npos)
    {
        return false;
    }
    std::string strExt = strPath.substr(pos, strPath.length() - pos);

    if (!CCommon::IsFileExist(strPath))
    {
        return false;
    }

    std::ifstream   hFile;
    hFile.open(strPath, std::ios::binary);
    if (!hFile)
    {
        return false;
    }
    //get file size
    hFile.seekg(0, std::ios::end);
    int nLen = (unsigned int)hFile.tellg();

    if (strExt == ".adp")
    {
        if (nLen % ED1AudioAdpFrameCount != 0)
        {
            hFile.close();
            return false;
        }

        hFile.seekg(0, std::ios::beg);

        //check header
        char adp[ED1AudioFrameEncodedByteCount];
        hFile.read(&adp[0], ED1AudioAdpFrameCount);
        AppendCheckSum((unsigned char*)&adp[0], ED1AudioAdpFrameCount);

        VpAudioCore::D1AudioAdpcmDecoder decoder;
        TAudioSample sample[ED1AudioFrameSampleCount];
        if (!decoder.DecodeOk(sample, (unsigned char*)&adp[0]))
        {
            hFile.close();
            return false;
        }

        //check tail
        hFile.seekg(-ED1AudioAdpFrameCount, std::ios::end);
        hFile.read(&adp[0], ED1AudioAdpFrameCount);
        AppendCheckSum((unsigned char*)&adp[0], ED1AudioAdpFrameCount);
        if (!decoder.DecodeOk(sample, (unsigned char*)&adp[0]))
        {
            hFile.close();
            return false;
        }
    }
    hFile.close();
    return true;
}

bool CPublic::CheckWavFile(std::string strPath,int &nChannel,int &nDeep,int &nRate)
{
    if (!CCommon::IsFileExist(strPath))
    {
        printf("%s","wav file not exist");
        return false;
    }

    //file type
    size_t pos = strPath.rfind(".");
    if (pos == std::string::npos)
    {
        printf("%s","wav file ext no ..");
        return false;
    }

    //ext
    std::string strExt = strPath.substr(pos, strPath.length() - pos);
    std::string strTmp = CCommon::strToLower(strExt);

    if (strTmp != ".wav")
    {
        printf("%s","wav file ext error");
        return false;
    }

    //open file
    std::ifstream   hFile;
    hFile.open(strPath, std::ios::binary);
    if (!hFile)
    {
        printf("%s","wav file open error");
        return false;
    }

    //get file size
    hFile.seekg(0, std::ios::end);
    int nLen = (unsigned int)hFile.tellg();
    if(nLen > 1024)
    {
        hFile.seekg(0, std::ios::beg);

        t_Wav_header header;
        hFile.read((char*)&header, sizeof(header));

        nChannel = header.nr_channels;
        nDeep = header.bits_per_sample;
        nRate = header.sample_rate;

        if ( (nChannel == 1 || nChannel == 2)
             && (nDeep == 8 || nDeep == 16 || nDeep == 24 || nDeep == 32)
             && (nRate >= 8000 && nRate <= 192000 ) )
        {
            hFile.close();
            return true;
        }
    }
    printf("%s","wav file format not support");
    hFile.close();
    return false;
}

void CPublic::AppendCheckSum(unsigned char *pBuffer, int len)
{
    unsigned int checksum = 0;
    for (int j = 0; j < len / 4; j++)
    {
        checksum += (static_cast<unsigned int>(pBuffer[4 * j + 0]) << 24)
            | (static_cast<unsigned int>(pBuffer[4 * j + 1]) << 16)
            | (static_cast<unsigned int>(pBuffer[4 * j + 2]) << 8)
            | (static_cast<unsigned int>(pBuffer[4 * j + 3]));
    }

    unsigned char *pBuf = pBuffer + len;

    //set checksum
    pBuf[0] = static_cast<unsigned char>(checksum >> 24);
    pBuf[1] = static_cast<unsigned char>(checksum >> 16);
    pBuf[2] = static_cast<unsigned char>(checksum >> 8);
    pBuf[3] = static_cast<unsigned char>(checksum);
}

bool CPublic::IsAllAdpFormat(std::vector<t_FILEMAP> &vPath)
{
    for(unsigned int i = 0;i<vPath.size();i++)
    {
        t_FILEMAP &file = vPath[i];
        for(unsigned int k = 0;k<file.vPath.size();k++)
        {
            if(!CheckAdpFile(file.vPath[k]))
            {
                return false;
            }
        }
    }
    return true;
}
