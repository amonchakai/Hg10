/****************************************************************************
** Meta object code from reading C++ file 'QXmppSocks.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/base/QXmppSocks.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QXmppSocks.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QXmppSocksClient[] = {

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
      18,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      26,   17,   17,   17, 0x08,
      42,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QXmppSocksClient[] = {
    "QXmppSocksClient\0\0ready()\0slotConnected()\0"
    "slotReadyRead()\0"
};

void QXmppSocksClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppSocksClient *_t = static_cast<QXmppSocksClient *>(_o);
        switch (_id) {
        case 0: _t->ready(); break;
        case 1: _t->slotConnected(); break;
        case 2: _t->slotReadyRead(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QXmppSocksClient::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppSocksClient::staticMetaObject = {
    { &QTcpSocket::staticMetaObject, qt_meta_stringdata_QXmppSocksClient,
      qt_meta_data_QXmppSocksClient, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppSocksClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppSocksClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppSocksClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppSocksClient))
        return static_cast<void*>(const_cast< QXmppSocksClient*>(this));
    return QTcpSocket::qt_metacast(_clname);
}

int QXmppSocksClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
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
void QXmppSocksClient::ready()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_QXmppSocksServer[] = {

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
      39,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      82,   17,   17,   17, 0x08,
     102,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QXmppSocksServer[] = {
    "QXmppSocksServer\0\0socket,hostName,port\0"
    "newConnection(QTcpSocket*,QString,quint16)\0"
    "slotNewConnection()\0slotReadyRead()\0"
};

void QXmppSocksServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppSocksServer *_t = static_cast<QXmppSocksServer *>(_o);
        switch (_id) {
        case 0: _t->newConnection((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        case 1: _t->slotNewConnection(); break;
        case 2: _t->slotReadyRead(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QXmppSocksServer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppSocksServer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QXmppSocksServer,
      qt_meta_data_QXmppSocksServer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppSocksServer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppSocksServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppSocksServer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppSocksServer))
        return static_cast<void*>(const_cast< QXmppSocksServer*>(this));
    return QObject::qt_metacast(_clname);
}

int QXmppSocksServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void QXmppSocksServer::newConnection(QTcpSocket * _t1, QString _t2, quint16 _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
