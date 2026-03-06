/********************************************************************************
** Form generated from reading UI file 'numkeyboard.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NUMKEYBOARD_H
#define UI_NUMKEYBOARD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NumKeyBoard
{
public:
    QAction *actionabout;
    QAction *actionchinese;
    QAction *actionEnglish;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QWidget *widget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QRadioButton *DEG_Radiobuttom;
    QRadioButton *RAD_Radiobuttom;
    QRadioButton *GRA_Radiobuttom;
    QCheckBox *REC_Check;
    QHBoxLayout *horizontalLayout_2;
    QWidget *scientific_panel;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_2;
    QPushButton *expr_const_Button;
    QPushButton *Fun_FIB_Button;
    QPushButton *Power_of_Button;
    QPushButton *Fun_exp_Button;
    QPushButton *fun_Tan_Button;
    QPushButton *Fun_Cos_Button;
    QPushButton *Fun_sin_Button;
    QPushButton *PI_Button;
    QPushButton *Remainder_Button;
    QPushButton *Shift_Button;
    QPushButton *square_root_Button;
    QPushButton *square_Button;
    QWidget *Main_widget;
    QWidget *layoutWidget2;
    QGridLayout *gridLayout;
    QPushButton *Button_9;
    QPushButton *Button_5;
    QPushButton *Button_8;
    QPushButton *equal_Button;
    QPushButton *Button_1;
    QPushButton *Menu_Button;
    QPushButton *Add_Button;
    QPushButton *Button_7;
    QPushButton *Subtraction_Button;
    QPushButton *Button_6;
    QPushButton *Multiplication_Button;
    QPushButton *Dot_Button;
    QPushButton *Button_0;
    QPushButton *Back_Button;
    QPushButton *Clear_Entry_Buttom;
    QPushButton *Button_4;
    QPushButton *Negate_Button;
    QPushButton *Button_2;
    QPushButton *Division_Button;
    QPushButton *Percentage_Button;
    QPushButton *Button_3;
    QPushButton *Clear_Button;
    QPushButton *RParen_Button;
    QPushButton *LParen_Button;
    QStatusBar *statusbar;
    QMenuBar *menuBar;
    QMenu *menuhelp;
    QMenu *menu_About;

    void setupUi(QMainWindow *NumKeyBoard)
    {
        if (NumKeyBoard->objectName().isEmpty())
            NumKeyBoard->setObjectName("NumKeyBoard");
        NumKeyBoard->resize(487, 648);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(NumKeyBoard->sizePolicy().hasHeightForWidth());
        NumKeyBoard->setSizePolicy(sizePolicy);
        NumKeyBoard->setMinimumSize(QSize(0, 0));
        NumKeyBoard->setStyleSheet(QString::fromUtf8("/* \345\205\250\345\237\237\350\203\214\346\231\257\350\250\255\345\256\232 */\n"
"#centralwidget {\n"
"    background-color: #f3f3f3; /* \346\267\272\347\201\260\350\211\262\350\203\214\346\231\257\357\274\214\347\234\213\350\265\267\344\276\206\346\257\224\350\274\203\351\253\230\347\264\232 */\n"
"}\n"
"\n"
"/* \350\236\242\345\271\225 (QLineEdit) \346\250\243\345\274\217 */\n"
"QLineEdit {\n"
"    background-color: transparent;\n"
"    border: 2px solid #7a7a7a; /* \346\267\261\347\201\260\350\211\262\345\257\246\347\267\232\351\202\212\346\241\206\357\274\2142\345\203\217\347\264\240\345\257\254 */\n"
"    border-radius: 4px;        /* \350\210\207\346\214\211\351\210\225\344\270\200\350\207\264\347\232\204\345\234\223\350\247\222 */\n"
"    color: #1a1a1a;\n"
"    font-family: \"Segoe UI\", \"Microsoft JhengHei\";\n"
"    font-size: 40px; /* \345\244\247\346\225\270\345\255\227\351\241\257\347\244\272 */\n"
"    font-weight: bold;\n"
"    padding: 10px;\n"
"}\n"
"\n"
"/* \344\270\212\346\226\271\345\260\217"
                        "\346\250\231\347\261\244 (QLabel) \346\250\243\345\274\217 */\n"
