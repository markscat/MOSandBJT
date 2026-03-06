/********************************************************************************
** Form generated from reading UI file 'mosandbjt.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MOSANDBJT_H
#define UI_MOSANDBJT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MOSandBJT
{
public:
    QAction *actioncall_keyboard;
    QWidget *centralwidget;
    QLineEdit *lineEdit;
    QMenuBar *menubar;
    QMenu *menuFunction;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MOSandBJT)
    {
        if (MOSandBJT->objectName().isEmpty())
            MOSandBJT->setObjectName("MOSandBJT");
        MOSandBJT->resize(800, 600);
        actioncall_keyboard = new QAction(MOSandBJT);
        actioncall_keyboard->setObjectName("actioncall_keyboard");
        centralwidget = new QWidget(MOSandBJT);
        centralwidget->setObjectName("centralwidget");
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(120, 40, 113, 21));
        MOSandBJT->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MOSandBJT);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        menuFunction = new QMenu(menubar);
        menuFunction->setObjectName("menuFunction");
        MOSandBJT->setMenuBar(menubar);
        statusbar = new QStatusBar(MOSandBJT);
        statusbar->setObjectName("statusbar");
        MOSandBJT->setStatusBar(statusbar);

        menubar->addAction(menuFunction->menuAction());
        menuFunction->addAction(actioncall_keyboard);

        retranslateUi(MOSandBJT);

        QMetaObject::connectSlotsByName(MOSandBJT);
    } // setupUi

    void retranslateUi(QMainWindow *MOSandBJT)
    {
        MOSandBJT->setWindowTitle(QCoreApplication::translate("MOSandBJT", "MOSandBJT", nullptr));
        actioncall_keyboard->setText(QCoreApplication::translate("MOSandBJT", "call keyboard", nullptr));
        menuFunction->setTitle(QCoreApplication::translate("MOSandBJT", "Function", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MOSandBJT: public Ui_MOSandBJT {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MOSANDBJT_H
