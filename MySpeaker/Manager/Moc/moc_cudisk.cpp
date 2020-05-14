/****************************************************************************
** Meta object code from reading C++ file 'cudisk.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/cudisk.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cudisk.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CUDisk_t {
    QByteArrayData data[19];
    char stringdata0[232];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CUDisk_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CUDisk_t qt_meta_stringdata_CUDisk = {
    {
QT_MOC_LITERAL(0, 0, 6), // "CUDisk"
QT_MOC_LITERAL(1, 7, 21), // "updateUDiskIconSignal"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 6), // "status"
QT_MOC_LITERAL(4, 37, 17), // "insertUDiskSignal"
QT_MOC_LITERAL(5, 55, 17), // "removeUDiskSignal"
QT_MOC_LITERAL(6, 73, 4), // "init"
QT_MOC_LITERAL(7, 78, 10), // "mountUDisk"
QT_MOC_LITERAL(8, 89, 11), // "umountUDisk"
QT_MOC_LITERAL(9, 101, 2), // "id"
QT_MOC_LITERAL(10, 104, 14), // "selfMountUDisk"
QT_MOC_LITERAL(11, 119, 15), // "selfUmountUDisk"
QT_MOC_LITERAL(12, 135, 14), // "isUDiskMounted"
QT_MOC_LITERAL(13, 150, 16), // "checkUDiskStatus"
QT_MOC_LITERAL(14, 167, 21), // "isUDiskMountPathExist"
QT_MOC_LITERAL(15, 189, 20), // "createUDiskMountPath"
QT_MOC_LITERAL(16, 210, 11), // "isUDiskBusy"
QT_MOC_LITERAL(17, 222, 6), // "uSleep"
QT_MOC_LITERAL(18, 229, 2) // "us"

    },
    "CUDisk\0updateUDiskIconSignal\0\0status\0"
    "insertUDiskSignal\0removeUDiskSignal\0"
    "init\0mountUDisk\0umountUDisk\0id\0"
    "selfMountUDisk\0selfUmountUDisk\0"
    "isUDiskMounted\0checkUDiskStatus\0"
    "isUDiskMountPathExist\0createUDiskMountPath\0"
    "isUDiskBusy\0uSleep\0us"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CUDisk[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,
       4,    0,   92,    2, 0x06 /* Public */,
       5,    0,   93,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   94,    2, 0x0a /* Public */,
       7,    0,   95,    2, 0x0a /* Public */,
       8,    0,   96,    2, 0x0a /* Public */,
       8,    1,   97,    2, 0x0a /* Public */,
      10,    0,  100,    2, 0x0a /* Public */,
      11,    0,  101,    2, 0x0a /* Public */,
      12,    0,  102,    2, 0x0a /* Public */,
      13,    0,  103,    2, 0x0a /* Public */,
      14,    0,  104,    2, 0x0a /* Public */,
      15,    0,  105,    2, 0x0a /* Public */,
      16,    0,  106,    2, 0x0a /* Public */,
      17,    1,  107,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int, QMetaType::Int,    9,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Bool,
    QMetaType::Int,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,   18,

       0        // eod
};

void CUDisk::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CUDisk *_t = static_cast<CUDisk *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateUDiskIconSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->insertUDiskSignal(); break;
        case 2: _t->removeUDiskSignal(); break;
        case 3: _t->init(); break;
        case 4: { int _r = _t->mountUDisk();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 5: { int _r = _t->umountUDisk();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 6: { int _r = _t->umountUDisk((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 7: { int _r = _t->selfMountUDisk();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 8: { int _r = _t->selfUmountUDisk();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 9: { bool _r = _t->isUDiskMounted();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: { int _r = _t->checkUDiskStatus();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 11: { bool _r = _t->isUDiskMountPathExist();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 12: { bool _r = _t->createUDiskMountPath();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 13: { int _r = _t->isUDiskBusy();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 14: _t->uSleep((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CUDisk::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CUDisk::updateUDiskIconSignal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CUDisk::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CUDisk::insertUDiskSignal)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CUDisk::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CUDisk::removeUDiskSignal)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject CUDisk::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CUDisk.data,
      qt_meta_data_CUDisk,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CUDisk::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CUDisk::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CUDisk.stringdata0))
        return static_cast<void*>(const_cast< CUDisk*>(this));
    return QThread::qt_metacast(_clname);
}

int CUDisk::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void CUDisk::updateUDiskIconSignal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CUDisk::insertUDiskSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void CUDisk::removeUDiskSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
