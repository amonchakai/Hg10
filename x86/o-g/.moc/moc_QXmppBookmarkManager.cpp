/****************************************************************************
** Meta object code from reading C++ file 'QXmppBookmarkManager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/client/QXmppBookmarkManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QXmppBookmarkManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QXmppBookmarkManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      32,   22,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
      68,   21,   21,   21, 0x08,
      84,   21,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QXmppBookmarkManager[] = {
    "QXmppBookmarkManager\0\0bookmarks\0"
    "bookmarksReceived(QXmppBookmarkSet)\0"
    "slotConnected()\0slotDisconnected()\0"
};

void QXmppBookmarkManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QXmppBookmarkManager *_t = static_cast<QXmppBookmarkManager *>(_o);
        switch (_id) {
        case 0: _t->bookmarksReceived((*reinterpret_cast< const QXmppBookmarkSet(*)>(_a[1]))); break;
        case 1: _t->slotConnected(); break;
        case 2: _t->slotDisconnected(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QXmppBookmarkManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QXmppBookmarkManager::staticMetaObject = {
    { &QXmppClientExtension::staticMetaObject, qt_meta_stringdata_QXmppBookmarkManager,
      qt_meta_data_QXmppBookmarkManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QXmppBookmarkManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QXmppBookmarkManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QXmppBookmarkManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QXmppBookmarkManager))
        return static_cast<void*>(const_cast< QXmppBookmarkManager*>(this));
    return QXmppClientExtension::qt_metacast(_clname);
}

int QXmppBookmarkManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QXmppClientExtension::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void QXmppBookmarkManager::bookmarksReceived(const QXmppBookmarkSet & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
