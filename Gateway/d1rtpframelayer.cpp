#include "d1rtpframelayer.h"

#include <stdlib.h> // rand()

///////////////////////////////////////////////////////
// internal utilities
///////////////////////////////////////////////////////

namespace {

uint_fast16_t randValue16Bit()
{
    // note: rand() generates a 15 bits random number
    return      (static_cast<uint_fast16_t>(rand())  & 1)
            |   (static_cast<uint_fast16_t>(rand()) << 1)
            ;
}

uint_fast32_t randValue32Bit()
{
    return      (static_cast<uint_fast32_t>(rand()) << (32-1*15))
            |   (static_cast<uint_fast32_t>(rand()) << (32-2*15))
            |   (static_cast<uint_fast32_t>(rand())  & (2 + 1  ))
            ;
}

uint_fast16_t get16BitFromBigEndianByteArray(const uint8_t* pData)
{
    return      static_cast<uint_fast16_t>( (uint_fast16_t)pData[0]  << 8 )
            |   static_cast<uint_fast16_t>( pData[1]       )
            ;
}

uint_fast32_t get32BitFromBigEndianByteArray(const uint8_t* pData)
{
    return      static_cast<uint_fast32_t>( (uint_fast16_t)pData[0] << 24 )
            |   static_cast<uint_fast32_t>( (uint_fast16_t)pData[1] << 16 )
            |   static_cast<uint_fast32_t>( (uint_fast16_t)pData[2] <<  8 )
            |   static_cast<uint_fast32_t>( pData[3]       )
            ;
}


} // namespace {

///////////////////////////////////////////////////////
// interface implementation
///////////////////////////////////////////////////////

