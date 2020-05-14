#ifndef __VPAUDIOCORE_D1ADPCMCODEC_H__
#define __VPAUDIOCORE_D1ADPCMCODEC_H__

#include "predefine.h"
#include "define.h"
#include <stdint.h>

// the number of samples of one audio frame(8.7 minisecond)
#define ED1AudioFrameSampleCount 418

// the size in bytes of an encoded audio frame
#define ED1AudioFrameEncodedByteCount 484

#define ED1AudioAdpFrameCount 480

#define EHistorySampleCount   4

typedef float TAudioSample;

enum TAdpcmDecodErrCode
{
    EAdpcmDecodErrCode_Success = 0
    , EAdpcmDecodErr_InvalidHeader
    , EAdpcmDecodErr_InvalidScaleFactor
    , EAdpcmDecodErr_InvalidCheckSum
};

namespace AUDIO_CODER 
{
    void AdpcmToByteRepresentationFromInt(uint8_t aAdpcmBytesOut[], const int_fast16_t aAdpcmIntsIn[], unsigned int aScaleFactorIdx);

    TAdpcmDecodErrCode AdpcmToIntFromByteRepresentation(int_fast16_t aAdpcmIntsOut[], unsigned int& aScaleFactorIdx, const uint8_t   aAdpcmBytesIn[]);
};

namespace VpAudioCore {

// the variodyn system compresses the audio data rate via differential pulse code modulation.
// Internally the algorithm uses a fixed set of prediction coefficients to estimate the next sample
// and build the differential value from the actual value. Thus the algorithm is in not adapative. However
// it is quite simular to the well known adpcm methods.
//
// The encoded samples are used as the payload of an RTP packet (payload type=39)
class D1AudioAdpcmEncoder
{
public:
     D1AudioAdpcmEncoder();
    ~D1AudioAdpcmEncoder();

    // Encodes the given audio frame
    //
    // aEncodedAudioOut: ED1AudioFrameEncodedByteCount output bytes
    // aInputSamples   : ED1AudioFrameSampleCount      input samples
    //
    // to avoid audio overruns/clipping the absolute value of each sample must be smaller than 0.513005
    void Encode(uint8_t aEncodedAudioOut[], const TAudioSample aInputSamples[]); // nothrow

    void Reset(); // nothrow

private:
    //EHistorySampleCount;
    TAudioSample m_history[EHistorySampleCount];
};

/////////////////////////

class D1AudioAdpcmDecoder
{
public:
    D1AudioAdpcmDecoder(); // nothrow
    ~D1AudioAdpcmDecoder(); // nothrow

    // Decodes a given audio frame
    //
    // aOutputSamples : ED1AudioFrameSampleCount output samples 
    // aEncodedAudioIn: ED1AudioFrameEncodedByteCount input bytes
    //
    // \return  if the encoded audio frame contains valid audio data the routine returns true.
    //          Otherwise the return value is false and the output samples do not change.
    bool DecodeOk(TAudioSample aOutputSamples[], const uint8_t aEncodedAudioIn[]); // nothrow

    void Reset(); // nothrow

private:
    //EHistorySampleCount
    TAudioSample m_history[EHistorySampleCount];
};


} // namespace VpAudioCore {

#endif // #ifndef __VPAUDIOCORE_D1ADPCMCODEC_H__

