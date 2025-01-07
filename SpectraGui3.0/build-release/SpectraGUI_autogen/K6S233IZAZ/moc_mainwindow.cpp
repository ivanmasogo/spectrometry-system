/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../SpectraGui/headers/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_mainw_t {
    QByteArrayData data[20];
    char stringdata0[190];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_mainw_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_mainw_t qt_meta_stringdata_mainw = {
    {
QT_MOC_LITERAL(0, 0, 5), // "mainw"
QT_MOC_LITERAL(1, 6, 4), // "exit"
QT_MOC_LITERAL(2, 11, 0), // ""
QT_MOC_LITERAL(3, 12, 5), // "start"
QT_MOC_LITERAL(4, 18, 4), // "stop"
QT_MOC_LITERAL(5, 23, 10), // "singleMode"
QT_MOC_LITERAL(6, 34, 8), // "contMode"
QT_MOC_LITERAL(7, 43, 7), // "calData"
QT_MOC_LITERAL(8, 51, 7), // "rawData"
QT_MOC_LITERAL(9, 59, 8), // "savePlot"
QT_MOC_LITERAL(10, 68, 8), // "loadPlot"
QT_MOC_LITERAL(11, 77, 9), // "clearPlot"
QT_MOC_LITERAL(12, 87, 10), // "setIntTime"
QT_MOC_LITERAL(13, 98, 7), // "setGain"
QT_MOC_LITERAL(14, 106, 8), // "showHelp"
QT_MOC_LITERAL(15, 115, 13), // "connectDevice"
QT_MOC_LITERAL(16, 129, 10), // "changePath"
QT_MOC_LITERAL(17, 140, 9), // "keepLogFn"
QT_MOC_LITERAL(18, 150, 19), // "toggleNormalization"
QT_MOC_LITERAL(19, 170, 19) // "toggleAutoDetection"

    },
    "mainw\0exit\0\0start\0stop\0singleMode\0"
    "contMode\0calData\0rawData\0savePlot\0"
    "loadPlot\0clearPlot\0setIntTime\0setGain\0"
    "showHelp\0connectDevice\0changePath\0"
    "keepLogFn\0toggleNormalization\0"
    "toggleAutoDetection"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mainw[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  104,    2, 0x08 /* Private */,
       3,    0,  105,    2, 0x08 /* Private */,
       4,    0,  106,    2, 0x08 /* Private */,
       5,    0,  107,    2, 0x08 /* Private */,
       6,    0,  108,    2, 0x08 /* Private */,
       7,    0,  109,    2, 0x08 /* Private */,
       8,    0,  110,    2, 0x08 /* Private */,
       9,    0,  111,    2, 0x08 /* Private */,
      10,    0,  112,    2, 0x08 /* Private */,
      11,    0,  113,    2, 0x08 /* Private */,
      12,    0,  114,    2, 0x08 /* Private */,
      13,    0,  115,    2, 0x08 /* Private */,
      14,    0,  116,    2, 0x08 /* Private */,
      15,    0,  117,    2, 0x08 /* Private */,
      16,    0,  118,    2, 0x08 /* Private */,
      17,    0,  119,    2, 0x08 /* Private */,
      18,    0,  120,    2, 0x08 /* Private */,
      19,    0,  121,    2, 0x08 /* Private */,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void mainw::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<mainw *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->exit(); break;
        case 1: _t->start(); break;
        case 2: _t->stop(); break;
        case 3: _t->singleMode(); break;
        case 4: _t->contMode(); break;
        case 5: _t->calData(); break;
        case 6: _t->rawData(); break;
        case 7: _t->savePlot(); break;
        case 8: _t->loadPlot(); break;
        case 9: _t->clearPlot(); break;
        case 10: _t->setIntTime(); break;
        case 11: _t->setGain(); break;
        case 12: _t->showHelp(); break;
        case 13: { int _r = _t->connectDevice();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 14: _t->changePath(); break;
        case 15: _t->keepLogFn(); break;
        case 16: _t->toggleNormalization(); break;
        case 17: _t->toggleAutoDetection(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject mainw::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_mainw.data,
    qt_meta_data_mainw,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *mainw::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mainw::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_mainw.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int mainw::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
