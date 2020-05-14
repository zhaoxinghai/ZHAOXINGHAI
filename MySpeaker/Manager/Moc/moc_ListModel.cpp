/****************************************************************************
** Meta object code from reading C++ file 'ListModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/ListModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ListModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Models__ListModel_t {
    QByteArrayData data[24];
    char stringdata0[214];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Models__ListModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Models__ListModel_t qt_meta_stringdata_Models__ListModel = {
    {
QT_MOC_LITERAL(0, 0, 17), // "Models::ListModel"
QT_MOC_LITERAL(1, 18, 12), // "countChanged"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 14), // "sortingChanged"
QT_MOC_LITERAL(4, 47, 10), // "updateItem"
QT_MOC_LITERAL(5, 58, 8), // "rowCount"
QT_MOC_LITERAL(6, 67, 6), // "parent"
QT_MOC_LITERAL(7, 74, 4), // "data"
QT_MOC_LITERAL(8, 79, 5), // "index"
QT_MOC_LITERAL(9, 85, 4), // "role"
QT_MOC_LITERAL(10, 90, 7), // "setData"
QT_MOC_LITERAL(11, 98, 5), // "value"
QT_MOC_LITERAL(12, 104, 9), // "roleNames"
QT_MOC_LITERAL(13, 114, 21), // "QHash<int,QByteArray>"
QT_MOC_LITERAL(14, 136, 16), // "appendRowFromQml"
QT_MOC_LITERAL(15, 153, 4), // "item"
QT_MOC_LITERAL(16, 158, 9), // "removeRow"
QT_MOC_LITERAL(17, 168, 3), // "row"
QT_MOC_LITERAL(18, 172, 3), // "get"
QT_MOC_LITERAL(19, 176, 14), // "rowIndexFromId"
QT_MOC_LITERAL(20, 191, 2), // "id"
QT_MOC_LITERAL(21, 194, 5), // "clear"
QT_MOC_LITERAL(22, 200, 5), // "count"
QT_MOC_LITERAL(23, 206, 7) // "sorting"

    },
    "Models::ListModel\0countChanged\0\0"
    "sortingChanged\0updateItem\0rowCount\0"
    "parent\0data\0index\0role\0setData\0value\0"
    "roleNames\0QHash<int,QByteArray>\0"
    "appendRowFromQml\0item\0removeRow\0row\0"
    "get\0rowIndexFromId\0id\0clear\0count\0"
    "sorting"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Models__ListModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       2,  126, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       3,    1,   87,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   90,    2, 0x08 /* Private */,

 // methods: name, argc, parameters, tag, flags
       5,    1,   91,    2, 0x02 /* Public */,
       5,    0,   94,    2, 0x22 /* Public | MethodCloned */,
       7,    2,   95,    2, 0x02 /* Public */,
      10,    3,  100,    2, 0x02 /* Public */,
      12,    0,  107,    2, 0x02 /* Public */,
      14,    1,  108,    2, 0x02 /* Public */,
      16,    2,  111,    2, 0x02 /* Public */,
      16,    1,  116,    2, 0x22 /* Public | MethodCloned */,
      18,    1,  119,    2, 0x02 /* Public */,
      19,    1,  122,    2, 0x02 /* Public */,
      21,    0,  125,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void,

 // methods: parameters
    QMetaType::Int, QMetaType::QModelIndex,    6,
    QMetaType::Int,
    QMetaType::QVariant, QMetaType::QModelIndex, QMetaType::Int,    8,    9,
    QMetaType::Bool, QMetaType::QModelIndex, QMetaType::QVariant, QMetaType::Int,    8,   11,    9,
    0x80000000 | 13,
    QMetaType::Void, QMetaType::QObjectStar,   15,
    QMetaType::Bool, QMetaType::Int, QMetaType::QModelIndex,   17,    8,
    QMetaType::Bool, QMetaType::Int,   17,
    QMetaType::QVariant, QMetaType::Int,    8,
    QMetaType::Int, QMetaType::Int,   20,
    QMetaType::Void,

 // properties: name, type, flags
      22, QMetaType::Int, 0x00495001,
      23, QMetaType::Bool, 0x00495103,

 // properties: notify_signal_id
       0,
       1,

       0        // eod
};

void Models::ListModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ListModel *_t = static_cast<ListModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->countChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->sortingChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->updateItem(); break;
        case 3: { int _r = _t->rowCount((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: { int _r = _t->rowCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 5: { QVariant _r = _t->data((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        case 6: { bool _r = _t->setData((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: { QHash<int,QByteArray> _r = _t->roleNames();
            if (_a[0]) *reinterpret_cast< QHash<int,QByteArray>*>(_a[0]) = _r; }  break;
        case 8: _t->appendRowFromQml((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 9: { bool _r = _t->removeRow((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: { bool _r = _t->removeRow((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: { QVariant _r = _t->get((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        case 12: { int _r = _t->rowIndexFromId((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 13: _t->clear(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ListModel::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ListModel::countChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ListModel::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ListModel::sortingChanged)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        ListModel *_t = static_cast<ListModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->rowCount(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->sortingEnabled(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        ListModel *_t = static_cast<ListModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setSorting(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject Models::ListModel::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_Models__ListModel.data,
      qt_meta_data_Models__ListModel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Models::ListModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Models::ListModel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Models__ListModel.stringdata0))
        return static_cast<void*>(const_cast< ListModel*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int Models::ListModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
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
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Models::ListModel::countChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Models::ListModel::sortingChanged(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
