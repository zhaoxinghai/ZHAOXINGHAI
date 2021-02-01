
#ifndef THREAD_CAPTURE_H
#define THREAD_CAPTURE_H

#include "sdkdefine.h"
#include "mythread.h"
#include "cyclebuffer.h"
#include "threaddevice.h"

#ifdef _MSC_VER
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <audiopolicy.h>
#include <devicetopology.h>
#include <endpointvolume.h>
#include <stdio.h>
#include "audiodevicewin.h"
#else
#include "audiodevicealsa.h"
#endif

namespace TOP
{
    //this is for mic capture
    class CThreadCapture : public CThreadDevice
    {
    public:
        CThreadCapture();
        virtual ~CThreadCapture();

        virtual void Run();

        bool Init();

        //audio device callback function
        virtual bool CopyData(unsigned char* pData, int nFrames);
        virtual void Loop();

        //audio stream play job
        void AddPlayJob();
        void RemovePlayJob();
        int  GetPlayJobCount();

        CAudioDevice    m_AudioDevice;

    private:

        unsigned char   m_sampleLeft[PCM_FRAME_SAMPLE_COUNT * 2];
        unsigned char   m_sampleRight[PCM_FRAME_SAMPLE_COUNT * 2];

        //for mic resample
        void           *m_resamplehandle[2];
        double          m_resamplefactor;
        int             m_resamplefwidth[2];
    };
}

#endif

