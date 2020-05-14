/****************************************************************************
** Meta object code from reading C++ file 'cgpiocontrol.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/cgpiocontrol.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cgpiocontrol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CGPIOControl_t {
    QByteArrayData data[19];
    char stringdata0[254];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CGPIOControl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CGPIOControl_t qt_meta_stringdata_CGPIOControl = {
    {
QT_MOC_LITERAL(0, 0, 12), // "CGPIOControl"
QT_MOC_LITERAL(1, 13, 16), // "touchEventSignal"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 4), // "init"
QT_MOC_LITERAL(4, 36, 14), // "enableWatchDog"
QT_MOC_LITERAL(5, 51, 16), // "getUImageVersion"
QT_MOC_LITERAL(6, 68, 13), // "getDtbVersion"
QT_MOC_LITERAL(7, 82, 18), // "getImageFitVersion"
QT_MOC_LITERAL(8, 101, 20), // "getFilesystemVersion"
QT_MOC_LITERAL(9, 122, 17), // "firstSetBacklight"
QT_MOC_LITERAL(10, 140, 5), // "value"
QT_MOC_LITERAL(11, 146, 12), // "setBacklight"
QT_MOC_LITERAL(12, 159, 12), // "getBacklight"
QT_MOC_LITERAL(13, 172, 12), // "openWatchdog"
QT_MOC_LITERAL(14, 185, 13), // "closeWatchdog"
QT_MOC_LITERAL(15, 199, 12), // "feadWatchdog"
QT_MOC_LITERAL(16, 212, 8), // "openBeep"
QT_MOC_LITERAL(17, 221, 9), // "closeBeep"
QT_MOC_LITERAL(18, 231, 22) // "isScreenProtectorValid"

    },
    "CGPIOControl\0touchEventSignal\0\0init\0"
    "enableWatchDog\0getUImageVersion\0"
    "getDtbVersion\0getImageFitVersion\0"
    "getFilesystemVersion\0firstSetBacklight\0"
    "value\0setBacklight\0getBacklight\0"
    "openWatchdog\0closeWatchdog\0feadWatchdog\0"
    "openBeep\0closeBeep\0isScreenProtectorValid"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CGPIOControl[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   90,    2, 0x0a /* Public */,
       5,    0,   93,    2, 0x0a /* Public */,
       6,    0,   94,    2, 0x0a /* Public */,
       7,    0,   95,    2, 0x0a /* Public */,
       8,    0,   96,    2, 0x0a /* Public */,
       9,    1,   97,    2, 0x0a /* Public */,
      11,    1,  100,    2, 0x0a /* Public */,
      12,    0,  103,    2, 0x0a /* Public */,
      13,    0,  104,    2, 0x0a /* Public */,
      14,    0,  105,    2, 0x0a /* Public */,
      15,    0,  106,    2, 0x0a /* Public */,
      16,    0,  107,    2, 0x0a /* Public */,
      17,    0,  108,    2, 0x0a /* Public */,
      18,    0,  109,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::Int, QMetaType::QString,   10,
    QMetaType::Int, QMetaType::Int,   10,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Bool,

       0        // eod
};

void CGPIOControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CGPIOControl *_t = static_cast<CGPIOControl *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->touchEventSignal(); break;
        case 1: _t->init((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: { QString _r = _t->getUImageVersion();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 3: { QString _r = _t->getDtbVersion();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 4: { QString _r = _t->getImageFitVersion();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 5: { QString _r = _t->getFilesystemVersion();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 6: { int _r = _t->firstSetBacklight((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 7: { int _r = _t->setBacklight((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 8: { int _r = _t->getBacklight();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 9: { int _r = _t->openWatchdog();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 10: { int _r = _t->closeWatchdog();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 11: { int _r = _t->feadWatchdog();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 12: { int _r = _t->openBeep();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 13: { int _r = _t->closeBeep();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 14: { bool _r = _t->isScreenProtectorValid();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CGPIOControl::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CGPIOControl::touchEventSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject CGPIOControl::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CGPIOControl.data,
      qt_meta_data_CGPIOControl,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CGPIOControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CGPIOControl::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CGPIOControl.stringdata0))
        return static_cast<void*>(const_cast< CGPIOControl*>(this));
    return QThread::qt_metacast(_clname);
}

int CGPIOControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CGPIOControl::touchEventSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
