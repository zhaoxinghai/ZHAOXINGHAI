#ifndef WIN32
#ifndef SEM_H
#define SEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <unistd.h>

namespace TEST {

struct semun
{
    int val;
};

class CMySem
{
public:
    void init();
    void p();
    void v();
    void destroy();
private:
    int semid;
};

} //namespace
#endif

#endif
