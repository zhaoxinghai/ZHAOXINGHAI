/****************************************************************************
** Meta object code from reading C++ file 'cqmlusermanage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/cqmlusermanage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cqmlusermanage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQmlUserManage_t {
    QByteArrayData data[24];
    char stringdata0[282];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CQmlUserManage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CQmlUserManage_t qt_meta_stringdata_CQmlUserManage = {
    {
QT_MOC_LITERAL(0, 0, 14), // "CQmlUserManage"
QT_MOC_LITERAL(1, 15, 4), // "init"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 16), // "getUserListCount"
QT_MOC_LITERAL(4, 38, 9), // "getUserId"
QT_MOC_LITERAL(5, 48, 3), // "pos"
QT_MOC_LITERAL(6, 52, 19), // "getUserListStrValue"
QT_MOC_LITERAL(7, 72, 2), // "id"
QT_MOC_LITERAL(8, 75, 3), // "key"
QT_MOC_LITERAL(9, 79, 19), // "setUserListIntValue"
QT_MOC_LITERAL(10, 99, 5), // "value"
QT_MOC_LITERAL(11, 105, 19), // "getUserListIntValue"
QT_MOC_LITERAL(12, 125, 14), // "setLoginUserId"
QT_MOC_LITERAL(13, 140, 14), // "getLoginUserId"
QT_MOC_LITERAL(14, 155, 16), // "setVALoginUserId"
QT_MOC_LITERAL(15, 172, 16), // "getVALoginUserId"
QT_MOC_LITERAL(16, 189, 7), // "setUser"
QT_MOC_LITERAL(17, 197, 15), // "encryptPassword"
QT_MOC_LITERAL(18, 213, 8), // "password"
QT_MOC_LITERAL(19, 222, 17), // "checkConfigFileOK"
QT_MOC_LITERAL(20, 240, 13), // "printUserList"
QT_MOC_LITERAL(21, 254, 18), // "isNeedResetAccount"
QT_MOC_LITERAL(22, 273, 4), // "user"
QT_MOC_LITERAL(23, 278, 3) // "pwd"

    },
    "CQmlUserManage\0init\0\0getUserListCount\0"
    "getUserId\0pos\0getUserListStrValue\0id\0"
    "key\0setUserListIntValue\0value\0"
    "getUserListIntValue\0setLoginUserId\0"
    "getLoginUserId\0setVALoginUserId\0"
    "getVALoginUserId\0setUser\0encryptPassword\0"
    "password\0checkConfigFileOK\0printUserList\0"
    "isNeedResetAccount\0user\0pwd"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQmlUserManage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x0a /* Public */,
       3,    0,   90,    2, 0x0a /* Public */,
       4,    1,   91,    2, 0x0a /* Public */,
       6,    2,   94,    2, 0x0a /* Public */,
       9,    2,   99,    2, 0x0a /* Public */,
      11,    2,  104,    2, 0x0a /* Public */,
      12,    1,  109,    2, 0x0a /* Public */,
      13,    0,  112,    2, 0x0a /* Public */,
      14,    1,  113,    2, 0x0a /* Public */,
      15,    0,  116,    2, 0x0a /* Public */,
      16,    1,  117,    2, 0x0a /* Public */,
      17,    1,  120,    2, 0x0a /* Public */,
      19,    0,  123,    2, 0x0a /* Public */,
      20,    0,  124,    2, 0x0a /* Public */,
      21,    2,  125,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::QString, QMetaType::Int,    5,
    QMetaType::QString, QMetaType::QString, QMetaType::QString,    7,    8,
    QMetaType::Int, QMetaType::QString, QMetaType::Int,    7,   10,
    QMetaType::Int, QMetaType::QString, QMetaType::QString,    7,    8,
    QMetaType::Int, QMetaType::QString,    7,
    QMetaType::QString,
    QMetaType::Int, QMetaType::QString,    7,
    QMetaType::QString,
    QMetaType::Int, QMetaType::QString,    7,
    QMetaType::QString, QMetaType::QString,   18,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,   22,   23,

       0        // eod
};

void CQmlUserManage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQmlUserManage *_t = static_cast<CQmlUserManage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->init(); break;
        case 1: { int _r = _t->getUserListCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: { QString _r = _t->getUserId((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 3: { QString _r = _t->getUserListStrValue((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 4: { int _r = _t->setUserListIntValue((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 5: { int _r = _t->getUserListIntValue((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 6: { int _r = _t->setLoginUserId((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 7: { QString _r = _t->getLoginUserId();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 8: { int _r = _t->setVALoginUserId((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 9: { QString _r = _t->getVALoginUserId();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 10: { int _r = _t->setUser((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 11: { QString _r = _t->encryptPassword((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 12: { int _r = _t->checkConfigFileOK();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 13: _t->printUserList(); break;
        case 14: { bool _r = _t->isNeedResetAccount((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject CQmlUserManage::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CQmlUserManage.data,
      qt_meta_data_CQmlUserManage,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CQmlUserManage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQmlUserManage::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CQmlUserManage.stringdata0))
        return static_cast<void*>(const_cast< CQmlUserManage*>(this));
    return QObject::qt_metacast(_clname);
}

int CQmlUserManage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
