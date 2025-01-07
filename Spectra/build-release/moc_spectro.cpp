/****************************************************************************
** Meta object code from reading C++ file 'spectro.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../spectro.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'spectro.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Spectro_t {
    QByteArrayData data[18];
    char stringdata0[341];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Spectro_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Spectro_t qt_meta_stringdata_Spectro = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Spectro"
QT_MOC_LITERAL(1, 8, 22), // "on_StopMeasBtn_clicked"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 23), // "on_StartMeasBtn_clicked"
QT_MOC_LITERAL(4, 56, 23), // "on_CloseCommBtn_clicked"
QT_MOC_LITERAL(5, 80, 22), // "on_OpenCommBtn_clicked"
QT_MOC_LITERAL(6, 103, 22), // "on_AutomateBtn_clicked"
QT_MOC_LITERAL(7, 126, 23), // "on_ChebyshevBtn_clicked"
QT_MOC_LITERAL(8, 150, 23), // "on_NormalizeBtn_clicked"
QT_MOC_LITERAL(9, 174, 18), // "on_infiBtn_clicked"
QT_MOC_LITERAL(10, 193, 21), // "on_loadLogBtn_clicked"
QT_MOC_LITERAL(11, 215, 21), // "on_saveLogBtn_clicked"
QT_MOC_LITERAL(12, 237, 11), // "saveCSVData"
QT_MOC_LITERAL(13, 249, 23), // "updatePredictionResults"
QT_MOC_LITERAL(14, 273, 17), // "predictionResults"
QT_MOC_LITERAL(15, 291, 19), // "initializeTextItems"
QT_MOC_LITERAL(16, 311, 21), // "updateLabelVisibility"
QT_MOC_LITERAL(17, 333, 7) // "visible"

    },
    "Spectro\0on_StopMeasBtn_clicked\0\0"
    "on_StartMeasBtn_clicked\0on_CloseCommBtn_clicked\0"
    "on_OpenCommBtn_clicked\0on_AutomateBtn_clicked\0"
    "on_ChebyshevBtn_clicked\0on_NormalizeBtn_clicked\0"
    "on_infiBtn_clicked\0on_loadLogBtn_clicked\0"
    "on_saveLogBtn_clicked\0saveCSVData\0"
    "updatePredictionResults\0predictionResults\0"
    "initializeTextItems\0updateLabelVisibility\0"
    "visible"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Spectro[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    0,   86,    2, 0x08 /* Private */,
       5,    0,   87,    2, 0x08 /* Private */,
       6,    0,   88,    2, 0x08 /* Private */,
       7,    0,   89,    2, 0x08 /* Private */,
       8,    0,   90,    2, 0x08 /* Private */,
       9,    0,   91,    2, 0x08 /* Private */,
      10,    0,   92,    2, 0x08 /* Private */,
      11,    0,   93,    2, 0x08 /* Private */,
      12,    0,   94,    2, 0x08 /* Private */,
      13,    1,   95,    2, 0x08 /* Private */,
      15,    0,   98,    2, 0x08 /* Private */,
      16,    1,   99,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   17,

       0        // eod
};

void Spectro::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Spectro *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_StopMeasBtn_clicked(); break;
        case 1: _t->on_StartMeasBtn_clicked(); break;
        case 2: _t->on_CloseCommBtn_clicked(); break;
        case 3: _t->on_OpenCommBtn_clicked(); break;
        case 4: _t->on_AutomateBtn_clicked(); break;
        case 5: _t->on_ChebyshevBtn_clicked(); break;
        case 6: _t->on_NormalizeBtn_clicked(); break;
        case 7: _t->on_infiBtn_clicked(); break;
        case 8: _t->on_loadLogBtn_clicked(); break;
        case 9: _t->on_saveLogBtn_clicked(); break;
        case 10: _t->saveCSVData(); break;
        case 11: _t->updatePredictionResults((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 12: _t->initializeTextItems(); break;
        case 13: _t->updateLabelVisibility((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Spectro::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_Spectro.data,
    qt_meta_data_Spectro,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Spectro::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Spectro::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Spectro.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Spectro::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
