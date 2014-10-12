/****************************************************************************
** Meta object code from reading C++ file 'QXmppMucManager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/client/QXmppMucManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QXmppMucManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QXmppMucManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       1,   34, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      40,   17,   16,   16, 0x05,
      89,   84,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
     122,  114,   16,   16, 0x08,
     162,  155,   16,   16, 0x08,

 // properties: name, type, flags
     210,  189, 0x00495009,

 // properties: notify_signal_id
       1,

       0        // eod
};

static const char qt_meta_stringdata_QXmppMucManager[] = {
    "QXmppMucManager\0\0roomJid,inviter,reason\0"
    "invitationReceived(QString,QString,QString)\0"
    "room\0roomAdded(QXmppMucRoom*)\0message\0"
    "_q_messageReceived(QXmppMessage)\0"
    "object\0_q_roomDestroyed(QObject*)\0"
    "QList<QXmppMucRoom*>\0rooms\0"
};

void QXmppMucManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppMucManager *_t = static_cast<QXmppMucManager *>(_o);
        switch (_id) {
        case 0: _t->invitationReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 1: _t->roomAdded((*reinterpret_cast< QXmppMucRoom*(*)>(_a[1]))); break;
        case 2: _t->_q_messageReceived((*reinterpret_cast< const QXmppMessage(*)>(_a[1]))); break;
        case 3: _t->_q_roomDestroyed((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QXmppMucManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppMucManager::staticMetaObject = {
    { &QXmppClientExtension::staticMetaObject, qt_meta_stringdata_QXmppMucManager,
      qt_meta_data_QXmppMucManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppMucManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppMucManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppMucManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppMucManager))
        return static_cast<void*>(const_cast< QXmppMucManager*>(this));
    return QXmppClientExtension::qt_metacast(_clname);
}

int QXmppMucManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QXmppClientExtension::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QList<QXmppMucRoom*>*>(_v) = rooms(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QXmppMucManager::invitationReceived(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QXmppMucManager::roomAdded(QXmppMucRoom * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_QXmppMucRoom[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      32,   14, // methods
       8,  174, // properties
       2,  206, // enums/sets
       0,    0, // constructors
       0,       // flags
      16,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   14,   13,   13, 0x05,
      81,   67,   13,   13, 0x05,
     124,  118,   13,   13, 0x05,
     150,   13,   13,   13, 0x05,
     170,  159,   13,   13, 0x05,
     194,   13,   13,   13, 0x05,
     212,   13,   13,   13, 0x05,
     227,  219,   13,   13, 0x05,
     262,  257,   13,   13, 0x05,
     292,  283,   13,   13, 0x05,
     321,  317,   13,   13, 0x05,
     347,  317,   13,   13, 0x05,
     375,  317,   13,   13, 0x05,
     403,   13,   13,   13, 0x05,
     437,  425,   13,   13, 0x05,
     486,  478,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     515,  159,  510,   13, 0x0a,
     536,   13,  510,   13, 0x0a,
     543,  159,  510,   13, 0x0a,
     565,  219,  510,   13, 0x0a,
     580,   13,  510,   13, 0x2a,
     588,   13,  510,   13, 0x0a,
     611,   13,  510,   13, 0x0a,
     637,  632,  510,   13, 0x0a,
     669,  425,  510,   13, 0x0a,
     705,  159,  510,   13, 0x0a,
     742,  737,  510,   13, 0x0a,
     763,   13,   13,   13, 0x08,
     784,  781,   13,   13, 0x08,
     827,  219,   13,   13, 0x08,
     869,  860,   13,   13, 0x08,

 // methods: signature, parameters, type, tag, flags
     912,  317,  904,   13, 0x02,

 // properties: name, type, flags
     962,  940, 0x00495009,
     977,  510, 0x01495001,
     317,  904, 0x0a095401,
     257,  904, 0x0a495001,
     283,  904, 0x0a495103,
     998,  986, 0x0b495001,
    1011,  904, 0x0a095103,
     478,  904, 0x0a495103,

 // properties: notify_signal_id
       0,
       5,
       0,
       8,
       9,
      13,
       0,
      15,

 // enums: name, flags, count, data
    1020, 0x1,    5,  214,
    1027, 0x1,    5,  224,

 // enum data: key, value
    1035, uint(QXmppMucRoom::NoAction),
    1044, uint(QXmppMucRoom::SubjectAction),
    1058, uint(QXmppMucRoom::ConfigurationAction),
    1078, uint(QXmppMucRoom::PermissionsAction),
    1096, uint(QXmppMucRoom::KickAction),
    1035, uint(QXmppMucRoom::NoAction),
    1044, uint(QXmppMucRoom::SubjectAction),
    1058, uint(QXmppMucRoom::ConfigurationAction),
    1078, uint(QXmppMucRoom::PermissionsAction),
    1096, uint(QXmppMucRoom::KickAction),

       0        // eod
};

static const char qt_meta_stringdata_QXmppMucRoom[] = {
    "QXmppMucRoom\0\0actions\0"
    "allowedActionsChanged(QXmppMucRoom::Actions)\0"
    "configuration\0configurationReceived(QXmppDataForm)\0"
    "error\0error(QXmppStanza::Error)\0"
    "joined()\0jid,reason\0kicked(QString,QString)\0"
    "isJoinedChanged()\0left()\0message\0"
    "messageReceived(QXmppMessage)\0name\0"
    "nameChanged(QString)\0nickName\0"
    "nickNameChanged(QString)\0jid\0"
    "participantAdded(QString)\0"
    "participantChanged(QString)\0"
    "participantRemoved(QString)\0"
    "participantsChanged()\0permissions\0"
    "permissionsReceived(QList<QXmppMucItem>)\0"
    "subject\0subjectChanged(QString)\0bool\0"
    "ban(QString,QString)\0join()\0"
    "kick(QString,QString)\0leave(QString)\0"
    "leave()\0requestConfiguration()\0"
    "requestPermissions()\0form\0"
    "setConfiguration(QXmppDataForm)\0"
    "setPermissions(QList<QXmppMucItem>)\0"
    "sendInvitation(QString,QString)\0text\0"
    "sendMessage(QString)\0_q_disconnected()\0"
    "iq\0_q_discoveryInfoReceived(QXmppDiscoveryIq)\0"
    "_q_messageReceived(QXmppMessage)\0"
    "presence\0_q_presenceReceived(QXmppPresence)\0"
    "QString\0participantFullJid(QString)\0"
    "QXmppMucRoom::Actions\0allowedActions\0"
    "isJoined\0QStringList\0participants\0"
    "password\0Action\0Actions\0NoAction\0"
    "SubjectAction\0ConfigurationAction\0"
    "PermissionsAction\0KickAction\0"
};

void QXmppMucRoom::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppMucRoom *_t = static_cast<QXmppMucRoom *>(_o);
        switch (_id) {
        case 0: _t->allowedActionsChanged((*reinterpret_cast< QXmppMucRoom::Actions(*)>(_a[1]))); break;
        case 1: _t->configurationReceived((*reinterpret_cast< const QXmppDataForm(*)>(_a[1]))); break;
        case 2: _t->error((*reinterpret_cast< const QXmppStanza::Error(*)>(_a[1]))); break;
        case 3: _t->joined(); break;
        case 4: _t->kicked((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->isJoinedChanged(); break;
        case 6: _t->left(); break;
        case 7: _t->messageReceived((*reinterpret_cast< const QXmppMessage(*)>(_a[1]))); break;
        case 8: _t->nameChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->nickNameChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->participantAdded((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->participantChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: _t->participantRemoved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->participantsChanged(); break;
        case 14: _t->permissionsReceived((*reinterpret_cast< const QList<QXmppMucItem>(*)>(_a[1]))); break;
        case 15: _t->subjectChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: { bool _r = _t->ban((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 17: { bool _r = _t->join();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 18: { bool _r = _t->kick((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 19: { bool _r = _t->leave((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 20: { bool _r = _t->leave();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 21: { bool _r = _t->requestConfiguration();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 22: { bool _r = _t->requestPermissions();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 23: { bool _r = _t->setConfiguration((*reinterpret_cast< const QXmppDataForm(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 24: { bool _r = _t->setPermissions((*reinterpret_cast< const QList<QXmppMucItem>(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 25: { bool _r = _t->sendInvitation((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 26: { bool _r = _t->sendMessage((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 27: _t->_q_disconnected(); break;
        case 28: _t->_q_discoveryInfoReceived((*reinterpret_cast< const QXmppDiscoveryIq(*)>(_a[1]))); break;
        case 29: _t->_q_messageReceived((*reinterpret_cast< const QXmppMessage(*)>(_a[1]))); break;
        case 30: _t->_q_presenceReceived((*reinterpret_cast< const QXmppPresence(*)>(_a[1]))); break;
        case 31: { QString _r = _t->participantFullJid((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QXmppMucRoom::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppMucRoom::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QXmppMucRoom,
      qt_meta_data_QXmppMucRoom, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppMucRoom::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppMucRoom::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppMucRoom::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppMucRoom))
        return static_cast<void*>(const_cast< QXmppMucRoom*>(this));
    return QObject::qt_metacast(_clname);
}

int QXmppMucRoom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QXmppMucRoom::Actions*>(_v) = allowedActions(); break;
        case 1: *reinterpret_cast< bool*>(_v) = isJoined(); break;
        case 2: *reinterpret_cast< QString*>(_v) = jid(); break;
        case 3: *reinterpret_cast< QString*>(_v) = name(); break;
        case 4: *reinterpret_cast< QString*>(_v) = nickName(); break;
        case 5: *reinterpret_cast< QStringList*>(_v) = participants(); break;
        case 6: *reinterpret_cast< QString*>(_v) = password(); break;
        case 7: *reinterpret_cast< QString*>(_v) = subject(); break;
        }
        _id -= 8;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 4: setNickName(*reinterpret_cast< QString*>(_v)); break;
        case 6: setPassword(*reinterpret_cast< QString*>(_v)); break;
        case 7: setSubject(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 8;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 8;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QXmppMucRoom::allowedActionsChanged(QXmppMucRoom::Actions _t1)const
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< QXmppMucRoom *>(this), &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QXmppMucRoom::configurationReceived(const QXmppDataForm & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QXmppMucRoom::error(const QXmppStanza::Error & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QXmppMucRoom::joined()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void QXmppMucRoom::kicked(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QXmppMucRoom::isJoinedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void QXmppMucRoom::left()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void QXmppMucRoom::messageReceived(const QXmppMessage & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void QXmppMucRoom::nameChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void QXmppMucRoom::nickNameChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void QXmppMucRoom::participantAdded(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void QXmppMucRoom::participantChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void QXmppMucRoom::participantRemoved(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void QXmppMucRoom::participantsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void QXmppMucRoom::permissionsReceived(const QList<QXmppMucItem> & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void QXmppMucRoom::subjectChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}
QT_END_MOC_NAMESPACE
