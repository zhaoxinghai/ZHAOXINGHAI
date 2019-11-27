
#ifndef SHARE_MEM_H
#define SHARE_MEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

#ifdef WIN32
#include <Windows.h>
#else
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/sem.h>
#endif

namespace TEST {

class CShareMem
{
public:
    CShareMem();
    ~CShareMem();

    bool Open();

    unsigned char   *m_pBuffer;

#ifdef WIN32
    HANDLE           m_hMapFile;
    LPVOID           m_pMapBuf;
    HANDLE           m_hMutex;
#else
    int              m_hMapFile;
    void*            m_pMapBuf;
    int              m_sem_mutex_id;
#endif
};


class CShareLock
{
public:
    CShareLock(CShareMem* pShare);
    ~CShareLock();
private:
    CShareMem*  m_pShare;
};

union semun
{
    int              val;
    struct semid_ds *buf;
    unsigned short  *array;
    struct seminfo  *__buf;
};

}  //namespace

#endif
