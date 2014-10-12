/****************************************************************************
** Meta object code from reading C++ file 'QXmppClient.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/client/QXmppClient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QXmppClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QXmppClient[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       2,  104, // properties
       2,  112, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      25,   12,   12,   12, 0x05,
      40,   12,   12,   12, 0x05,
      73,   66,   12,   12, 0x05,
     109,  101,   12,   12, 0x05,
     148,  139,   12,   12, 0x05,
     183,  180,   12,   12, 0x05,
     209,  203,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     255,  242,   12,   12, 0x0a,
     288,   12,   12,   12, 0x0a,
     316,   12,  311,   12, 0x0a,
     356,  340,   12,   12, 0x0a,
     401,  385,   12,   12, 0x08,
     439,   12,   12,   12, 0x08,
     454,  203,   12,   12, 0x08,
     506,   12,   12,   12, 0x08,
     527,   12,   12,   12, 0x08,
     557,  551,   12,   12, 0x08,

 // properties: name, type, flags
      66,  592, 0x0049510b,
     203,  605, 0x00495009,

 // properties: notify_signal_id
       3,
       7,

 // enums: name, flags, count, data
     611, 0x0,    4,  120,
     605, 0x0,    3,  128,

 // enum data: key, value
     617, uint(QXmppClient::NoError),
     625, uint(QXmppClient::SocketError),
     637, uint(QXmppClient::KeepAliveError),
     652, uint(QXmppClient::XmppStreamError),
     668, uint(QXmppClient::DisconnectedState),
     686, uint(QXmppClient::ConnectingState),
     702, uint(QXmppClient::ConnectedState),

       0        // eod
};

static const char qt_meta_stringdata_QXmppClient[] = {
    "QXmppClient\0\0connected()\0disconnected()\0"
    "error(QXmppClient::Error)\0logger\0"
    "loggerChanged(QXmppLogger*)\0message\0"
    "messageReceived(QXmppMessage)\0presence\0"
    "presenceReceived(QXmppPresence)\0iq\0"
    "iqReceived(QXmppIq)\0state\0"
    "stateChanged(QXmppClient::State)\0"
    "jid,password\0connectToServer(QString,QString)\0"
    "disconnectFromServer()\0bool\0"
    "sendPacket(QXmppStanza)\0bareJid,message\0"
    "sendMessage(QString,QString)\0"
    "element,handled\0_q_elementReceived(QDomElement,bool&)\0"
    "_q_reconnect()\0"
    "_q_socketStateChanged(QAbstractSocket::SocketState)\0"
    "_q_streamConnected()\0_q_streamDisconnected()\0"
    "error\0_q_streamError(QXmppClient::Error)\0"
    "QXmppLogger*\0State\0Error\0NoError\0"
    "SocketError\0KeepAliveError\0XmppStreamError\0"
    "DisconnectedState\0ConnectingState\0"
    "ConnectedState\0"
};

void QXmppClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppClient *_t = static_cast<QXmppClient *>(_o);
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->error((*reinterpret_cast< QXmppClient::Error(*)>(_a[1]))); break;
        case 3: _t->loggerChanged((*reinterpret_cast< QXmppLogger*(*)>(_a[1]))); break;
        case 4: _t->messageReceived((*reinterpret_cast< const QXmppMessage(*)>(_a[1]))); break;
        case 5: _t->presenceReceived((*reinterpret_cast< const QXmppPresence(*)>(_a[1]))); break;
        case 6: _t->iqReceived((*reinterpret_cast< const QXmppIq(*)>(_a[1]))); break;
        case 7: _t->stateChanged((*reinterpret_cast< QXmppClient::State(*)>(_a[1]))); break;
        case 8: _t->connectToServer((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 9: _t->disconnectFromServer(); break;
        case 10: { bool _r = _t->sendPacket((*reinterpret_cast< const QXmppStanza(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: _t->sendMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 12: _t->_q_elementReceived((*reinterpret_cast< const QDomElement(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 13: _t->_q_reconnect(); break;
        case 14: _t->_q_socketStateChanged((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        case 15: _t->_q_streamConnected(); break;
        case 16: _t->_q_streamDisconnected(); break;
        case 17: _t->_q_streamError((*reinterpret_cast< QXmppClient::Error(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QXmppClient::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppClient::staticMetaObject = {
    { &QXmppLoggable::staticMetaObject, qt_meta_stringdata_QXmppClient,
      qt_meta_data_QXmppClient, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppClient))
        return static_cast<void*>(const_cast< QXmppClient*>(this));
    return QXmppLoggable::qt_metacast(_clname);
}

int QXmppClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QXmppLoggable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QXmppLogger**>(_v) = logger(); break;
        case 1: *reinterpret_cast< State*>(_v) = state(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setLogger(*reinterpret_cast< QXmppLogger**>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
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
void QXmppClient::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QXmppClient::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QXmppClient::error(QXmppClient::Error _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QXmppClient::loggerChanged(QXmppLogger * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QXmppClient::messageReceived(const QXmppMessage & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QXmppClient::presenceReceived(const QXmppPresence & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QXmppClient::iqReceived(const QXmppIq & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void QXmppClient::stateChanged(QXmppClient::State _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
