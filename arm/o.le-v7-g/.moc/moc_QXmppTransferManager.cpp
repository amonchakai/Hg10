/****************************************************************************
** Meta object code from reading C++ file 'QXmppTransferManager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/client/QXmppTransferManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QXmppTransferManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QXmppTransferJob[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       7,   59, // properties
       5,   87, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   18,   17,   17, 0x05,
      55,   17,   17,   17, 0x05,
      79,   66,   17,   17, 0x05,
     116,  105,   17,   17, 0x05,
     146,  140,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     184,   17,   17,   17, 0x0a,
     201,  192,   17,   17, 0x0a,
     224,  217,   17,   17, 0x0a,
     243,   17,   17,   17, 0x08,

 // properties: name, type, flags
     269,  259, 0x00095409,
      66,  279, 0x11495103,
     292,  284, 0x0a095401,
     303,  296, 0x00095409,
     140,  310, 0x00495009,
     316,  284, 0x0a095401,
     332,  325, 0x04095401,

 // properties: notify_signal_id
       0,
       2,
       0,
       0,
       4,
       0,
       0,

 // enums: name, flags, count, data
     259, 0x0,    2,  107,
     341, 0x0,    5,  111,
     296, 0x1,    4,  121,
     347, 0x1,    4,  129,
     310, 0x0,    4,  137,

 // enum data: key, value
     355, uint(QXmppTransferJob::IncomingDirection),
     373, uint(QXmppTransferJob::OutgoingDirection),
     391, uint(QXmppTransferJob::NoError),
     399, uint(QXmppTransferJob::AbortError),
     410, uint(QXmppTransferJob::FileAccessError),
     426, uint(QXmppTransferJob::FileCorruptError),
     443, uint(QXmppTransferJob::ProtocolError),
     457, uint(QXmppTransferJob::NoMethod),
     466, uint(QXmppTransferJob::InBandMethod),
     479, uint(QXmppTransferJob::SocksMethod),
     491, uint(QXmppTransferJob::AnyMethod),
     457, uint(QXmppTransferJob::NoMethod),
     466, uint(QXmppTransferJob::InBandMethod),
     479, uint(QXmppTransferJob::SocksMethod),
     491, uint(QXmppTransferJob::AnyMethod),
     501, uint(QXmppTransferJob::OfferState),
     512, uint(QXmppTransferJob::StartState),
     523, uint(QXmppTransferJob::TransferState),
     537, uint(QXmppTransferJob::FinishedState),

       0        // eod
};

static const char qt_meta_stringdata_QXmppTransferJob[] = {
    "QXmppTransferJob\0\0error\0"
    "error(QXmppTransferJob::Error)\0"
    "finished()\0localFileUrl\0"
    "localFileUrlChanged(QUrl)\0done,total\0"
    "progress(qint64,qint64)\0state\0"
    "stateChanged(QXmppTransferJob::State)\0"
    "abort()\0filePath\0accept(QString)\0"
    "output\0accept(QIODevice*)\0_q_terminated()\0"
    "Direction\0direction\0QUrl\0QString\0jid\0"
    "Method\0method\0State\0fileName\0qint64\0"
    "fileSize\0Error\0Methods\0IncomingDirection\0"
    "OutgoingDirection\0NoError\0AbortError\0"
    "FileAccessError\0FileCorruptError\0"
    "ProtocolError\0NoMethod\0InBandMethod\0"
    "SocksMethod\0AnyMethod\0OfferState\0"
    "StartState\0TransferState\0FinishedState\0"
};

void QXmppTransferJob::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppTransferJob *_t = static_cast<QXmppTransferJob *>(_o);
        switch (_id) {
        case 0: _t->error((*reinterpret_cast< QXmppTransferJob::Error(*)>(_a[1]))); break;
        case 1: _t->finished(); break;
        case 2: _t->localFileUrlChanged((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 3: _t->progress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 4: _t->stateChanged((*reinterpret_cast< QXmppTransferJob::State(*)>(_a[1]))); break;
        case 5: _t->abort(); break;
        case 6: _t->accept((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->accept((*reinterpret_cast< QIODevice*(*)>(_a[1]))); break;
        case 8: _t->_q_terminated(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QXmppTransferJob::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppTransferJob::staticMetaObject = {
    { &QXmppLoggable::staticMetaObject, qt_meta_stringdata_QXmppTransferJob,
      qt_meta_data_QXmppTransferJob, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppTransferJob::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppTransferJob::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppTransferJob::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppTransferJob))
        return static_cast<void*>(const_cast< QXmppTransferJob*>(this));
    return QXmppLoggable::qt_metacast(_clname);
}

int QXmppTransferJob::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QXmppLoggable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< Direction*>(_v) = direction(); break;
        case 1: *reinterpret_cast< QUrl*>(_v) = localFileUrl(); break;
        case 2: *reinterpret_cast< QString*>(_v) = jid(); break;
        case 3: *reinterpret_cast<int*>(_v) = QFlag(method()); break;
        case 4: *reinterpret_cast< State*>(_v) = state(); break;
        case 5: *reinterpret_cast< QString*>(_v) = fileName(); break;
        case 6: *reinterpret_cast< qint64*>(_v) = fileSize(); break;
        }
        _id -= 7;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 1: setLocalFileUrl(*reinterpret_cast< QUrl*>(_v)); break;
        }
        _id -= 7;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 7;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QXmppTransferJob::error(QXmppTransferJob::Error _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QXmppTransferJob::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QXmppTransferJob::localFileUrlChanged(const QUrl & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QXmppTransferJob::progress(qint64 _t1, qint64 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QXmppTransferJob::stateChanged(QXmppTransferJob::State _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
static const uint qt_meta_data_QXmppTransferManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       3,   79, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   22,   21,   21, 0x05,
      58,   22,   21,   21, 0x05,
      88,   22,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
     162,  137,  119,   21, 0x0a,
     209,  196,  119,   21, 0x2a,
     259,  235,  119,   21, 0x0a,
     338,  318,  119,   21, 0x2a,
     389,   21,   21,   21, 0x08,
     419,  412,   21,   21, 0x08,
     451,  445,   21,   21, 0x08,
     488,   21,   21,   21, 0x08,
     511,  505,   21,   21, 0x08,
     576,  555,   21,   21, 0x08,

 // properties: name, type, flags
     637,  629, 0x0a095103,
     648,  643, 0x01095103,
     684,  658, 0x0009510b,

       0        // eod
};

static const char qt_meta_stringdata_QXmppTransferManager[] = {
    "QXmppTransferManager\0\0job\0"
    "fileReceived(QXmppTransferJob*)\0"
    "jobStarted(QXmppTransferJob*)\0"
    "jobFinished(QXmppTransferJob*)\0"
    "QXmppTransferJob*\0jid,filePath,description\0"
    "sendFile(QString,QString,QString)\0"
    "jid,filePath\0sendFile(QString,QString)\0"
    "jid,device,fileInfo,sid\0"
    "sendFile(QString,QIODevice*,QXmppTransferFileInfo,QString)\0"
    "jid,device,fileInfo\0"
    "sendFile(QString,QIODevice*,QXmppTransferFileInfo)\0"
    "_q_iqReceived(QXmppIq)\0object\0"
    "_q_jobDestroyed(QObject*)\0error\0"
    "_q_jobError(QXmppTransferJob::Error)\0"
    "_q_jobFinished()\0state\0"
    "_q_jobStateChanged(QXmppTransferJob::State)\0"
    "socket,hostName,port\0"
    "_q_socksServerConnected(QTcpSocket*,QString,quint16)\0"
    "QString\0proxy\0bool\0proxyOnly\0"
    "QXmppTransferJob::Methods\0supportedMethods\0"
};

void QXmppTransferManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppTransferManager *_t = static_cast<QXmppTransferManager *>(_o);
        switch (_id) {
        case 0: _t->fileReceived((*reinterpret_cast< QXmppTransferJob*(*)>(_a[1]))); break;
        case 1: _t->jobStarted((*reinterpret_cast< QXmppTransferJob*(*)>(_a[1]))); break;
        case 2: _t->jobFinished((*reinterpret_cast< QXmppTransferJob*(*)>(_a[1]))); break;
        case 3: { QXmppTransferJob* _r = _t->sendFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QXmppTransferJob**>(_a[0]) = _r; }  break;
        case 4: { QXmppTransferJob* _r = _t->sendFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QXmppTransferJob**>(_a[0]) = _r; }  break;
        case 5: { QXmppTransferJob* _r = _t->sendFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QIODevice*(*)>(_a[2])),(*reinterpret_cast< const QXmppTransferFileInfo(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QXmppTransferJob**>(_a[0]) = _r; }  break;
        case 6: { QXmppTransferJob* _r = _t->sendFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QIODevice*(*)>(_a[2])),(*reinterpret_cast< const QXmppTransferFileInfo(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QXmppTransferJob**>(_a[0]) = _r; }  break;
        case 7: _t->_q_iqReceived((*reinterpret_cast< const QXmppIq(*)>(_a[1]))); break;
        case 8: _t->_q_jobDestroyed((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 9: _t->_q_jobError((*reinterpret_cast< QXmppTransferJob::Error(*)>(_a[1]))); break;
        case 10: _t->_q_jobFinished(); break;
        case 11: _t->_q_jobStateChanged((*reinterpret_cast< QXmppTransferJob::State(*)>(_a[1]))); break;
        case 12: _t->_q_socksServerConnected((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        default: ;
        }
    }
}

#ifdef Q_NO_DATA_RELOCATION
static const QMetaObjectAccessor qt_meta_extradata_QXmppTransferManager[] = {
        QXmppTransferJob::getStaticMetaObject,
#else
static const QMetaObject *qt_meta_extradata_QXmppTransferManager[] = {
        &QXmppTransferJob::staticMetaObject,
#endif //Q_NO_DATA_RELOCATION
    0
};

const QMetaObjectExtraData QXmppTransferManager::staticMetaObjectExtraData = {
    qt_meta_extradata_QXmppTransferManager,  qt_static_metacall 
};

const QMetaObject QXmppTransferManager::staticMetaObject = {
    { &QXmppClientExtension::staticMetaObject, qt_meta_stringdata_QXmppTransferManager,
      qt_meta_data_QXmppTransferManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppTransferManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppTransferManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppTransferManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppTransferManager))
        return static_cast<void*>(const_cast< QXmppTransferManager*>(this));
    return QXmppClientExtension::qt_metacast(_clname);
}

int QXmppTransferManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QXmppClientExtension::qt_metacall(_c, _id, _a);
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
        case 0: *reinterpret_cast< QString*>(_v) = proxy(); break;
        case 1: *reinterpret_cast< bool*>(_v) = proxyOnly(); break;
        case 2: *reinterpret_cast< QXmppTransferJob::Methods*>(_v) = supportedMethods(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setProxy(*reinterpret_cast< QString*>(_v)); break;
        case 1: setProxyOnly(*reinterpret_cast< bool*>(_v)); break;
        case 2: setSupportedMethods(*reinterpret_cast< QXmppTransferJob::Methods*>(_v)); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QXmppTransferManager::fileReceived(QXmppTransferJob * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QXmppTransferManager::jobStarted(QXmppTransferJob * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QXmppTransferManager::jobFinished(QXmppTransferJob * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
