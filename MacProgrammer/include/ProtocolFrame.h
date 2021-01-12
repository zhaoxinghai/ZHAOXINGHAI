#ifndef PROTOCOLFRAME_H
#define PROTOCOLFRAME_H

#include "Define.h"
#include "DeviceID.h"
#include <QByteArray>
#include <QString>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#define D_Unlock                    0x0000
#define D_Locked                    0x0001
#define D_SystemCtrlActionReset             0x0001
#define D_SystemCtrlActionUpdateSetting     0x0002
#define D_SystemCtrlActionUpdateVoiceList   0x0003
#define D_MonitorActionReleaseMode  0x00
#define D_MonitorActionStartupMode  0x01
#define D_PointPlayRelease          0x00
#define D_PointPlayStartup          0x01
#define D_PointPlayNext             0x02
#define D_PointPlayPrevious         0x03
#define D_PointPlayPause            0x04
#define D_PointPlayResume           0x05
#define D_PointPlayInvalid          0x06
#define D_PointPlayLoop             0x10
#define D_PointPlayRepeat           0x20
#define D_PointPlayOnce             0x30
#define D_PointPlayGeneral          0x40
#define D_LatticeEN                 0x01
#define D_LatticeCN                 0x02
#define D_LatticeUN                 0x03
#define D_LatticeDir                0x01
#define D_LatticeMp3                0x02
#define D_LatticeEmpty              0x03
#define D_CurrentPlaySong           0x04
#define D_CurrentStopSong           0x05
#define D_LatticePoint              (24 * 24 / 8)
#define D_LatticePointUN            (16 * 12 / 8)
#define D_DownPage                  0x01
#define D_UpPage                    0x02
#define D_Current                   0x03
#define D_DownFolder                0x03
#define D_UpFolder                  0x04
#define D_ClosePriority             0xFF
#define D_NaturalPriority           0xFE
#define D_PPSPriority               0x10
#define D_GatherPriority            0x20
#define D_MicPriority               0x30
#define D_FAPriority                0x40
#define D_FAMicPriority             0x50
#define D_MaxPriority               0x00
#define D_SongListSize              7
#define D_SongSize                  7
#define D_ZoneSize                  6
#define D_TerminalSize              7
#define D_AddInAudioReleaseMode                 0x00
#define D_AddInAudioUnitMode                    0x01
#define D_AddInAudioMultMode                    0x02
#define D_AddInAudioMicUnitMode                 0x03
#define D_AddInAudioMicMultMode                 0x04
#define D_AddInAudioGatherUnitMode              0x05
#define D_AddInAudioGatherMultMode              0x06
#define D_AddInAudioGatherToMp3UnitMode         0x07
#define D_AddInAudioGatherToMp3MultMode         0x08
#define D_AddInAudioAlarmUnitMode               0x09
#define D_AddInAudioAlarmMultMode               0x0A
#define D_AddInAudioAlarmGatherToMp3UnitMode    0x0B
#define D_AddInAudioAlarmGatherToMp3MultMode    0x0C
//2017.5.18 lyf //add PCM unitcase mode and multcast mode///
#define D_AddInAudioPcmUnitMode                 0x0D    ////
#define D_AddInAudioPcmMultMode                 0x0E    ////
////////////////////////////////////////////////////////////
#define D_AddInAudioCompleteReleaseMode         0x80
#define D_PointPlayPlayMode         0x01
#define D_PointPlayMonitorMode      0x02
#define D_PointPlayTalkMode         0x03
#define D_FrameOk                   0x0001
#define D_FrameFail                 0x0000
#define D_FrameCrcEnable            false
#define D_FrameMiniSize             20
#define D_FrameDataMaxSize          1280
#define D_FrameHeader               0x4A79
#define D_FrameHeaderSize           2
#define D_FrameCrcSize              2
#define D_FrameVersion_1            0x01
#define D_FrameVersion_2            0x02
#define D_FrameFlagCheck            0x80
#define D_FrameFlagNotCheckScrDest  0x40
#define D_FrameFlagAnswer           0x02
#define D_FrameFlagNotification     0x01
#define D_FrameFlagCommand          0x00
#define D_FirstAudio                0xE0000200
#define D_LastAudio                 0xEEFFFFFF
#define D_AudioTotalMin             0x00000001
//#define D_AudioTotalMax           0x0DE40000
#define D_AudioTotalMax             0x0EFFFDFF
#define D_MicFirstAudio             0xE0000300
#define D_ChannelBackup             0x01
#define D_ChannelMonitor            0x02
#define D_ServerOperatorIp          0xFFFFFFFF

