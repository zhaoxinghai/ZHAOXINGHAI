
#ifndef AUDIO_BUFFER_H
#define AUDIO_BUFFER_H

#include "predefine.h"
#include "cyclebuffer.h"

#define SAMPLE_BUFFER_SIZE (ED1AudioFrameSampleCount*sizeof(TAudioSample))

class CAudioBuffer
{
public:
    CAudioBuffer(std::vector<int> &vPort);
    virtual ~CAudioBuffer();

    bool IsPortExit(int nPort);
    void setframe(int nframe);
    int write(unsigned char* buf, int len);
    int read(unsigned char* buf, int len);
    int freesize();
    int usedsize();
    void reset();

    std::vector<int>     m_vPort;
    std::mutex      m_mutex;
    CCycleBuffer    m_Audiobuffer;
};

class CAudioData
{
public:
    static CAudioBuffer* CreateBuf(bool bCapture, std::vector<int> &vPort);
    static void release();

    static int write(bool bCapture,int nPort,unsigned char* buf, int len);
    static int read(bool bCapture,int nPort,unsigned char* buf, int len);

    static CAudioBuffer* GetAudioBuffer(bool bCapture, int nPort);
    static void GetAudioBuffer(bool bCapture, std::vector<int> &vPort, std::vector<CAudioBuffer*> &vAudioBuf);

protected:
    static std::vector<std::shared_ptr<CAudioBuffer>>  m_vCaptureBuf;
    static std::vector<std::shared_ptr<CAudioBuffer>>  m_vPlayBuf;
};
#endif