"QLabel#label {\n"
"    color: #666666;\n"
"    font-family: \"Segoe UI\";\n"
"    font-size: 14px;\n"
"    padding-right: 15px;\n"
"}\n"
"\n"
"/* \345\237\272\347\244\216\346\214\211\351\210\225\346\250\243\345\274\217 (\346\211\200\346\234\211\346\214\211\351\210\225\345\205\261\351\200\232) */\n"
"QPushButton {\n"
"    background-color: #ffffff; /* \351\240\220\350\250\255\347\231\275\350\211\262 */\n"
"    border: 1px solid #e5e5e5;\n"
"    border-radius: 4px; /* \350\274\225\345\276\256\345\234\223\350\247\222 */\n"
"    font-family: \"Segoe UI\", \"Microsoft JhengHei\";\n"
"    font-size: 16px;\n"
"    color: #1a1a1a;\n"
"}\n"
"\n"
"/* \346\273\221\351\274\240\347\247\273\345\205\245\350\256\212\350\211\262 */\n"
"QPushButton:hover {\n"
"    background-color: #f9f9f9;\n"
"    border: 1px solid #d0d0d0;\n"
"}\n"
"\n"
"/* \346\214\211\344\270\213\346\231\202\350\256\212\350\211\262 */\n"
"QPushButton:pressed {\n"
"    background-color: #eeeee"
                        "e;\n"
"}\n"
"\n"
"/* \351\201\213\347\256\227\347\254\246\346\214\211\351\210\225 (\344\275\277\347\224\250\347\211\251\351\253\224\345\220\215\347\250\261\344\276\206\346\214\207\345\256\232\347\211\271\345\256\232\347\232\204\346\214\211\351\210\225) */\n"
"QPushButton#Add_Button, QPushButton#Subtraction_Button, \n"
"QPushButton#Multiplication_Button, QPushButton#Division_Button,\n"
"QPushButton#Back_Button, QPushButton#Clear_Button, QPushButton#Clear_Entry_Buttom {\n"
"    background-color: #fafafa;\n"
"    font-weight: bold;\n"
"    color: #0078d4; /* \351\201\213\347\256\227\347\254\246\347\224\250\350\227\215\350\211\262\345\274\267\350\252\277 */\n"
"}\n"
"\n"
"/* \347\255\211\346\226\274\346\214\211\351\210\225 (\347\252\201\345\207\272\347\232\204\351\207\215\351\273\236\350\211\262) */\n"
"QPushButton#equal_Button {\n"
"    background-color: #0078d4;\n"
"    color: white;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton#equal_Button:hover {\n"
"    background-color: #006abc;\n"
"}\n"
"\n"
"QPushButto"
                        "n#equal_Button:pressed {\n"
