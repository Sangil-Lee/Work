/****************************************************************************
** Meta object code from reading C++ file 'widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Widget_t {
    QByteArrayData data[21];
    char stringdata0[367];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Widget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Widget_t qt_meta_stringdata_Widget = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Widget"
QT_MOC_LITERAL(1, 7, 10), // "sigDisplay"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 6), // "pixmap"
QT_MOC_LITERAL(4, 26, 11), // "slotDisplay"
QT_MOC_LITERAL(5, 38, 21), // "on_btnRefresh_clicked"
QT_MOC_LITERAL(6, 60, 26), // "on_cmbCameraList_activated"
QT_MOC_LITERAL(7, 87, 5), // "index"
QT_MOC_LITERAL(8, 93, 19), // "on_btnStart_clicked"
QT_MOC_LITERAL(9, 113, 18), // "on_btnStop_clicked"
QT_MOC_LITERAL(10, 132, 27), // "on_cmbPixelFormat_activated"
QT_MOC_LITERAL(11, 160, 4), // "arg1"
QT_MOC_LITERAL(12, 165, 18), // "on_btnGain_clicked"
QT_MOC_LITERAL(13, 184, 21), // "on_btnShutter_clicked"
QT_MOC_LITERAL(14, 206, 22), // "on_chkGainAuto_clicked"
QT_MOC_LITERAL(15, 229, 7), // "checked"
QT_MOC_LITERAL(16, 237, 25), // "on_chkShutterAuto_clicked"
QT_MOC_LITERAL(17, 263, 27), // "on_chkTriggerEnable_clicked"
QT_MOC_LITERAL(18, 291, 29), // "on_cmbTriggerSource_activated"
QT_MOC_LITERAL(19, 321, 25), // "on_btnTriggerPara_clicked"
QT_MOC_LITERAL(20, 347, 19) // "on_btnRunSW_clicked"

    },
    "Widget\0sigDisplay\0\0pixmap\0slotDisplay\0"
    "on_btnRefresh_clicked\0on_cmbCameraList_activated\0"
    "index\0on_btnStart_clicked\0on_btnStop_clicked\0"
    "on_cmbPixelFormat_activated\0arg1\0"
    "on_btnGain_clicked\0on_btnShutter_clicked\0"
    "on_chkGainAuto_clicked\0checked\0"
    "on_chkShutterAuto_clicked\0"
    "on_chkTriggerEnable_clicked\0"
    "on_cmbTriggerSource_activated\0"
    "on_btnTriggerPara_clicked\0on_btnRunSW_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Widget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   92,    2, 0x0a /* Public */,
       5,    0,   95,    2, 0x08 /* Private */,
       6,    1,   96,    2, 0x08 /* Private */,
       8,    0,   99,    2, 0x08 /* Private */,
       9,    0,  100,    2, 0x08 /* Private */,
      10,    1,  101,    2, 0x08 /* Private */,
      12,    0,  104,    2, 0x08 /* Private */,
      13,    0,  105,    2, 0x08 /* Private */,
      14,    1,  106,    2, 0x08 /* Private */,
      16,    1,  109,    2, 0x08 /* Private */,
      17,    1,  112,    2, 0x08 /* Private */,
      18,    1,  115,    2, 0x08 /* Private */,
      19,    0,  118,    2, 0x08 /* Private */,
      20,    0,  119,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPixmap,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QPixmap,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Widget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigDisplay((*reinterpret_cast< QPixmap(*)>(_a[1]))); break;
        case 1: _t->slotDisplay((*reinterpret_cast< QPixmap(*)>(_a[1]))); break;
        case 2: _t->on_btnRefresh_clicked(); break;
        case 3: _t->on_cmbCameraList_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_btnStart_clicked(); break;
        case 5: _t->on_btnStop_clicked(); break;
        case 6: _t->on_cmbPixelFormat_activated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->on_btnGain_clicked(); break;
        case 8: _t->on_btnShutter_clicked(); break;
        case 9: _t->on_chkGainAuto_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->on_chkShutterAuto_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->on_chkTriggerEnable_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->on_cmbTriggerSource_activated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->on_btnTriggerPara_clicked(); break;
        case 14: _t->on_btnRunSW_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Widget::*)(QPixmap );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Widget::sigDisplay)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Widget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_Widget.data,
    qt_meta_data_Widget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Widget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Widget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void Widget::sigDisplay(QPixmap _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
