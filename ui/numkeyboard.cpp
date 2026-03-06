#include "numkeyboard.h"
#include "ui/ui_numkeyboard.h"


#include <QKeyEvent>
#include <string>
#include <QClipboard>
#include <QGuiApplication>

#include <QMessageBox>
/*
CalculatorUI::CalculatorUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
*/


    NumKeyBoard::NumKeyBoard(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NumKeyBoard)

{
    //UI Begin

    ui->setupUi(this);

    // --- 1. 暴力修復「消失的單元選擇區」 ---
    // 這裡解決你說 Layout 不見的問題
    // 強制讓那個名為 "widget" 的容器使用水平佈局，並把 RadioButton 塞進去
    if (ui->widget->layout() == nullptr) {
        QHBoxLayout *unitLayout = new QHBoxLayout(ui->widget);
        unitLayout->setContentsMargins(5, 0, 5, 0); // 左右留一點邊
        unitLayout->addWidget(ui->DEG_Radiobuttom);
        unitLayout->addWidget(ui->RAD_Radiobuttom);
        unitLayout->addWidget(ui->GRA_Radiobuttom);
        unitLayout->addWidget(ui->REC_Check);
        ui->widget->setLayout(unitLayout);
    }
    ui->widget->setFixedHeight(35); // 強制給它高度，不准消失

    // --- 2. 修正截圖中「中間空一大塊」的問題 ---
    // 你的 verticalLayout 裡面有 Label, LineEdit, Widget
    // 我們要分配比例：Label(0), LineEdit(1), Widget(0)
    ui->verticalLayout->setStretch(0, 0); // Label 固定
    ui->verticalLayout->setStretch(1, 1); // LineEdit 填滿
    ui->verticalLayout->setStretch(2, 0); // 單元選擇區固定

    // --- 3. 修正主視窗比例與對齊 ---
    ui->scientific_panel->setFixedHeight(430);
    ui->Main_widget->setFixedHeight(430);
    ui->scientific_panel->setFixedWidth(140);
    ui->Main_widget->setFixedWidth(300);


    // --- 3. 解決「下面被削掉」的問題 ---
    // 調整外層佈局 (verticalLayout_2) 的邊距
    ui->verticalLayout_2->setContentsMargins(5, 5, 5, 10);
    ui->verticalLayout_2->setSpacing(5);


    // 強制設定視窗啟動狀態
    ui->scientific_panel->hide();
    ui->statusbar->hide();
    this->setFixedWidth(310);

    // 強制讓 QLineEdit 不要佔用過多高度，把空間讓給下面的按鈕
    ui->lineEdit->setMaximumHeight(100);

    // 【最重要的一行】自動調整高度，確保最後一排按鈕不會被削掉
    this->adjustSize();
    // 強制將視窗高度鎖定在計算出來的完美高度上
    this->setFixedHeight(this->sizeHint().height());

    // --- 4. 視窗與螢幕初始化 ---
    ui->lineEdit->setMinimumWidth(0); // 拿掉 486 限制
    ui->lineEdit->setAlignment(Qt::AlignRight);
    ui->lineEdit->setText("0");
    ui->lineEdit->setReadOnly(true);
    ui->label->setText("DEG Mode");

    // 1. 強放「禁言術」：不准螢幕搶奪鍵盤焦點
    ui->lineEdit->setFocusPolicy(Qt::NoFocus);

    // 2. 「吸星大法」：強制讓主視窗拿到焦點
    this->setFocus();

    // 在建構子連線
    connect(ui->REC_Check, &QCheckBox::toggled, this, [this](bool isRecording) {

        /*
        if (isRecording) {
            // 呼叫你的錄製啟動功能
            engine.startLogging();
            ui->label->setText("REC ON");
            ui->label->setStyleSheet("color: red;"); // 錄製時標籤變紅提醒
        } else {
            // 呼叫你的停止錄製功能
            engine.stopLogging();
            ui->label->setText("REC OFF");
            ui->label->setStyleSheet(""); // 恢復原色
        }
        */
    });


    //UI End

    //Function Begin

    QList<QPushButton*> digitButtons = {
        ui->Button_0, ui->Button_1, ui->Button_2, ui->Button_3, ui->Button_4,
        ui->Button_5, ui->Button_6, ui->Button_7, ui->Button_8, ui->Button_9,
        ui->Dot_Button
    };

    // 用循環把他們全部連到同一個功能函式
    for (QPushButton* btn : digitButtons) {
        if (btn) {

            /*  connect的用法
             *  connect(
             *   const typename QtPrivate::FunctionPointer<Func1>::Object *sender,
             *   Func1 signal,
             *   const typename QtPrivate::ContextTypeForFunctor<Func2>::ContextType *context,
             *   Func2 &&slot,
             *   Qt::ConnectionType type = Qt::AutoConnection)
             */
            connect(btn, &QPushButton::clicked, this, &NumKeyBoard::onDigitClicked);
        }
    }

    // --- 運算符連線 ---
    QList<QPushButton*> opButtons = {
        ui->Add_Button, ui->Subtraction_Button,
        ui->Multiplication_Button, ui->Division_Button
    };

    for (QPushButton* btn : opButtons) {
        if (btn) {
            connect(btn, &QPushButton::clicked, this, &NumKeyBoard::onOperatorClicked);
        }
    }
    QList<QPushButton*> sciButtons = {
        ui->Fun_sin_Button, ui->Fun_Cos_Button, ui->fun_Tan_Button,
        ui->Fun_exp_Button, ui->Power_of_Button, ui->Fun_FIB_Button,
        ui->Remainder_Button
    };
    for (QPushButton* btn : sciButtons) {
        connect(btn, &QPushButton::clicked, this, &NumKeyBoard::onScientificClicked);
    }

    // 2. 連線角度單位單選鈕 (名字是根據你之前的 UI XML)
    connect(ui->DEG_Radiobuttom, &QRadioButton::toggled, this, &NumKeyBoard::on_AngleUnit_toggled);
    connect(ui->RAD_Radiobuttom, &QRadioButton::toggled, this, &NumKeyBoard::on_AngleUnit_toggled);
    connect(ui->GRA_Radiobuttom, &QRadioButton::toggled, this, &NumKeyBoard::on_AngleUnit_toggled);

    //鍵盤輸入


}

