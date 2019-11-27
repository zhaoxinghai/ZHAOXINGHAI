
#ifndef MEM_CONTENT_H
#define MEM_CONTENT_H

#ifndef WIN32
#include <pthread.h>
#endif

namespace TEST {

#pragma pack(8)
struct CMemContent
{
    void Init();

    int  m_Test;
};

} //namespace
#endif
