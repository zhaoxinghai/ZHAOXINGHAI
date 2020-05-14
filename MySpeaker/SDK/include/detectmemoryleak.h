#ifdef WIN32

#ifndef _DETECT_MEMORY_LEAK_H
#define _DETECT_MEMORY_LEAK_H


#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_CLIENTBLOCK
#endif

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif


#endif   //_DETECT_MEMORY_LEAK_H
#endif   //WIN32
