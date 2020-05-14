
#include "d1adpcmcodec.h"
#include <memory.h>

namespace AUDIO_CODER{

enum
{
        PREDICTOR_SIZE                  =  4
    ,   SCALE_FACTOR_COUNT              = 63
    ,   ADPCM_BIT_COUNT                 =  9
};

static const uint_fast32_t ENCODED_AUDIO_HEADER  = static_cast<uint_fast32_t>( 0xAAAAAAAAul );


template<typename FloatType>
const FloatType* GetScaleFactorCoefficients()
{
    static const FloatType SCALE_FACTORS[SCALE_FACTOR_COUNT] =
    {
        static_cast<FloatType>(2.00000000000000E+000),
        static_cast<FloatType>(1.58740105196820E+000),
        static_cast<FloatType>(1.25992104989487E+000),
        static_cast<FloatType>(1.00000000000000E+000),
        static_cast<FloatType>(7.93700525984100E-001),
        static_cast<FloatType>(6.29960524947437E-001),
        static_cast<FloatType>(5.00000000000000E-001),
        static_cast<FloatType>(3.96850262992050E-001),
        static_cast<FloatType>(3.14980262473718E-001),
        static_cast<FloatType>(2.50000000000000E-001),
        static_cast<FloatType>(1.98425131496025E-001),
        static_cast<FloatType>(1.57490131236859E-001),
        static_cast<FloatType>(1.25000000000000E-001),
        static_cast<FloatType>(9.92125657480125E-002),
        static_cast<FloatType>(7.87450656184296E-002),
        static_cast<FloatType>(6.25000000000000E-002),
        static_cast<FloatType>(4.96062828740062E-002),
        static_cast<FloatType>(3.93725328092148E-002),
        static_cast<FloatType>(3.12500000000000E-002),
        static_cast<FloatType>(2.48031414370031E-002),
        static_cast<FloatType>(1.96862664046074E-002),
        static_cast<FloatType>(1.56250000000000E-002),
        static_cast<FloatType>(1.24015707185016E-002),
        static_cast<FloatType>(9.84313320230370E-003),
        static_cast<FloatType>(7.81250000000000E-003),
        static_cast<FloatType>(6.20078535925078E-003),
        static_cast<FloatType>(4.92156660115185E-003),
        static_cast<FloatType>(3.90625000000000E-003),
        static_cast<FloatType>(3.10039267962539E-003),
        static_cast<FloatType>(2.46078330057593E-003),
        static_cast<FloatType>(1.95312500000000E-003),
        static_cast<FloatType>(1.55019633981269E-003),
        static_cast<FloatType>(1.23039165028796E-003),
        static_cast<FloatType>(9.76562500000000E-004),
        static_cast<FloatType>(7.75098169906347E-004),
        static_cast<FloatType>(6.15195825143981E-004),
        static_cast<FloatType>(4.88281250000000E-004),
        static_cast<FloatType>(3.87549084953174E-004),
        static_cast<FloatType>(3.07597912571991E-004),
        static_cast<FloatType>(2.44140625000000E-004),
        static_cast<FloatType>(1.93774542476587E-004),
        static_cast<FloatType>(1.53798956285995E-004),
        static_cast<FloatType>(1.22070312500000E-004),
        static_cast<FloatType>(9.68872712382934E-005),
        static_cast<FloatType>(7.68994781429977E-005),
        static_cast<FloatType>(6.10351562500000E-005),
        static_cast<FloatType>(4.84436356191467E-005),
        static_cast<FloatType>(3.84497390714988E-005),
        static_cast<FloatType>(3.05175781250000E-005),
        static_cast<FloatType>(2.42218178095734E-005),
        static_cast<FloatType>(1.92248695357494E-005),
        static_cast<FloatType>(1.52587890625000E-005),
        static_cast<FloatType>(1.21109089047867E-005),
        static_cast<FloatType>(9.61243476787470E-006),
        static_cast<FloatType>(7.62939453125000E-006),
        static_cast<FloatType>(6.05545445239334E-006),
        static_cast<FloatType>(4.80621738393735E-006),
        static_cast<FloatType>(3.81469726562500E-006),
        static_cast<FloatType>(3.02772722619667E-006),
        static_cast<FloatType>(2.40310869196867E-006),
        static_cast<FloatType>(1.90734863281250E-006),
        static_cast<FloatType>(1.51386361309834E-006),
        static_cast<FloatType>(1.20155434598434E-006)
    };
    return SCALE_FACTORS;
}

template<typename FloatType>
const FloatType* GetPredictionCoeffsFloatingPoint()
{
    static const FloatType PREDICTOR_COEFFS[PREDICTOR_SIZE] =
    {
#if 1
        // \todo verify whether a delta pulse code modulation (fixed predicition coefficients can improve audio quality)
        //
        //       the 0 coefficients below seems to be much better (signal to noise ratio)!
        static_cast<FloatType>( 1.2315),
        static_cast<FloatType>(-0.7297),
        static_cast<FloatType>( 0.5863),
        static_cast<FloatType>(-0.3511)
#else
        static_cast<FloatType>(0),
        static_cast<FloatType>(0),
        static_cast<FloatType>(0),
        static_cast<FloatType>(0)
#endif
    };
    return PREDICTOR_COEFFS;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Audio conversion helpers: "differential <--> linear"
///////////////////////////////////////////////////////////////////////////////////////////////

// Get differential signal (=original signal - estimated signal) from linear pcm audio frame
//
// aDiffSignalOut   [   out]: differential signal vector (ED1AudioFrameSampleCount elements)
// aPcmSamplesIn    [in    ]: input audio vector (linear=pcm, ED1AudioFrameSampleCount elements)
// aHistory         [in/out]: contains the PREDICTOR_SIZE elements. all elements must be zero if calling this routine the 1st time
template<typename FloatType>
void GetDifferentialSignalFloatingPoint(FloatType aDiffSignalOut[], const FloatType aPcmSamplesIn[], FloatType aHistory[])
{
    const FloatType* PREDICTOR_COEFFS = GetPredictionCoeffsFloatingPoint<FloatType>();

    static_assert( PREDICTOR_SIZE           == 4, "This routine has been optimzed for a special prediction size" );
    static_assert( ED1AudioFrameSampleCount >= 4, "This routine has been optimzed for a special prediction size" );

    // get the 1st PREDICTOR_SIZE (=4) output values using the sample history
    {
        FloatType startPcmSamples[2*PREDICTOR_SIZE];

        memcpy(&startPcmSamples[      0       ], aHistory     , PREDICTOR_SIZE * sizeof(FloatType));
        memcpy(&startPcmSamples[PREDICTOR_SIZE], aPcmSamplesIn, PREDICTOR_SIZE * sizeof(FloatType));

        for(unsigned int outputIdx = 0; outputIdx < PREDICTOR_SIZE; outputIdx++)
        {
            // calculate prediction FIR filter (with unrolled loop)
            FloatType pcmSampleEstimated;

            pcmSampleEstimated  = startPcmSamples[outputIdx + PREDICTOR_SIZE - 1] * PREDICTOR_COEFFS[0];
            pcmSampleEstimated += startPcmSamples[outputIdx + PREDICTOR_SIZE - 2] * PREDICTOR_COEFFS[1];
            pcmSampleEstimated += startPcmSamples[outputIdx + PREDICTOR_SIZE - 3] * PREDICTOR_COEFFS[2];
            pcmSampleEstimated += startPcmSamples[outputIdx + PREDICTOR_SIZE - 4] * PREDICTOR_COEFFS[3];

            aDiffSignalOut[outputIdx] = aPcmSamplesIn[outputIdx] - pcmSampleEstimated;
        }
    }

    // get the final differential output samples
    {
        for(unsigned int outputIdx = PREDICTOR_SIZE; outputIdx < ED1AudioFrameSampleCount; ++outputIdx)
        {
            // calculate prediction FIR filter (with unrolled loop)
            FloatType pcmSampleEstimated;

            pcmSampleEstimated  = aPcmSamplesIn[outputIdx - 1] * PREDICTOR_COEFFS[0];
            pcmSampleEstimated += aPcmSamplesIn[outputIdx - 2] * PREDICTOR_COEFFS[1];
            pcmSampleEstimated += aPcmSamplesIn[outputIdx - 3] * PREDICTOR_COEFFS[2];
            pcmSampleEstimated += aPcmSamplesIn[outputIdx - 4] * PREDICTOR_COEFFS[3];

            aDiffSignalOut[outputIdx] = aPcmSamplesIn[outputIdx] - pcmSampleEstimated;
        }
    }

    // keep the sample history for the next call
    memcpy(aHistory, &aPcmSamplesIn[ED1AudioFrameSampleCount - PREDICTOR_SIZE], PREDICTOR_SIZE * sizeof(FloatType));
}

/////////////////////

// This routine is used by the decoder. It performs the inverse operation of 'GetDifferentialSignalFloatingPoint'
//
// aPcmSamplesOut   [   out]:   output audio vector (linear=pcm, ED1AudioFrameSampleCount elements)
// aDiffSignalIn    [in    ]:   differential audio input vector (ED1AudioFrameSampleCount elements)
// aHistory         [in/out]:   contains the last output samples (PREDICTOR_SIZE elements) of the previous function call.
//                              all elements must be zero if calling this routine the 1st time
template<typename FloatType>
void GetPcmSignalFloatingPoint(FloatType aPcmSamplesOut[], const FloatType aDiffSignalIn[], FloatType aHistory[])
{
    const FloatType* PREDICTOR_COEFFS = GetPredictionCoeffsFloatingPoint<FloatType>();

    static_assert( PREDICTOR_SIZE           == 4               , "This routine has been optimzed for a special prediction size" );
    static_assert( ED1AudioFrameSampleCount >= 2*PREDICTOR_SIZE, "This routine has been optimzed for a special prediction size" );

    // get the 1st PREDICTOR_SIZE (=4) output values using the last output values (of the last frame)
    {
        // use aPcmSamplesOut as output memory
        memcpy(aPcmSamplesOut, aHistory, PREDICTOR_SIZE * sizeof(FloatType));

        for(unsigned int outputIdx = 0; outputIdx < PREDICTOR_SIZE; outputIdx++)
        {
            // calculate prediction FIR filter (with unrolled loop)
            FloatType pcmSampleEstimated;

            pcmSampleEstimated   = aPcmSamplesOut[outputIdx + PREDICTOR_SIZE - 1] * PREDICTOR_COEFFS[0];
            pcmSampleEstimated  += aPcmSamplesOut[outputIdx + PREDICTOR_SIZE - 2] * PREDICTOR_COEFFS[1];
            pcmSampleEstimated  += aPcmSamplesOut[outputIdx + PREDICTOR_SIZE - 3] * PREDICTOR_COEFFS[2];
            pcmSampleEstimated  += aPcmSamplesOut[outputIdx + PREDICTOR_SIZE - 4] * PREDICTOR_COEFFS[3];

            aPcmSamplesOut[outputIdx + PREDICTOR_SIZE] = aDiffSignalIn[outputIdx] + pcmSampleEstimated;
        }

        memcpy(&aPcmSamplesOut[0], &aPcmSamplesOut[PREDICTOR_SIZE], PREDICTOR_SIZE * sizeof(FloatType));
    }

    // get the final differential output samples
    {
        for(unsigned int outputIdx = PREDICTOR_SIZE; outputIdx < ED1AudioFrameSampleCount; ++outputIdx)
        {
            // calculate prediction FIR filter (with unrolled loop)
            FloatType pcmSampleEstimated;

            pcmSampleEstimated   = aPcmSamplesOut[outputIdx - 1] * PREDICTOR_COEFFS[0];
            pcmSampleEstimated  += aPcmSamplesOut[outputIdx - 2] * PREDICTOR_COEFFS[1];
            pcmSampleEstimated  += aPcmSamplesOut[outputIdx - 3] * PREDICTOR_COEFFS[2];
            pcmSampleEstimated  += aPcmSamplesOut[outputIdx - 4] * PREDICTOR_COEFFS[3];

            aPcmSamplesOut[outputIdx] = aDiffSignalIn[outputIdx] + pcmSampleEstimated;
        }
    }

    // keep the sample history for the next call
    memcpy(&aHistory[0], &aPcmSamplesOut[ED1AudioFrameSampleCount - PREDICTOR_SIZE], PREDICTOR_SIZE * sizeof(FloatType));
}

#if 0

///////////////////
// Test code: Audio conversion helpers: "differential <--> linear"
//
// This code tests the conversion of pcm data to linear data against its inverse operation.
// The error between the original pcm samples and their conversion to the differential signal
// and back to the pcm representation should be very small.
//
///////////////////

#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>

template<typename FloatType>
void TestPcmDifferentialConversionFloatingPoint()
{
    FloatType pcmSamples    [ED1AudioFrameSampleCount];
    FloatType encoderHistory[PREDICTOR_SIZE];

    FloatType diffSamples   [ED1AudioFrameSampleCount];
    FloatType decoderHistory[PREDICTOR_SIZE];

    // undo operation above:
    FloatType pcmSamplesAfterDecoding[ED1AudioFrameSampleCount];

    // initialisation
    {
        for(unsigned int idx = 0; idx < PREDICTOR_SIZE;++idx)
        {
            encoderHistory[idx] = static_cast<FloatType>(0.0);
            decoderHistory[idx] = static_cast<FloatType>(0.0);
        }
    }

    const int MaxFrameTests = 100000;
    FloatType errorAbsMax  = 0;
    FloatType sampleAbsMax = 0;

     srand( (unsigned)time( NULL ) );

    for(int frameIdx = 0; frameIdx < MaxFrameTests; ++frameIdx)
    {
        // initialize input samples
        for(unsigned int pcmIdx = 0; pcmIdx < ED1AudioFrameSampleCount; ++pcmIdx)
        {
            if( frameIdx == 0 )
            {
                pcmSamples[pcmIdx] = //static_cast<FloatType>(1.0 / (1.0 + pcmIdx));
                                     //static_cast<FloatType>(0.001 * (1.0 + pcmIdx));
                                     static_cast<FloatType>(1.0 - 2.0 * rand() / static_cast<FloatType>(RAND_MAX));
            }
            else
            {
                pcmSamples[pcmIdx] = //static_cast<FloatType>(1.0 / (418.0 - pcmIdx));
                                     //static_cast<FloatType>(0.001 * (418.0 - pcmIdx));
                                     static_cast<FloatType>(1.0 - 2.0 * rand() / static_cast<FloatType>(RAND_MAX));
            }
        }

        GetDifferentialSignalFloatingPoint(diffSamples   , pcmSamples , encoderHistory);
        GetPcmSignalFloatingPoint(pcmSamplesAfterDecoding, diffSamples, decoderHistory);

        //for(unsigned int idx = 0; idx < ED1AudioFrameSampleCount; ++idx)
        //{
        //    FloatType errorAbs = pcmSamplesAfterDecoding[idx] - pcmSamples[idx];

        //    if( fabs(pcmSamples[idx]) > 1.0E-4 )
        //    {
        //        FloatType errorRel = static_cast<FloatType>(100.0 * errorAbs / pcmSamples[idx]);
        //        if      ( errorRel > 100 )
        //        {
        //            errorRel = 100;
        //        }
        //        else if ( errorRel < -100 )
        //        {
        //            errorRel = -100;
        //        }

        //        printf("%10.7f%8.2f %c", errorAbs, errorRel, '%' );
        //    }
        //    else
        //    {
        //        printf("%10.7f%10s", errorAbs, "");
        //    }
        //}
        //printf("\n");
        for(unsigned int idx = 0; idx < ED1AudioFrameSampleCount; ++idx)
        {
            FloatType errorAbs = static_cast<FloatType>( fabs(pcmSamplesAfterDecoding[idx] - pcmSamples[idx]) );
            if( errorAbs > errorAbsMax )
            {
                errorAbsMax = errorAbs;
            }

            FloatType sample = static_cast<FloatType>( pcmSamples[idx] );
            if( fabs(sample) > sampleAbsMax )
            {
                sampleAbsMax = fabs(sample);
            }
        }
    }

    if( sampleAbsMax > 0 )
    {
        long double maxErrorAbsFullScale = errorAbsMax / sampleAbsMax;
        if( maxErrorAbsFullScale > 0 )
        {
            printf("Max differential / linear conversion error = %.2f [dBFs]\n", 20.0 * log10(maxErrorAbsFullScale));
        }
        else
        {
            assert(false);
        }
    }
}

// test code usage:
void VPAUDIOCORE_PUBLIC TestPcmDifferentialConversion()
{
    TestPcmDifferentialConversionFloatingPoint<float>();
    //TestPcmDifferentialConversionFloatingPoint<double>();
    //TestPcmDifferentialConversionFloatingPoint<long double>();
    //return 0;
}

#endif

///////////////////////////////////////////////////////////////////////////////////////////////
// Audio conversion helpers: scaling of the differential signal
///////////////////////////////////////////////////////////////////////////////////////////////

template<typename FloatType>
FloatType GetAbsMaxValue(FloatType aVector[], unsigned int aElements)
{
    FloatType max = static_cast<FloatType>(0.0);

    // get maximum absolute sample value (and store the result in 'max')
    {
        for(unsigned int idx = 0; idx < aElements; ++idx)
        {
            FloatType absSample = aVector[idx];
            if( absSample < 0 )
            {
                absSample = -absSample;
            }

            if( absSample > max )
            {
                max = absSample;
            }
        }
    }

    return max;
}

// aDiffSignalInOut [in+out]:   on input: unscaled differential audio vector on input
//                              on output: scaled differential audio vector on output.
//                              ED1AudioFrameSampleCount elements
//
// aUsedScaleFactorIdx [out]:   the used scale factor index (see GetScaleFactorCoefficients).
//                              Range=0,1,..,SCALE_FACTOR_COUNT-1
template<typename FloatType>
void ScaleDifferentialSignalFloatingPoint(FloatType aDiffSignalInOut[], unsigned int& aUsedScaleFactorIdx)
{
    FloatType           max               = GetAbsMaxValue(aDiffSignalInOut, ED1AudioFrameSampleCount);
    const FloatType*    scaleFactorCoeffs = GetScaleFactorCoefficients<FloatType>();

    // find the scale factor (could be optimized. however this is irrelevant here)
    unsigned int scaleFactorIdx = SCALE_FACTOR_COUNT - 1;
    while( (scaleFactorIdx > 0) && (scaleFactorCoeffs[scaleFactorIdx] <= max) )
    {
        --scaleFactorIdx;
    }

    // scale the samples and store the used scale factor index
    {
        FloatType scaleFactor = static_cast<FloatType>( static_cast<FloatType>(1.0) / scaleFactorCoeffs[scaleFactorIdx] );
        for(unsigned int idx = 0; idx < ED1AudioFrameSampleCount; ++idx)
        {
            aDiffSignalInOut[idx] *= scaleFactor;
        }
        aUsedScaleFactorIdx = scaleFactorIdx;
    }
}

// aDiffSignalInOut [in+out]:   on input: unscaled differential audio vector on input
//                              on output: scaled differential audio vector on output.
//                              ED1AudioFrameSampleCount elements
// aUsedScaleFactorIdx [in]:    the scale factor index to use for descaling. Range=0,1,..,SCALE_FACTOR_COUNT-1
template<typename FloatType>
void UnscaleDifferentialSignalFloatingPoint(FloatType aDiffSignalInOut[], unsigned int aUsedScaleFactorIdx)
{
    const FloatType*    scaleFactorCoeffs   = GetScaleFactorCoefficients<FloatType>();
    FloatType           scaleFactor         = scaleFactorCoeffs[aUsedScaleFactorIdx];
    for(unsigned int idx = 0; idx < ED1AudioFrameSampleCount; ++idx)
    {
        aDiffSignalInOut[idx] *= scaleFactor;
    }
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

// Note: the caller is responsible (!) for scaling the input samples
//
// aAdpcmOut: contains ED1AudioFrameSampleCount adpcm output samples.
//            each sample is in the range -2^(ADPCM_BIT_COUNT-1),..,0,1,..,2^(ADPCM_BIT_COUNT-1)-1
//
// aAdpcmIn : adpcm input samples in floating point format
template<typename FloatType>
void AdpcmToIntFromScaledFloatingPoint(int_fast16_t aAdpcmOut[], const FloatType aAdpcmIn[])
{
    const int_fast16_t MAX_EXPON = ADPCM_BIT_COUNT - 1;
    for(unsigned int idx = 0; idx < ED1AudioFrameSampleCount; ++idx)
    {
        // convert floating point sample to ADPCM_BIT_COUNT integer sample (with rounding)
        int_fast16_t sample = static_cast<int_fast16_t>( (1 << (MAX_EXPON + 1)) * aAdpcmIn[idx] );

        // round sample (--> +1 (means 0.5) and divide by 2 (== arithmetic right shift)
        // (rounding avoids a small negative constant component)
        sample  += 1;
        sample >>= 1;

        if      ( sample >= (1 << MAX_EXPON) )
        {
            // note: this case (sample ==  (1 << MAX_EXPON)) happens in reality, even if
            //       the absolute value of all pcm audio samples are smaller than the max-level (=0.513005).
            //       however, the sample must never be greater than (1 << MAX_EXPON)
          
            aAdpcmOut[idx] = (1 << MAX_EXPON) - 1;
        }
        else if ( sample < -(1 << MAX_EXPON) )
        {
            // this case should never happen if the maximum of all pcm audio samples are smaller than
            // the maximum allowed value (=0.513005)
            aAdpcmOut[idx] =  -(1 << MAX_EXPON);
        }
        else
        {
            aAdpcmOut[idx] = sample;
        }
    }
}

// aAdpcmOut :  adpcm output samples in floating point format
//
// aAdpcmIn:    contains ED1AudioFrameSampleCount adpcm input samples.
//              Each sample is in the range -2^(ADPCM_BIT_COUNT-1),..,0,1,..,2^(ADPCM_BIT_COUNT-1)-1
template<typename FloatType>
void AdpcmToScaledFloatingPointFromInt(FloatType aAdpcmOut[], const int_fast16_t aAdpcmIn[])
{
    FloatType factor = static_cast<FloatType>(1.0) / static_cast<FloatType>(1 << (ADPCM_BIT_COUNT-1));
    for(unsigned int idx = 0; idx < ED1AudioFrameSampleCount; ++idx)
    {
        aAdpcmOut[idx] = factor * static_cast<FloatType>(aAdpcmIn[idx]) ;
    }
}

///////////////////////////////////////////////////////

void Save32BitValueBigEndian(uint8_t* apMemory, uint_fast32_t aValue)
{
    apMemory[0] = static_cast<uint8_t>(aValue >> 24);
    apMemory[1] = static_cast<uint8_t>(aValue >> 16);
    apMemory[2] = static_cast<uint8_t>(aValue >>  8);
    apMemory[3] = static_cast<uint8_t>(aValue      );
}

uint_fast32_t Get32BitValueFromBigEndian(const uint8_t* apMemory)
{
    return      static_cast<uint_fast32_t>((uint_fast32_t)apMemory[0] << 24)
            |   static_cast<uint_fast32_t>((uint_fast32_t)apMemory[1] << 16)
            |   static_cast<uint_fast32_t>((uint_fast32_t)apMemory[2] <<  8)
            |   static_cast<uint_fast32_t>((uint_fast32_t)apMemory[3]      );
}

uint8_t* Save32BitPairBigEndian(uint8_t* apMemory, uint_fast32_t& checkSum, uint_fast32_t aValue1, uint_fast32_t aValue2)
{
    Save32BitValueBigEndian(&apMemory[0], aValue1);
    Save32BitValueBigEndian(&apMemory[4], aValue2);

    checkSum += aValue1;
    checkSum += aValue2;

    return apMemory + 8;
}

// aAdpcmOut:               contains ED1AudioFrameEncodedByteCount bytes
//                          (=484 bytes=4 bytes header + 4 bytes scalefactor + 472 bytes adpcm encoded audio + 4 bytes checksum)
//
// aPcmSamplesIn:           input audio vector (linear=pcm, ED1AudioFrameSampleCount elements)
//
// aHistory [in/out]:       contains PREDICTOR_SIZE elements. all elements must be zero if calling this routine the 1st time
void AdpcmToByteRepresentationFromInt(uint8_t aAdpcmBytesOut[], const int_fast16_t aAdpcmIntsIn[], unsigned int aScaleFactorIdx)

{
    static_assert( (ED1AudioFrameEncodedByteCount % 4) == 0, "The encoded audio size must be divisible by 4");

    const unsigned int numAudioValuesEncodedAs32Bit = ED1AudioFrameEncodedByteCount / 4 - 3; // -3 = header + scale factor + check sum

    static_assert(  (numAudioValuesEncodedAs32Bit % 2) == 0, "The encoding requires two fold parallel processing (SIMD).");

    const unsigned int BITS_PER_HALF_FRAME = (numAudioValuesEncodedAs32Bit / 2) * 32;

    static_assert( BITS_PER_HALF_FRAME / ADPCM_BIT_COUNT == ED1AudioFrameSampleCount / 2,
                    "The encoded audio size in bytes does not correspond to the number of samples per audio frame");

    uint8_t*        pOutput  = &aAdpcmBytesOut[0];
    uint_fast32_t   checkSum = 0;

    // save frame header + scale factor index
    pOutput = Save32BitPairBigEndian(pOutput,
                                     checkSum,

                                     ENCODED_AUDIO_HEADER,
                                     aScaleFactorIdx

                                     );

    unsigned int   bitPos  = 0;
    uint_fast32_t  valEven = 0;
    uint_fast32_t  valOdd  = 0;
    for(unsigned int inputIdx = 0; inputIdx < ED1AudioFrameSampleCount / 2; ++inputIdx)
    {
        // warning: temp must be an unsigned type (logical right shift required, not(!) arithmetical)
        uint32_t tempEven;
        uint32_t tempOdd ;

        tempEven   = static_cast<uint32_t>( aAdpcmIntsIn[2*inputIdx + 0] );
        tempOdd    = static_cast<uint32_t>( aAdpcmIntsIn[2*inputIdx + 1] );

        tempEven <<= 32-ADPCM_BIT_COUNT;
        tempOdd  <<= 32-ADPCM_BIT_COUNT;

        valEven   |= tempEven >> bitPos;
        valOdd    |= tempOdd  >> bitPos;

        bitPos    += ADPCM_BIT_COUNT;

        if ( bitPos >= 32 )
        {
            // keep bits of 'valEven' and 'valOdd'
            pOutput = Save32BitPairBigEndian(pOutput, checkSum,   valEven, valOdd);

            // wrap around (== modulo 32)
            bitPos  &= 31;
          
            valEven  = tempEven << (ADPCM_BIT_COUNT - bitPos);
            valOdd   = tempOdd  << (ADPCM_BIT_COUNT - bitPos);
        }
    }
    if( bitPos > 0 )
    {
        // flush trailing audio bits
        pOutput = Save32BitPairBigEndian(pOutput, checkSum,   valEven, valOdd);
    }
    Save32BitValueBigEndian(pOutput,   checkSum);
}

// aAdpcmIntsOut:           output audio vector (adpcm, ED1AudioFrameSampleCount elements.
//                          Each sample is in the range -2^(ADPCM_BIT_COUNT-1),..,0,1,..,2^(ADPCM_BIT_COUNT-1)-1)
//
// aScaleFactorIdx [out]:   the used scaled factor for conversion (=0,1,..,SCALE_FACTOR_COUNT-1)
//
// aAdpcmBytesIn:           contains ED1AudioFrameEncodedByteCount bytes
//                          (=484 bytes=4 bytes header + 4 bytes scalefactor + 472 bytes adpcm encoded audio + 4 bytes checksum)
TAdpcmDecodErrCode AdpcmToIntFromByteRepresentation(int_fast16_t    aAdpcmIntsOut[],
                                                    unsigned int&   aScaleFactorIdx,
                                                    const uint8_t   aAdpcmBytesIn[])
{
    int_fast32_t encodedAudio[ED1AudioFrameEncodedByteCount / 4];

    // get validated int_fast32_t representation of the encoded audio data
    {
        int_fast32_t checkSum = 0;

        for(unsigned int encodedAudioIdx = 0; encodedAudioIdx < ED1AudioFrameEncodedByteCount / 4; ++encodedAudioIdx)
        {
            encodedAudio[encodedAudioIdx] = static_cast<int32_t>( Get32BitValueFromBigEndian( &aAdpcmBytesIn[4*encodedAudioIdx] ) );

            checkSum += encodedAudio[encodedAudioIdx];
        }
        checkSum -= encodedAudio[(ED1AudioFrameEncodedByteCount / 4) - 1];
        checkSum  = static_cast<int32_t>( checkSum );

        if( encodedAudio[(ED1AudioFrameEncodedByteCount / 4) - 1] != checkSum )
        {
            return EAdpcmDecodErr_InvalidCheckSum;
        }

        // validate header
        static_assert( sizeof(encodedAudio[0]) == sizeof(ENCODED_AUDIO_HEADER), "invalid size" );
        if( static_cast<int32_t>(encodedAudio[0]) != static_cast<int32_t>(ENCODED_AUDIO_HEADER) )
        {
            return EAdpcmDecodErr_InvalidHeader;
        }

        if( (encodedAudio[1] < 0)  || (SCALE_FACTOR_COUNT <= encodedAudio[1]) )
        {
            return EAdpcmDecodErr_InvalidScaleFactor;
        }

        aScaleFactorIdx = static_cast<unsigned int>( encodedAudio[1] );
    }

    const unsigned int AUDIO_VALUES_COUNT = (ED1AudioFrameEncodedByteCount / 4) - 3;
    static_assert( (AUDIO_VALUES_COUNT % 2) == 0, "The number of audio values must be divisible by 2.");

    static_assert( sizeof(encodedAudio[0]) == sizeof(int_fast32_t), "Cast requirement check." );
    const int_fast32_t*     pAudioSigned = &encodedAudio[2];

    unsigned int            idxOut       =  0;
    int                     bitsLeft     =  0;

    // the type must (!) be a signed type, because an arithmetic right shift is intended
    int32_t valEven = 0;
    int32_t valOdd  = 0;
    for(unsigned int audioLongIdx = 0; audioLongIdx < AUDIO_VALUES_COUNT / 2; ++audioLongIdx)
    {
        if( bitsLeft > 0 )
        {
            // get carry bits. note: this case requires a logical right shift (!) --> use unsigned type
            valEven |= static_cast<uint32_t>(pAudioSigned[2*audioLongIdx + 0]) >> bitsLeft;
            valOdd  |= static_cast<uint32_t>(pAudioSigned[2*audioLongIdx + 1]) >> bitsLeft;

            aAdpcmIntsOut[2*idxOut + 0] = static_cast<int16_t>(valEven >> (32-ADPCM_BIT_COUNT));
            aAdpcmIntsOut[2*idxOut + 1] = static_cast<int16_t>(valOdd  >> (32-ADPCM_BIT_COUNT));

            ++idxOut;
            bitsLeft -= ADPCM_BIT_COUNT;
        }

        bitsLeft += 32;
        valEven   = static_cast<int32_t>( pAudioSigned[2*audioLongIdx + 0] << (32-bitsLeft) );
        valOdd    = static_cast<int32_t>( pAudioSigned[2*audioLongIdx + 1] << (32-bitsLeft) );

        do
        {
            static_assert( ADPCM_BIT_COUNT < 8 * sizeof(int), "The algorithm does not work with this ADPCM bit count." );

            // note: arithmetic right shift required (!)
            aAdpcmIntsOut[2*idxOut + 0] = static_cast<int16_t>(valEven >> (32-ADPCM_BIT_COUNT));
            aAdpcmIntsOut[2*idxOut + 1] = static_cast<int16_t>(valOdd  >> (32-ADPCM_BIT_COUNT));

            ++idxOut;
            bitsLeft -= ADPCM_BIT_COUNT;

            valEven <<= ADPCM_BIT_COUNT;
            valOdd  <<= ADPCM_BIT_COUNT;

        } while( bitsLeft >= ADPCM_BIT_COUNT );
    }
    return EAdpcmDecodErrCode_Success;
}

///////////////////////////////////////////////////////

// to avoid audio overruns/clipping the absolute value of each sample must be smaller than 0.513005
//
// NOTE: 0.513005 = (sum of predictor coefficients + 1.0) / 2.0
// +1.0 because of the substraction of the estimated sample from the current sample (implicit/hard coded coefficient)
// 2.0 = maximum scale factor
//
// aAdpcmOut:               raw adpcm output buffer (contains 484 bytes:
//                                  =       4 bytes header
//                                      +   4 bytes scalefactor
//                                      + 472 bytes adpcm encoded audio
//                                      +   4 bytes checksum)
//
// aPcmSamplesIn:           input audio vector (linear=pcm, ED1AudioFrameSampleCount elements)
//
// aHistory [in/out]:       contains PREDICTOR_SIZE elements. all elements must be zero if calling this routine the 1st time
template<typename FloatType>
void Pcm2Adpcm(uint8_t aAdpcmOut[], const FloatType aPcmSamplesIn[], FloatType aEncoderHistory[])
{
    FloatType diffSamples[ED1AudioFrameSampleCount];
    GetDifferentialSignalFloatingPoint(diffSamples, aPcmSamplesIn, aEncoderHistory);

    // scale the differential signal and get the used scale factor index
    unsigned int scaleFactorIdx;
    ScaleDifferentialSignalFloatingPoint(diffSamples, scaleFactorIdx);

    // convert the differential signal to 9-bit integer format
    int_fast16_t adpcmAs9BitInt[ED1AudioFrameSampleCount];
    AdpcmToIntFromScaledFloatingPoint(adpcmAs9BitInt, diffSamples);

    // convert integer format to packed byte format
    AdpcmToByteRepresentationFromInt(aAdpcmOut, adpcmAs9BitInt, scaleFactorIdx);
}

// aPcmSamplesOut:         output audio vector (linear=pcm, ED1AudioFrameSampleCount elements)
//
// aAdpcmIn:               raw adpcm input buffer (contains 484 bytes:
//                                  =       4 bytes header
//                                      +   4 bytes scalefactor
//                                      + 472 bytes adpcm encoded audio
//                                      +   4 bytes checksum)
//
// aDecoderHistory [in/out]: contains PREDICTOR_SIZE elements. all elements must be zero if calling this routine the 1st time
//
// note: the output vector does not change if the input buffer does not contain valid audio data
template<typename FloatType>
TAdpcmDecodErrCode Adpcm2Pcm(FloatType aPcmSamplesOut[], const uint8_t aAdpcmRawIn[], FloatType aDecoderHistory[])
{
    // get 9-bit integer format (differential signal) from adpcm raw representation

    int_fast16_t    adpcmAs9BitInt[ED1AudioFrameSampleCount];
    unsigned int    adpcmScaleFactorIdx;

    TAdpcmDecodErrCode errCode;

    errCode = AdpcmToIntFromByteRepresentation(adpcmAs9BitInt, adpcmScaleFactorIdx, aAdpcmRawIn);
    if( errCode == EAdpcmDecodErrCode_Success )
    {
        // get floating point representation from 9-bit integer
        FloatType diffSamples[ED1AudioFrameSampleCount];

        AdpcmToScaledFloatingPointFromInt     (diffSamples, adpcmAs9BitInt     );
        UnscaleDifferentialSignalFloatingPoint(diffSamples, adpcmScaleFactorIdx);

        //
        GetPcmSignalFloatingPoint(aPcmSamplesOut, diffSamples, aDecoderHistory);
    }
    else
    {
        // nothing to do
    }

    return errCode;
}

} // namespace {

///////////////////////////////////////////////////////
// interface implementation
///////////////////////////////////////////////////////

namespace VpAudioCore {

D1AudioAdpcmEncoder::D1AudioAdpcmEncoder()
{
    Reset();
}

D1AudioAdpcmEncoder::~D1AudioAdpcmEncoder()
{
}

void D1AudioAdpcmEncoder::Encode(uint8_t aEncodedAudioOut[], const TAudioSample aInputSamples[])
{
    AUDIO_CODER::Pcm2Adpcm<float>(aEncodedAudioOut, aInputSamples, m_history);
}

void D1AudioAdpcmEncoder::Reset()
{
    for(size_t idx = 0; idx < EHistorySampleCount; ++idx)
    {
        m_history[idx] = static_cast<TAudioSample>( 0 );
    }
}

//////////////////////////////////////

D1AudioAdpcmDecoder::D1AudioAdpcmDecoder()
{
    Reset();
}

D1AudioAdpcmDecoder::~D1AudioAdpcmDecoder()
{
}

bool D1AudioAdpcmDecoder::DecodeOk(TAudioSample aOutputSamples[], const uint8_t aEncodedAudioIn[])
{
    return AUDIO_CODER::Adpcm2Pcm<float>(aOutputSamples, aEncodedAudioIn, m_history) == EAdpcmDecodErrCode_Success;
}

void D1AudioAdpcmDecoder::Reset()
{
    for(size_t idx = 0; idx < EHistorySampleCount; ++idx)
    {
        m_history[idx] = static_cast<TAudioSample>( 0 );
    }
}

#if 0

#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>

//////
// initial decoding algorithm
//////

#if ! defined(NDEBUG)
#define VERIFY(x) assert(x)
#else
#define VERIFY(x) x
#endif

#define EAdpcmFrameSizeInSamples 418        //even number !!!!
#define PREDICTOR_SIZE 4
#define FRAME_WORDS 120            //Numof 32bit words per ADPCM-frame
#define MASK 0xff800000            //Mask N_BITS MSBs from every Sample
#define N_BITS 9                //Numof bits per Sample

typedef unsigned char byte;

static const double scalefactors[63] = {
    2.00000000000000,
    1.58740105196820,
    1.25992104989487,
    1.00000000000000,
    0.79370052598410,
    0.62996052494744,
    0.50000000000000,
    0.39685026299205,
    0.31498026247372,
    0.25000000000000,
    0.19842513149602,
    0.15749013123686,
    0.12500000000000,
    0.09921256574801,
    0.07874506561843,
    0.06250000000000,
    0.04960628287401,
    0.03937253280921,
    0.03125000000000,
    0.02480314143700,
    0.01968626640461,
    0.01562500000000,
    0.01240157071850,
    0.00984313320230,
    0.00781250000000,
    0.00620078535925,
    0.00492156660115,
    0.00390625000000,
    0.00310039267963,
    0.00246078330058,
    0.00195312500000,
    0.00155019633981,
    0.00123039165029,
    0.00097656250000,
    0.00077509816991,
    0.00061519582514,
    0.00048828125000,
    0.00038754908495,
    0.00030759791257,
    0.00024414062500,
    0.00019377454248,
    0.00015379895629,
    0.00012207031250,
    0.00009688727124,
    0.00007689947814,
    0.00006103515625,
    0.00004844363562,
    0.00003844973907,
    0.00003051757813,
    0.00002422181781,
    0.00001922486954,
    0.00001525878906,
    0.00001211090890,
    0.00000961243477,
    0.00000762939453,
    0.00000605545445,
    0.00000480621738,
    0.00000381469727,
    0.00000302772723,
    0.00000240310869,
    0.00000190734863,
    0.00000151386361,
    0.00000120155435
};

static const double ap[PREDICTOR_SIZE] = {1.2315, -0.7297, 0.5863, -0.3511};

static void decodeAdpcmFrame(TAudioSample aAudioBufferOut[], const uint8_t aAdpcmData[])
{
    double x_d;
    double e_q[EAdpcmFrameSizeInSamples];
    long int e_q_int[EAdpcmFrameSizeInSamples];
    static double x_q[PREDICTOR_SIZE] = {0.0, 0.0, 0.0, 0.0};

    unsigned long int   frame       [FRAME_WORDS + 1];
    byte                frame_byte  [FRAME_WORDS*4 + 4];
    int i, s, read_pos;
    unsigned long int tnew_byte;


    //read one frame of adpcm data
    memcpy(frame_byte, aAdpcmData, sizeof(frame_byte));

    // initialize frame
    for(i=0; i<FRAME_WORDS + 1; i++) {
        frame[i] = 0;
    }

    //read from frame_byte 32-bit wide into frame
    for (s=0; s<FRAME_WORDS + 1; s++) {
        for (i=0; i<4; i++) {
            tnew_byte = (frame_byte[4*s+i])<<(8*(3-i));
            //new_byte = (frame_byte[4*s+i])<<(8*(i));
            frame[s] |= tnew_byte;
        }
    }

    // control checksum
#if 1
    unsigned long checksum = 0;
    for (s=0; s<FRAME_WORDS/*-1*/; s++) {
        checksum += frame[s];
    }
    if (checksum != frame[FRAME_WORDS/*-1*/])
    {
        return ;
    }
#endif

    // read adpcm-Samples into e_q_int (9 bit, *** SIMD !!! ****)
    // hint: The frame buffer 'frame' contains 2 bit streams
    // one stream is stored in odd indexed values. the other stream uses even indices.
    // the 1st value of the frame is a dummy. the 2nd one an index to a table.
    // thus the array contains two bitstreams with (120-2)/2 * 32 bits = 59*8 bits = 472 bits each
    read_pos = 0;        //bit position in 32-bit word to write next sample
    i = 2;                // index into (adpcm)frame
    for (s=0; s<EAdpcmFrameSizeInSamples; s+=2)
    {
        e_q_int[s  ] = (frame[i  ] << read_pos) & MASK;
        e_q_int[s+1] = (frame[i+1] << read_pos) & MASK;
        if ( (read_pos + (N_BITS - 1)) > 31)
        {
            // add carry flags
            i += 2;
            e_q_int[s  ] |= (frame[i  ] >> (32-read_pos)) & MASK;
            e_q_int[s+1] |= (frame[i+1] >> (32-read_pos)) & MASK;
        }
        if ( (read_pos = (read_pos + N_BITS) % 32) == 0)
        {
            i += 2;
        }
    }

    // convert e_q_int to float e_q and divide by scalefactor
    i = frame[1];    //scf-index
    for (s=0; s<EAdpcmFrameSizeInSamples; s++) {
        e_q[s] = e_q_int[s];
        e_q[s] /= 2147483648.0;
        if( (i < 63) && (i >= 0) )
        {
            e_q[s] *= scalefactors[i];
        }
        else
        {
            // ERROR
            return ;
        }
    }

    // calculate output Sample x_q[0]
    for (s=0; s<EAdpcmFrameSizeInSamples; s++) {
        x_d = 0;
        for (i=0; i<PREDICTOR_SIZE; i++) {
            x_d += ap[i] * x_q[i];
        }

        /*
        for (i=0; i<PREDICTOR_SIZE-1; i++) {
            x_q[i+1] = x_q[i];
        }
        */
        for (i=PREDICTOR_SIZE-1; i>0; i--) {
            x_q[i] = x_q[i-1];
        }


        x_q[0] = x_d + e_q[s];

        aAudioBufferOut[s] = (float)x_q[0];
    }

    //return retval;
}

void VPAUDIOCORE_PUBLIC TestD1AudioCoding()
{
    const bool testAgainstReferenceCode = false;

    TAudioSample    pcmSamples  [ED1AudioFrameSampleCount    ];
    uint8_t         adpcmSamples[ED1AudioFrameEncodedByteCount];

    // undo operation above:
    TAudioSample   pcmSamplesAfterDecoding1[ED1AudioFrameSampleCount];
    TAudioSample   pcmSamplesAfterDecoding2[ED1AudioFrameSampleCount];


    D1AudioAdpcmEncoder encoder;
    D1AudioAdpcmDecoder decoder;

    const int MaxFrameTests = 500;

    //srand( (unsigned)time( NULL ) );
    const double minDecibel = -200.0;
    double maxDecibel = minDecibel;
    for(int frameIdx = 0; frameIdx < MaxFrameTests; ++frameIdx)
    {
        //double dBval = -110.0 * static_cast<double>(rand() / static_cast<TAudioSample>(RAND_MAX));
        //double factor = pow(10.0, dBval / 20.0);

        double factor = 1.0;

        // initialize input samples
        for(unsigned int pcmIdx = 0; pcmIdx < ED1AudioFrameSampleCount; ++pcmIdx)
        {
            pcmSamples[pcmIdx] = static_cast<TAudioSample>(1.0 - 2.0 * rand() / static_cast<TAudioSample>(RAND_MAX));

            pcmSamples[pcmIdx] *= static_cast<TAudioSample>(factor * 0.5f);
        }


                encoder.Encode   (adpcmSamples            , pcmSamples  )  ;
        VERIFY( decoder.DecodeOk (pcmSamplesAfterDecoding1, adpcmSamples) );

        decodeAdpcmFrame (pcmSamplesAfterDecoding2, adpcmSamples);

        //Pcm2Adpcm(adpcmSamples, scaleFactorIdx, pcmSamples, encoderHistory);

        TAudioSample maxAbsError = 0;
        for(unsigned int idx = 0; idx < ED1AudioFrameSampleCount; ++idx)
        {
            TAudioSample errorAbs;
            if( testAgainstReferenceCode  )
            {
                errorAbs = static_cast<TAudioSample>( fabs(pcmSamplesAfterDecoding1[idx] - pcmSamplesAfterDecoding2[idx]) );
            }
            else
            {
                errorAbs = static_cast<TAudioSample>( fabs(pcmSamplesAfterDecoding1[idx] - pcmSamples[idx]) );
            }

            if( errorAbs > maxAbsError )
            {
                maxAbsError = errorAbs;
            }
        }

        double dBFs;
        if( (maxAbsError > 1.0E-5) )
        {
            dBFs = 20.0 * log10( maxAbsError );
            if( dBFs > maxDecibel )
            {
                maxDecibel = dBFs;
            }

            printf("%10.2f", dBFs );
        }
        else
        {
            printf("%10s", "< -100" );
        }
    }

    printf("\n\n");
    printf("Maximum total = %9.2f [dBFs]\n", maxDecibel );
    if( (maxDecibel <= minDecibel) && testAgainstReferenceCode )
    {
        printf("The reference decoding algorithm does not differ from the current decoding implementation.\n");
    }
}

#endif

} // namespace VpAudioCore {

