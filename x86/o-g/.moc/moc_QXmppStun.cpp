/****************************************************************************
** Meta object code from reading C++ file 'QXmppStun.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/base/QXmppStun.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QXmppStun.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QXmppStunTransaction[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x05,
      41,   33,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
      78,   69,   21,   21, 0x0a,
     105,   21,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QXmppStunTransaction[] = {
    "QXmppStunTransaction\0\0finished()\0"
    "request\0writeStun(QXmppStunMessage)\0"
    "response\0readStun(QXmppStunMessage)\0"
    "retry()\0"
};

void QXmppStunTransaction::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppStunTransaction *_t = static_cast<QXmppStunTransaction *>(_o);
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->writeStun((*reinterpret_cast< const QXmppStunMessage(*)>(_a[1]))); break;
        case 2: _t->readStun((*reinterpret_cast< const QXmppStunMessage(*)>(_a[1]))); break;
        case 3: _t->retry(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QXmppStunTransaction::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppStunTransaction::staticMetaObject = {
    { &QXmppLoggable::staticMetaObject, qt_meta_stringdata_QXmppStunTransaction,
      qt_meta_data_QXmppStunTransaction, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppStunTransaction::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppStunTransaction::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppStunTransaction::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppStunTransaction))
        return static_cast<void*>(const_cast< QXmppStunTransaction*>(this));
    return QXmppLoggable::qt_metacast(_clname);
}

int QXmppStunTransaction::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QXmppLoggable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void QXmppStunTransaction::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QXmppStunTransaction::writeStun(const QXmppStunMessage & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_QXmppTurnAllocation[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x05,
      48,   33,   20,   20, 0x05,
      98,   20,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
     113,   20,   20,   20, 0x0a,
     129,   20,   20,   20, 0x0a,
     150,   20,   20,   20, 0x08,
     162,   20,   20,   20, 0x08,
     172,   20,   20,   20, 0x08,
     190,   20,   20,   20, 0x08,
     220,  212,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QXmppTurnAllocation[] = {
    "QXmppTurnAllocation\0\0connected()\0"
    "data,host,port\0"
    "datagramReceived(QByteArray,QHostAddress,quint16)\0"
    "disconnected()\0connectToHost()\0"
    "disconnectFromHost()\0readyRead()\0"
    "refresh()\0refreshChannels()\0"
    "transactionFinished()\0message\0"
    "writeStun(QXmppStunMessage)\0"
};

void QXmppTurnAllocation::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppTurnAllocation *_t = static_cast<QXmppTurnAllocation *>(_o);
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->datagramReceived((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QHostAddress(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        case 2: _t->disconnected(); break;
        case 3: _t->connectToHost(); break;
        case 4: _t->disconnectFromHost(); break;
        case 5: _t->readyRead(); break;
        case 6: _t->refresh(); break;
        case 7: _t->refreshChannels(); break;
        case 8: _t->transactionFinished(); break;
        case 9: _t->writeStun((*reinterpret_cast< const QXmppStunMessage(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QXmppTurnAllocation::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppTurnAllocation::staticMetaObject = {
    { &QXmppLoggable::staticMetaObject, qt_meta_stringdata_QXmppTurnAllocation,
      qt_meta_data_QXmppTurnAllocation, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppTurnAllocation::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppTurnAllocation::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppTurnAllocation::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppTurnAllocation))
        return static_cast<void*>(const_cast< QXmppTurnAllocation*>(this));
    return QXmppLoggable::qt_metacast(_clname);
}

int QXmppTurnAllocation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QXmppLoggable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void QXmppTurnAllocation::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QXmppTurnAllocation::datagramReceived(const QByteArray & _t1, const QHostAddress & _t2, quint16 _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QXmppTurnAllocation::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
static const uint qt_meta_data_QXmppIceComponent[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      40,   31,   18,   18, 0x05,
      69,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      94,   18,   18,   18, 0x0a,
     102,   18,   18,   18, 0x0a,
     125,   31,  118,   18, 0x0a,
     150,   18,   18,   18, 0x08,
     168,   18,   18,   18, 0x08,
     206,  180,   18,   18, 0x08,
     285,  266,   18,   18, 0x28,
     333,   18,   18,   18, 0x08,
     345,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QXmppIceComponent[] = {
    "QXmppIceComponent\0\0connected()\0datagram\0"
    "datagramReceived(QByteArray)\0"
    "localCandidatesChanged()\0close()\0"
    "connectToHost()\0qint64\0sendDatagram(QByteArray)\0"
    "checkCandidates()\0checkStun()\0"
    "datagram,host,port,socket\0"
    "handleDatagram(QByteArray,QHostAddress,quint16,QUdpSocket*)\0"
    "datagram,host,port\0"
    "handleDatagram(QByteArray,QHostAddress,quint16)\0"
    "readyRead()\0turnConnected()\0"
};

void QXmppIceComponent::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppIceComponent *_t = static_cast<QXmppIceComponent *>(_o);
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->datagramReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 2: _t->localCandidatesChanged(); break;
        case 3: _t->close(); break;
        case 4: _t->connectToHost(); break;
        case 5: { qint64 _r = _t->sendDatagram((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< qint64*>(_a[0]) = _r; }  break;
        case 6: _t->checkCandidates(); break;
        case 7: _t->checkStun(); break;
        case 8: _t->handleDatagram((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QHostAddress(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3])),(*reinterpret_cast< QUdpSocket*(*)>(_a[4]))); break;
        case 9: _t->handleDatagram((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QHostAddress(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        case 10: _t->readyRead(); break;
        case 11: _t->turnConnected(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QXmppIceComponent::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppIceComponent::staticMetaObject = {
    { &QXmppLoggable::staticMetaObject, qt_meta_stringdata_QXmppIceComponent,
      qt_meta_data_QXmppIceComponent, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppIceComponent::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppIceComponent::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppIceComponent::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppIceComponent))
        return static_cast<void*>(const_cast< QXmppIceComponent*>(this));
    return QXmppLoggable::qt_metacast(_clname);
}

int QXmppIceComponent::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QXmppLoggable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void QXmppIceComponent::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QXmppIceComponent::datagramReceived(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QXmppIceComponent::localCandidatesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
static const uint qt_meta_data_QXmppIceConnection[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,
      32,   19,   19,   19, 0x05,
      47,   19,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      72,   19,   19,   19, 0x0a,
      80,   19,   19,   19, 0x0a,
      96,   19,   19,   19, 0x08,
     112,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QXmppIceConnection[] = {
    "QXmppIceConnection\0\0connected()\0"
    "disconnected()\0localCandidatesChanged()\0"
    "close()\0connectToHost()\0slotConnected()\0"
    "slotTimeout()\0"
};

void QXmppIceConnection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppIceConnection *_t = static_cast<QXmppIceConnection *>(_o);
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->localCandidatesChanged(); break;
        case 3: _t->close(); break;
        case 4: _t->connectToHost(); break;
        case 5: _t->slotConnected(); break;
        case 6: _t->slotTimeout(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QXmppIceConnection::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppIceConnection::staticMetaObject = {
    { &QXmppLoggable::staticMetaObject, qt_meta_stringdata_QXmppIceConnection,
      qt_meta_data_QXmppIceConnection, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppIceConnection::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppIceConnection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppIceConnection::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppIceConnection))
        return static_cast<void*>(const_cast< QXmppIceConnection*>(this));
    return QXmppLoggable::qt_metacast(_clname);
}

int QXmppIceConnection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QXmppLoggable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void QXmppIceConnection::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QXmppIceConnection::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QXmppIceConnection::localCandidatesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
