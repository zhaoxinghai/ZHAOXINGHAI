
#ifndef CYCLE_BUFFER_H  
#define CYCLE_BUFFER_H  

#include <memory>
#include <vector>

class CCycleBuffer  
{  
public:  
    CCycleBuffer();  
    virtual ~CCycleBuffer();  

    void SetSize(int size = 1024 * 1024);

    virtual int write(unsigned char* buf, int count);
    virtual int read(unsigned char* buf, int count);

    virtual int getUsedSize();
    virtual int getFreeSize();

    void setEmpty(); 

private: 
    bool isFull();  
    bool isEmpty();  
    

    bool m_bEmpty;  
    bool m_bFull;  

    char * m_pBuf;  
    int m_nBufSize;  
    int m_nReadPos;  
    int m_nWritePos;  
}; 

#endif // CCycleBuffer_H  
