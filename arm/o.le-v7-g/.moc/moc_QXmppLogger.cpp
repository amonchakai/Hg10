/****************************************************************************
** Meta object code from reading C++ file 'QXmppLogger.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/base/QXmppLogger.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QXmppLogger.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QXmppLogger[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       3,   39, // properties
       3,   48, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   13,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      77,   65,   12,   12, 0x0a,
     117,  102,   12,   12, 0x0a,
     147,   13,   12,   12, 0x0a,
     185,   12,   12,   12, 0x0a,

 // properties: name, type, flags
     202,  194, 0x0a095103,
     226,  214, 0x0009510b,
     251,  238, 0x0009510b,

 // enums: name, flags, count, data
     214, 0x0,    4,   60,
     264, 0x1,    7,   68,
     238, 0x1,    7,   82,

 // enum data: key, value
     276, uint(QXmppLogger::NoLogging),
     286, uint(QXmppLogger::FileLogging),
     298, uint(QXmppLogger::StdoutLogging),
     312, uint(QXmppLogger::SignalLogging),
     326, uint(QXmppLogger::NoMessage),
     336, uint(QXmppLogger::DebugMessage),
     349, uint(QXmppLogger::InformationMessage),
     368, uint(QXmppLogger::WarningMessage),
     383, uint(QXmppLogger::ReceivedMessage),
     399, uint(QXmppLogger::SentMessage),
     411, uint(QXmppLogger::AnyMessage),
     326, uint(QXmppLogger::NoMessage),
     336, uint(QXmppLogger::DebugMessage),
     349, uint(QXmppLogger::InformationMessage),
     368, uint(QXmppLogger::WarningMessage),
     383, uint(QXmppLogger::ReceivedMessage),
     399, uint(QXmppLogger::SentMessage),
     411, uint(QXmppLogger::AnyMessage),

       0        // eod
};

static const char qt_meta_stringdata_QXmppLogger[] = {
    "QXmppLogger\0\0type,text\0"
    "message(QXmppLogger::MessageType,QString)\0"
    "gauge,value\0setGauge(QString,double)\0"
    "counter,amount\0updateCounter(QString,qint64)\0"
    "log(QXmppLogger::MessageType,QString)\0"
    "reopen()\0QString\0logFilePath\0LoggingType\0"
    "loggingType\0MessageTypes\0messageTypes\0"
    "MessageType\0NoLogging\0FileLogging\0"
    "StdoutLogging\0SignalLogging\0NoMessage\0"
    "DebugMessage\0InformationMessage\0"
    "WarningMessage\0ReceivedMessage\0"
    "SentMessage\0AnyMessage\0"
};

void QXmppLogger::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppLogger *_t = static_cast<QXmppLogger *>(_o);
        switch (_id) {
        case 0: _t->message((*reinterpret_cast< QXmppLogger::MessageType(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->setGauge((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 2: _t->updateCounter((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 3: _t->log((*reinterpret_cast< QXmppLogger::MessageType(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->reopen(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QXmppLogger::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppLogger::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QXmppLogger,
      qt_meta_data_QXmppLogger, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppLogger::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppLogger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppLogger::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppLogger))
        return static_cast<void*>(const_cast< QXmppLogger*>(this));
    return QObject::qt_metacast(_clname);
}

int QXmppLogger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = logFilePath(); break;
        case 1: *reinterpret_cast< LoggingType*>(_v) = loggingType(); break;
        case 2: *reinterpret_cast<int*>(_v) = QFlag(messageTypes()); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setLogFilePath(*reinterpret_cast< QString*>(_v)); break;
        case 1: setLoggingType(*reinterpret_cast< LoggingType*>(_v)); break;
        case 2: setMessageTypes(QFlag(*reinterpret_cast<int*>(_v))); break;
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
void QXmppLogger::message(QXmppLogger::MessageType _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_QXmppLoggable[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      27,   15,   14,   14, 0x05,
      61,   52,   14,   14, 0x05,
     121,  106,   14,   14, 0x05,
     159,  151,   14,   14, 0x25,

       0        // eod
};

static const char qt_meta_stringdata_QXmppLoggable[] = {
    "QXmppLoggable\0\0gauge,value\0"
    "setGauge(QString,double)\0type,msg\0"
    "logMessage(QXmppLogger::MessageType,QString)\0"
    "counter,amount\0updateCounter(QString,qint64)\0"
    "counter\0updateCounter(QString)\0"
};

void QXmppLoggable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppLoggable *_t = static_cast<QXmppLoggable *>(_o);
        switch (_id) {
        case 0: _t->setGauge((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->logMessage((*reinterpret_cast< QXmppLogger::MessageType(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->updateCounter((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 3: _t->updateCounter((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QXmppLoggable::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppLoggable::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QXmppLoggable,
      qt_meta_data_QXmppLoggable, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppLoggable::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppLoggable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppLoggable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppLoggable))
        return static_cast<void*>(const_cast< QXmppLoggable*>(this));
    return QObject::qt_metacast(_clname);
}

int QXmppLoggable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void QXmppLoggable::setGauge(const QString & _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QXmppLoggable::logMessage(QXmppLogger::MessageType _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QXmppLoggable::updateCounter(const QString & _t1, qint64 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
