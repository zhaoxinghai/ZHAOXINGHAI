#ifndef PUBLIC_H
#define PUBLIC_H

#include <QString>

#define SD_CARD_PATH                     "/run/media/mmcblk0p1/"
#define U_DISK_PATH                      "/run/media/sda1/"
#define U_DISK_DEVICE_PATH               "/dev/"
#define HOME_PATH                        "/home/root/Bin/"

#define PRESIGNAL_DIR                    "Presignal/"

#define CONFIG_DIR                       "Config/"
#define CONFIG_FILE                      "config.xml"
#define CONFIG_ROOT_ITEM                 "D1SysConf"

#define LOCAL_CONFIG_FILE                "localConfig.xml"
#define LOCAL_CONFIG_BACKUP_FILE         "localConfig_backup.xml"
#define LOGIC_CONFIG                     "LogicConfig"
#define CONFIG                           "Config"
#define LOCAL                            "Local"
#define NAME                             "Name"
#define VALUE                            "Value"

#define LANGUAGE_XML_FILE                "langStrings.xml"
#define FONT_FILENAME                    "fonts.xml"

#define AUDIO_DATABASE_FILE              "audio-database.xml"
#define AUDIO_DIR                        "Audio/"
#define VAAUDIO_DIR                      "VAAudio/"
#define TXT_SUFFIX                       ".txt"
#define XML_SUFFIX                       ".xml"
#define CONFIG_IOMAP                     "ConfIOMap"
#define COMMON_GROUP                     "Group"
#define RECORD_GROUP                     "Record"
#define IOMAP_ITEM_NAME                  "IOMap"
#define GROUP_NAME_ATTR                  "name"
#define DESCRIPTION_ATTR                 "Description"
#define FILENAME_ATTR                    "Filename"
#define INDEX_FST_ATTR                   "IndexFst"
#define INDEX_SND_ATTR                   "IndexSnd"
#define IOMAP_ITEM_SIZE                  "Size"

#define RECORD_DIR                        "Record/"
#define RECORD_CACHE_DIR                  "Record/Cache/"

#define LOG_DIR                          "Logs/"

#define FONTS_DIR                        "Fonts/"
#define SYSTEM_FONTS_PATH                "/usr/local/fonts/"

#define IMAGES_DIR                       "Images/"
#define IMAGE_DIR                        "testImages"

#define SYSTEM_VERSION_FILE              "/etc/sysVersion.ini"
#define BACKLIGHT_PATH                   "/sys/class/backlight/backlight/brightness"
#define WATCHDOG_PATH                    "/dev/watchdog"
#define TOUCH_SCREEN_DEVICE_FILE         "/dev/input/touchscreen0"
#define LINUX_PARTITIONS_PATH            "/proc/partitions"
#define LINUX_MOUNTS_PATH                "/proc/mounts"
#define UDISK_DEVICE_NAME                "sda1"

#define LOCK_IMAGE_FILE                  "lockImg.png"
#define DEFAULT_LOCK_IMAGE_FILE          "lockImg_default.png"

#define INIT_STATUS    0
#define INSERT_STATUS  1
#define REMOVE_STATUS  2

#define HAVE_UDISK     0
#define NO_UDISK       1
#define ERR_UDISK     -1

#ifdef _DEBUG_
#define _SINGLE_                0
#define _LOGIC_                 1
#define _PAGE_                  1
#define _SOURCE_MANAGE_         1
#define _SCHEDULE_MANAGE_       1
#define _HISTORY_               1
#define _KEYBOARD_              1
#define _RECORD_                1
#define _LANGUAGE_LIST_         1
#else
#define _SINGLE_                1
#define _LOGIC_                 0
#define _PAGE_                  0
#define _SOURCE_MANAGE_         0
#define _SCHEDULE_MANAGE_       0
#define _HISTORY_               0
#define _KEYBOARD_              0
#define _RECORD_                0
#define _LANGUAGE_LIST_         0
#endif

#define SYSTEM_CMD_COPY_LOG      1
#define SYSTEM_CMD_MOUNT_UDISK   10
#define SYSTEM_CMD_UMOUNT_UDISK  11

#define WAIT_STATUS "wait"
#define FULL_STATUS "full"
#define PART_STATUS "part"
#define DIS_STATUS  "disconnect"

#define PRELISTEN_NAME "Prelisten"
#define PRELISTEN_ZONE "Speaker"

