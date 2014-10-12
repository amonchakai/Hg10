/****************************************************************************
** Meta object code from reading C++ file 'QXmppStream.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/base/QXmppStream.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QXmppStream.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QXmppStream[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      25,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      40,   12,   12,   12, 0x0a,
      66,   12,   61,   12, 0x0a,
      87,   12,   12,   12, 0x08,
     108,   12,   12,   12, 0x08,
     135,  129,   12,   12, 0x08,
     180,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QXmppStream[] = {
    "QXmppStream\0\0connected()\0disconnected()\0"
    "disconnectFromHost()\0bool\0"
    "sendData(QByteArray)\0_q_socketConnected()\0"
    "_q_socketEncrypted()\0error\0"
    "_q_socketError(QAbstractSocket::SocketError)\0"
    "_q_socketReadyRead()\0"
};

void QXmppStream::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppStream *_t = static_cast<QXmppStream *>(_o);
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->disconnectFromHost(); break;
        case 3: { bool _r = _t->sendData((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: _t->_q_socketConnected(); break;
        case 5: _t->_q_socketEncrypted(); break;
        case 6: _t->_q_socketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 7: _t->_q_socketReadyRead(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QXmppStream::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppStream::staticMetaObject = {
    { &QXmppLoggable::staticMetaObject, qt_meta_stringdata_QXmppStream,
      qt_meta_data_QXmppStream, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppStream::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppStream::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppStream::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppStream))
        return static_cast<void*>(const_cast< QXmppStream*>(this));
    return QXmppLoggable::qt_metacast(_clname);
}

int QXmppStream::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QXmppLoggable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void QXmppStream::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QXmppStream::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