void NumKeyBoard::on_Menu_Button_clicked()
{
    bool isVisible = !ui->scientific_panel->isVisible();
    qDebug() << "Menu clicked, setting visible to:" << isVisible;
    qDebug() << "Panel geometry before:" << ui->scientific_panel->geometry();

    // 先記住目前的高度，避免寬度變動時高度也跟著跳
    int targetHeight = this->height();

    ui->scientific_panel->setVisible(isVisible);
    qDebug() << "Panel geometry after:" << ui->scientific_panel->geometry();
    qDebug() << "Main_widget geometry:" << ui->Main_widget->geometry();

    if (isVisible) {
        // 展開寬度：左150 + 右300 + 邊距
        this->setFixedWidth(465);
    } else {
        // 縮回寬度：右300 + 邊距
        this->setFixedWidth(315);
    }

    // 確保高度維持不變，不會被削掉
    this->setFixedHeight(targetHeight);
    this->layout()->activate();  // 強制重新計算佈局
    this->update();              // 強制重繪
}

// 核心：所有數字按鈕點了都會跑進這裡
void NumKeyBoard::onDigitClicked() {
    // 誰點了我？
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    QString buttonText = button->text();
    QString currentText = ui->lineEdit->text();

    // 如果螢幕現在是 0，直接換成點擊的數字；除非點的是小數點
    if (currentText == "0") {
        if (buttonText == ".") {
            ui->lineEdit->setText("0.");
        } else {
            ui->lineEdit->setText(buttonText);
        }
    } else {
        // 防止出現兩個小數點
        if (buttonText == "." && currentText.contains(".")) return;
        ui->lineEdit->setText(currentText + buttonText);
    }
}