#define D_NetworkArg                0x0001
#define D_NetworkArgAnswer          0x8001
#define D_QueryNetworkArg           0x0002
#define D_QueryNetworkArgAnswer     0x8002
#define D_ServerArg                 0x0003
#define D_ServerArgAnswer           0x8003
#define D_QueryServerArg            0x0004
#define D_QueryServerArgAnswer      0x8004
#define D_Volume                    0x0005
#define D_VolumeAnswer              0x8005
#define D_QueryVolume               0x0006
#define D_QueryVolumeAnswer         0x8006
#define D_SystemCtrl                0x0013
#define D_SystemCtrlAnswer          0x8013
#define D_HeartHeat                 0x0055
#define D_HeartHeatAnswer           0x8055

#define D_AddInAudio                0x0801
#define D_AddInAudioAnswer          0x8801
#define D_QueryAudio                0x0802
#define D_QueryAudioAnswer          0x8802
#define D_QueryList                 0x0803
#define D_QueryListAnswer           0x8803
#define D_QuerySong                 0x0804
#define D_QuerySongAnswer           0x8804
#define D_PointPlay                 0x0805
#define D_PointPlayAnswer           0x8805
#define D_QueryPointPlay            0x0806
#define D_QueryPointPlayAnswer      0x8806
#define D_Monitor                   0x0807
#define D_MonitorAnswer             0x8807
#define D_QueryMonitor              0x0808
#define D_QueryMonitorAnswer        0x8808
#define D_QueryZone                 0x0809
#define D_QueryZoneAnswer           0x8809
#define D_QueryTerminalFZ           0x080A
#define D_QueryTerminalFZAnswer     0x880A
#define D_QueryPlayStatus           0x080D
#define D_QueryPlayStatusAnswer     0x880D
#define D_RegisterMulticast         0x080E
#define D_RegisterMulticastAnswer   0x880E
#define D_ChangeMulticast           0x080F
#define D_ChangeMulticastAnswer     0x880F
#define D_RequestMulticast          0x0810
#define D_RequestMulticastAnswer    0x8810
#define D_QuerySongLattice          0x0811
#define D_QuerySongLatticeAnswer    0x8811
#define D_Lock                      0x0812
#define D_LockAnswer                0x8812
#define D_QueryLock                 0x0813
#define D_QueryLockAnswer           0x8813
#define D_QueryMulticast            0x0814
#define D_QueryMulticastAnswer      0x8814
#define D_QueryZoneLattice          0x0815
#define D_QueryZoneLatticeAnswer    0x8815
#define D_QueryTerminal             0x0816
#define D_QueryTerminalAnswer       0x8816
#define D_HeartBeatPlayStatus       0x0817
#define D_HeartBeatPlayStatusAnswer 0x8817
#define D_RequestMicResource        0x0818
#define D_RequestMicResourceAnswer  0x8818
#define D_19ADownload               0x0819
#define D_19ADownloadAnswer         0x8819
#define D_19AStatusUpload           0x081A
#define D_19AStatusUploadAnswer     0x881A
#define D_DefectiveStatus           0x081B
#define D_DefectiveStatusAnswer     0x881B
#define D_RemoteDevice              0x081C
#define D_RemoteDeviceAnswer        0x881C

#define D_InfoNotification          0x0901

