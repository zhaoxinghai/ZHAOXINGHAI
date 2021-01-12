
#ifndef _GNP_THREAD_H_
#define _GNP_THREAD_H_

#include <WinSock2.h>
#include <Windows.h>
#include "MainWindow.h"

//D1 ETCS result
#define ETCS_OK               0
#define ETCS_SOCKET_CREATE    1
#define ETCS_SOCKET_CON       2
#define ETCS_SOCKET_RECV      3
#define ETCS_SOCKET_SEND      4
#define ETCS_RET_ERROR        5

typedef HANDLE threadhandle;

enum eRUN_STATE
{
    STATE_NOT_RUN = 0,
    STATE_RUNING,
    STATE_FINISH
};

class MainWindow;
class CGNPThread
{
public:
    CGNPThread();
    ~CGNPThread();

    void Init(MainWindow* p);

    void Run(int type);

    void RunETCS();
    void RunINC();

    eRUN_STATE   D1State;
    int          D1FlashError;

protected:
    void ETCSTelnet(SOCKET sock);
    bool ETCSWrite(SOCKET sock,const char *strWrite,int nWrite);

    MainWindow* m_pMain;
};

#endif 
