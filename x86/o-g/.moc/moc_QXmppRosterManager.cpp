/****************************************************************************
** Meta object code from reading C++ file 'QXmppRosterManager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/client/QXmppRosterManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QXmppRosterManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QXmppRosterManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,
      54,   37,   19,   19, 0x05,
      95,   87,   19,   19, 0x05,
     125,   87,   19,   19, 0x05,
     144,   87,   19,   19, 0x05,
     165,   87,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
     206,  191,  186,   19, 0x0a,
     242,   87,  186,   19, 0x2a,
     270,  191,  186,   19, 0x0a,
     306,   87,  186,   19, 0x2a,
     354,  334,  186,   19, 0x0a,
     406,  393,  186,   19, 0x2a,
     431,   87,  186,   19, 0x2a,
     448,   87,  186,   19, 0x0a,
     468,  393,  186,   19, 0x0a,
     496,  191,  186,   19, 0x0a,
     523,   87,  186,   19, 0x2a,
     542,  191,  186,   19, 0x0a,
     571,   87,  186,   19, 0x2a,
     592,   19,   19,   19, 0x08,
     607,   19,   19,   19, 0x08,
     625,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QXmppRosterManager[] = {
    "QXmppRosterManager\0\0rosterReceived()\0"
    "bareJid,resource\0presenceChanged(QString,QString)\0"
    "bareJid\0subscriptionReceived(QString)\0"
    "itemAdded(QString)\0itemChanged(QString)\0"
    "itemRemoved(QString)\0bool\0bareJid,reason\0"
    "acceptSubscription(QString,QString)\0"
    "acceptSubscription(QString)\0"
    "refuseSubscription(QString,QString)\0"
    "refuseSubscription(QString)\0"
    "bareJid,name,groups\0"
    "addItem(QString,QString,QSet<QString>)\0"
    "bareJid,name\0addItem(QString,QString)\0"
    "addItem(QString)\0removeItem(QString)\0"
    "renameItem(QString,QString)\0"
    "subscribe(QString,QString)\0"
    "subscribe(QString)\0unsubscribe(QString,QString)\0"
    "unsubscribe(QString)\0_q_connected()\0"
    "_q_disconnected()\0_q_presenceReceived(QXmppPresence)\0"
};

void QXmppRosterManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppRosterManager *_t = static_cast<QXmppRosterManager *>(_o);
        switch (_id) {
        case 0: _t->rosterReceived(); break;
        case 1: _t->presenceChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->subscriptionReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->itemAdded((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->itemChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->itemRemoved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: { bool _r = _t->acceptSubscription((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: { bool _r = _t->acceptSubscription((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: { bool _r = _t->refuseSubscription((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 9: { bool _r = _t->refuseSubscription((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: { bool _r = _t->addItem((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QSet<QString>(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: { bool _r = _t->addItem((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 12: { bool _r = _t->addItem((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 13: { bool _r = _t->removeItem((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 14: { bool _r = _t->renameItem((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 15: { bool _r = _t->subscribe((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 16: { bool _r = _t->subscribe((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 17: { bool _r = _t->unsubscribe((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 18: { bool _r = _t->unsubscribe((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 19: _t->_q_connected(); break;
        case 20: _t->_q_disconnected(); break;
        case 21: _t->_q_presenceReceived((*reinterpret_cast< const QXmppPresence(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QXmppRosterManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppRosterManager::staticMetaObject = {
    { &QXmppClientExtension::staticMetaObject, qt_meta_stringdata_QXmppRosterManager,
      qt_meta_data_QXmppRosterManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppRosterManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppRosterManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppRosterManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppRosterManager))
        return static_cast<void*>(const_cast< QXmppRosterManager*>(this));
    return QXmppClientExtension::qt_metacast(_clname);
}

int QXmppRosterManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QXmppClientExtension::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void QXmppRosterManager::rosterReceived()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QXmppRosterManager::presenceChanged(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QXmppRosterManager::subscriptionReceived(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QXmppRosterManager::itemAdded(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QXmppRosterManager::itemChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QXmppRosterManager::itemRemoved(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