#define D_QueryZoneStatus           0x0C01
#define D_QueryZoneStatusAnswer     0x8C01
#define D_QueryDeviceStatus         0x0C02
#define D_QueryDeviceStatusAnswer   0x8C02
#define D_AudioCtrl                 0x0C03
#define D_AudioCtrlAnswer           0x8C03
#define D_UpdateRequest             0x0C04
#define D_UpdateRequestAnswer       0x8C04
#define D_UpdateZoneInfo            0x0C05
#define D_UpdateZoneInfoAnswer      0x8C05
#define D_UpdateTerminalInfo        0x0C06
#define D_UpdateTerminalInfoAnswer  0x8C06
#define D_UpdateAudioInfo           0x0C07
#define D_UpdateAudioInfoAnswer     0x8C07
#define D_UpdateOver                0x0C08
#define D_UpdateOverAnswer          0x8C08
#define D_DeviceVolume              0x0C09
#define D_DeviceVolumeAnswer        0x8C09
#define D_QueryDeviceVolume         0x0C0A
#define D_QueryDeviceVolumeAnswer   0x8C0A
#define D_DeviceMonitor             0x0C0B
#define D_DeviceMonitorAnswer       0x8C0B
#define D_QueryDeviceMonitor        0x0C0C
#define D_QueryDeviceMonitorAnswer  0x8C0C
#define D_DeviceSystemCtrl          0x0C0D
#define D_DeviceSystemCtrlAnswer    0x8C0D
#define D_UpdateAudio               0x0C0E
#define D_UpdateAudioAnswer         0x8C0E
#define D_ZoneAudioCtrl             0x0C0F
#define D_ZoneAudioCtrlAnswer       0x8C0F
#define D_UpdateAlarmInfo           0x0C10
#define D_UpdateAlarmInfoAnswer     0x8C10
#define D_Alarm                     0x0C11
#define D_AlarmAnswer               0x8C11
#define D_UpdatePriorityInfo        0x0C12
#define D_UpdatePriorityInfoAnswer  0x8C12
#define D_UpdateMicInfo             0x0C13
#define D_UpdateMicInfoAnswer       0x8C13
#define D_UpdateRepeaterInfo        0x0C14
#define D_UpdateRepeaterInfoAnswer  0x8C14
#define D_TTS                       0x0C15
#define D_TTSAnswer                 0x8C15
#define D_RemoteCtrl                0x0C16
#define D_RemoteCtrlAnswer          0x8C16
#define D_RequestTime               0x0C17
#define D_RequestTimeAnswer         0x8C17
//                                  0x8C18为通知帧,所以没有对应的0x0C18
#define D_TextInfo                  0x8C18

#define D_SDKGetSong                0x1001
#define D_SDKGetSongAnswer          0x9001
#define D_SDKGetZone                0x1002
#define D_SDKGetZoneAnswer          0x9002
#define D_SDKGetZoneStatus          0x1003
#define D_SDKGetZoneStatusAnswer    0x9003
#define D_SDKZoneVolumeCtrl         0x1004
//#define D_SDKZoneVolumeCtrlAnswer   0x9004
#define D_SDKZoneAudioSwitch        0x1005
//#define D_SDKZoneAudioSwitchAnswer  0x9005
#define D_SDKAudioPlay              0x1006
//#define D_SDKAudioPlayAnswer  0x9006

#define D_FrameHeaderHigh           0
#define D_FrameHeaderLow            1
#define D_FrameVerion               2
#define D_FrameFlag                 3
#define D_FrameNumberHigh           4
#define D_FrameNumberLow            5
#define D_FrameSrcSystem            6
#define D_FrameSrcModel             7
#define D_FrameSrcNumberHigh        8
#define D_FrameSrcNumberLow         9
#define D_FrameDestSystem           10
#define D_FrameDestModel            11
#define D_FrameDestNumberHigh       12
#define D_FrameDestNumberLow        13
#define D_FrameCmdHigh              14
#define D_FrameCmdLow               15
#define D_FrameDataSizeHigh         16
#define D_FrameDataSizeLow          17

#define D_DataStreamFrameMiniSize       28
#define D_DataStreamFrameVerion         2
#define D_DataStreamFrameFlag           3
#define D_DataStreamFrameNumberHigh     4
#define D_DataStreamFrameNumberLow      5
#define D_DataStreamFrameSrcSystem      6
#define D_DataStreamFrameSrcModel       7
#define D_DataStreamFrameSrcNumberHigh  8
#define D_DataStreamFrameSrcNumberLow   9
#define D_DataStreamFrameDestSystem     10
#define D_DataStreamFrameDestModel      11
#define D_DataStreamFrameDestNumberHigh 12
#define D_DataStreamFrameDestNumberLow  13
#define D_DataStreamFrameTypeHigh       14
#define D_DataStreamFrameTypeLow        15
#define D_DataStreamFrameHPixelHigh     16
#define D_DataStreamFrameHPixelLow      17
#define D_DataStreamFrameVPixelHigh     18
#define D_DataStreamFrameVPixelLow      19
#define D_DataStreamFrameLengthHigh     24
#define D_DataStreamFrameLengthLow      25