// 清除鍵：直接回歸原始
void NumKeyBoard::on_Clear_Button_clicked() {
    ui->lineEdit->setText("0");
    ui->label->setText("Ready"); // 隨便寫個狀態
}

// 退格鍵：砍掉最後一個字
void NumKeyBoard::on_Back_Button_clicked() {
    QString text = ui->lineEdit->text();
    if (text == "0") return;

    text.chop(1); // 刪掉最後一個字元

    // 如果刪完變空的，或是只剩一個負號，就填 0
    if (text.isEmpty() || text == "-") {
        text = "0";
    }
    ui->lineEdit->setText(text);
}

// 左括號 (
void NumKeyBoard::on_LParen_Button_clicked() {
    QString currentText = ui->lineEdit->text();

    // 如果目前是 0，直接取代成 (；否則追加
    if (currentText == "0") {
        ui->lineEdit->setText("(");
    } else {
        ui->lineEdit->setText(currentText + "(");
    }
}

// 右括號 )
void NumKeyBoard::on_RParen_Button_clicked() {
    QString currentText = ui->lineEdit->text();

    // 右括號通常不單獨存在，也不取代 0，直接追加即可
    // 如果目前是 0，可以不做反應或變成 0)
    if (currentText != "0") {
        ui->lineEdit->setText(currentText + ")");
    }
}




void NumKeyBoard::keyPressEvent(QKeyEvent *event)
{
    // 檢查是否有按下 Control 鍵
    if (event->modifiers() & Qt::ControlModifier) {

        // --- 複製功能 (Ctrl + C) ---
        if (event->key() == Qt::Key_C) {
            QClipboard *clipboard = QGuiApplication::clipboard();
            clipboard->setText(ui->lineEdit->text());
            ui->label->setText("Copied to clipboard");
            return; // 處理完就跳出
        }

        // --- 貼上功能 (Ctrl + V 或 Ctrl + P) ---
        // 雖然標準是 V，但照你的要求加入 P
        else if (event->key() == Qt::Key_V) {
            QClipboard *clipboard = QGuiApplication::clipboard();
            QString clipText = clipboard->text();

            if (!clipText.isEmpty()) {
                QString currentText = ui->lineEdit->text();
                if (currentText == "0") {
                    ui->lineEdit->setText(clipText);
                } else {
                    ui->lineEdit->setText(currentText + clipText);
                }
                ui->label->setText("Pasted from clipboard");
            }
            return;
        }
    }

    // 取得按下的按鍵代碼
    int key = event->key();

    // 1. 處理數字鍵與小數點 (支援主鍵盤與數字小鍵盤)
    if (key >= Qt::Key_0 && key <= Qt::Key_9) {
        // 算出對應的按鈕。例如按下 Key_5，就去找 Button_5
        QString btnName = QString("Button_%1").arg(key - Qt::Key_0);
        QPushButton* btn = this->findChild<QPushButton*>(btnName);
        if (btn) btn->animateClick();
    }
    else if (key == Qt::Key_Period) {
        ui->Dot_Button->animateClick();
    }

    // 2. 處理倒退鍵 (Backspace)
    else if (key == Qt::Key_Backspace) {
        ui->Back_Button->animateClick();
    }

    // 3. 處理清除鍵 (ESC) - 依照您的要求連結到 ESC
    else if (key == Qt::Key_Escape) {
        ui->Clear_Button->animateClick();
    }

    // 4. 處理運算子
    else if (key == Qt::Key_Plus)      ui->Add_Button->animateClick();
    else if (key == Qt::Key_Minus)     ui->Subtraction_Button->animateClick();
    else if (key == Qt::Key_Asterisk)  ui->Multiplication_Button->animateClick(); // 鍵盤的 *
    else if (key == Qt::Key_Slash)     ui->Division_Button->animateClick();       // 鍵盤的 /

    // 5. 處理等於 (Enter 或 Return)
    else if (key == Qt::Key_Enter || key == Qt::Key_Return) {
        ui->equal_Button->animateClick();
    }


    // 處理左括號 ( 通常是 Shift + 9
    if (event->text() == "(") {
        ui->LParen_Button->animateClick();
    }
    // 處理右括號 ) 通常是 Shift + 0
    else if (event->text() == ")") {
        ui->RParen_Button->animateClick();
    }

    // 如果是其他沒處理的鍵，丟回給父類別處理，不要吃掉它
    else {
        QMainWindow::keyPressEvent(event);
    }
}

