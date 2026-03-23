// mosandbjt.cpp
#include "ui/ui_mosandbjt.h"
//mosandbjt.cpp:2:10: In included file: 'plotcanvas.h' file not found
//ui_mosandbjt.h:27:10: error occurred here
#include "../ui/numkeyboard.h"
#include "../ui/mosandbjt.h"
#include "../include/mosfet.h"
#include "../include/bjt.h"
#include "curve_utils.h"
#include "plotcanvas.h"

#include <QPainter>
#include <QPainterPath>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDebug>
#include <utility>
#include <qpainterpath.h>
#include <QDebug>



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


    // 初始化元件
    initializeTransistors();

    // 設定 UI
    setupUi();

    // 設定繪圖
    setupPlot();

    // 設定訊號連接
    setupConnections();


}

//------------------------------------------------------------------------------
// 初始化函式
//------------------------------------------------------------------------------

void MOSandBJT::initializeTransistors()
{
    //------------------------------------------------------------------------
    // 1. 核心元件初始化
    //------------------------------------------------------------------------

    // 建立 MOSFET 並設定預設值
    m_mosfet = new MOSFET();

    m_mosfet->setParameter("Vth", 2.0);
    m_mosfet->setParameter("lambda", 0.01);
    m_mosfet->setParameter("Rds_on", 0.1);
    m_mosfet->setParameter("gfs", 1.0);
    m_mosfet->setParameter("Id_max", 10.0);
    m_mosfet->setParameter("Vds_max", 100.0);
    m_mosfet->setParameter("kn",0.1);



    // 將 MOSFET 建構子內的預設物理參數搬到 UI
    ui->Vth_lineEdit->setText(QString::number(m_mosfet->getParameter("Vth")));
    ui->Kn_lineEdit->setText(QString::number(m_mosfet->getParameter("Kn")));
    ui->Lmpda_lineEdit->setText(QString::number(m_mosfet->getParameter("lambda")));

    // 補上 Rds(on) 的 UI 顯示（例如建構子是 0.1 Ohm，UI 顯示 100 mOhm）
    ui->Rds_lineEdit->setText(QString::number(m_mosfet->getParameter("Rds_on") * 1000.0));



    // 建立 BJT 並設定預設值（先保留，以後再用）
    m_bjt = new BJT();


    //------------------------------------------------------------------------
    // 2. UI 元件初始化
    //------------------------------------------------------------------------

    m_numpadWindow = nullptr;
    m_plotWidget = ui->mosPlot_widget;

    //------------------------------------------------------------------------
    // 3. 繪圖參數初始化
    //------------------------------------------------------------------------

    // 座標範圍
    m_xMin = 0.0;
    m_xMax = 30.0;
    m_yMin = 0.0;
    m_yMax = 5.0;

    m_xLabel = "Vds (V)";
    m_yLabel = "Id (A)";

    // 曲線樣式
    m_curveColor = Qt::blue;
    m_curveWidth = 2;

    // 曲線資料
    m_curveData.clear();

    //------------------------------------------------------------------------
    // 4. 狀態標記初始化
    //------------------------------------------------------------------------

    m_isMosfetMode = true;    // 預設 MOS 模式
    m_isOutputCurve = true;    // 預設輸出特性

    //------------------------------------------------------------------------
    // 5. 工作點初始化
    //------------------------------------------------------------------------

    //MOS
    m_currentVgs = 0.0;
    m_currentVds = 0.0;
    m_currentId = 0.0;

    //BJT
    m_currentIb = 0.0;
    m_currentVce = 0.0;
    m_currentIc = 0.0;
}


void MOSandBJT::setupUi()
{
    // 隱藏暫時不用的元件
    ui->Input_pushButton->setVisible(false);
    ui->plot_vScrollBar->setVisible(false);
    ui->plot_hScrollBar->setVisible(false);

    // 設定 Id 標籤旋轉
    ui->Id_label->setProperty("rotation", -90);

    // 設定下拉選單
    ui->OutOrtran_comboBox->addItem("輸出特性");
    ui->OutOrtran_comboBox->addItem("轉移特性");
    ui->OutOrtran_comboBox->setCurrentIndex(0);

    // 設定工作點標籤
    ui->Work_Point_label->setText("工作點: --");
}

void MOSandBJT::setupPlot()
{
    // 取得繪圖區域的 widget
    m_plotWidget = ui->mosPlot_widget;
    if (!m_plotWidget) return;

    // 設定一些繪圖相關的初始值
    m_xMin = 0.0;
    m_xMax = 30.0;
    m_yMin = 0.0;
    m_yMax = 5.0;

    m_curveColor = Qt::blue;
    m_curveWidth = 2;

    // 確保 widget 可以接收繪圖事件
    m_plotWidget->setAttribute(Qt::WA_OpaquePaintEvent, true);
    m_plotWidget->setAttribute(Qt::WA_NoSystemBackground, true);

    // 啟用滑鼠追蹤
    m_plotWidget->setMouseTracking(true);

    // 清空曲線資料
    m_curveData.clear();

    // 初始重繪
    m_plotWidget->update();
}


