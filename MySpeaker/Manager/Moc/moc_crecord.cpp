/****************************************************************************
** Meta object code from reading C++ file 'crecord.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/crecord.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'crecord.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CRecord_t {
    QByteArrayData data[63];
    char stringdata0[989];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CRecord_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CRecord_t qt_meta_stringdata_CRecord = {
    {
QT_MOC_LITERAL(0, 0, 7), // "CRecord"
QT_MOC_LITERAL(1, 8, 4), // "init"
QT_MOC_LITERAL(2, 13, 0), // ""
QT_MOC_LITERAL(3, 14, 23), // "checkAndInitRecordGroup"
QT_MOC_LITERAL(4, 38, 18), // "initRecordsToIOmap"
QT_MOC_LITERAL(5, 57, 15), // "clearRecordList"
QT_MOC_LITERAL(6, 73, 13), // "getRecordList"
QT_MOC_LITERAL(7, 87, 18), // "getRecordListCount"
QT_MOC_LITERAL(8, 106, 12), // "getItemIndex"
QT_MOC_LITERAL(9, 119, 3), // "pos"
QT_MOC_LITERAL(10, 123, 15), // "getItemStrValue"
QT_MOC_LITERAL(11, 139, 5), // "index"
QT_MOC_LITERAL(12, 145, 3), // "key"
QT_MOC_LITERAL(13, 149, 15), // "getItemIntValue"
QT_MOC_LITERAL(14, 165, 16), // "getItemBoolValue"
QT_MOC_LITERAL(15, 182, 24), // "insertRecordToRecordList"
QT_MOC_LITERAL(16, 207, 8), // "baseName"
QT_MOC_LITERAL(17, 216, 24), // "updateRecordToRecordList"
QT_MOC_LITERAL(18, 241, 11), // "oldBaseName"
QT_MOC_LITERAL(19, 253, 11), // "newBaseName"
QT_MOC_LITERAL(20, 265, 26), // "removeRecordFromRecordList"
QT_MOC_LITERAL(21, 292, 15), // "printRecordList"
QT_MOC_LITERAL(22, 308, 10), // "saveRecord"
QT_MOC_LITERAL(23, 319, 12), // "updateRecord"
QT_MOC_LITERAL(24, 332, 19), // "insertRecordToIOmap"
QT_MOC_LITERAL(25, 352, 19), // "updateRecordToIOmap"
QT_MOC_LITERAL(26, 372, 25), // "removeItemFromRecordGroup"
QT_MOC_LITERAL(27, 398, 20), // "removeAllCacheRecord"
QT_MOC_LITERAL(28, 419, 11), // "startRecord"
QT_MOC_LITERAL(29, 431, 10), // "stopRecord"
QT_MOC_LITERAL(30, 442, 20), // "prelistenCacheRecord"
QT_MOC_LITERAL(31, 463, 24), // "stopPrelistenCacheRecord"
QT_MOC_LITERAL(32, 488, 15), // "prelistenRecord"
QT_MOC_LITERAL(33, 504, 19), // "stopPrelistenRecord"
QT_MOC_LITERAL(34, 524, 19), // "stopPrelistenFromId"
QT_MOC_LITERAL(35, 544, 2), // "id"
QT_MOC_LITERAL(36, 547, 11), // "setRecordId"
QT_MOC_LITERAL(37, 559, 11), // "getRecordId"
QT_MOC_LITERAL(38, 571, 14), // "setPrelistenId"
QT_MOC_LITERAL(39, 586, 14), // "getPrelistenId"
QT_MOC_LITERAL(40, 601, 19), // "setCachePrelistenId"
QT_MOC_LITERAL(41, 621, 19), // "getCachePrelistenId"
QT_MOC_LITERAL(42, 641, 16), // "selectRecordItem"
QT_MOC_LITERAL(43, 658, 19), // "getRecordItemSelect"
QT_MOC_LITERAL(44, 678, 18), // "unselectRecordItem"
QT_MOC_LITERAL(45, 697, 19), // "selectAllRecordItem"
QT_MOC_LITERAL(46, 717, 21), // "unselectAllRecordItem"
QT_MOC_LITERAL(47, 739, 16), // "fillRecordSource"
QT_MOC_LITERAL(48, 756, 25), // "std::vector<std::string>&"
QT_MOC_LITERAL(49, 782, 5), // "vPath"
QT_MOC_LITERAL(50, 788, 11), // "getAutoName"
QT_MOC_LITERAL(51, 800, 11), // "doubleCheck"
QT_MOC_LITERAL(52, 812, 4), // "name"
QT_MOC_LITERAL(53, 817, 14), // "isReservedName"
QT_MOC_LITERAL(54, 832, 13), // "getFormatTime"
QT_MOC_LITERAL(55, 846, 5), // "count"
QT_MOC_LITERAL(56, 852, 16), // "getRecordMaxTime"
QT_MOC_LITERAL(57, 869, 20), // "isOverRecordMaxCount"
QT_MOC_LITERAL(58, 890, 19), // "isOverRecordStorage"
QT_MOC_LITERAL(59, 910, 21), // "getRemainRecordSecond"
QT_MOC_LITERAL(60, 932, 26), // "updateAutosaveToRecordList"
QT_MOC_LITERAL(61, 959, 13), // "isCCCFVersion"
QT_MOC_LITERAL(62, 973, 15) // "getAutoSaveName"

    },
    "CRecord\0init\0\0checkAndInitRecordGroup\0"
    "initRecordsToIOmap\0clearRecordList\0"
    "getRecordList\0getRecordListCount\0"
    "getItemIndex\0pos\0getItemStrValue\0index\0"
    "key\0getItemIntValue\0getItemBoolValue\0"
    "insertRecordToRecordList\0baseName\0"
    "updateRecordToRecordList\0oldBaseName\0"
    "newBaseName\0removeRecordFromRecordList\0"
    "printRecordList\0saveRecord\0updateRecord\0"
    "insertRecordToIOmap\0updateRecordToIOmap\0"
    "removeItemFromRecordGroup\0"
    "removeAllCacheRecord\0startRecord\0"
    "stopRecord\0prelistenCacheRecord\0"
    "stopPrelistenCacheRecord\0prelistenRecord\0"
    "stopPrelistenRecord\0stopPrelistenFromId\0"
    "id\0setRecordId\0getRecordId\0setPrelistenId\0"
    "getPrelistenId\0setCachePrelistenId\0"
    "getCachePrelistenId\0selectRecordItem\0"
    "getRecordItemSelect\0unselectRecordItem\0"
    "selectAllRecordItem\0unselectAllRecordItem\0"
    "fillRecordSource\0std::vector<std::string>&\0"
    "vPath\0getAutoName\0doubleCheck\0name\0"
    "isReservedName\0getFormatTime\0count\0"
    "getRecordMaxTime\0isOverRecordMaxCount\0"
    "isOverRecordStorage\0getRemainRecordSecond\0"
    "updateAutosaveToRecordList\0isCCCFVersion\0"
    "getAutoSaveName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CRecord[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      50,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  264,    2, 0x0a /* Public */,
       3,    0,  265,    2, 0x0a /* Public */,
       4,    0,  266,    2, 0x0a /* Public */,
       5,    0,  267,    2, 0x0a /* Public */,
       6,    0,  268,    2, 0x0a /* Public */,
       7,    0,  269,    2, 0x0a /* Public */,
       8,    1,  270,    2, 0x0a /* Public */,
      10,    2,  273,    2, 0x0a /* Public */,
      13,    2,  278,    2, 0x0a /* Public */,
      14,    2,  283,    2, 0x0a /* Public */,
      15,    1,  288,    2, 0x0a /* Public */,
      17,    2,  291,    2, 0x0a /* Public */,
      20,    1,  296,    2, 0x0a /* Public */,
      21,    0,  299,    2, 0x0a /* Public */,
      22,    1,  300,    2, 0x0a /* Public */,
      23,    2,  303,    2, 0x0a /* Public */,
      24,    1,  308,    2, 0x0a /* Public */,
      25,    2,  311,    2, 0x0a /* Public */,
      26,    1,  316,    2, 0x0a /* Public */,
      27,    0,  319,    2, 0x0a /* Public */,
      28,    0,  320,    2, 0x0a /* Public */,
      29,    0,  321,    2, 0x0a /* Public */,
      30,    0,  322,    2, 0x0a /* Public */,
      31,    0,  323,    2, 0x0a /* Public */,
      32,    1,  324,    2, 0x0a /* Public */,
      33,    0,  327,    2, 0x0a /* Public */,
      34,    1,  328,    2, 0x0a /* Public */,
      36,    1,  331,    2, 0x0a /* Public */,
      37,    0,  334,    2, 0x0a /* Public */,
      38,    1,  335,    2, 0x0a /* Public */,
      39,    0,  338,    2, 0x0a /* Public */,
      40,    1,  339,    2, 0x0a /* Public */,
      41,    0,  342,    2, 0x0a /* Public */,
      42,    1,  343,    2, 0x0a /* Public */,
      43,    1,  346,    2, 0x0a /* Public */,
      44,    1,  349,    2, 0x0a /* Public */,
      45,    0,  352,    2, 0x0a /* Public */,
      46,    0,  353,    2, 0x0a /* Public */,
      47,    1,  354,    2, 0x0a /* Public */,
      50,    0,  357,    2, 0x0a /* Public */,
      51,    1,  358,    2, 0x0a /* Public */,
      53,    1,  361,    2, 0x0a /* Public */,
      54,    1,  364,    2, 0x0a /* Public */,
      56,    0,  367,    2, 0x0a /* Public */,
      57,    0,  368,    2, 0x0a /* Public */,
      58,    0,  369,    2, 0x0a /* Public */,
      59,    0,  370,    2, 0x0a /* Public */,
      60,    0,  371,    2, 0x0a /* Public */,
      61,    0,  372,    2, 0x0a /* Public */,
      62,    0,  373,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int, QMetaType::UInt,    9,
    QMetaType::QString, QMetaType::Int, QMetaType::QString,   11,   12,
    QMetaType::Int, QMetaType::Int, QMetaType::QString,   11,   12,
    QMetaType::Bool, QMetaType::Int, QMetaType::QString,   11,   12,
    QMetaType::Int, QMetaType::QString,   16,
    QMetaType::Int, QMetaType::QString, QMetaType::QString,   18,   19,
    QMetaType::Int, QMetaType::Int,   11,
    QMetaType::Int,
    QMetaType::Int, QMetaType::QString,   16,
    QMetaType::Int, QMetaType::QString, QMetaType::QString,   18,   19,
    QMetaType::Int, QMetaType::QString,   16,
    QMetaType::Int, QMetaType::QString, QMetaType::QString,   18,   19,
    QMetaType::Int, QMetaType::QString,   16,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int, QMetaType::Int,   11,
    QMetaType::Int,
    QMetaType::Int, QMetaType::Int,   35,
    QMetaType::Int, QMetaType::Int,   35,
    QMetaType::Int,
    QMetaType::Int, QMetaType::Int,   35,
    QMetaType::Int,
    QMetaType::Int, QMetaType::Int,   35,
    QMetaType::Int,
    QMetaType::Int, QMetaType::Int,   11,
    QMetaType::Bool, QMetaType::Int,   11,
    QMetaType::Int, QMetaType::Int,   11,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int, 0x80000000 | 48,   49,
    QMetaType::QString,
    QMetaType::Bool, QMetaType::QString,   52,
    QMetaType::Bool, QMetaType::QString,   52,
    QMetaType::QString, QMetaType::Int,   55,
    QMetaType::Int,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::QString,

       0        // eod
};

