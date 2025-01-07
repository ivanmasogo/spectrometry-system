/****************************************************************************
** Meta object code from reading C++ file 'spectro.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "spectro.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'spectro.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Spectro[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,   33,   33,   33, 0x08,
      34,   33,   33,   33, 0x08,
      60,   33,   33,   33, 0x08,
      86,   33,   33,   33, 0x08,
     111,   33,   33,   33, 0x08,
     137,   33,   33,   33, 0x08,
     163,   33,   33,   33, 0x08,
     184,   33,   33,   33, 0x08,
     208,   33,   33,   33, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Spectro[] = {
    "Spectro\0on_StopMeasBtn_clicked()\0\0"
    "on_StartMeasBtn_clicked()\0"
    "on_CloseCommBtn_clicked()\0"
    "on_OpenCommBtn_clicked()\0"
    "on_ChebyshevBtn_clicked()\0"
    "on_NormalizeBtn_clicked()\0"
    "on_infiBtn_clicked()\0on_loadLogBtn_clicked()\0"
    "on_saveLogBtn_clicked()\0"
};

void Spectro::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Spectro *_t = static_cast<Spectro *>(_o);
        switch (_id) {
        case 0: _t->on_StopMeasBtn_clicked(); break;
        case 1: _t->on_StartMeasBtn_clicked(); break;
        case 2: _t->on_CloseCommBtn_clicked(); break;
        case 3: _t->on_OpenCommBtn_clicked(); break;
        case 4: _t->on_ChebyshevBtn_clicked(); break;
        case 5: _t->on_NormalizeBtn_clicked(); break;
        case 6: _t->on_infiBtn_clicked(); break;
        case 7: _t->on_loadLogBtn_clicked(); break;
        case 8: _t->on_saveLogBtn_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Spectro::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Spectro::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Spectro,
      qt_meta_data_Spectro, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Spectro::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Spectro::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Spectro::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Spectro))
        return static_cast<void*>(const_cast< Spectro*>(this));
    return QDialog::qt_metacast(_clname);
}

int Spectro::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
