
#include "cyclebuffer.h"   
#include <string.h>
#include <assert.h>

CCycleBuffer::CCycleBuffer()    
{
	m_pBuf = NULL;
	m_nReadPos = 0;
	m_nWritePos = 0;
	m_bEmpty = true;
	m_bFull = false;
	m_nBufSize = 0;
}    
CCycleBuffer::~CCycleBuffer()    
{    
    if(m_pBuf != NULL)
    {
        delete [] m_pBuf;
        m_pBuf = NULL;
    }
}   
 
void CCycleBuffer::SetSize(int size)
{
    if(m_pBuf != NULL)
    {
        delete [] m_pBuf;
        m_pBuf = NULL;
    }

    m_nBufSize = size;
    m_nReadPos = 0;
    m_nWritePos = 0;
    m_pBuf = new char[m_nBufSize];
    m_pBuf[0] = '\0';
    m_bEmpty = true;
    m_bFull = false;
}

int CCycleBuffer::write(unsigned char* buf, int count)    
{    
    if (count <= 0)
    {
        return 0;
    }
    m_bEmpty = false;    
   
    if(m_bFull)     
    {    
        return 0;    
    }    
    else if(m_nReadPos == m_nWritePos) //buffer empty
    {    
        int leftcount = m_nBufSize - m_nWritePos;    
        if(leftcount > count)    
        {    
            memcpy(m_pBuf + m_nWritePos, buf, count);    
            m_nWritePos += count;    
            m_bFull = (m_nWritePos == m_nReadPos);    
            return count;    
        }    
        else    
        {    
            memcpy(m_pBuf + m_nWritePos, buf, leftcount);    
            m_nWritePos = (m_nReadPos > count - leftcount) ? count - leftcount : m_nWritePos;    
            memcpy(m_pBuf, buf + leftcount, m_nWritePos);    
            m_bFull = (m_nWritePos == m_nReadPos);    
            return leftcount + m_nWritePos;    
        }    
    }     
    else if(m_nReadPos < m_nWritePos) //have buffer    
    {    
        int leftcount = m_nBufSize - m_nWritePos;  
        if(leftcount > count)
        {    
            memcpy(m_pBuf + m_nWritePos, buf, count);    
            m_nWritePos += count;    
            m_bFull = (m_nReadPos == m_nWritePos);    
            assert(m_nReadPos <= m_nBufSize);    
            assert(m_nWritePos <= m_nBufSize);        
            return count;    
        }    
        else
        {    
            memcpy(m_pBuf + m_nWritePos, buf, leftcount);   
  
            m_nWritePos = (m_nReadPos >= count - leftcount) ? count - leftcount : m_nReadPos;    
            memcpy(m_pBuf, buf + leftcount, m_nWritePos);    
            m_bFull = (m_nReadPos == m_nWritePos);       
            assert(m_nReadPos <= m_nBufSize);    
            assert(m_nWritePos <= m_nBufSize);     
            return leftcount + m_nWritePos;    
        }    
    }    
    else    
    {    
        int leftcount = m_nReadPos - m_nWritePos;    
        if(leftcount > count)    
        {     
            memcpy(m_pBuf + m_nWritePos, buf, count);    
            m_nWritePos += count;    
            m_bFull = (m_nReadPos == m_nWritePos);    
            assert(m_nReadPos <= m_nBufSize);    
            assert(m_nWritePos <= m_nBufSize);     
            return count;    
        }    
        else    
        {
            memcpy(m_pBuf + m_nWritePos, buf, leftcount);    
            m_nWritePos += leftcount;    
            m_bFull = (m_nReadPos == m_nWritePos);    
            assert(m_bFull);    
            assert(m_nReadPos <= m_nBufSize);    
            assert(m_nWritePos <= m_nBufSize);       
            return leftcount;    
        }    
    }    
}    
  
int CCycleBuffer::read(unsigned char* buf, int count)    
{    
    if (count <= 0)
    {
        return 0;
    }
    m_bFull = false;    
    if(m_bEmpty)
    {    
        return 0;    
    }    
    else if(m_nReadPos == m_nWritePos)   
    {    
        int leftcount = m_nBufSize - m_nReadPos;    
        if(leftcount > count)    
        {    
            memcpy(buf, m_pBuf + m_nReadPos, count);    
            m_nReadPos += count;    
            m_bEmpty = (m_nReadPos == m_nWritePos);    
            return count;    
        }    
        else    
        {    
            memcpy(buf, m_pBuf + m_nReadPos, leftcount);    
            m_nReadPos = (m_nWritePos > count - leftcount) ? count - leftcount : m_nWritePos;    
            memcpy(buf + leftcount, m_pBuf, m_nReadPos);    
            m_bEmpty = (m_nReadPos == m_nWritePos);    
            return leftcount + m_nReadPos;    
        }    
    }    
    else if(m_nReadPos < m_nWritePos)
    {      
        int leftcount = m_nWritePos - m_nReadPos;    
        int c = (leftcount > count) ? count : leftcount;    
        memcpy(buf, m_pBuf + m_nReadPos, c);    
        m_nReadPos += c;    
        m_bEmpty = (m_nReadPos == m_nWritePos);    
        assert(m_nReadPos <= m_nBufSize);    
        assert(m_nWritePos <= m_nBufSize);    
        return c;    
    }    
    else
    {     
        int leftcount = m_nBufSize - m_nReadPos;    
        if(leftcount > count)
        {    
            memcpy(buf, m_pBuf + m_nReadPos, count);    
            m_nReadPos += count;    
            m_bEmpty = (m_nReadPos == m_nWritePos);    
            assert(m_nReadPos <= m_nBufSize);    
            assert(m_nWritePos <= m_nBufSize);    
            return count;    
        }    
        else
        {    
            memcpy(buf, m_pBuf + m_nReadPos, leftcount);    
            m_nReadPos = (m_nWritePos >= count - leftcount) ? count - leftcount : m_nWritePos;    
            memcpy(buf + leftcount, m_pBuf, m_nReadPos);    
            m_bEmpty = (m_nReadPos == m_nWritePos);    
            assert(m_nReadPos <= m_nBufSize);    
            assert(m_nWritePos <= m_nBufSize);    
            return leftcount + m_nReadPos;    
        }      
    }    
}   
  
bool CCycleBuffer::isFull()    
{    
    return m_bFull;    
}   

bool CCycleBuffer::isEmpty()    
{    
    return m_bEmpty;    
}    
  
void CCycleBuffer::setEmpty()    
{    
    m_nReadPos = 0;    
    m_nWritePos = 0;    
    m_bEmpty = true;    
    m_bFull = false;    
}    
  
int CCycleBuffer::getUsedSize()    
{    
    if(m_bEmpty)    
    {    
        return 0;    
    }    
    else if(m_bFull)    
    {    
        return m_nBufSize;    
    }    
    else if(m_nReadPos < m_nWritePos)    
    {    
        return m_nWritePos - m_nReadPos;    
    }    
    else    
    {    
        return m_nBufSize - m_nReadPos + m_nWritePos;    
    }    
}  
 
int CCycleBuffer::getFreeSize()   
{    
    if(m_bEmpty)    
    {    
        return m_nBufSize;    
    }    
    else if(m_bFull)    
    {    
        return 0;    
    }    
    else if(m_nReadPos > m_nWritePos)    
    {    
        return m_nReadPos - m_nWritePos;    
    }    
    else    
    {    
        return m_nBufSize - m_nWritePos + m_nReadPos;    
    }    
}
