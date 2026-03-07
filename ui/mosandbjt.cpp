// mosandbjt.cpp
#include "ui/ui_mosandbjt.h"
#include "../ui/numkeyboard.h"
#include "../ui/mosandbjt.h"
/* 本程式使用簡化物理模型計算電晶體特性曲線，所有參數皆來自元件規格書。
 * 由於實際元件存在製程變異、溫度效應、寄生參數等因素，計算結果僅供參考。
 * 若要獲得精確數值，請以實際量測為準。
 */



/*
╔════════════════════════════════════╗
║        重要聲明 - 必讀             ║
╠═══════════════════════════════╣
║ 本程式使用簡化物理模型，僅適用於   ║
║ 傳統矽基 MOSFET 和 BJT。           ║
║                                    ║
║ 若要獲得精確結果，請：             ║
║ 1. 確認你的電晶體符合上述類型      ║
║ 2. 輸入合理的參數（可參考規格書）  ║
║ 3. 自行驗證計算結果                ║
║                                    ║
║ 亂玩參數導致結果不準，恕不負責。   ║
║                                    ║
║         [ 我同意，開始使用 ]        ║
╚════════════════════════════════════╝
*/
MOSandBJT::MOSandBJT(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MOSandBJT)
{
    ui->setupUi(this);


}


// 在建構子裡，ui->setupUi(this) 之後可以不用寫 connect
// 因為 Qt 會自動把 on_actioncall_keyboard_triggered 連到 actioncall_keyboard 的 triggered 訊號

void MOSandBJT::on_actioncall_keyboard_triggered()
{
    qDebug() << "numpadWindow before new:" << numpadWindow;

    static NumKeyBoard *numpad = nullptr;  // static 變數，只初始化一次

    if (!numpadWindow) {
        numpadWindow = new NumKeyBoard(nullptr);

        // 設定當 numpad 被銷毀時自動把指標設為 nullptr
        QObject::connect(numpadWindow, &QObject::destroyed, [&]() { numpadWindow = nullptr; });

        connect(numpadWindow, &NumKeyBoard::equalPressed,
                this, &MOSandBJT::onNumpadEqualPressed);

        connect(numpadWindow, &QObject::destroyed, this, [this]() {
            numpadWindow = nullptr;
        });
    }

    numpadWindow->show();
    numpadWindow->raise();
    numpadWindow->activateWindow();
}

void MOSandBJT::onNumpadEqualPressed(QString expression)
{
    qDebug() << "MOSandBJT: onNumpadEqualPressed received =" << expression;  // 加這行

    ui->lineEdit->setText(expression);  // 把整個算式放進去
    // 或者你要在這裡計算結果也可以
}

// 如果用傳統 SIGNAL/SLOT
void MOSandBJT::onNumpadClosed()
{
    numpadWindow = nullptr;
}

MOSandBJT::~MOSandBJT()
{
    delete ui;
}
