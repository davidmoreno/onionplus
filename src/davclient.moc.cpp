/****************************************************************************
** Meta object code from reading C++ file 'davclient.h'
**
** Created: Tue Jan 15 22:03:23 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "davclient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'davclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_DavClient[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x09,
      25,   10,   10,   10, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_DavClient[] = {
    "DavClient\0\0readRequest()\0processPetition()\0"
};

const QMetaObject DavClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DavClient,
      qt_meta_data_DavClient, 0 }
};

const QMetaObject *DavClient::metaObject() const
{
    return &staticMetaObject;
}

void *DavClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DavClient))
	return static_cast<void*>(const_cast< DavClient*>(this));
    return QObject::qt_metacast(_clname);
}

int DavClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: readRequest(); break;
        case 1: processPetition(); break;
        }
        _id -= 2;
    }
    return _id;
}
