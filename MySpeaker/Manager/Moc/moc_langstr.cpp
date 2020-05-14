/****************************************************************************
** Meta object code from reading C++ file 'langstr.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/langstr.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'langstr.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_LangStr_t {
    QByteArrayData data[16];
    char stringdata0[160];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LangStr_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LangStr_t qt_meta_stringdata_LangStr = {
    {
QT_MOC_LITERAL(0, 0, 7), // "LangStr"
QT_MOC_LITERAL(1, 8, 4), // "init"
QT_MOC_LITERAL(2, 13, 0), // ""
QT_MOC_LITERAL(3, 14, 11), // "readLangMap"
QT_MOC_LITERAL(4, 26, 12), // "printLangMap"
QT_MOC_LITERAL(5, 39, 11), // "getMapIndex"
QT_MOC_LITERAL(6, 51, 4), // "lang"
QT_MOC_LITERAL(7, 56, 14), // "getMapLanguage"
QT_MOC_LITERAL(8, 71, 5), // "index"
QT_MOC_LITERAL(9, 77, 10), // "getMapSize"
QT_MOC_LITERAL(10, 88, 19), // "readLangConfigTable"
QT_MOC_LITERAL(11, 108, 16), // "printConfigTable"
QT_MOC_LITERAL(12, 125, 13), // "GetLangString"
QT_MOC_LITERAL(13, 139, 5), // "resId"
QT_MOC_LITERAL(14, 145, 6), // "number"
QT_MOC_LITERAL(15, 152, 7) // "channel"

    },
    "LangStr\0init\0\0readLangMap\0printLangMap\0"
    "getMapIndex\0lang\0getMapLanguage\0index\0"
    "getMapSize\0readLangConfigTable\0"
    "printConfigTable\0GetLangString\0resId\0"
    "number\0channel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LangStr[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x0a /* Public */,
       3,    0,   65,    2, 0x0a /* Public */,
       4,    0,   66,    2, 0x0a /* Public */,
       5,    1,   67,    2, 0x0a /* Public */,
       7,    1,   70,    2, 0x0a /* Public */,
       9,    0,   73,    2, 0x0a /* Public */,
      10,    1,   74,    2, 0x0a /* Public */,
      11,    0,   77,    2, 0x0a /* Public */,
      12,    1,   78,    2, 0x0a /* Public */,
      12,    3,   81,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Int, QMetaType::QString,    6,
    QMetaType::QString, QMetaType::Int,    8,
    QMetaType::Int,
    QMetaType::Bool, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::QString, QMetaType::QString,   13,
    QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int,   13,   14,   15,

       0        // eod
};

void LangStr::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LangStr *_t = static_cast<LangStr *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->init(); break;
        case 1: { int _r = _t->readLangMap();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: _t->printLangMap(); break;
        case 3: { int _r = _t->getMapIndex((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: { QString _r = _t->getMapLanguage((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 5: { int _r = _t->getMapSize();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 6: { bool _r = _t->readLangConfigTable((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: _t->printConfigTable(); break;
        case 8: { QString _r = _t->GetLangString((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 9: { QString _r = _t->GetLangString((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject LangStr::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LangStr.data,
      qt_meta_data_LangStr,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *LangStr::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LangStr::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_LangStr.stringdata0))
        return static_cast<void*>(const_cast< LangStr*>(this));
    return QObject::qt_metacast(_clname);
}

int LangStr::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