#define D_RemoteCtrlNotScreenShot   0x00
#define D_RemoteCtrlScreenShot      0x01

#define D_RemoteCtrlKeyZero         0x0000
#define D_RemoteCtrlKeyOne          0x0001
#define D_RemoteCtrlKeyTwo          0x0002
#define D_RemoteCtrlKeyThree        0x0003
#define D_RemoteCtrlKeyFour         0x0004
#define D_RemoteCtrlKeyFive         0x0005
#define D_RemoteCtrlKeySix          0x0006
#define D_RemoteCtrlKeySeven        0x0007
#define D_RemoteCtrlKeyEight        0x0008
#define D_RemoteCtrlKeyNine         0x0009
#define D_RemoteCtrlKeyBacklight    0x000A
#define D_RemoteCtrlKeySetting      0x000B
#define D_RemoteCtrlKeyMute         0x000C
#define D_RemoteCtrlKeySelectSong   0x000D
#define D_RemoteCtrlKeyCall         0x000E
#define D_RemoteCtrlKeyUSB          0x000F

#define D_RemoteCtrlKeyMode         0x0010
#define D_RemoteCtrlKeyExtend       0x0011
#define D_RemoteCtrlKeyCancel       0x0012
#define D_RemoteCtrlKeyUp           0x0013
#define D_RemoteCtrlKeyDown         0x0014
#define D_RemoteCtrlKeyLeft         0x0015
#define D_RemoteCtrlKeyRight        0x0016
#define D_RemoteCtrlKeyConfirm      0x0017
#define D_RemoteCtrlKeyPrevious     0x0018
#define D_RemoteCtrlKeyNext         0x0019
#define D_RemoteCtrlKeyPlayPause    0x001A
#define D_RemoteCtrlKeyStop         0x001B
#define D_RemoteCtrlKeyVolPlus      0x001C
#define D_RemoteCtrlKeyVolSubtract  0x001D
#define D_RemoteCtrlKeyChannel      0x001E
#define D_RemoteCtrlKeyLocal        0x001F

#define D_RemoteCtrlKeyMonitor      0x0020
#define D_RemoteCtrlKeyTalk         0x0021
#define D_RemoteCtrlKeyBroadcast    0x0022
#define D_RemoteCtrlInvalid         0xFFFF

const unsigned short CRC16_Table[256] =
{//CRC16原始数据表
         0x0000,0xC0C1,0xC181,0x0140,0xC301,0x03C0,0x0280,0xC241,
         0xC601,0x06C0,0x0780,0xC741,0x0500,0xC5C1,0xC481,0x0440,
         0xCC01,0x0CC0,0x0D80,0xCD41,0x0F00,0xCFC1,0xCE81,0X0E40,
         0x0A00,0xCAC1,0xCB81,0x0B40,0xC901,0x09C0,0x0880,0xC841,
         0xD801,0x18C0,0x1980,0xD941,0x1B00,0xDBC1,0xDA81,0x1A40,
         0x1E00,0xDEC1,0xDF81,0x1F40,0xDD01,0x1DC0,0x1C80,0xDC41,
         0x1400,0xD4C1,0xD581,0x1540,0xD701,0x17C0,0x1680,0xD641,
         0xD201,0x12C0,0x1380,0xD341,0x1100,0xD1C1,0xD081,0x1040,
         0xF001,0x30C0,0x3180,0xF141,0x3300,0xF3C1,0xF281,0x3240,
         0x3600,0xF6C1,0xF781,0x3740,0xF501,0x35C0,0x3480,0xF441,
         0x3C00,0xFCC1,0xFD81,0x3D40,0xFF01,0x3FC0,0x3E80,0xFE41,
         0xFA01,0x3AC0,0x3B80,0xFB41,0x3900,0xF9C1,0xF881,0x3840,
         0x2800,0xE8C1,0xE981,0x2940,0xEB01,0x2BC0,0x2A80,0xEA41,
         0xEE01,0x2EC0,0x2F80,0xEF41,0x2D00,0xEDC1,0xEC81,0x2C40,
         0xE401,0x24C0,0x2580,0xE541,0x2700,0xE7C1,0xE681,0x2640,
         0x2200,0xE2C1,0xE381,0x2340,0xE101,0x21C0,0x2080,0xE041,
         0xA001,0x60C0,0x6180,0xA141,0x6300,0xA3C1,0xA281,0x6240,
         0x6600,0xA6C1,0xA781,0x6740,0xA501,0x65C0,0x6480,0xA441,
         0x6C00,0xACC1,0xAD81,0x6D40,0xAF01,0x6FC0,0x6E80,0xAE41,
         0xAA01,0x6AC0,0x6B80,0xAB41,0x6900,0xA9C1,0xA881,0x6840,
         0x7800,0xB8C1,0xB981,0x7940,0xBB01,0x7BC0,0x7A80,0xBA41,
         0xBE01,0x7EC0,0x7F80,0xBF41,0x7D00,0xBDC1,0xBC81,0x7C40,
         0xB401,0x74C0,0x7580,0xB541,0x7700,0xB7C1,0xB681,0x7640,
         0x7200,0xB2C1,0xB381,0x7340,0xB101,0x71C0,0x7080,0xB041,
         0x5000,0x90C1,0x9181,0x5140,0x9301,0x53C0,0x5280,0x9241,
         0x9601,0x56C0,0x5780,0x9741,0x5500,0x95C1,0x9481,0x5440,
         0x9C01,0x5CC0,0x5D80,0x9D41,0x5F00,0x9FC1,0x9E81,0x5E40,
         0X5A00,0x9AC1,0x9B81,0x5B40,0x9901,0x59C0,0x5880,0x9841,
         0x8801,0x48C0,0x4980,0x8941,0x4B00,0x8BC1,0x8A81,0x4A40,
         0x4E00,0x8EC1,0x8F81,0x4F40,0x8D01,0x4DC0,0x4C80,0x8C41,
         0x4400,0x84C1,0x8581,0x4540,0x8701,0x47C0,0x4680,0x8641,
         0x8201,0x42C0,0x4380,0x8341,0x4100,0x81C1,0x8081,0x4040,
};