void MOSandBJT::setupConnections()
{
    // 按鈕連線
    connect(ui->calculate_pushButton, &QPushButton::clicked,
            this, &MOSandBJT::on_calculate_pushButton_clicked);

    // 下拉選單連線
    connect(ui->OutOrtran_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MOSandBJT::on_OutOrtran_comboBox_currentIndexChanged);

    // 頁籤切換連線
    connect(ui->tabWidget, &QTabWidget::currentChanged,
            this, &MOSandBJT::on_tabWidget_currentChanged);


    // 當 Vth 或 Rds(on) 輸入完成時，觸發自動換算
    connect(ui->Vth_lineEdit, &QLineEdit::editingFinished,
            this, &MOSandBJT::on_mosfetParameter_changed);
    connect(ui->Rds_lineEdit, &QLineEdit::editingFinished,
            this, &MOSandBJT::on_mosfetParameter_changed);

}


//------------------------------------------------------------------------------
// 事件處理
//------------------------------------------------------------------------------
void MOSandBJT::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_plotWidget) return;

    // 取得滑鼠在繪圖區域的位置
    QPoint pos = event->pos();

    // 檢查是否在繪圖區域內
    if (!m_plotWidget->geometry().contains(pos)) return;

    // 轉換為圖表座標
    QPointF plotCoord = pixelToPlotCoord(pos);

    // 更新工作點標籤
    updateWorkPointLabel(plotCoord);
}


//------------------------------------------------------------------------------
// 按鈕事件
//------------------------------------------------------------------------------

void MOSandBJT::on_calculate_pushButton_clicked()
{
     qDebug() << "calculate button clicked";  // ← 加這行
    // 載入當前頁籤的參數
    int currentTab = ui->tabWidget->currentIndex();

    if (currentTab == 0) {  // MOS 分析
        loadMosfetParameters();

        // 驗證參數
        std::string errorMsg;
        if (!m_mosfet->validateParameters(errorMsg)) {
            QMessageBox::warning(this, "參數錯誤", QString::fromStdString(errorMsg));
            return;
        }

        // 繪製曲線
        plotMosfetCurves();

    } else if (currentTab == 2) {  // BJT 分析（以後實作）
        // 暫時顯示訊息
        QMessageBox::information(this, "提示", "BJT 功能尚未實作");
    }
}

void MOSandBJT::on_OutOrtran_comboBox_currentIndexChanged(int index)
{
    // 0: 輸出特性, 1: 轉移特性
    m_isOutputCurve = (index == 0);

    // 更新座標軸標籤（現在只是更新變數，不直接操作 UI）
    if (m_isMosfetMode) {
        if (m_isOutputCurve) {
            m_xLabel = "Vds (V)";
            m_yLabel = "Id (A)";
        } else {
            m_xLabel = "Vgs (V)";
            m_yLabel = "Id (A)";
        }
    } else {
        if (m_isOutputCurve) {
            m_xLabel = "Vce (V)";
            m_yLabel = "Ic (A)";
        } else {
            m_xLabel = "Ib (A)";
            m_yLabel = "Ic (A)";
        }
    }

    // 觸發重繪，paintEvent 會用新的標籤
    if (m_plotWidget) {
        m_plotWidget->update();
    }
}


void MOSandBJT::on_tabWidget_currentChanged(int index)
{
    // 0: MOS 分析, 1: MOS小訊號, 2: BJT分析, 3: BJT小訊號
    m_isMosfetMode = (index == 0 || index == 1);

    // 更新 UI 顯示
    if (index == 0) {
        ui->Id_label->setText("Id(A)");
        ui->V_label->setText("Vds(V)");
    } else if (index == 2) {
        ui->Id_label->setText("Ic(A)");
        ui->V_label->setText("Vce(V)");
    }

    // 清空工作點標籤
    ui->Work_Point_label->setText("工作點: --");
}



// 在建構子裡，ui->setupUi(this) 之後可以不用寫 connect
// 因為 Qt 會自動把 on_actioncall_keyboard_triggered 連到 actioncall_keyboard 的 triggered 訊號