void CRecord::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CRecord *_t = static_cast<CRecord *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->init(); break;
        case 1: { int _r = _t->checkAndInitRecordGroup();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: { int _r = _t->initRecordsToIOmap();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->clearRecordList();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: { int _r = _t->getRecordList();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 5: { int _r = _t->getRecordListCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 6: { int _r = _t->getItemIndex((*reinterpret_cast< uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 7: { QString _r = _t->getItemStrValue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 8: { int _r = _t->getItemIntValue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 9: { bool _r = _t->getItemBoolValue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: { int _r = _t->insertRecordToRecordList((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 11: { int _r = _t->updateRecordToRecordList((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 12: { int _r = _t->removeRecordFromRecordList((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 13: { int _r = _t->printRecordList();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 14: { int _r = _t->saveRecord((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 15: { int _r = _t->updateRecord((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 16: { int _r = _t->insertRecordToIOmap((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 17: { int _r = _t->updateRecordToIOmap((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 18: { int _r = _t->removeItemFromRecordGroup((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 19: { int _r = _t->removeAllCacheRecord();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 20: { int _r = _t->startRecord();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 21: { int _r = _t->stopRecord();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 22: { int _r = _t->prelistenCacheRecord();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 23: { int _r = _t->stopPrelistenCacheRecord();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 24: { int _r = _t->prelistenRecord((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 25: { int _r = _t->stopPrelistenRecord();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 26: { int _r = _t->stopPrelistenFromId((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 27: { int _r = _t->setRecordId((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 28: { int _r = _t->getRecordId();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 29: { int _r = _t->setPrelistenId((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 30: { int _r = _t->getPrelistenId();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 31: { int _r = _t->setCachePrelistenId((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 32: { int _r = _t->getCachePrelistenId();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 33: { int _r = _t->selectRecordItem((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 34: { bool _r = _t->getRecordItemSelect((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 35: { int _r = _t->unselectRecordItem((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 36: { int _r = _t->selectAllRecordItem();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 37: { int _r = _t->unselectAllRecordItem();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 38: { int _r = _t->fillRecordSource((*reinterpret_cast< std::vector<std::string>(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 39: { QString _r = _t->getAutoName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 40: { bool _r = _t->doubleCheck((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 41: { bool _r = _t->isReservedName((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 42: { QString _r = _t->getFormatTime((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 43: { int _r = _t->getRecordMaxTime();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 44: { bool _r = _t->isOverRecordMaxCount();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 45: { bool _r = _t->isOverRecordStorage();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 46: { int _r = _t->getRemainRecordSecond();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 47: _t->updateAutosaveToRecordList(); break;
        case 48: { bool _r = _t->isCCCFVersion();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 49: { QString _r = _t->getAutoSaveName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject CRecord::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CRecord.data,
      qt_meta_data_CRecord,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CRecord::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CRecord::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CRecord.stringdata0))
        return static_cast<void*>(const_cast< CRecord*>(this));
    return QObject::qt_metacast(_clname);
}

int CRecord::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 50)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 50;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 50)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 50;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