void NumKeyBoard::onOperatorClicked() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    QString op = button->text();
    QString currentText = ui->lineEdit->text();

    //如果螢幕是 0 且按了負號，當作負數開頭
    if (currentText == "0" && op == "-") {
        ui->lineEdit->setText("-");
        return;
    }

    // 防止重複輸入符號 (例如 5++3)
    // 如果最後一個字已經是符號，就替換掉它
    if (!currentText.isEmpty()) {
        QChar lastChar = currentText.at(currentText.length() - 1);
        QString operators = "+-X÷";
        if (operators.contains(lastChar)) {
            currentText.chop(1); // 砍掉最後一個重複的符號
        }
    }

    // 轉換符號為引擎認識的標準算式
    if (op == "X") op = "*";
    if (op == "÷") op = "/";

    ui->lineEdit->setText(currentText + op);
}


void NumKeyBoard::on_equal_Button_clicked() {
    QString formula = ui->lineEdit->text();

    if (formula == "0" || formula.isEmpty()) return;
    qDebug() << "NumKeyBoard: emitting equalPressed, formula =" << formula;  // 加這行


    // 發送目前的算式
    emit equalPressed(formula);

    // 如果你想要等號後清空或顯示結果，可以在這裡加
     ui->lineEdit->setText("0");


/*
    try {
        std::string expression = formula.toStdString();
        auto res = engine.execute(expression);

        // 這裡要檢查你的引擎是否有 Success 狀態
        // 假設你的引擎狀態叫 res.status，成功是 Success
        if (res.status == Calculator::CalcStatus::Success ){
            //語法:
            //使用enum class的方法
            //<ClassName>::<EmmuClassName>::<EmmuClassElement>
            //因為Success是定義在CalcStatus之中,又是在Calculator之中,所以使用的方式為:
            //Calculator::CalcStatus::Success

            ui->label->setText(formula + " =");
            ui->lineEdit->setText(QString::number(res.value));
        } else {
            // 引擎告訴你算式有問題 (例如括號沒對齊)
            ui->label->setText("Error: " + QString::fromStdString(engine.statusToText(res.status)));
        }

    } catch (...) {
        ui->label->setText("System Error!");
    }*/
}

// 1. 正負號 (±) - 最簡單的字串反轉
void NumKeyBoard::on_Negate_Button_clicked() {
    QString text = ui->lineEdit->text();
    /*
    if (text == "0" || text.isEmpty()) return;

    // 如果開頭有負號就去掉，沒負號就加上去
    if (text.startsWith("-")) {
        text.remove(0, 1);
    } else {
        text.prepend("-");
    }
    */
    ui->lineEdit->setText(text);
}

// 2. 開根號 (√) - 轉換為引擎認識的 sqrt( )
void NumKeyBoard::on_square_root_Button_clicked() {
    QString text = ui->lineEdit->text();
    ui->lineEdit->setText(text + "sqrt(");
/*
    // 如果目前是 0，直接變成 sqrt(
    if (text == "0") {
        ui->lineEdit->setText("sqrt(");
    } else {
        // 如果前面已經有數字，通常是乘上根號或是直接追加
        ui->lineEdit->setText(text + "sqrt(");
    }
*/
}

// 3. 平方 (x²) - 轉換為引擎認識的 ^2
void NumKeyBoard::on_square_Button_clicked() {
    QString text = ui->lineEdit->text();

    //if (text == "0") return;
    // 在當前算式後面加上平方符號

    ui->lineEdit->setText(text + "^2");
}

