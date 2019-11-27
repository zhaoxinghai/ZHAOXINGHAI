#ifndef WIN32
#include <sem.h>

namespace TEST {

void CMySem::init()
{
    semid = semget((key_t)1234,1,IPC_CREAT | IPC_EXCL| 0600);
    if(semid == -1)
    {
        semid = semget((key_t)1234,1,IPC_CREAT | 0600);
        if(semid == -1)
        {
            printf("semget error");
        }
    }
    else
    {
        struct semun a;//a传值
        a.val = 1;
        if(semctl(semid,0,SETVAL,a)==-1)//0代表信号量下表
        {
            perror("semctl init error");
        }
    }
}

void CMySem::p()
{
    struct sembuf buf;
    buf.sem_num = 0;//信号量下标
    buf.sem_op = -1;//p操作
    buf.sem_flg = SEM_UNDO;
    if(semop(semid,&buf,1)==-1)
    {
        perror("p error");
    }
}

void CMySem::v()
{
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = SEM_UNDO;//设置在进程出现错误时信号量值自动恢复，防止一个进程占着信号量
    if(semop(semid,&buf,1)==-1)//1表示操作数，sembuf的数量
    {
        perror("v error");
    }
}

void CMySem::destroy()
{
    if(semctl(semid,0,IPC_RMID)==-1)//0代表信号量集
    {
        perror("semctl destroy error");
    }
}

}//namespace
#endif
