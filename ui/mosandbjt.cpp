// mosandbjt.cpp
#include "ui/ui_mosandbjt.h"
#include "../ui/numkeyboard.h"
#include "../ui/mosandbjt.h"
//#include "../qcustomplot.h"  // 上一層目錄
#include "QMessageBox"

/* 本程式使用簡化物理模型計算電晶體特性曲線，所有參數皆來自元件規格書。
 * 由於實際元件存在製程變異、溫度效應、寄生參數等因素，計算結果僅供參考。
 * 若要獲得精確數值，請以實際量測為準。
 */



/*
╔═══════════════════════════════╗
║        重要聲明 - 必讀          ║
╠═══════════════════════════════╣
║ 本程式使用簡化物理模型，僅適用於   ║
║ 傳統矽基 MOSFET 和 BJT。        ║
║                               ║
║ 若要獲得精確結果，請：           ║
║ 1. 確認你的電晶體符合上述類型     ║
║ 2. 輸入合理的參數（可參考規格書）  ║
║ 3. 自行驗證計算結果              ║
║                                ║
║ 亂玩參數導致結果不準，恕不負責。    ║
║                                ║
║         [ 我同意，開始使用 ]     ║
╚════════════════════════════════╝

╔═════════════════════════════╗

║ Important Notice - Must Read ║

╠══════════════════════════════╣
║ This program uses a simplified physical model and is only applicable to ║
║ Traditional silicon-based MOSFETs and BJTs. ║
║
║ To obtain accurate results, please: ║
║ 1. Confirm that your transistor conforms to the above type. ║
║ 2. Enter reasonable parameters (refer to the datasheet). ║
║ 3. Verify the calculation results yourself. ║
║ We are not responsible for inaccurate results caused by arbitrarily adjusting parameters.
║ [I agree, start using] ║

*/
MOSandBJT::MOSandBJT(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MOSandBJT)
{
    ui->setupUi(this);

    //限制宣告
    QMessageBox::StandardButton reply;
    reply=QMessageBox::question(this, "重要聲明 - 必讀",
                         "本程式使用簡化物理模型計算電晶體特性曲線，所有參數皆來自元件規格書。\n\n"
                         "由於實際元件存在製程變異、溫度效應、寄生參數等因素，計算結果僅供參考。\n"
                         "本程式僅適用於傳統矽基 MOSFET 和 BJT。\n\n"
                         "若要獲得精確數值，請:\n\n"
                         "1. 確認你的電晶體符合上述類型\n"
                         "2. 輸入合理的參數（可參考規格書）\n"
                         "3. 自行驗證計算結果\n"
                         "亂玩參數導致結果不準，恕不負責。\n"
                         "點擊「OK」即表示你同意上述聲明。\n\n"
                         "Important Notice - Must Read\n\n"
                         "This program uses a simplified physical model and is only applicable to\n"
                         "Traditional silicon-based MOSFETs and BJTs.\n"
                         "To obtain accurate results, please:\n\n"
                         "1. Confirm that your transistor conforms to the above type.\n"
                         "2. Enter reasonable parameters (refer to the datasheet).\n"
                         "3. Verify the calculation results yourself.\n"
                         "We are not responsible for inaccurate results caused by arbitrarily adjusting parameters.\n"
                         "[I agree, start using]\n",
                        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        // 使用者不同意，直接關閉程式
        exit(0);
    }

    ui->Input_pushButton->setVisible(false);
    ui->plot_vScrollBar->setVisible(false);
    ui->plot_hScrollBar->setVisible(false);
    ui->Id_label->setProperty("rotation", -90);


    // 在 MainWindow 建構子中
    ui->OutOrtran_comboBox->addItem("輸出特性");
    ui->OutOrtran_comboBox->addItem("轉移特性");
    ui->OutOrtran_comboBox->setCurrentIndex(0);  // 預設選第一個

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

    ui->Vth_lineEdit->setText(expression);  // 把整個算式放進去
    // 或者你要在這裡計算結果也可以
}

// 如果用傳統 SIGNAL/SLOT
void MOSandBJT::onNumpadClosed()
{
    numpadWindow = nullptr;
}

/*
// 在 ui/mainwindow.cpp 裡
std::vector<Point> corePoints = mos.outputCurve(3.5);
QVector<QPointF> qtPoints;
for (const auto& p : corePoints) {
    qtPoints.append(QPointF(p.x, p.y));
}
// 再用 qtPoints 畫圖
*/

MOSandBJT::~MOSandBJT()
{
    delete ui;
}
