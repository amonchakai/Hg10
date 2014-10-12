/****************************************************************************
** Meta object code from reading C++ file 'QXmppCallManager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/client/QXmppCallManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QXmppCallManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QXmppCall[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       5,   79, // properties
       2,   99, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      23,   10,   10,   10, 0x05,
      34,   10,   10,   10, 0x05,
      50,   44,   10,   10, 0x05,
      86,   81,   10,   10, 0x05,
     124,   81,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     162,   10,   10,   10, 0x0a,
     171,   10,   10,   10, 0x0a,
     180,   10,   10,   10, 0x0a,
     193,   10,   10,   10, 0x0a,
     205,   10,   10,   10, 0x08,
     230,   10,   10,   10, 0x08,
     243,   10,   10,   10, 0x08,

 // properties: name, type, flags
     270,  260, 0x00095409,
     288,  280, 0x0a095401,
      44,  292, 0x00495009,
     318,  298, 0x00495009,
     328,  298, 0x00495009,

 // properties: notify_signal_id
       0,
       0,
       3,
       4,
       5,

 // enums: name, flags, count, data
     260, 0x0,    2,  107,
     292, 0x0,    4,  111,

 // enum data: key, value
     338, uint(QXmppCall::IncomingDirection),
     356, uint(QXmppCall::OutgoingDirection),
     374, uint(QXmppCall::ConnectingState),
     390, uint(QXmppCall::ActiveState),
     402, uint(QXmppCall::DisconnectingState),
     421, uint(QXmppCall::FinishedState),

       0        // eod
};

static const char qt_meta_stringdata_QXmppCall[] = {
    "QXmppCall\0\0connected()\0finished()\0"
    "ringing()\0state\0stateChanged(QXmppCall::State)\0"
    "mode\0audioModeChanged(QIODevice::OpenMode)\0"
    "videoModeChanged(QIODevice::OpenMode)\0"
    "accept()\0hangup()\0startVideo()\0"
    "stopVideo()\0localCandidatesChanged()\0"
    "terminated()\0updateOpenMode()\0Direction\0"
    "direction\0QString\0jid\0State\0"
    "QIODevice::OpenMode\0audioMode\0videoMode\0"
    "IncomingDirection\0OutgoingDirection\0"
    "ConnectingState\0ActiveState\0"
    "DisconnectingState\0FinishedState\0"
};

void QXmppCall::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppCall *_t = static_cast<QXmppCall *>(_o);
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->finished(); break;
        case 2: _t->ringing(); break;
        case 3: _t->stateChanged((*reinterpret_cast< QXmppCall::State(*)>(_a[1]))); break;
        case 4: _t->audioModeChanged((*reinterpret_cast< QIODevice::OpenMode(*)>(_a[1]))); break;
        case 5: _t->videoModeChanged((*reinterpret_cast< QIODevice::OpenMode(*)>(_a[1]))); break;
        case 6: _t->accept(); break;
        case 7: _t->hangup(); break;
        case 8: _t->startVideo(); break;
        case 9: _t->stopVideo(); break;
        case 10: _t->localCandidatesChanged(); break;
        case 11: _t->terminated(); break;
        case 12: _t->updateOpenMode(); break;
        default: ;
        }
    }
}

#ifdef Q_NO_DATA_RELOCATION
static const QMetaObjectAccessor qt_meta_extradata_QXmppCall[] = {
        QIODevice::getStaticMetaObject,
#else
static const QMetaObject *qt_meta_extradata_QXmppCall[] = {
        &QIODevice::staticMetaObject,
#endif //Q_NO_DATA_RELOCATION
    0
};

const QMetaObjectExtraData QXmppCall::staticMetaObjectExtraData = {
    qt_meta_extradata_QXmppCall,  qt_static_metacall 
};

const QMetaObject QXmppCall::staticMetaObject = {
    { &QXmppLoggable::staticMetaObject, qt_meta_stringdata_QXmppCall,
      qt_meta_data_QXmppCall, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppCall::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppCall::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppCall::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppCall))
        return static_cast<void*>(const_cast< QXmppCall*>(this));
    return QXmppLoggable::qt_metacast(_clname);
}

int QXmppCall::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QXmppLoggable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< Direction*>(_v) = direction(); break;
        case 1: *reinterpret_cast< QString*>(_v) = jid(); break;
        case 2: *reinterpret_cast< State*>(_v) = state(); break;
        case 3: *reinterpret_cast<int*>(_v) = QFlag(audioMode()); break;
        case 4: *reinterpret_cast<int*>(_v) = QFlag(videoMode()); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 5;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QXmppCall::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QXmppCall::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QXmppCall::ringing()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void QXmppCall::stateChanged(QXmppCall::State _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QXmppCall::audioModeChanged(QIODevice::OpenMode _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QXmppCall::videoModeChanged(QIODevice::OpenMode _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
static const uint qt_meta_data_QXmppCallManager[] = {

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
      23,   18,   17,   17, 0x05,
      48,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      87,   83,   72,   17, 0x0a,
     108,  101,   17,   17, 0x08,
     135,   17,   17,   17, 0x08,
     156,  153,   17,   17, 0x08,
     179,  153,   17,   17, 0x08,
     223,  214,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QXmppCallManager[] = {
    "QXmppCallManager\0\0call\0callReceived(QXmppCall*)\0"
    "callStarted(QXmppCall*)\0QXmppCall*\0"
    "jid\0call(QString)\0object\0"
    "_q_callDestroyed(QObject*)\0_q_disconnected()\0"
    "iq\0_q_iqReceived(QXmppIq)\0"
    "_q_jingleIqReceived(QXmppJingleIq)\0"
    "presence\0_q_presenceReceived(QXmppPresence)\0"
};

void QXmppCallManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppCallManager *_t = static_cast<QXmppCallManager *>(_o);
        switch (_id) {
        case 0: _t->callReceived((*reinterpret_cast< QXmppCall*(*)>(_a[1]))); break;
        case 1: _t->callStarted((*reinterpret_cast< QXmppCall*(*)>(_a[1]))); break;
        case 2: { QXmppCall* _r = _t->call((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QXmppCall**>(_a[0]) = _r; }  break;
        case 3: _t->_q_callDestroyed((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 4: _t->_q_disconnected(); break;
        case 5: _t->_q_iqReceived((*reinterpret_cast< const QXmppIq(*)>(_a[1]))); break;
        case 6: _t->_q_jingleIqReceived((*reinterpret_cast< const QXmppJingleIq(*)>(_a[1]))); break;
        case 7: _t->_q_presenceReceived((*reinterpret_cast< const QXmppPresence(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QXmppCallManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppCallManager::staticMetaObject = {
    { &QXmppClientExtension::staticMetaObject, qt_meta_stringdata_QXmppCallManager,
      qt_meta_data_QXmppCallManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppCallManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppCallManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppCallManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppCallManager))
        return static_cast<void*>(const_cast< QXmppCallManager*>(this));
    return QXmppClientExtension::qt_metacast(_clname);
}

int QXmppCallManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QXmppClientExtension::qt_metacall(_c, _id, _a);
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
void QXmppCallManager::callReceived(QXmppCall * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QXmppCallManager::callStarted(QXmppCall * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
