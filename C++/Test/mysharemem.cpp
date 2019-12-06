
#include "mysharemem.h"
#include "memcontent.h"

#ifdef WIN32
#define M_NAME L"sharememory"
#define MUTEX_NAME L"sharemutex"
#define EVENT_NAME L"waitevent"
#else
#define SHM_KEY       123
#define SEM_KEY       1234
#define SEM_KEY_WAIT  2345
#endif

namespace TEST {

CShareMem::CShareMem()
{
#ifdef WIN32
    m_hMapFile = INVALID_HANDLE_VALUE;
#else
    m_hMapFile = 0;
#endif
    m_pMapBuf = NULL;
    m_pBuffer = NULL;
}

CShareMem::~CShareMem()
{
#ifdef WIN32
    if(m_pMapBuf != NULL)
    {
        UnmapViewOfFile(m_pMapBuf);
    }
    CloseHandle(m_hMapFile);
    CloseHandle(m_hMutex);
#else
    if(m_pMapBuf != NULL)
    {
        shmdt(m_pMapBuf);
    }
    shmctl(m_hMapFile, IPC_RMID, 0);
#endif

    m_pBuffer = NULL;
}

bool CShareMem::Open()
{
#ifdef WIN32
    //share memory
    m_hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, NULL, M_NAME);
    if(m_hMapFile == NULL)
    {
        m_hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,
                                       0,sizeof(CMemContent),M_NAME);
    }
    if(m_hMapFile == INVALID_HANDLE_VALUE)
    {
        printf("OpenFileMapping error");
        return false;
    }
    m_pMapBuf = MapViewOfFile(m_hMapFile,FILE_MAP_ALL_ACCESS,0,0,sizeof(CMemContent));

    //mutex
    m_hMutex = CreateMutex(NULL,FALSE,MUTEX_NAME);

    //event
    m_hEvent = CreateEvent(NULL, FALSE, FALSE, EVENT_NAME);
#else
    //share memory
    key_t key=ftok("/tmp",SHM_KEY);
    if(key < 0)
    {
        perror("get key error\n");
        return -1;
    }
    m_hMapFile = shmget(key, sizeof(CMemContent), IPC_CREAT);
    if (m_hMapFile < 0)
    {
        m_hMapFile = shmget(key, sizeof(CMemContent), IPC_CREAT | IPC_EXCL | 0666);
        if(m_hMapFile < 0)
        {
            perror("shmget error!");
            return false;
        }
    }       
    m_pMapBuf = shmat(m_hMapFile, 0, 0);

    //semaphore
    m_sem_mutex_id = semget((key_t)SEM_KEY,1,IPC_CREAT | IPC_EXCL| 0600);
    if(m_sem_mutex_id == -1)
    {
        m_sem_mutex_id = semget((key_t)SEM_KEY,1,IPC_CREAT | 0600);
        if(m_sem_mutex_id == -1)
        {
            printf("semget error");
        }
    }
    else
    {
        union semun a;
        a.val = 1;
        if(semctl(m_sem_mutex_id,0,SETVAL,a)==-1)
        {
            perror("semctl init error");
        }
    }

    //semaphore wait
    m_sem_wait_id = semget((key_t)SEM_KEY_WAIT,1,IPC_CREAT | IPC_EXCL| 0600);
    if(m_sem_wait_id == -1)
    {
        m_sem_wait_id = semget((key_t)SEM_KEY_WAIT,1,IPC_CREAT | 0600);
        if(m_sem_wait_id == -1)
        {
            printf("semget error");
        }
    }
    else
    {
        union semun a;
        a.val = 1;
        if(semctl(m_sem_wait_id,0,SETVAL,a)==-1)
        {
            perror("semctl init error");
        }
    }
#endif

    m_pBuffer = (unsigned char*)m_pMapBuf;
    if(m_pBuffer == NULL)
    {
        printf("m_pBuffer == NULL");
        return false;
    }
    return true;
}

void CShareMem::Wait()
{
#ifdef WIN32
    ::WaitForSingleObject(m_hEvent, INFINITE);
#else
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = -1;//p control
    buf.sem_flg = SEM_UNDO;
    if(semop(m_pShare->m_sem_mutex_id,&buf,1)==-1)
    {
       perror("p error");
    }
#endif
}

void CShareMem::Notify()
{
#ifdef WIN32
    SetEvent(m_hEvent);
#else
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = 1;  //v control
    buf.sem_flg = SEM_UNDO;
    if(semop(m_pShare->m_sem_mutex_id,&buf,1)==-1)
    {
        perror("v error");
    }
#endif
}

CShareLock::CShareLock(CShareMem* pShare)
{
    m_pShare = pShare;
#ifdef WIN32
    ::WaitForSingleObject(m_pShare->m_hMutex, INFINITE);
#else
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = -1;//p control
    buf.sem_flg = SEM_UNDO;
    if(semop(m_pShare->m_sem_mutex_id,&buf,1)==-1)
    {
       perror("p error");
    }
#endif
}

CShareLock::~CShareLock()
{
#ifdef WIN32
    ReleaseMutex(m_pShare->m_hMutex);
#else
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = 1;  //v control
    buf.sem_flg = SEM_UNDO;
    if(semop(m_pShare->m_sem_mutex_id,&buf,1)==-1)
    {
        perror("v error");
    }
#endif
}

}//namespace
