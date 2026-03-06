/****************************************************************************
** Meta object code from reading C++ file 'numkeyboard.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../ui/numkeyboard.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'numkeyboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN11NumKeyBoardE_t {};
} // unnamed namespace

template <> constexpr inline auto NumKeyBoard::qt_create_metaobjectdata<qt_meta_tag_ZN11NumKeyBoardE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "NumKeyBoard",
        "equalPressed",
        "",
        "result",
        "on_Menu_Button_clicked",
        "onDigitClicked",
        "on_Back_Button_clicked",
        "on_Clear_Button_clicked",
        "onOperatorClicked",
        "on_equal_Button_clicked",
        "on_Negate_Button_clicked",
        "on_square_root_Button_clicked",
        "on_square_Button_clicked",
        "on_Percentage_Button_clicked",
        "on_LParen_Button_clicked",
        "on_RParen_Button_clicked",
        "on_Shift_Button_clicked",
        "onScientificClicked",
        "on_PI_Button_clicked",
        "on_expr_const_Button_clicked",
        "on_AngleUnit_toggled",
        "on_actionchinese_triggered",
        "on_actionEnglish_triggered"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'equalPressed'
        QtMocHelpers::SignalData<void(QString)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Slot 'on_Menu_Button_clicked'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDigitClicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_Back_Button_clicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_Clear_Button_clicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onOperatorClicked'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_equal_Button_clicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_Negate_Button_clicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_square_root_Button_clicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_square_Button_clicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_Percentage_Button_clicked'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_LParen_Button_clicked'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_RParen_Button_clicked'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_Shift_Button_clicked'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onScientificClicked'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_PI_Button_clicked'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_expr_const_Button_clicked'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_AngleUnit_toggled'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_actionchinese_triggered'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_actionEnglish_triggered'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<NumKeyBoard, qt_meta_tag_ZN11NumKeyBoardE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject NumKeyBoard::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11NumKeyBoardE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11NumKeyBoardE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11NumKeyBoardE_t>.metaTypes,
    nullptr
} };

void NumKeyBoard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<NumKeyBoard *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->equalPressed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->on_Menu_Button_clicked(); break;
        case 2: _t->onDigitClicked(); break;
        case 3: _t->on_Back_Button_clicked(); break;
        case 4: _t->on_Clear_Button_clicked(); break;
        case 5: _t->onOperatorClicked(); break;
        case 6: _t->on_equal_Button_clicked(); break;
        case 7: _t->on_Negate_Button_clicked(); break;
        case 8: _t->on_square_root_Button_clicked(); break;
        case 9: _t->on_square_Button_clicked(); break;
        case 10: _t->on_Percentage_Button_clicked(); break;
        case 11: _t->on_LParen_Button_clicked(); break;
        case 12: _t->on_RParen_Button_clicked(); break;
        case 13: _t->on_Shift_Button_clicked(); break;
        case 14: _t->onScientificClicked(); break;
        case 15: _t->on_PI_Button_clicked(); break;
        case 16: _t->on_expr_const_Button_clicked(); break;
        case 17: _t->on_AngleUnit_toggled(); break;
        case 18: _t->on_actionchinese_triggered(); break;
        case 19: _t->on_actionEnglish_triggered(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (NumKeyBoard::*)(QString )>(_a, &NumKeyBoard::equalPressed, 0))
            return;
    }
}

const QMetaObject *NumKeyBoard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NumKeyBoard::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11NumKeyBoardE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int NumKeyBoard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void NumKeyBoard::equalPressed(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}
QT_WARNING_POP
