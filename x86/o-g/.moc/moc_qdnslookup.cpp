/****************************************************************************
** Meta object code from reading C++ file 'qdnslookup.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/base/qdnslookup.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qdnslookup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QDnsLookup[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       4,   44, // properties
       2,   60, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      28,   23,   11,   11, 0x05,
      54,   49,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      72,   11,   11,   11, 0x0a,
      80,   11,   11,   11, 0x0a,
      95,   89,   11,   11, 0x08,

 // properties: name, type, flags
     136,  130, 0x00495009,
     150,  142, 0x0a495001,
      23,  142, 0x0a495103,
      49,  162, 0x0049510b,

 // properties: notify_signal_id
       0,
       0,
       1,
       2,

 // enums: name, flags, count, data
     130, 0x0,    8,   68,
     162, 0x0,    9,   84,

 // enum data: key, value
     167, uint(QDnsLookup::NoError),
     175, uint(QDnsLookup::ResolverError),
     189, uint(QDnsLookup::OperationCancelledError),
     213, uint(QDnsLookup::InvalidRequestError),
     233, uint(QDnsLookup::InvalidReplyError),
     251, uint(QDnsLookup::ServerFailureError),
     270, uint(QDnsLookup::ServerRefusedError),
     289, uint(QDnsLookup::NotFoundError),
     303, uint(QDnsLookup::A),
     305, uint(QDnsLookup::AAAA),
     310, uint(QDnsLookup::ANY),
     314, uint(QDnsLookup::CNAME),
     320, uint(QDnsLookup::MX),
     323, uint(QDnsLookup::NS),
     326, uint(QDnsLookup::PTR),
     330, uint(QDnsLookup::SRV),
     334, uint(QDnsLookup::TXT),

       0        // eod
};

static const char qt_meta_stringdata_QDnsLookup[] = {
    "QDnsLookup\0\0finished()\0name\0"
    "nameChanged(QString)\0type\0typeChanged(Type)\0"
    "abort()\0lookup()\0reply\0"
    "_q_lookupFinished(QDnsLookupReply)\0"
    "Error\0error\0QString\0errorString\0Type\0"
    "NoError\0ResolverError\0OperationCancelledError\0"
    "InvalidRequestError\0InvalidReplyError\0"
    "ServerFailureError\0ServerRefusedError\0"
    "NotFoundError\0A\0AAAA\0ANY\0CNAME\0MX\0NS\0"
    "PTR\0SRV\0TXT\0"
};

void QDnsLookup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QDnsLookup *_t = static_cast<QDnsLookup *>(_o);
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->nameChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->typeChanged((*reinterpret_cast< Type(*)>(_a[1]))); break;
        case 3: _t->abort(); break;
        case 4: _t->lookup(); break;
        case 5: _t->d_func()->_q_lookupFinished((*reinterpret_cast< const QDnsLookupReply(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QDnsLookup::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QDnsLookup::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QDnsLookup,
      qt_meta_data_QDnsLookup, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QDnsLookup::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QDnsLookup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QDnsLookup::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QDnsLookup))
        return static_cast<void*>(const_cast< QDnsLookup*>(this));
    return QObject::qt_metacast(_clname);
}

int QDnsLookup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< Error*>(_v) = error(); break;
        case 1: *reinterpret_cast< QString*>(_v) = errorString(); break;
        case 2: *reinterpret_cast< QString*>(_v) = name(); break;
        case 3: *reinterpret_cast< Type*>(_v) = type(); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 2: setName(*reinterpret_cast< QString*>(_v)); break;
        case 3: setType(*reinterpret_cast< Type*>(_v)); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QDnsLookup::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QDnsLookup::nameChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QDnsLookup::typeChanged(Type _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
