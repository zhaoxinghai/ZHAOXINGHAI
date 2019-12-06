
#include "mysharemem.h"
#include "memcontent.h"
#include <assert.h>
#include <iostream>
#include <string.h>

#ifndef WIN32
#include <unistd.h>
#endif

void SleepMinisecond(int minisecond)
{
#ifdef _MSC_VER
    Sleep(minisecond);
#else
    usleep(minisecond * 1000);
#endif
}

TEST::CShareMem file;

void Demo1()
{
    if(!file.Open() || file.m_pBuffer==NULL)
    {
        return;
    }
    TEST::CMemContent* pMem = (TEST::CMemContent*)file.m_pBuffer;
    pMem->Init();

    int a = 0;
    while(true)
    {
        std::cin>>a;

        {
            TEST::CShareLock lck(&file);
            pMem->m_Test = a;
        }
        file.Notify();
    }
}

void Demo2()
{
    if(!file.Open() || file.m_pBuffer==NULL)
    {
        return;
    }
    TEST::CMemContent* pMem = (TEST::CMemContent*)file.m_pBuffer;
    int a = 0;
    while(true)
    {
        file.Wait();
        {
            TEST::CShareLock lck(&file);
            std::cout<<pMem->m_Test<<std::endl;
        }
    }
}

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);

    //loop
    char cmd[1024];
    while (true)
    {
        std::cout << std::endl << "#>";
        memset(cmd,0,sizeof(cmd));
        std::cin.getline(cmd, 1024);
        std::string str(cmd);

        if(str == "1")
        {
            Demo1();
        }
        else if(str == "2")
        {
            Demo2();
        }
        else
        {
            break;
        }
    }
    printf("Exit");
    SleepMinisecond(5000);
    return 0;
}
