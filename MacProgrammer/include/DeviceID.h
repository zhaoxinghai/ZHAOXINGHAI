#ifndef DEVICEID_H
#define DEVICEID_H

#include "TranslatorStr.h"

//DeviceSubSystem
#define D_AS12XX        0x00

//DeviceMode
#define D_AS1200CS      0x00
#define D_AS1200WS      0x01
#define D_AS1201        0x02
#define D_AS1203        0x03
#define D_AS1204        0x04
#define D_AS1205        0x05
#define D_AS1206        0x06
#define D_AS1207        0x07

#define D_FirstModel    D_AS1200CS
#define D_LastModel     D_AS1207

#define D_AS12XXStr         TranslatorStr::getAS12XXStr()
#define D_AS1200CSStr       TranslatorStr::getAS1200CSStr()
#define D_AS1200WSStr       TranslatorStr::getAS1200WSStr()
#define D_AS1201Str         TranslatorStr::getAS1201Str()
#define D_AS1203Str         TranslatorStr::getAS1203Str()
#define D_AS1204Str         TranslatorStr::getAS1204Str()
#define D_AS1205Str         TranslatorStr::getAS1205Str()
#define D_AS1206Str         TranslatorStr::getAS1206Str()
#define D_AS1207Str         TranslatorStr::getAS1207Str()

//NCX-1902I对应我们的AS-5215T终端
//NCX-I01R对应AS-5203P寻呼麦克风
//NCX-I01S_2.0对应我们的AS-5200主机



#endif // DEVICEID_H
