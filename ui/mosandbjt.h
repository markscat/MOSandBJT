#ifndef MOSANDBJT_H
#define MOSANDBJT_H

#include <QMainWindow>
#include <QPointF>
#include <vector>
#include "QVector"
#include "curve_utils.h"

using UICurveData = struct UICurveData;  // 明確告訴編譯器這是個類型

// 前向宣告
QT_BEGIN_NAMESPACE
namespace Ui { class MOSandBJT; }
QT_END_NAMESPACE

class MOSFET;
class BJT;
class NumKeyBoard;
class QCustomPlot;
class QCPGraph;

class MOSandBJT : public QMainWindow
{
    Q_OBJECT

public:
    explicit MOSandBJT(QWidget *parent = nullptr);
    ~MOSandBJT();

protected:
    // 事件處理
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    // UI 按鈕事件
    void on_calculate_pushButton_clicked();
    void on_actioncall_keyboard_triggered();
    void on_OutOrtran_comboBox_currentIndexChanged(int index);
    void on_tabWidget_currentChanged(int index);

    // 鍵盤相關
    void onNumpadEqualPressed(QString expression);
    void onNumpadClosed();

    void on_mosfetParameter_changed(); // 處理 Vth 或 Rds(on) 的連動換算


private:
    // 初始化相關
    void initializeTransistors();
    void setupUi();
    void setupPlot();
    void setupConnections();
    void loadMosfetParameters();

    // 繪圖相關
    void plotMosfetCurves();
    void plotBjtCurves();
    void clearPlot();
    void updateWorkPointLabel(const QPointF& pos);

    // 座標轉換
    QPointF pixelToPlotCoord(const QPoint& pos) const;
    bool isPointInPlotArea(const QPoint& pos) const;

    // 計算相關
    void calculateMosfetQPoint();
    void calculateBjtQPoint();
    double findCurrentFromVoltage(double voltage) const;
    double findVoltageFromCurrent(double current) const;


    // 工具函式
    QString formatWorkPoint(double value, const QString& unit) const;

    NumKeyBoard *numpadWindow = nullptr;  // 用來記住 numkeyboard 視窗


    Ui::MOSandBJT *ui;

    // 核心元件
    MOSFET *m_mosfet;
    BJT *m_bjt;
    NumKeyBoard *m_numpadWindow;

    // 繪圖元件
    QWidget *m_plotWidget;           // 繪圖區域
    QVector<UICurveData> m_curveData; // 曲線資料
    double m_xMin, m_xMax;            // X軸範圍
    double m_yMin, m_yMax;            // Y軸範圍
    QColor m_curveColor;              // 曲線顏色
    int m_curveWidth;                  // 曲線寬度

    QString m_xLabel;
    QString m_yLabel;

    // 狀態標記
    bool m_isMosfetMode;
    bool m_isOutputCurve;

    // 當前工作點
    double m_currentVgs;
    double m_currentVds;
    double m_currentId;
    double m_currentIb;
    double m_currentVce;
    double m_currentIc;

    // 常數定義
    static constexpr double PLOT_MARGIN_LEFT = 60;
    static constexpr double PLOT_MARGIN_RIGHT = 20;
    static constexpr double PLOT_MARGIN_TOP = 20;
    static constexpr double PLOT_MARGIN_BOTTOM = 50;

protected:
    void paintEvent(QPaintEvent *event) override;

};

#endif // MOSANDBJT_H











