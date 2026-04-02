#ifndef PLOTCANVAS_H
#define PLOTCANVAS_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <vector>
#include "../include/transistor.h" // 為了使用 Point 結構
#include "curve_utils.h" // 確保有這個來識別 UICurveData

struct PlotAxisSettings {
    QString xLabel;      // X 軸名稱 (例如: "Vds")
    QString yLabel;      // Y 軸名稱 (例如: "Id")
    QString xUnit;       // X 軸單位 (例如: "V")
    QString yUnit;       // Y 軸單位 (例如: "A")
    int xTicks = 10;     // X 軸刻度數量
    int yTicks = 5;      // Y 軸刻度數量
    int xPrecision = 1;  // X 軸小數點位數
    int yPrecision = 2;  // Y 軸小數點位數
};

class PlotCanvas : public QWidget
{
    Q_OBJECT
public:

    // 設定負載線點 (Vds, Id) 以及 X 軸截距 (Vdd)
    void setLoadLine(double vq, double iq, double vdd) {
        m_showLoadLine = true;
        m_vQ = vq; m_iQ = iq; m_vDD = vdd;
        update();
    }


    explicit PlotCanvas(QWidget *parent = nullptr);

    // 讓主視窗傳入數據的方法
    void setData(const std::vector<Point>& points, double xMax, double yMax);
    //多載
    void setData(const QVector<UICurveData>& curves, double xMax, double yMax,const PlotAxisSettings& settings);

    void setCrosshairEnabled(bool enabled) { m_enableCrosshair = enabled; update(); }


    // 清除畫布
    void clear();

protected:
    // 繪圖事件
    void paintEvent(QPaintEvent *event) override;
    // 滑鼠移動事件
    void mouseMoveEvent(QMouseEvent *event) override;
    // 離開事件 (滑鼠離開時隱藏提示框)
    void leaveEvent(QEvent *event) override;

private:

    QVector<UICurveData> m_curves; // 儲存多條曲線

    QString m_xLabel = "Vds (V)";
    QString m_yLabel = "Id (A)";

     // --- 引擎的模組化繪圖方法 ---
    void drawGridAndAxes(QPainter &painter); // 繪製背景網格與座標軸
    void drawCurves(QPainter &painter);// 繪製數據曲線
    void drawHintBox(QPainter &painter);// 繪製左下角資訊框
    void drawCrosshair(QPainter &painter);   // 繪製 X-Y 輔助虛線 (核心需求)


    // 核心數據
    std::vector<Point> m_curvePoints;

    // 座標範圍 (以物理單位計：V 和 A)
    double m_xMin;
    double m_xMax;
    double m_yMin;
    double m_yMax;

    // 邊距設定 (像素)
    int m_marginLeft;
    int m_marginBottom;
    int m_marginRight;
    int m_marginTop;

    // 滑鼠目前的像素座標與狀態
    QPoint m_mousePos;
    bool m_showHint;

    // 內部輔助函式：將「物理數值」轉為「像素座標」
    int valToPxX(double val);
    int valToPxY(double val);

    // 內部輔助函式：將「像素座標」反推為「物理數值」
    double pxToValX(int px);
    double pxToValY(int py);

    //輔助線
    bool m_enableCrosshair = true; // 預設開啟

    PlotAxisSettings m_settings;

    bool m_showLoadLine = false;
    double m_vQ = 0, m_iQ = 0, m_vDD = 0;


};

#endif // PLOTCANVAS_H