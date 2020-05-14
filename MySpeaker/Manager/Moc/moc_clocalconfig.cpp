/****************************************************************************
** Meta object code from reading C++ file 'clocalconfig.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/clocalconfig.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'clocalconfig.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CLocalConfig_t {
    QByteArrayData data[21];
    char stringdata0[244];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CLocalConfig_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CLocalConfig_t qt_meta_stringdata_CLocalConfig = {
    {
QT_MOC_LITERAL(0, 0, 12), // "CLocalConfig"
QT_MOC_LITERAL(1, 13, 4), // "init"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 19), // "initLocalConfigFile"
QT_MOC_LITERAL(4, 39, 11), // "writeConfig"
QT_MOC_LITERAL(5, 51, 10), // "configType"
QT_MOC_LITERAL(6, 62, 4), // "name"
QT_MOC_LITERAL(7, 67, 5), // "value"
QT_MOC_LITERAL(8, 73, 12), // "insertConfig"
QT_MOC_LITERAL(9, 86, 10), // "readConfig"
QT_MOC_LITERAL(10, 97, 18), // "getProtocolVersion"
QT_MOC_LITERAL(11, 116, 15), // "getUploadMaster"
QT_MOC_LITERAL(12, 132, 14), // "getStationName"
QT_MOC_LITERAL(13, 147, 12), // "getLocalNode"
QT_MOC_LITERAL(14, 160, 14), // "getUsedStorage"
QT_MOC_LITERAL(15, 175, 4), // "type"
QT_MOC_LITERAL(16, 180, 15), // "getTotalStorage"
QT_MOC_LITERAL(17, 196, 10), // "getLockImg"
QT_MOC_LITERAL(18, 207, 15), // "setIsConfigMode"
QT_MOC_LITERAL(19, 223, 4), // "mode"
QT_MOC_LITERAL(20, 228, 15) // "getIsConfigMode"

    },
    "CLocalConfig\0init\0\0initLocalConfigFile\0"
    "writeConfig\0configType\0name\0value\0"
    "insertConfig\0readConfig\0getProtocolVersion\0"
    "getUploadMaster\0getStationName\0"
    "getLocalNode\0getUsedStorage\0type\0"
    "getTotalStorage\0getLockImg\0setIsConfigMode\0"
    "mode\0getIsConfigMode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CLocalConfig[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x0a /* Public */,
       3,    0,   85,    2, 0x0a /* Public */,
       4,    3,   86,    2, 0x0a /* Public */,
       8,    2,   93,    2, 0x0a /* Public */,
       9,    2,   98,    2, 0x0a /* Public */,
      10,    0,  103,    2, 0x0a /* Public */,
      11,    0,  104,    2, 0x0a /* Public */,
      12,    0,  105,    2, 0x0a /* Public */,
      13,    0,  106,    2, 0x0a /* Public */,
      14,    1,  107,    2, 0x0a /* Public */,
      16,    1,  110,    2, 0x0a /* Public */,
      17,    0,  113,    2, 0x0a /* Public */,
      18,    1,  114,    2, 0x0a /* Public */,
      20,    0,  117,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString,    5,    6,    7,
    QMetaType::Int, QMetaType::QString, QMetaType::QString,    6,    7,
    QMetaType::QString, QMetaType::QString, QMetaType::QString,    5,    6,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::Int,
    QMetaType::Int, QMetaType::Int,   15,
    QMetaType::Int, QMetaType::Int,   15,
    QMetaType::QString,
    QMetaType::Int, QMetaType::Bool,   19,
    QMetaType::Bool,

       0        // eod
};

void CLocalConfig::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CLocalConfig *_t = static_cast<CLocalConfig *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->init(); break;
        case 1: { int _r = _t->initLocalConfigFile();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: { int _r = _t->writeConfig((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->insertConfig((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: { QString _r = _t->readConfig((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 5: { QString _r = _t->getProtocolVersion();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 6: { QString _r = _t->getUploadMaster();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 7: { QString _r = _t->getStationName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 8: { int _r = _t->getLocalNode();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 9: { int _r = _t->getUsedStorage((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 10: { int _r = _t->getTotalStorage((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 11: { QString _r = _t->getLockImg();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 12: { int _r = _t->setIsConfigMode((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 13: { bool _r = _t->getIsConfigMode();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject CLocalConfig::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CLocalConfig.data,
      qt_meta_data_CLocalConfig,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CLocalConfig::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CLocalConfig::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CLocalConfig.stringdata0))
        return static_cast<void*>(const_cast< CLocalConfig*>(this));
    return QObject::qt_metacast(_clname);
}

int CLocalConfig::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
