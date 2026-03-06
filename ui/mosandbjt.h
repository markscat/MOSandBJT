#ifndef MOSANDBJT_H
#define MOSANDBJT_H

#include <QMainWindow>
#include "numkeyboard.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MOSandBJT;
}
QT_END_NAMESPACE

class MOSandBJT : public QMainWindow
{
    Q_OBJECT

public:
    MOSandBJT(QWidget *parent = nullptr);
    ~MOSandBJT();

private slots:
    void on_actioncall_keyboard_triggered();  // 自動連線版
    void onNumpadClosed();


private:
    NumKeyBoard *numpadWindow = nullptr;  // 用來記住 numkeyboard 視窗
    void onNumpadEqualPressed(QString expression);

    Ui::MOSandBJT *ui;
};
#endif // MOSANDBJT_H
