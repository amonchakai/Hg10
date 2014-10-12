/****************************************************************************
** Meta object code from reading C++ file 'QXmppDiscoveryManager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/client/QXmppDiscoveryManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QXmppDiscoveryManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QXmppDiscoveryManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x05,
      54,   22,   22,   22, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_QXmppDiscoveryManager[] = {
    "QXmppDiscoveryManager\0\0"
    "infoReceived(QXmppDiscoveryIq)\0"
    "itemsReceived(QXmppDiscoveryIq)\0"
};

void QXmppDiscoveryManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppDiscoveryManager *_t = static_cast<QXmppDiscoveryManager *>(_o);
        switch (_id) {
        case 0: _t->infoReceived((*reinterpret_cast< const QXmppDiscoveryIq(*)>(_a[1]))); break;
        case 1: _t->itemsReceived((*reinterpret_cast< const QXmppDiscoveryIq(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QXmppDiscoveryManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppDiscoveryManager::staticMetaObject = {
    { &QXmppClientExtension::staticMetaObject, qt_meta_stringdata_QXmppDiscoveryManager,
      qt_meta_data_QXmppDiscoveryManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppDiscoveryManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppDiscoveryManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppDiscoveryManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppDiscoveryManager))
        return static_cast<void*>(const_cast< QXmppDiscoveryManager*>(this));
    return QXmppClientExtension::qt_metacast(_clname);
}

int QXmppDiscoveryManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QXmppClientExtension::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QXmppDiscoveryManager::infoReceived(const QXmppDiscoveryIq & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QXmppDiscoveryManager::itemsReceived(const QXmppDiscoveryIq & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
