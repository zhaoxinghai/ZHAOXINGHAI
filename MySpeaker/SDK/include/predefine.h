
/*
* Date:     2016-8-4
* Author :  xinghai,zhao
* Content : base define for windows and linux
*/

#ifndef DEF_H
#define DEF_H

#ifdef _MSC_VER

#if defined(DLL_EXPORTS)
#define DLL_API __declspec(dllexport)
#else 
	
#ifdef _CONSOLE
#define DLL_API  
#else
#define DLL_API  __declspec(dllimport)
#endif

#endif
#define NOMINMAX
#include <Windows.h>

#pragma  comment (lib,"wsock32.lib")

#else

#define DLL_API

#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>
#include <net/if.h>
#include <string.h>

#endif  //_MSC_VER

#include <fstream>
#include <ostream>
#include <vector>
#include <queue>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <memory>
#include <math.h>

#define TF_PATH    "/run/media/mmcblk0p1/"
#define USB_PATH   "/run/media/sda1/"
#define ARECORD    "/run/media/mmcblk0p1/Record/autoRecord.adp"

#endif  //DEF_H
