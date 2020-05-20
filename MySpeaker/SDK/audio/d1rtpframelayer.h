#ifndef __VPAUDIOCORE_D1RTPFRAMELAYER_H__
#define __VPAUDIOCORE_D1RTPFRAMELAYER_H__

#include "sdkdefine.h"
#include "d1adpcmcodec.h"

#define ED1RtpPayloadId 39

//the pa protocol uses rtp header with fixed size
#define ED1RtpHeaderSizeInBytes 12

//rtp frame size 496
#define  ED1RtpFrameSizeInBytes (ED1RtpHeaderSizeInBytes + ED1AudioFrameEncodedByteCount)

namespace VpAudioCore {

class D1RtpFrameCreator
{
public:
    D1RtpFrameCreator(); // nothrow
    ~D1RtpFrameCreator(); // nothrow

    // Encodes the given audio frame
    //
    // aRtpFrameOut : an RTP frame on output. The output buffer contains #ED1RtpFrameSizeInBytes elements
    // aAudioFrameIn: ED1AudioFrameSampleCount input samples
    //
    // to avoid audio overruns/clipping the absolute value of each sample must be smaller than 0.513005
    void CreateNextRtpFrame(uint8_t aRtpFrameOut[], const TAudioSample aAudioFrameIn[]); // nothrow

    void Reset(); // nothrow

    void CreateRtpHeader(uint8_t aRtpFrameOut[]);

private:
    // RTP header data
    uint_fast16_t           m_sequenceNumber;
    uint_fast32_t           m_timeStamp;
    uint_fast32_t           m_syncSrcId;

    // payload
    D1AudioAdpcmEncoder     m_audioEncoder;
};

///////////////

// This class can be used to get audio samples from RTP-frames used by the D1 system.
// The intention of this class is not (!) to reorder incoming RTP-packets if they are received with
// incorrect order from the network. However the routines #GetSequenceNumber(..) and #GetTimeStampOfD1RtpFrame(..)
// can be used to implement this functionality.
class  D1RtpFrameDecoder
{
public:
    D1RtpFrameDecoder(); // nothrow
    ~D1RtpFrameDecoder(); // nothrow

    // \brief Gets the payload-ID of the RTP frame
    //
    // If the returned value is == ED1RtpPayloadId, the Rtp frame contains a variodyn D1 audio payload.
    // In this case the routine #getNextSamplesOk(..) can be used for decoding.
    //
    // aRtpFrame: contains #ED1RtpFrameSizeInBytes elements
    //
    static uint_fast16_t    GetPayloadId            (const uint8_t aRtpFrame[]);

    // aRtpFrame: contains #ED1RtpFrameSizeInBytes elements
    static uint_fast16_t    GetSequenceNumber       (const uint8_t aRtpFrame[]);

    // aRtpFrame: contains #ED1RtpFrameSizeInBytes elements
    static uint_fast32_t    GetTimeStampOfD1RtpFrame(const uint8_t aRtpFrame[]);

    // aRtpFrame: contains #ED1RtpFrameSizeInBytes elements
    static uint_fast32_t    GetSyncSourceIdentifier (const uint8_t aRtpFrame[]);

    // aAudioFrameSamplesOut: contains #ED1AudioFrameSampleCount output samples
    // aRtpFrameIn: contains #ED1RtpFrameSizeInBytes elements
    bool GetNextSamplesOk(TAudioSample aAudioFrameSamplesOut[], const uint8_t aRtpFrameIn[]); // nothrow

    // this routine should be called once, if a new audio stream starts.
    // This is not necessary for newly created objects (the constructor calls reset internally).
    void Reset(); // nothrow

private:
    D1AudioAdpcmDecoder  m_audioDecoder;
};

} // namespace VpAudioCore {

#endif // #ifndef __VPAUDIOCORE_D1RTPFRAMELAYER_H__