/* Table of CRC values for high-order byte */
const unsigned char table_crc_hi[] = {
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* Table of CRC values for low-order byte */
const unsigned char table_crc_lo[] = {
        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
        0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
        0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
        0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
        0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
        0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
        0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
        0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
        0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
        0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
        0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
        0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
        0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
        0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
        0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
        0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
        0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
        0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
        0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
        0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
        0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
        0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

/********************************************************************************************/
/*函数名称：unsigned short CRC16Count(QByteArray data)*/
/*功能描述：从指定的ChkAddr地址起，对数据进行CRC16校验*/
/*输入： *ChkAddr 数据的内存地址,  Length 要校验的数据长度*/
/*输出： CRC16的校验值*/
/*全局变量：*/
/*调用函数：*/
/********************************************************************************************/
unsigned short CRC16Count(QByteArray data);

/********************************************************************************************/
/*函数名称：unsigned short CRC16Count(char *ChkAddr, unsigned int Length)*/
/*功能描述：从指定的ChkAddr地址起，对数据进行CRC16校验*/
/*输入： *ChkAddr 数据的内存地址,  Length 要校验的数据长度*/
/*输出： CRC16的校验值*/
/*全局变量：*/
/*调用函数：*/
/********************************************************************************************/
unsigned short CRC16Count(char *ChkAddr, unsigned int Length);




QString getErrorString(E_Error flag);
QString getCmdString(unsigned short cmd);
QString hexToString(QByteArray data);
QString hexToString(unsigned int data);
QString hexToString(unsigned short data);
bool checkIp(QString ip);
bool checkMask(QString ip);
QString getStatusStr(E_PAState status);
bool isServer(unsigned char model);
bool isClient(unsigned char model);
bool isTerminal_Amp(unsigned char model);
bool isMicrophone(unsigned char model);
bool isSingle_CH_Pre_Amp(unsigned char model);
bool isDual_CH_Pre_Amp(unsigned char model);
bool isSingle_CH_IP_Amp(unsigned char model);
bool isDual_CH_IP_Amp(unsigned char model);

bool isAmp(unsigned char model);
bool isDevice(unsigned char model);
bool isSingle_CH_Amp(unsigned char model);
bool isDual_CH_Amp(unsigned char model);
bool isPre_Amp(unsigned char model);
bool isIP_Amp(unsigned char model);

#endif // PROTOCOLFRAME_H