namespace VpAudioCore {

D1RtpFrameCreator::D1RtpFrameCreator() // nothrow
{
    // note: reset() must initialize all member variables (!)
    Reset();
}

D1RtpFrameCreator::~D1RtpFrameCreator() // nothrow
{
}

void D1RtpFrameCreator::CreateRtpHeader(uint8_t aRtpFrameOut[])
{
    // initialize RTP header
    {
        // 1st byte:
        //      bit 0+1 : RTP version       : = 2
        //      bit 2   : padding           : always 0 (the payload contains never padding bytes)
        //      bit 3   : extension         : always 0 (no header extension used)
        //      bit 4..7: CSRC              : always 0 (no contributing source identifiers)
        aRtpFrameOut[0] = 0x02;

        // 2nd byte:
        //      bit 0   : application specific  : always 0 (unused)
        //      bit 1..7: payload type          : 39 for AV-Digital specific audio codec (see TD1AudioAdpcmEncoder)
        static_assert((0 <= ED1RtpPayloadId) && (ED1RtpPayloadId < 128), "The RTP-payload identifier must not contain more the 7 bits.");
        aRtpFrameOut[1] = static_cast<uint8_t>(ED1RtpPayloadId << 1);

        // byte 3+4 = sequence number (16 bit unsigned integer, big endian)
        aRtpFrameOut[2] = static_cast<uint8_t>((m_sequenceNumber >> 8) & 0xFF);
        aRtpFrameOut[3] = static_cast<uint8_t>((m_sequenceNumber) & 0xFF);

        // byte 5..8 = time stamp (32 bit unsigned integer, big endian)
        aRtpFrameOut[4] = static_cast<uint8_t>((m_timeStamp >> 24) & 0xFF);
        aRtpFrameOut[5] = static_cast<uint8_t>((m_timeStamp >> 16) & 0xFF);
        aRtpFrameOut[6] = static_cast<uint8_t>((m_timeStamp >> 8) & 0xFF);
        aRtpFrameOut[7] = static_cast<uint8_t>((m_timeStamp) & 0xFF);

        // byte 9..12 = synchronization source identifier (32 bit unsigned integer, big endian)
        aRtpFrameOut[8] = static_cast<uint8_t>((m_syncSrcId >> 24) & 0xFF);
        aRtpFrameOut[9] = static_cast<uint8_t>((m_syncSrcId >> 16) & 0xFF);
        aRtpFrameOut[10] = static_cast<uint8_t>((m_syncSrcId >> 8) & 0xFF);
        aRtpFrameOut[11] = static_cast<uint8_t>((m_syncSrcId) & 0xFF);
    }
    // update sequence number and time-stamp for the next method call.
    // The synchronizatiom source identifier must remain constant
    {
        m_sequenceNumber += 1;
        m_timeStamp += ED1AudioFrameSampleCount;
    }
}

void D1RtpFrameCreator::CreateNextRtpFrame(uint8_t aRtpFrameOut[], const TAudioSample aAudioFrameIn[]) // nothrow
{
    CreateRtpHeader(&aRtpFrameOut[0]);

    // initialize RTP payload
    {
        //static_assert( ED1RtpHeaderSizeInBytes == 12, "" )
        m_audioEncoder.Encode( &aRtpFrameOut[ED1RtpHeaderSizeInBytes], aAudioFrameIn );
    }
}

// note: reset() must initialize all member variables (!)
void D1RtpFrameCreator::Reset() // nothrow
{
    // generate 16-bit sequence number
    m_sequenceNumber = randValue16Bit();
    m_timeStamp      = randValue32Bit();
    m_syncSrcId      = randValue32Bit();

    m_audioEncoder.Reset();
}

//////////////////////////

D1RtpFrameDecoder::D1RtpFrameDecoder() : m_audioDecoder()
{
    // no need to call reset here: the audio decoder is the only variable
    // and it is resetted automatically by the default constructor.
    //reset();
}

D1RtpFrameDecoder::~D1RtpFrameDecoder()
{
}

//static
uint_fast16_t D1RtpFrameDecoder::GetPayloadId(const uint8_t aRtpFrame[])
{
    return (aRtpFrame[1] >> 1);
}

//static
uint_fast16_t D1RtpFrameDecoder::GetSequenceNumber(const uint8_t aRtpFrame[])
{
    return get16BitFromBigEndianByteArray(&aRtpFrame[2]);
}

//static
uint_fast32_t D1RtpFrameDecoder::GetTimeStampOfD1RtpFrame(const uint8_t aRtpFrame[])
{
    return get32BitFromBigEndianByteArray(&aRtpFrame[4]);
}

//static
uint_fast32_t D1RtpFrameDecoder::GetSyncSourceIdentifier (const uint8_t aRtpFrame[])
{
    return get32BitFromBigEndianByteArray(&aRtpFrame[8]);
}

// aAudioFrameSamplesOut: contains #ED1AudioFrameSampleCount output samples
// aRtpFrameIn: contains #ED1RtpFrameSizeInBytes elements
bool D1RtpFrameDecoder::GetNextSamplesOk(TAudioSample aAudioFrameSamplesOut[], const uint8_t aRtpFrameIn[]) // nothrow
{
    if( GetPayloadId(aRtpFrameIn) != ED1RtpPayloadId )
    {
        return false;
    }

    return m_audioDecoder.DecodeOk(aAudioFrameSamplesOut, &aRtpFrameIn[ED1RtpHeaderSizeInBytes]);
}

// this routine should be called once, if a new audio stream starts.
// This is not necessary for newly created objects (the constructor calls reset internally).
void D1RtpFrameDecoder::Reset() // nothrow
{
    m_audioDecoder.Reset();
}

///////////////////////////////////////////////
// test code
///////////////////////////////////////////////

#if 0

// test the RtpLayerCode

#include "D1RtpFrameLayer.h"
#include <math.h>

#include <stdlib.h>
#include <time.h>


#if ! defined(NDEBUG)
#include <assert.h>
#define DEBUG_ASSERT assert
#else
#define DEBUG_ASSERT
#endif

///////////////////////

void TestD1RtpLayer()
{
    srand( (unsigned) time(0) );

    D1RtpFrameCreator  creator;
    D1RtpFrameDecoder  decoder;
    int                 timeIdx = 0;

    for(int frameCount = 0; frameCount < 8 * 25; ++frameCount)
    {
        float audioSamples[ED1AudioFrameSampleCount];
        for(int idx = 0; idx < ED1AudioFrameSampleCount; ++idx)
        {
            audioSamples[idx] = float(0.5 *   sin(2.0 * 3.1415927 * 1000.0f * timeIdx / 48000.0));
            timeIdx++;
        }


        // +4 for testing purposes only
        uint8_t rtpFrame[ED1RtpFrameSizeInBytes + 4];

        rtpFrame[ED1RtpFrameSizeInBytes+0] = 0x12;
        rtpFrame[ED1RtpFrameSizeInBytes+1] = 0x34;
        rtpFrame[ED1RtpFrameSizeInBytes+2] = 0x56;
        rtpFrame[ED1RtpFrameSizeInBytes+3] = 0x78;

        creator.CreateNextRtpFrame(rtpFrame, audioSamples);

        DEBUG_ASSERT( rtpFrame[ED1RtpFrameSizeInBytes+0] == 0x12 );
        DEBUG_ASSERT( rtpFrame[ED1RtpFrameSizeInBytes+1] == 0x34 );
        DEBUG_ASSERT( rtpFrame[ED1RtpFrameSizeInBytes+2] == 0x56 );
        DEBUG_ASSERT( rtpFrame[ED1RtpFrameSizeInBytes+3] == 0x78 );


        printf("%010u"    , D1RtpFrameDecoder::GetTimeStampOfD1RtpFrame(rtpFrame) );
        printf("  s:%05d ", D1RtpFrameDecoder::GetSequenceNumber       (rtpFrame) );
        printf("%010X"    , D1RtpFrameDecoder::GetSyncSourceIdentifier (rtpFrame) );

        if( D1RtpFrameDecoder::GetPayloadId(rtpFrame) == ED1RtpPayloadId )
        {
            float audioSamplesDecoded[ED1AudioFrameSampleCount + 1];
            audioSamplesDecoded[ED1AudioFrameSampleCount] = 1.2345f;

            if( decoder.GetNextSamplesOk(audioSamplesDecoded, rtpFrame) )
            {
                TAudioSample maxAbsError = 0;
                for(int idx = 0; idx < ED1AudioFrameSampleCount; ++idx)
                {
                    TAudioSample errorAbs = static_cast<TAudioSample>( fabs(audioSamplesDecoded[idx] - audioSamples[idx]) );
                    if( errorAbs > maxAbsError )
                    {
                        maxAbsError = errorAbs;
                    }
                }

                if( (maxAbsError > 1.0E-5) )
                {
                    double dBFs = 20.0 * log10( maxAbsError );
                    printf(" %8.2f ", dBFs );
                }
                else
                {
                    printf("%10s", "< -100");
                }
            }
            else
            {
                DEBUG_ASSERT(false);
            }

            DEBUG_ASSERT( audioSamplesDecoded[ED1AudioFrameSampleCount] == 1.2345f );
        }
        else
        {
            DEBUG_ASSERT(false);
        }
    }
}

#endif // TestRtpLayer

} // namespace AudioApi {

