

#ifndef _NOISE_H_
#define _NOISE_H_

class CNoise
{
public:
    CNoise();
    virtual ~CNoise();

    void Init();
    void Exit();

    void Reduce(unsigned char *pInData,int nFrame,short nNoise);

    void Reduce2(unsigned char *pInData);

protected:

#ifndef _MSC_VER
    SpeexPreprocessState *m_SpeexState;
#endif

};

#endif
