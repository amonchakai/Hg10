/****************************************************************************
** Meta object code from reading C++ file 'QXmppRtpChannel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/base/QXmppRtpChannel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QXmppRtpChannel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QXmppRtpAudioChannel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       1,   49, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   22,   21,   21, 0x05,
      59,   50,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
     104,   22,   21,   21, 0x0a,
     138,  133,   21,   21, 0x0a,
     176,  133,   21,   21, 0x0a,
     213,   21,   21,   21, 0x08,
     227,   21,   21,   21, 0x08,

 // enums: name, flags, count, data
     243, 0x0,   16,   53,

 // enum data: key, value
     248, uint(QXmppRtpAudioChannel::Tone_0),
     255, uint(QXmppRtpAudioChannel::Tone_1),
     262, uint(QXmppRtpAudioChannel::Tone_2),
     269, uint(QXmppRtpAudioChannel::Tone_3),
     276, uint(QXmppRtpAudioChannel::Tone_4),
     283, uint(QXmppRtpAudioChannel::Tone_5),
     290, uint(QXmppRtpAudioChannel::Tone_6),
     297, uint(QXmppRtpAudioChannel::Tone_7),
     304, uint(QXmppRtpAudioChannel::Tone_8),
     311, uint(QXmppRtpAudioChannel::Tone_9),
     318, uint(QXmppRtpAudioChannel::Tone_Star),
     328, uint(QXmppRtpAudioChannel::Tone_Pound),
     339, uint(QXmppRtpAudioChannel::Tone_A),
     346, uint(QXmppRtpAudioChannel::Tone_B),
     353, uint(QXmppRtpAudioChannel::Tone_C),
     360, uint(QXmppRtpAudioChannel::Tone_D),

       0        // eod
};

static const char qt_meta_stringdata_QXmppRtpAudioChannel[] = {
    "QXmppRtpAudioChannel\0\0ba\0"
    "sendDatagram(QByteArray)\0type,msg\0"
    "logMessage(QXmppLogger::MessageType,QString)\0"
    "datagramReceived(QByteArray)\0tone\0"
    "startTone(QXmppRtpAudioChannel::Tone)\0"
    "stopTone(QXmppRtpAudioChannel::Tone)\0"
    "emitSignals()\0writeDatagram()\0Tone\0"
    "Tone_0\0Tone_1\0Tone_2\0Tone_3\0Tone_4\0"
    "Tone_5\0Tone_6\0Tone_7\0Tone_8\0Tone_9\0"
    "Tone_Star\0Tone_Pound\0Tone_A\0Tone_B\0"
    "Tone_C\0Tone_D\0"
};

void QXmppRtpAudioChannel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppRtpAudioChannel *_t = static_cast<QXmppRtpAudioChannel *>(_o);
        switch (_id) {
        case 0: _t->sendDatagram((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->logMessage((*reinterpret_cast< QXmppLogger::MessageType(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->datagramReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 3: _t->startTone((*reinterpret_cast< QXmppRtpAudioChannel::Tone(*)>(_a[1]))); break;
        case 4: _t->stopTone((*reinterpret_cast< QXmppRtpAudioChannel::Tone(*)>(_a[1]))); break;
        case 5: _t->emitSignals(); break;
        case 6: _t->writeDatagram(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QXmppRtpAudioChannel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppRtpAudioChannel::staticMetaObject = {
    { &QIODevice::staticMetaObject, qt_meta_stringdata_QXmppRtpAudioChannel,
      qt_meta_data_QXmppRtpAudioChannel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppRtpAudioChannel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppRtpAudioChannel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppRtpAudioChannel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppRtpAudioChannel))
        return static_cast<void*>(const_cast< QXmppRtpAudioChannel*>(this));
    if (!strcmp(_clname, "QXmppRtpChannel"))
        return static_cast< QXmppRtpChannel*>(const_cast< QXmppRtpAudioChannel*>(this));
    return QIODevice::qt_metacast(_clname);
}

int QXmppRtpAudioChannel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QIODevice::qt_metacall(_c, _id, _a);
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
void QXmppRtpAudioChannel::sendDatagram(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QXmppRtpAudioChannel::logMessage(QXmppLogger::MessageType _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_QXmppRtpVideoChannel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   22,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   22,   21,   21, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QXmppRtpVideoChannel[] = {
    "QXmppRtpVideoChannel\0\0ba\0"
    "sendDatagram(QByteArray)\0"
    "datagramReceived(QByteArray)\0"
};

void QXmppRtpVideoChannel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppRtpVideoChannel *_t = static_cast<QXmppRtpVideoChannel *>(_o);
        switch (_id) {
        case 0: _t->sendDatagram((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->datagramReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QXmppRtpVideoChannel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppRtpVideoChannel::staticMetaObject = {
    { &QXmppLoggable::staticMetaObject, qt_meta_stringdata_QXmppRtpVideoChannel,
      qt_meta_data_QXmppRtpVideoChannel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppRtpVideoChannel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppRtpVideoChannel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppRtpVideoChannel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppRtpVideoChannel))
        return static_cast<void*>(const_cast< QXmppRtpVideoChannel*>(this));
    if (!strcmp(_clname, "QXmppRtpChannel"))
        return static_cast< QXmppRtpChannel*>(const_cast< QXmppRtpVideoChannel*>(this));
    return QXmppLoggable::qt_metacast(_clname);
}

int QXmppRtpVideoChannel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QXmppLoggable::qt_metacall(_c, _id, _a);
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
void QXmppRtpVideoChannel::sendDatagram(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
