
/*
* Date:     2016-8-4
* Author :  xinghai,zhao
* Content : this is errorcode and description
*/

#ifndef ERROR_H
#define ERROR_H

#define RESULT_VAL_OK                       0

#define RESULT_VAL_ERROR                    1

//Maybe network disconnect
#define ERROR_NETWORK_ERROR                 2

//the Command timeout
#define ERROR_TIME_OUT                      3

//Audio source empty
#define ERROR_SOURCE_EMPTY                  4

//Audio dest empty
#define ERROR_DEST_EMPYT                    5

//local speaker is busy
#define ERROR_LOCAL_SPEAKER_BUSY            6

//local microphone is busy
#define ERROR_LOCAL_MICROPHONE_BUSY         7

//announcement is not exist
#define ERROR_ANNOUONCEMENT_NOTEXIST        8

//socket create error
#define ERROR_SOCKET_CREATE                 9

//set udp multiple cast address
#define ERROR_MULTIPLE_ADDRESS              10

//error parameter
#define ERROR_PARAMETER                     11

//error audio running busy
#define ERROR_THREADPOOL_FULL               12

//join udp group error
#define ERROR_JOIN_UDP_GROUP                13

//socket set reuse addr error
#define ERROR_SOCKET_SET_REUSE              14

//socket bind error
#define ERROR_SOCKET_BIND                   15

//user manual stop announcement
#define ERROR_MANUAL_STOP                   16

//the remote is disconnect
#define ERROR_REMOTE_DISCONNECT_STOP        17

#define ERROR_POINT_NULL                    18

//write udp to net error
#define ERROR_WRITE_UDP                     19

//audio device error
#define ERROR_AUDIO_DEVICE                  20

//socket set timeout
#define ERROR_SOCKET_SET_TIMEOUT            21

//open file error
#define ERROR_FILE_OPEN                     22

//socket set buffer
#define ERROR_SOCKET_SET_BUFFER             23

//socket set udp address
#define ERROR_SOCKET_SET_UDP                24

//read udp from net error
#define ERROR_READ_UDP                      25

//socket set block
#define ERROR_SOCKET_SET_BLOCK              26

//socket reuse
#define ERROR_SOCKET_SET_REUSEADDR          27

//file format error
#define ERROR_DECODE_ADP                    28

#define ERROR_AUDIO_FILE_TYPE               29

#define ERROR_AUDIO_FILE_FORMAT             30

#define ERROR_SET_SOCKET_BLOCK              31

#define ERROR_NO_ADP_DATA                   32

#define ERROR_SYSTEM_NOEXIST                33

#define ERROR_THREAD_POOL_FULL              34

#define ERROR_QUEUE_FULL                    35

#endif  //ERROR_H