void NumKeyBoard::on_Percentage_Button_clicked() {
    QString text = ui->lineEdit->text();
/*
    // 如果是 0，按百分比沒意義，直接無視
    if (text == "0" || text.isEmpty()) return;

    // 檢查最後一個字元，防止連續出現 5/100/100 (除非你真的想這樣算)
    if (text.endsWith("/100")) return;

    // 直接在現有的算式後面加上 /100
    // 這樣當你輸入 50 然後按 %，螢幕顯示 50/100，按下 = 就會得到 0.5
    ui->lineEdit->setText(text + "/100");*/
    ui->lineEdit->setText(text + "%");
}

void NumKeyBoard::on_Shift_Button_clicked() {
    isShifted = !isShifted; // 切換狀態

    if (isShifted) {
        ui->Fun_sin_Button->setText("asin(");
        ui->Fun_Cos_Button->setText("acos(");
        ui->fun_Tan_Button->setText("atan(");
        ui->Power_of_Button->setText("y√x"); // 開 Y 次根號
        ui->Fun_exp_Button->setText("ln(x)"); //<<新增的
        ui->Shift_Button->setStyleSheet("background-color: #ffaa00; color: white;"); // 變色提醒
    } else {
        ui->Fun_sin_Button->setText("sin(");
        ui->Fun_Cos_Button->setText("cos(");
        ui->fun_Tan_Button->setText("tan(");
        ui->Power_of_Button->setText("x^y");
        ui->Fun_exp_Button->setText("exp(x)");
        //ui->Shift_Button->setStyleSheet(""); // 恢復原色
        ui->Shift_Button->setStyleSheet("background-color: #fbfbfb; color: #555555; font-weight: normal;");
    }


}
void NumKeyBoard::onScientificClicked() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    QString currentText = ui->lineEdit->text();

#define use_string_func

#ifdef use_string_func
    QString func = button->text();
    //qDebug() << "當前按鈕的文字是:" << func;

#endif
    // 如果現在是 0，先清空再加函數
    if (currentText == "0") currentText = "";

    // 根據按鈕處理字串

#ifdef use_Buttom_obj
    if (button == ui->Fun_sin_Button) {
        ui->lineEdit->setText(currentText + (isShifted ? "asin(" : "sin("));
    }
    else if (button == ui->Fun_Cos_Button) {
        ui->lineEdit->setText(currentText + (isShifted ? "acos(" : "cos("));
    }
    else if (button == ui->fun_Tan_Button) {
        ui->lineEdit->setText(currentText + (isShifted ? "atan(" : "tan("));
    }
    else if (button == ui->Fun_exp_Button) {
        // 這裡解決 exp(x) 沒反應的問題
        ui->lineEdit->setText(currentText + (isShifted ? "ln(" : "exp("));
    }
    else if (button == ui->Power_of_Button) {
        ui->lineEdit->setText(currentText + (isShifted ? "yroot(" : "^"));
    }
#endif

#ifdef use_string_func

    if (func.contains("sin"))  ui->lineEdit->setText(currentText + (isShifted ? "asin(" : "sin("));
    else if (func.contains("cos")) ui->lineEdit->setText(currentText + (isShifted ? "acos(" : "cos("));
    else if (func.contains("tan")) ui->lineEdit->setText(currentText + (isShifted ? "atan(" : "tan("));
    else if (func == "exp(x)")     ui->lineEdit->setText(currentText + "exp(");
    else if (func == "ln(x)")      ui->lineEdit->setText(currentText + "ln(");
    else if (func == "x^y")        ui->lineEdit->setText(currentText + "^");
    else if (func == "Fib(x)")     ui->lineEdit->setText(currentText + "fib(");
    else if (func == "Rem")        ui->lineEdit->setText(currentText + " % ");
#endif

    if (isShifted) {
        on_Shift_Button_clicked(); // 直接呼叫該函數來執行切換回來的邏輯
    }
}

