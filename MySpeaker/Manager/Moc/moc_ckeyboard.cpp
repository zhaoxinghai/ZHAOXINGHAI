/****************************************************************************
** Meta object code from reading C++ file 'ckeyboard.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Keyboard/ckeyboard.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ckeyboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CKeyboard_t {
    QByteArrayData data[11];
    char stringdata0[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CKeyboard_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CKeyboard_t qt_meta_stringdata_CKeyboard = {
    {
QT_MOC_LITERAL(0, 0, 9), // "CKeyboard"
QT_MOC_LITERAL(1, 10, 10), // "initKeyMap"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 8), // "language"
QT_MOC_LITERAL(4, 31, 11), // "printKeyMap"
QT_MOC_LITERAL(5, 43, 17), // "getStringListSize"
QT_MOC_LITERAL(6, 61, 3), // "key"
QT_MOC_LITERAL(7, 65, 17), // "getCharFromKeyMap"
QT_MOC_LITERAL(8, 83, 3), // "pos"
QT_MOC_LITERAL(9, 87, 10), // "getKeyText"
QT_MOC_LITERAL(10, 98, 9) // "isTwoPage"

    },
    "CKeyboard\0initKeyMap\0\0language\0"
    "printKeyMap\0getStringListSize\0key\0"
    "getCharFromKeyMap\0pos\0getKeyText\0"
    "isTwoPage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CKeyboard[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x0a /* Public */,
       4,    0,   47,    2, 0x0a /* Public */,
       5,    1,   48,    2, 0x0a /* Public */,
       7,    2,   51,    2, 0x0a /* Public */,
       9,    1,   56,    2, 0x0a /* Public */,
      10,    0,   59,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Int, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Int, QMetaType::QString,    6,
    QMetaType::QString, QMetaType::QString, QMetaType::UInt,    6,    8,
    QMetaType::QString, QMetaType::QString,    6,
    QMetaType::Int,

       0        // eod
};

void CKeyboard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CKeyboard *_t = static_cast<CKeyboard *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { int _r = _t->initKeyMap((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: _t->printKeyMap(); break;
        case 2: { int _r = _t->getStringListSize((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { QString _r = _t->getCharFromKeyMap((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 4: { QString _r = _t->getKeyText((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 5: { int _r = _t->isTwoPage();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject CKeyboard::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CKeyboard.data,
      qt_meta_data_CKeyboard,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CKeyboard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CKeyboard::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CKeyboard.stringdata0))
        return static_cast<void*>(const_cast< CKeyboard*>(this));
    return QObject::qt_metacast(_clname);
}

int CKeyboard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