"    background-color: #005a9e;\n"
"}\n"
"\n"
"/* \347\247\221\345\255\270\351\235\242\346\235\277\346\214\211\351\210\225 (\347\250\215\345\276\256\344\275\216\350\252\277\344\270\200\351\273\236\347\232\204\351\241\217\350\211\262) */\n"
"#scientific_panel QPushButton {\n"
"    background-color: #fbfbfb;\n"
"    font-size: 13px;\n"
"    color: #555555;\n"
"}\n"
"\n"
"/* \345\226\256\351\201\270\351\210\225 (RadioButton) \346\250\243\345\274\217 */\n"
"QRadioButton {\n"
"    font-family: \"Segoe UI\";\n"
"    font-size: 12px;\n"
"    color: #333333;\n"
"    spacing: 5px;\n"
"}"));
        actionabout = new QAction(NumKeyBoard);
        actionabout->setObjectName("actionabout");
        actionchinese = new QAction(NumKeyBoard);
        actionchinese->setObjectName("actionchinese");
        actionEnglish = new QAction(NumKeyBoard);
        actionEnglish->setObjectName("actionEnglish");
        centralwidget = new QWidget(NumKeyBoard);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setMaximumSize(QSize(16777215, 30));
        label->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        verticalLayout->addWidget(label);

        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setMinimumSize(QSize(0, 100));
        lineEdit->setMaximumSize(QSize(16777215, 100));

        verticalLayout->addWidget(lineEdit);

        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setMaximumSize(QSize(16777215, 30));
        layoutWidget = new QWidget(widget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(0, 0, 297, 40));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        DEG_Radiobuttom = new QRadioButton(layoutWidget);
        DEG_Radiobuttom->setObjectName("DEG_Radiobuttom");

        horizontalLayout->addWidget(DEG_Radiobuttom);

        RAD_Radiobuttom = new QRadioButton(layoutWidget);
        RAD_Radiobuttom->setObjectName("RAD_Radiobuttom");

        horizontalLayout->addWidget(RAD_Radiobuttom);

        GRA_Radiobuttom = new QRadioButton(layoutWidget);
        GRA_Radiobuttom->setObjectName("GRA_Radiobuttom");

        horizontalLayout->addWidget(GRA_Radiobuttom);

        REC_Check = new QCheckBox(layoutWidget);
        REC_Check->setObjectName("REC_Check");
        REC_Check->setStyleSheet(QString::fromUtf8("/* 1. \350\250\255\345\256\232\346\226\207\345\255\227\350\210\207\351\226\223\350\267\235 */\n"
"QCheckBox {\n"
"    spacing: 8px;\n"
"    color: #333333;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"/* 2. \345\256\232\347\276\251\345\244\226\345\234\210\357\274\210\345\234\223\345\275\242\345\256\271\345\231\250\357\274\211 */\n"
"QCheckBox::indicator {\n"
"    width: 10px;\n"
"    height: 10px;\n"
"    border-radius: 14px; /* \345\215\212\345\276\221\347\202\272\345\257\254\351\253\230\347\232\204\344\270\200\345\215\212\344\273\245\344\270\212\357\274\214\345\274\267\345\210\266\350\256\212\345\234\223\345\275\242 */\n"
"    border: 2px solid #999999;\n"
"    background-color: #ffffff;\n"
"}\n"
"\n"
"/* 3. \346\234\252\351\201\270\345\217\226\347\213\200\346\205\213\357\274\232\347\264\205\350\211\262\345\257\246\345\277\203\346\255\243\346\226\271\345\275\242 (\345\201\234\346\255\242\351\214\204\350\243\275\345\234\226\347\244\272) */\n"
"QCheckBox::indicator:unchecked {\n"
"    background-color: #cc0000; /"
                        "* \347\264\205\350\211\262 */\n"
"    /* \351\227\234\351\215\265\346\212\200\345\267\247\357\274\232\347\224\250 6px \347\232\204\345\216\232\351\202\212\346\241\206\357\274\210\351\241\217\350\211\262\351\234\200\350\267\237\350\203\214\346\231\257 #f3f3f3 \344\270\200\350\207\264\357\274\211 */\n"
"    /* \351\202\212\346\241\206\346\234\203\345\276\200\345\205\247\346\223\240\345\243\223\350\203\214\346\231\257\350\211\262\357\274\214\350\203\214\346\231\257\350\211\262\345\215\200\345\237\237\345\260\261\346\234\203\350\256\212\346\210\220\346\255\243\346\226\271\345\275\242 */\n"
"    border: 7px solid #f3f3f3; \n"
"}\n"
"\n"
"/* 4. \351\201\270\345\217\226\347\213\200\346\205\213\357\274\232\347\264\205\350\211\262\351\233\231\345\236\202\347\233\264\347\267\232 (\351\214\204\350\243\275/\346\232\253\345\201\234\345\234\226\347\244\272) */\n"
"QCheckBox::indicator:checked {\n"
"    background-color: white;\n"
"    border: 2px solid #cc0000; /* \345\244\226\345\234\210\350\256\212\347\264\205 */\n"
"    "
                        "/* \344\275\277\347\224\250\347\267\232\346\200\247\346\274\270\345\261\244\345\234\250\350\203\214\346\231\257\347\225\253\345\207\272\345\205\251\346\242\235\345\236\202\347\233\264\347\264\205\347\267\232 */\n"
"    background-image: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, \n"
"        stop:0.20 transparent, \n"
"        stop:0.21 #cc0000, stop:0.40 #cc0000, \n"
"        stop:0.41 transparent, stop:0.59 transparent, \n"
"        stop:0.60 #cc0000, stop:0.79 #cc0000, \n"
"        stop:0.80 transparent);\n"
"}\n"
"\n"
"/* 5. \346\273\221\351\274\240\347\247\273\351\201\216\346\231\202\347\232\204\345\276\256\350\252\277 */\n"
"QCheckBox::indicator:hover {\n"
"    border: 2px solid #555555;\n"
"}"));

        horizontalLayout->addWidget(REC_Check);


        verticalLayout->addWidget(widget);

        verticalLayout->setStretch(1, 1);

        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        scientific_panel = new QWidget(centralwidget);
        scientific_panel->setObjectName("scientific_panel");
        scientific_panel->setMinimumSize(QSize(135, 400));
        scientific_panel->setMaximumSize(QSize(150, 450));
        scientific_panel->setStyleSheet(QString::fromUtf8(""));
        layoutWidget1 = new QWidget(scientific_panel);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(0, 0, 141, 381));
        gridLayout_2 = new QGridLayout(layoutWidget1);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        expr_const_Button = new QPushButton(layoutWidget1);
        expr_const_Button->setObjectName("expr_const_Button");
        expr_const_Button->setMinimumSize(QSize(0, 0));
        expr_const_Button->setMaximumSize(QSize(55, 55));

        gridLayout_2->addWidget(expr_const_Button, 4, 1, 1, 1);

        Fun_FIB_Button = new QPushButton(layoutWidget1);
        Fun_FIB_Button->setObjectName("Fun_FIB_Button");
        Fun_FIB_Button->setMinimumSize(QSize(0, 0));
        Fun_FIB_Button->setMaximumSize(QSize(55, 55));

        gridLayout_2->addWidget(Fun_FIB_Button, 5, 1, 1, 1);

        Power_of_Button = new QPushButton(layoutWidget1);
        Power_of_Button->setObjectName("Power_of_Button");
        Power_of_Button->setMinimumSize(QSize(0, 0));
        Power_of_Button->setMaximumSize(QSize(55, 55));
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe UI")});
        Power_of_Button->setFont(font);

        gridLayout_2->addWidget(Power_of_Button, 5, 0, 1, 1);

        Fun_exp_Button = new QPushButton(layoutWidget1);
        Fun_exp_Button->setObjectName("Fun_exp_Button");
        Fun_exp_Button->setMinimumSize(QSize(0, 0));
        Fun_exp_Button->setMaximumSize(QSize(55, 55));

        gridLayout_2->addWidget(Fun_exp_Button, 4, 0, 1, 1);

        fun_Tan_Button = new QPushButton(layoutWidget1);
        fun_Tan_Button->setObjectName("fun_Tan_Button");
        fun_Tan_Button->setMinimumSize(QSize(0, 0));
        fun_Tan_Button->setMaximumSize(QSize(55, 55));

        gridLayout_2->addWidget(fun_Tan_Button, 3, 0, 1, 1);

        Fun_Cos_Button = new QPushButton(layoutWidget1);
        Fun_Cos_Button->setObjectName("Fun_Cos_Button");
        Fun_Cos_Button->setMinimumSize(QSize(0, 0));
        Fun_Cos_Button->setMaximumSize(QSize(55, 55));

        gridLayout_2->addWidget(Fun_Cos_Button, 2, 0, 1, 1);

        Fun_sin_Button = new QPushButton(layoutWidget1);
        Fun_sin_Button->setObjectName("Fun_sin_Button");
        Fun_sin_Button->setMinimumSize(QSize(0, 0));
        Fun_sin_Button->setMaximumSize(QSize(55, 55));

        gridLayout_2->addWidget(Fun_sin_Button, 1, 0, 1, 1);

        PI_Button = new QPushButton(layoutWidget1);
        PI_Button->setObjectName("PI_Button");
        PI_Button->setMinimumSize(QSize(0, 0));
        PI_Button->setMaximumSize(QSize(55, 55));

        gridLayout_2->addWidget(PI_Button, 3, 1, 1, 1);

        Remainder_Button = new QPushButton(layoutWidget1);
        Remainder_Button->setObjectName("Remainder_Button");
        Remainder_Button->setMinimumSize(QSize(0, 0));
        Remainder_Button->setMaximumSize(QSize(55, 55));

        gridLayout_2->addWidget(Remainder_Button, 2, 1, 1, 1);

        Shift_Button = new QPushButton(layoutWidget1);
        Shift_Button->setObjectName("Shift_Button");
        Shift_Button->setMinimumSize(QSize(0, 0));
        Shift_Button->setMaximumSize(QSize(55, 55));

        gridLayout_2->addWidget(Shift_Button, 1, 1, 1, 1);

        square_root_Button = new QPushButton(layoutWidget1);
        square_root_Button->setObjectName("square_root_Button");
        square_root_Button->setMinimumSize(QSize(0, 0));
        square_root_Button->setMaximumSize(QSize(55, 55));

        gridLayout_2->addWidget(square_root_Button, 0, 0, 1, 1);

        square_Button = new QPushButton(layoutWidget1);
        square_Button->setObjectName("square_Button");
        square_Button->setMinimumSize(QSize(0, 0));
        square_Button->setMaximumSize(QSize(55, 55));

        gridLayout_2->addWidget(square_Button, 0, 1, 1, 1);


        horizontalLayout_2->addWidget(scientific_panel);

        Main_widget = new QWidget(centralwidget);
        Main_widget->setObjectName("Main_widget");
        Main_widget->setMinimumSize(QSize(60, 60));
        Main_widget->setMaximumSize(QSize(300, 400));
        layoutWidget2 = new QWidget(Main_widget);
        layoutWidget2->setObjectName("layoutWidget2");
        layoutWidget2->setGeometry(QRect(10, 0, 308, 384));
        layoutWidget2->setMinimumSize(QSize(60, 60));
        gridLayout = new QGridLayout(layoutWidget2);
        gridLayout->setSpacing(2);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        Button_9 = new QPushButton(layoutWidget2);
        Button_9->setObjectName("Button_9");
        Button_9->setMinimumSize(QSize(0, 0));
        Button_9->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Button_9, 2, 3, 1, 1);

        Button_5 = new QPushButton(layoutWidget2);
        Button_5->setObjectName("Button_5");
        Button_5->setMinimumSize(QSize(0, 0));
        Button_5->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Button_5, 3, 2, 1, 1);

        Button_8 = new QPushButton(layoutWidget2);
        Button_8->setObjectName("Button_8");
        Button_8->setMinimumSize(QSize(0, 0));
        Button_8->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Button_8, 2, 2, 1, 1);

        equal_Button = new QPushButton(layoutWidget2);
        equal_Button->setObjectName("equal_Button");
        equal_Button->setMinimumSize(QSize(0, 0));
        equal_Button->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(equal_Button, 5, 3, 1, 1);

        Button_1 = new QPushButton(layoutWidget2);
        Button_1->setObjectName("Button_1");
        Button_1->setMinimumSize(QSize(0, 0));
        Button_1->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Button_1, 4, 0, 1, 1);

        Menu_Button = new QPushButton(layoutWidget2);
        Menu_Button->setObjectName("Menu_Button");
        Menu_Button->setMinimumSize(QSize(0, 0));
        Menu_Button->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Menu_Button, 0, 4, 1, 1);

        Add_Button = new QPushButton(layoutWidget2);
        Add_Button->setObjectName("Add_Button");
        Add_Button->setMinimumSize(QSize(0, 0));
        Add_Button->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Add_Button, 2, 4, 1, 1);

        Button_7 = new QPushButton(layoutWidget2);
        Button_7->setObjectName("Button_7");
        Button_7->setMinimumSize(QSize(0, 0));
        Button_7->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Button_7, 2, 0, 1, 1);

        Subtraction_Button = new QPushButton(layoutWidget2);
        Subtraction_Button->setObjectName("Subtraction_Button");
        Subtraction_Button->setMinimumSize(QSize(0, 0));
        Subtraction_Button->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Subtraction_Button, 3, 4, 1, 1);

        Button_6 = new QPushButton(layoutWidget2);
        Button_6->setObjectName("Button_6");
        Button_6->setMinimumSize(QSize(0, 0));
        Button_6->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Button_6, 3, 3, 1, 1);

        Multiplication_Button = new QPushButton(layoutWidget2);
        Multiplication_Button->setObjectName("Multiplication_Button");
        Multiplication_Button->setMinimumSize(QSize(0, 0));
        Multiplication_Button->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Multiplication_Button, 4, 4, 1, 1);

        Dot_Button = new QPushButton(layoutWidget2);
        Dot_Button->setObjectName("Dot_Button");
        Dot_Button->setMinimumSize(QSize(0, 0));
        Dot_Button->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Dot_Button, 5, 0, 1, 1);

        Button_0 = new QPushButton(layoutWidget2);
        Button_0->setObjectName("Button_0");
        Button_0->setMinimumSize(QSize(0, 0));
        Button_0->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Button_0, 5, 2, 1, 1);

        Back_Button = new QPushButton(layoutWidget2);
        Back_Button->setObjectName("Back_Button");
        Back_Button->setMinimumSize(QSize(0, 0));
        Back_Button->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Back_Button, 1, 4, 1, 1);

        Clear_Entry_Buttom = new QPushButton(layoutWidget2);
        Clear_Entry_Buttom->setObjectName("Clear_Entry_Buttom");
        Clear_Entry_Buttom->setMinimumSize(QSize(0, 0));
        Clear_Entry_Buttom->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Clear_Entry_Buttom, 1, 3, 1, 1);

        Button_4 = new QPushButton(layoutWidget2);
        Button_4->setObjectName("Button_4");
        Button_4->setMinimumSize(QSize(0, 0));
        Button_4->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Button_4, 3, 0, 1, 1);

        Negate_Button = new QPushButton(layoutWidget2);
        Negate_Button->setObjectName("Negate_Button");
        Negate_Button->setMinimumSize(QSize(0, 0));
        Negate_Button->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Negate_Button, 1, 0, 1, 1);

        Button_2 = new QPushButton(layoutWidget2);
        Button_2->setObjectName("Button_2");
        Button_2->setMinimumSize(QSize(0, 0));
        Button_2->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Button_2, 4, 2, 1, 1);

        Division_Button = new QPushButton(layoutWidget2);
        Division_Button->setObjectName("Division_Button");
        Division_Button->setMinimumSize(QSize(0, 0));
        Division_Button->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Division_Button, 5, 4, 1, 1);

        Percentage_Button = new QPushButton(layoutWidget2);
        Percentage_Button->setObjectName("Percentage_Button");
        Percentage_Button->setMinimumSize(QSize(0, 0));
        Percentage_Button->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Percentage_Button, 0, 0, 1, 1);

        Button_3 = new QPushButton(layoutWidget2);
        Button_3->setObjectName("Button_3");
        Button_3->setMinimumSize(QSize(0, 0));
        Button_3->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Button_3, 4, 3, 1, 1);

        Clear_Button = new QPushButton(layoutWidget2);
        Clear_Button->setObjectName("Clear_Button");
        Clear_Button->setMinimumSize(QSize(0, 0));
        Clear_Button->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(Clear_Button, 1, 2, 1, 1);

        RParen_Button = new QPushButton(layoutWidget2);
        RParen_Button->setObjectName("RParen_Button");
        RParen_Button->setEnabled(true);
        RParen_Button->setMinimumSize(QSize(0, 0));
        RParen_Button->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(RParen_Button, 0, 3, 1, 1);

        LParen_Button = new QPushButton(layoutWidget2);
        LParen_Button->setObjectName("LParen_Button");
        LParen_Button->setEnabled(true);
        LParen_Button->setMinimumSize(QSize(0, 0));
        LParen_Button->setMaximumSize(QSize(55, 55));

        gridLayout->addWidget(LParen_Button, 0, 2, 1, 1);


        horizontalLayout_2->addWidget(Main_widget);


        verticalLayout_2->addLayout(horizontalLayout_2);

        NumKeyBoard->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(NumKeyBoard);
        statusbar->setObjectName("statusbar");
        NumKeyBoard->setStatusBar(statusbar);
        menuBar = new QMenuBar(NumKeyBoard);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 487, 21));
        menuhelp = new QMenu(menuBar);
        menuhelp->setObjectName("menuhelp");
        menu_About = new QMenu(menuhelp);
        menu_About->setObjectName("menu_About");
        NumKeyBoard->setMenuBar(menuBar);

        menuBar->addAction(menuhelp->menuAction());
        menuhelp->addAction(menu_About->menuAction());
        menu_About->addAction(actionchinese);
        menu_About->addAction(actionEnglish);

        retranslateUi(NumKeyBoard);

        QMetaObject::connectSlotsByName(NumKeyBoard);
    } // setupUi

    void retranslateUi(QMainWindow *NumKeyBoard)
    {
        NumKeyBoard->setWindowTitle(QCoreApplication::translate("NumKeyBoard", "NumKeyBoard", nullptr));
        actionabout->setText(QCoreApplication::translate("NumKeyBoard", "about", nullptr));
        actionchinese->setText(QCoreApplication::translate("NumKeyBoard", "\344\270\255\346\226\207", nullptr));
        actionEnglish->setText(QCoreApplication::translate("NumKeyBoard", "English", nullptr));
        label->setText(QCoreApplication::translate("NumKeyBoard", "TextLabel", nullptr));
        DEG_Radiobuttom->setText(QCoreApplication::translate("NumKeyBoard", "DEG", nullptr));
        RAD_Radiobuttom->setText(QCoreApplication::translate("NumKeyBoard", "RAD", nullptr));
        GRA_Radiobuttom->setText(QCoreApplication::translate("NumKeyBoard", "GRA", nullptr));
        REC_Check->setText(QCoreApplication::translate("NumKeyBoard", "REC", nullptr));
        expr_const_Button->setText(QCoreApplication::translate("NumKeyBoard", "Expr(c)", nullptr));
        Fun_FIB_Button->setText(QCoreApplication::translate("NumKeyBoard", "Fib(x)", nullptr));
        Power_of_Button->setText(QCoreApplication::translate("NumKeyBoard", "x^y", nullptr));
        Fun_exp_Button->setText(QCoreApplication::translate("NumKeyBoard", "exp(x)", nullptr));
        fun_Tan_Button->setText(QCoreApplication::translate("NumKeyBoard", "tan(x)", nullptr));
        Fun_Cos_Button->setText(QCoreApplication::translate("NumKeyBoard", "cos(x)", nullptr));
        Fun_sin_Button->setText(QCoreApplication::translate("NumKeyBoard", "sin(x)", nullptr));
        PI_Button->setText(QCoreApplication::translate("NumKeyBoard", "Pi", nullptr));
        Remainder_Button->setText(QCoreApplication::translate("NumKeyBoard", "Rem", nullptr));
        Shift_Button->setText(QCoreApplication::translate("NumKeyBoard", "Shift", nullptr));
        square_root_Button->setText(QCoreApplication::translate("NumKeyBoard", "\342\210\232", nullptr));
        square_Button->setText(QCoreApplication::translate("NumKeyBoard", " x\302\262", nullptr));
        Button_9->setText(QCoreApplication::translate("NumKeyBoard", "9", nullptr));
        Button_5->setText(QCoreApplication::translate("NumKeyBoard", "5", nullptr));
        Button_8->setText(QCoreApplication::translate("NumKeyBoard", "8", nullptr));
        equal_Button->setText(QCoreApplication::translate("NumKeyBoard", "=", nullptr));
        Button_1->setText(QCoreApplication::translate("NumKeyBoard", "1", nullptr));
        Menu_Button->setText(QCoreApplication::translate("NumKeyBoard", "Menu", nullptr));
        Add_Button->setText(QCoreApplication::translate("NumKeyBoard", "+", nullptr));
        Button_7->setText(QCoreApplication::translate("NumKeyBoard", "7", nullptr));
        Subtraction_Button->setText(QCoreApplication::translate("NumKeyBoard", "-", nullptr));
        Button_6->setText(QCoreApplication::translate("NumKeyBoard", "6", nullptr));
        Multiplication_Button->setText(QCoreApplication::translate("NumKeyBoard", "X", nullptr));
        Dot_Button->setText(QCoreApplication::translate("NumKeyBoard", ".", nullptr));
        Button_0->setText(QCoreApplication::translate("NumKeyBoard", "0", nullptr));
        Back_Button->setText(QCoreApplication::translate("NumKeyBoard", "\342\214\253", nullptr));
        Clear_Entry_Buttom->setText(QCoreApplication::translate("NumKeyBoard", "CE", nullptr));
        Button_4->setText(QCoreApplication::translate("NumKeyBoard", "4", nullptr));
        Negate_Button->setText(QCoreApplication::translate("NumKeyBoard", "\302\261", nullptr));
        Button_2->setText(QCoreApplication::translate("NumKeyBoard", "2", nullptr));
        Division_Button->setText(QCoreApplication::translate("NumKeyBoard", "\303\267", nullptr));
        Percentage_Button->setText(QCoreApplication::translate("NumKeyBoard", "%", nullptr));
        Button_3->setText(QCoreApplication::translate("NumKeyBoard", "3", nullptr));
        Clear_Button->setText(QCoreApplication::translate("NumKeyBoard", "Clear", nullptr));
        RParen_Button->setText(QCoreApplication::translate("NumKeyBoard", ")", nullptr));
        LParen_Button->setText(QCoreApplication::translate("NumKeyBoard", "(", nullptr));
        menuhelp->setTitle(QCoreApplication::translate("NumKeyBoard", "Help", nullptr));
        menu_About->setTitle(QCoreApplication::translate("NumKeyBoard", " About", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NumKeyBoard: public Ui_NumKeyBoard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NUMKEYBOARD_H