#define RECORD_AUTO_SAVE_NAME "autoRecord"

/*
"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" English 英语
"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZäöüÄÖÜß" Deutsch 德语
"AaÁáBbCcČčDdĎďEeÉéĚěFfGgHhChchIiÍíJjKkLlMmNnŇňOoÓóPpQqRrŘřSsŠšTtŤťUuÚúŮůVvWwXxYyÝýZzŽž" Čeština 捷克语
"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя" Pycckий 俄语
"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" Nederlands 荷兰语
"AaĂăÂâBbCcDdEeFfGgHhIiÎîJjKkLlMmNnOoPpQqRrSsȘșTtȚțUuVvWwXxYyZz" Română 罗马尼亚语
"AaBbCcÇçDdEeFfGgĞğHhIıİiJjKkLlMmNnOoÖöPpRrSsŞşTtUuÜüVvYyZz" Türkçe 土耳其语
"AaĄąBbCcĆćDdEeĘęFfGgHhIiJjKkLlŁłMmNnŃńOoóPpQqRrSsŚśTtUuVvWwXxYyZzŹźŻż" Polski 波兰语
"ㅏㅑㅓㅕㅗㅛㅜㅠㅡㅣㅐㅔㅒㅖㅘㅙㅚㅢㅝㅞㅟ" +
"ㄱ(가갸거겨고교구규그기개게걔계과괘괴긔궈궤귀)" +
"ㄴ(나냐너녀노뇨누뉴느니내네냬녜놔놰뇌늬눠눼뉘)" +
"ㄷ(다댜더뎌도됴두듀드디대데댸뎨돠돼되듸둬뒈뒤)" +
"ㄹ(라랴러려로료루류르리래레럐례롸뢔뢰릐뤄뤠뤼)" +
"ㅁ(마먀머며모묘무뮤므미매메먜몌뫄뫠뫼믜뭐뭬뮈)" +
"ㅂ(바뱌머벼보뵤부뷰브비배베뱨볘봐봬뵈븨붜붸뷔)" +
"ㅅ(사샤서셔소쇼수슈스시새세섀셰솨쇄쇠싀숴쉐쉬)" +
"ㅇ(아야어여오요우유으이애에얘예와왜외의워웨위)" +
"ㅈ(자쟈저져조죠주쥬즈지재제쟤졔좌좨죄즤줘줴쥐)" +
"ㅊ(차챠처쳐초쵸추츄츠치채체챼쳬촤쵀최츼춰췌취)" +
"ㅋ(카캬커켜코쿄쿠큐크키캐케컈켸콰쾌쾨킈쿼퀘퀴)" +
"ㅌ(타탸터텨토툐투튜트티태테턔톄톼퇘퇴틔퉈퉤튀)" +
"ㅍ(파퍄퍼펴포표푸퓨프피패페퍠폐퐈퐤푀픠풔풰퓌)" +
"ㅎ(하햐허혀호효후휴흐히해헤햬혜화홰회희훠훼휘)" +
"ㄲ(까꺄꺼껴꼬꾜꾸뀨끄끼깨께꺠꼐꽈꽤꾀끠꿔꿰뀌)" +
"ㄸ(따땨떠뗘또뚀뚜뜌뜨띠때떼떄뗴똬뙈뙤띄뚸뛔뛰)" +
"ㅃ(빠뺘뻐뼈뽀뾰뿌쀼쁘삐빼뻬뺴뼤뽜뽸뾔쁴뿨쀄쀠)" +
"ㅆ(싸쌰써쎠쏘쑈쑤쓔쓰씨쌔쎄썌쎼쏴쐐쐬씌쒀쒜쒸)" +
"ㅉ(짜쨔쩌쪄쪼쬬쭈쮸쯔찌째쩨쨰쪠쫘쫴쬐쯰쭤쮀쮜)" +
"각간갇갈감갑강나낙난낟날남납낭다닥단닫달담답당" +
"라락란랃랄람랍랑마막만맏말맘맙망바박반받발밤밥" +
"방사삭산삳살삼삽상아악안앋알암압앙자작잔잗잘잠" +
"잡장차착찬찯찰참찹창카칵칸칻칼캄캅캉타탁탄탇탈" +
"탐탑탕파팍판팓팔팜팝팡하학한핟할함합항"  한국어 朝鲜语
*/

QString getCurrentPath();
QString getLocalUriHead();

#endif // PUBLIC_H
