/****************************************************************************
** Meta object code from reading C++ file 'QXmppRemoteMethod.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/client/QXmppRemoteMethod.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QXmppRemoteMethod.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QXmppRemoteMethod[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      33,   30,   18,   18, 0x08,
      59,   30,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QXmppRemoteMethod[] = {
    "QXmppRemoteMethod\0\0callDone()\0iq\0"
    "gotError(QXmppRpcErrorIq)\0"
    "gotResult(QXmppRpcResponseIq)\0"
};

void QXmppRemoteMethod::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppRemoteMethod *_t = static_cast<QXmppRemoteMethod *>(_o);
        switch (_id) {
        case 0: _t->callDone(); break;
        case 1: _t->gotError((*reinterpret_cast< const QXmppRpcErrorIq(*)>(_a[1]))); break;
        case 2: _t->gotResult((*reinterpret_cast< const QXmppRpcResponseIq(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QXmppRemoteMethod::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppRemoteMethod::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QXmppRemoteMethod,
      qt_meta_data_QXmppRemoteMethod, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppRemoteMethod::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppRemoteMethod::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppRemoteMethod::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppRemoteMethod))
        return static_cast<void*>(const_cast< QXmppRemoteMethod*>(this));
    return QObject::qt_metacast(_clname);
}

int QXmppRemoteMethod::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void QXmppRemoteMethod::callDone()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
