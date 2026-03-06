#ifndef NUMKEYBOARD_H
#define NUMKEYBOARD_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class NumKeyBoard;
}
QT_END_NAMESPACE

class NumKeyBoard : public QMainWindow
{
    Q_OBJECT

public:
    NumKeyBoard(QWidget *parent = nullptr);
    ~NumKeyBoard();

private slots:

    // Menu 按鈕的點擊事件 (Qt 會自動根據名稱連接)
    void on_Menu_Button_clicked();
    void onDigitClicked();           // 處理 0-9 數字與小數點
    /*
    Qt功能，叫做 「按名稱自動連線 (Auto-Connection by Name)」。
        1. 觸發自動連線的條件
        當你在代碼裡寫了一個 Slot，且名字符合以下格式時：
        void on_ [物體名稱] _ [訊號名稱] ();
    */
    void on_Back_Button_clicked();    // 退格鍵
    void on_Clear_Button_clicked();   // 清除鍵
    void onOperatorClicked();               // 統一處理 + - X ÷
    void on_equal_Button_clicked();         // 處理 = 號
    void on_Negate_Button_clicked();        // 正負號
    void on_square_root_Button_clicked();   //平方根
    void on_square_Button_clicked();        //平方
    void on_Percentage_Button_clicked();    //百分比
    void on_LParen_Button_clicked();
    void on_RParen_Button_clicked();

    // --- 科學運算符 ---
    void on_Shift_Button_clicked();   // 切換按鈕文字
    void onScientificClicked();       // 處理 Sin, Cos, Tan, exp...
    void on_PI_Button_clicked();      // 常數 Pi
    //void on_Fun_exp_Button_clicked();   // 處理 exp(x)
    void on_expr_const_Button_clicked(); // 處理常數 e

    // --- 角度單位切換 ---
    void on_AngleUnit_toggled();      // 處理 DEG, RAD, GRA

    //----關於----
    void on_actionchinese_triggered();
    void on_actionEnglish_triggered();

private:
    Ui::NumKeyBoard *ui;
    // 這是 Qt 視窗內建的鍵盤監聽功能
    void keyPressEvent(QKeyEvent *event) override;
    bool isShifted = false; // 紀錄 Shift 是否按下了

signals:  // 新增 signals 區域
    void equalPressed(QString result);  // 按下等於時發送這個 signal，帶計算結果

};
#endif // NUMKEYBOARD_H