void NumKeyBoard::on_AngleUnit_toggled() {

    // 1. 確保名稱與 UI 編輯器一致：DEG_Radiobuttom
    // 2. 透過 engine 的 public 函數來設定，而不是直接碰 .trig
/*
    if (ui->DEG_Radiobuttom->isChecked()) {
        engine.setAngleUnit(TrigEngine::AngleUnit::Degree);
        ui->label->setText("DEG Mode");
    }
    else if (ui->RAD_Radiobuttom->isChecked()) {
        engine.setAngleUnit(TrigEngine::AngleUnit::Radian);
        ui->label->setText("RAD Mode");
    }
    else if (ui->GRA_Radiobuttom->isChecked()) {
        engine.setAngleUnit(TrigEngine::AngleUnit::Gradian);
        ui->label->setText("GRA Mode");
    }
    */
}

// Pi 是常數，單獨處理
void NumKeyBoard::on_PI_Button_clicked() {
    QString currentText = ui->lineEdit->text();
    ui->lineEdit->setText(currentText + "PI");

    /*
    if (currentText == "0") ui->lineEdit->setText("PI");
    else ui->lineEdit->setText(currentText + "PI");
    */
}

// 2. 常數 e (Expr(c))：噴出 "E"
void NumKeyBoard::on_expr_const_Button_clicked() {
    QString currentText = ui->lineEdit->text();
    ui->lineEdit->setText(currentText + "Expr");
/*
    // 如果目前是 0，直接變成 E
    if (currentText == "0") {
        ui->lineEdit->setText("Expr");
    } else {
        // 追加常數 E
        ui->lineEdit->setText(currentText + "Expr");
    }
*/
}





void NumKeyBoard::on_actionchinese_triggered(){
    // 使用 QMessageBox 的靜態函數 about
    QMessageBox::about(this,
                       tr("關於本軟體"),
                       tr("<h3>科學計算機 V1.0</h3>"
                          "<p>版權所有 © 2026 Ethan Yang (Markscat)</p>"
                          "<p>本程式提供 :<br/>"
                          "1. 基本四則運算<br/>"
                          "2. 三角函數計算<br/>"
                          "3. 反三角函數計算<br/>"
                          "4. 自然對數e和ln計算<br/>"
                          "5. 百分率、餘數計算<br/>"

                          "有興趣討論的話,請發郵件給我"
                          "<a href='mailto:markscat@gmail.com'>markscat@gmail.com</a></p>"

                          "<p>本程式為自由軟體；您可以根據自由軟體基金會所發佈的 "
                          "GNU 通用公共許可證 (GNU General Public License) 條款，"
                          "對其進行重新分發和/或修改；無論您使用的是許可證的第三版，"

                          "<p>發佈此程式是希望它能發揮作用，但<b>不提供任何保證</b>；"
                          "甚至不包含對<b>適銷性</b>或<b>特定用途適用性</b>的暗示性保證。"
                          "詳情請參閱 GNU 通用公共許可證。</p>"
                          "<p>請參閱 <a href='https://www.gnu.org/licenses/'>https://www.gnu.org/licenses/</a>。</p>"));

}


void NumKeyBoard::on_actionEnglish_triggered(){
    QMessageBox::about(this,tr("About This:"),
                       tr("Engineering Computer"
                          "<p>all rights reserved © 2026 Ethan Yang (Markscat)</p>"
                          "<p>eMail:<a href='mailto:markscat@gmail.com'>markscat@gmail.com</a></p>"
                          "<p>This program is released in the hope that it will work, but <b>no guarantees are made</b>;"
                          "not even including the implied warranties of <b>merchantability</b> or <b>fitness for a particular purpose</b>."
                          "For details, please refer to the GNU General Public License.</p>"
                          "<p><a href='https://www.gnu.org/licenses/'>https://www.gnu.org/licenses/</a>。</p>"
                          ));
}

/*
CalculatorUI::~CalculatorUI()
{
    delete ui;
}
*/





NumKeyBoard::~NumKeyBoard()
{
    delete ui;
}
