/****************************************************************************
** Meta object code from reading C++ file 'QXmppTransferManager_p.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/client/QXmppTransferManager_p.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QXmppTransferManager_p.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QXmppTransferIncomingJob[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      26,   25,   25,   25, 0x08,
      53,   25,   25,   25, 0x08,
      73,   25,   25,   25, 0x08,
      91,   25,   25,   25, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QXmppTransferIncomingJob[] = {
    "QXmppTransferIncomingJob\0\0"
    "_q_candidateDisconnected()\0"
    "_q_candidateReady()\0_q_disconnected()\0"
    "_q_receiveData()\0"
};

void QXmppTransferIncomingJob::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppTransferIncomingJob *_t = static_cast<QXmppTransferIncomingJob *>(_o);
        switch (_id) {
        case 0: _t->_q_candidateDisconnected(); break;
        case 1: _t->_q_candidateReady(); break;
        case 2: _t->_q_disconnected(); break;
        case 3: _t->_q_receiveData(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QXmppTransferIncomingJob::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppTransferIncomingJob::staticMetaObject = {
    { &QXmppTransferJob::staticMetaObject, qt_meta_stringdata_QXmppTransferIncomingJob,
      qt_meta_data_QXmppTransferIncomingJob, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppTransferIncomingJob::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppTransferIncomingJob::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppTransferIncomingJob::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppTransferIncomingJob))
        return static_cast<void*>(const_cast< QXmppTransferIncomingJob*>(this));
    return QXmppTransferJob::qt_metacast(_clname);
}

int QXmppTransferIncomingJob::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QXmppTransferJob::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
static const uint qt_meta_data_QXmppTransferOutgoingJob[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      26,   25,   25,   25, 0x08,
      44,   25,   25,   25, 0x08,
      60,   25,   25,   25, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QXmppTransferOutgoingJob[] = {
    "QXmppTransferOutgoingJob\0\0_q_disconnected()\0"
    "_q_proxyReady()\0_q_sendData()\0"
};

void QXmppTransferOutgoingJob::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppTransferOutgoingJob *_t = static_cast<QXmppTransferOutgoingJob *>(_o);
        switch (_id) {
        case 0: _t->_q_disconnected(); break;
        case 1: _t->_q_proxyReady(); break;
        case 2: _t->_q_sendData(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QXmppTransferOutgoingJob::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppTransferOutgoingJob::staticMetaObject = {
    { &QXmppTransferJob::staticMetaObject, qt_meta_stringdata_QXmppTransferOutgoingJob,
      qt_meta_data_QXmppTransferOutgoingJob, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppTransferOutgoingJob::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppTransferOutgoingJob::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppTransferOutgoingJob::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppTransferOutgoingJob))
        return static_cast<void*>(const_cast< QXmppTransferOutgoingJob*>(this));
    return QXmppTransferJob::qt_metacast(_clname);
}

int QXmppTransferOutgoingJob::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QXmppTransferJob::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