void MOSandBJT::on_actioncall_keyboard_triggered()
{


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


//------------------------------------------------------------------------------
// 參數載入
//------------------------------------------------------------------------------

void MOSandBJT::loadMosfetParameters()
{
    bool ok;

    // Vth
    double vth = ui->Vth_lineEdit->text().toDouble(&ok);
    if (ok) m_mosfet->setParameter("Vth", vth);   

    // Kn
    double kn = ui->Kn_lineEdit->text().toDouble(&ok);
    if (ok) m_mosfet->setParameter("Kn", kn);

    // lambda
    double lambda = ui->Lmpda_lineEdit->text().toDouble(&ok);
    if (ok) m_mosfet->setParameter("lambda", lambda);

    // Rds_on
    double rds = ui->Rds_lineEdit->text().toDouble(&ok);
    if (ok) m_mosfet->setParameter("Rds_on", rds);

    // gfs
    double gfs = ui->gfs_lineEdit->text().toDouble(&ok);
    if (ok) m_mosfet->setParameter("gfs", gfs);

    // Id_max
    double idMax = ui->Id_lineEdit->text().toDouble(&ok);
    if (ok) m_mosfet->setParameter("Id_max", idMax);

    // Vds_max
    double vdsMax = ui->Vdss_lineEdit->text().toDouble(&ok);
    if (ok) m_mosfet->setParameter("Vds_max", vdsMax);
}


//------------------------------------------------------------------------------
// 繪圖相關
//------------------------------------------------------------------------------

void MOSandBJT::plotMosfetCurves()
{

    qDebug() << "plotMosfetCurves called";  // ← 加這行

    if (!m_plotWidget) return;  // 改用 QWidget

    // 清除舊的繪圖
    clearPlot();

    // 產生參數列表
    QVector<double> paramList;

    if (m_isOutputCurve) {
        qDebug() << "Output curve mode";

        // 輸出特性：多條 Vgs 曲線
        paramList = generateParamList(2.0, 5.0, 4);

        // 產生曲線
        QVector<UICurveData> curves = generateOutputCurves(*m_mosfet, paramList);

        qDebug() << "curves size:" << curves.size();


        // 儲存曲線資料（供 paintEvent 使用）
        m_curveData = curves;


        // 設定顯示範圍
        m_xMin = 0;
        m_xMax = m_mosfet->getParameter("Vds_max");
        m_yMin = 0;
        m_yMax = m_mosfet->getParameter("Id_max");

    } else {
        // 轉移特性
        double vds = 10.0;
        std::vector<Point> points = m_mosfet->transferCurve(vds);

        UICurveData curve(toQtPoints(points), QString("Vds = %1V").arg(vds));
        m_curveData.clear();
        m_curveData.append(curve);

        m_xMin = 0;
        m_xMax = 5;
        m_yMin = 0;
        m_yMax = m_mosfet->getParameter("Id_max");
    }

    // 觸發重繪
    m_plotWidget->update();
}

void MOSandBJT::paintEvent(QPaintEvent *event)
{
    qDebug() << "paintEvent called";  // ← 加這行


    QMainWindow::paintEvent(event);

    if (!m_plotWidget) return;

    QPainter painter(m_plotWidget);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rect = m_plotWidget->rect();
    int margin = 50;
    QRect plotRect = rect.adjusted(margin, margin, -margin, -margin);

    // 繪製背景
    painter.fillRect(plotRect, Qt::white);

    // 繪製座標軸
    painter.setPen(Qt::black);
    painter.drawLine(plotRect.bottomLeft(), plotRect.bottomRight());
    painter.drawLine(plotRect.bottomLeft(), plotRect.topLeft());

    // 繪製刻度標記
    painter.setPen(Qt::gray);
    for (int i = 1; i <= 5; ++i) {
        int x = plotRect.left() + i * plotRect.width() / 5;
        painter.drawLine(x, plotRect.bottom(), x, plotRect.bottom() - 5);

        int y = plotRect.top() + i * plotRect.height() / 5;
        painter.drawLine(plotRect.left(), y, plotRect.left() + 5, y);
    }

    // 繪製 X 軸標籤
    painter.setPen(Qt::black);
    painter.drawText(plotRect.center().x() - 20, rect.bottom() - 10, m_xLabel);

    // 繪製 Y 軸標籤
    painter.save();
    painter.translate(15, plotRect.center().y());
    painter.rotate(-90);
    painter.drawText(0, 0, m_yLabel);
    painter.restore();

    // 繪製曲線
    if (!m_curveData.isEmpty()) {
        QPen pen(m_curveColor);
        pen.setWidth(m_curveWidth);
        painter.setPen(pen);

        for (const auto& curve : std::as_const(m_curveData)) {
            QPainterPath path;
            bool first = true;

            for (const auto& point : std::as_const(curve.points)) {
                double x = qBound(m_xMin, point.x(), m_xMax);
                double y = qBound(m_yMin, point.y(), m_yMax);

                int px = plotRect.left() + (x - m_xMin) / (m_xMax - m_xMin) * plotRect.width();
                int py = plotRect.bottom() - (y - m_yMin) / (m_yMax - m_yMin) * plotRect.height();

                if (first) {
                    path.moveTo(px, py);
                    first = false;
                } else {
                    path.lineTo(px, py);
                }
            }

            painter.drawPath(path);
        }
    }
}


void MOSandBJT::plotBjtCurves()
{
    // 以後實作
}

void MOSandBJT::clearPlot()
{
    m_curveData.clear();
    if (m_plotWidget) {
        m_plotWidget->update();
    }
}


void MOSandBJT::updateWorkPointLabel(const QPointF& pos)
{
    // 檢查座標是否在合理範圍內
    if (pos.x() < 0 || pos.y() < 0) {
        ui->Work_Point_label->setText("工作點: --");
        return;
    }

    QString text;

    if (m_isMosfetMode) {
        if (m_isOutputCurve) {
            // 輸出特性：Vds, Id
            text = QString("工作點: Vds = %1 V, Id = %2 A")
                       .arg(pos.x(), 0, 'f', 3)
                       .arg(pos.y(), 0, 'f', 3);
        } else {
            // 轉移特性：Vgs, Id
            text = QString("工作點: Vgs = %1 V, Id = %2 A")
                       .arg(pos.x(), 0, 'f', 3)
                       .arg(pos.y(), 0, 'f', 3);
        }
    } else {
        if (m_isOutputCurve) {
            // BJT 輸出特性：Vce, Ic
            text = QString("工作點: Vce = %1 V, Ic = %2 A")
                       .arg(pos.x(), 0, 'f', 3)
                       .arg(pos.y(), 0, 'f', 3);
        } else {
            // BJT 轉移特性：Ib, Ic
            text = QString("工作點: Ib = %1 A, Ic = %2 A")
                       .arg(pos.x(), 0, 'f', 6)
                       .arg(pos.y(), 0, 'f', 3);
        }
    }

    ui->Work_Point_label->setText(text);
}



//------------------------------------------------------------------------------
// 座標轉換
//------------------------------------------------------------------------------


QPointF MOSandBJT::pixelToPlotCoord(const QPoint& pos) const
{
    if (!m_plotWidget) return QPointF(-1, -1);

    QRect rect = m_plotWidget->rect();
    int margin = 50;
    QRect plotRect = rect.adjusted(margin, margin, -margin, -margin);

    if (!plotRect.contains(pos)) return QPointF(-1, -1);

    double x = m_xMin + (pos.x() - plotRect.left()) * (m_xMax - m_xMin) / plotRect.width();
    double y = m_yMax - (pos.y() - plotRect.top()) * (m_yMax - m_yMin) / plotRect.height();

    return QPointF(x, y);
}



//------------------------------------------------------------------------------
// 工具函式
//------------------------------------------------------------------------------

QString MOSandBJT::formatWorkPoint(double value, const QString& unit) const
{
    if (qAbs(value) < 1e-6) {
        return QString("0 %1").arg(unit);
    } else if (qAbs(value) < 1e-3) {
        return QString("%1 u%2").arg(value * 1e6, 0, 'f', 2).arg(unit);
    } else if (qAbs(value) < 1) {
        return QString("%1 m%2").arg(value * 1e3, 0, 'f', 2).arg(unit);
    } else {
        return QString("%1 %2").arg(value, 0, 'f', 3).arg(unit);
    }
}

void MOSandBJT::on_mosfetParameter_changed()
{
    bool okVth, okRds;
    double vth = ui->Vth_lineEdit->text().toDouble(&okVth);
    double rds_mOhm = ui->Rds_lineEdit->text().toDouble(&okRds);

    // 預設規格書測試時的 Vgs 電壓為 10V (一般常用值)
    double vgs_test = 10.0;

    // 驗證守門員：確保兩個數字都成功轉換為 double
    if (okVth) {
        if (okRds) {
            // 單位換算：mOhm 轉成 Ohm
            double rds_Ohm = rds_mOhm / 1000.0;

            // 呼叫你已經寫在 MOSFET 裡的演算法
            double calculatedKn = m_mosfet->calculateKnFromRds(rds_Ohm, vgs_test, vth);

            // 安全檢查：只有結果大於 0 且有效才填入
            if (calculatedKn > 0.0) {
                // 將計算結果填入介面，取小數點後 4 位
                ui->Kn_lineEdit->setText(QString::number(calculatedKn, 'f', 4));
            }

            // 同步算出 gfs 並更新 UI
            double calculatedGfs_S = 1.0 / rds_Ohm; // 簡化邏輯：gfs = 1/Rds
            ui->gfs_lineEdit->setText(QString::number(calculatedGfs_S * 1000.0, 'f', 2));
        }
    }
}

MOSandBJT::~MOSandBJT()
{
    delete ui;
}
