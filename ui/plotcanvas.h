#ifndef PLOTCANVAS_H
#define PLOTCANVAS_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <vector>
#include "../include/transistor.h" // 為了使用 Point 結構
#include "curve_utils.h" // 確保有這個來識別 UICurveData


class PlotCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit PlotCanvas(QWidget *parent = nullptr);

    // 讓主視窗傳入數據的方法
    void setData(const std::vector<Point>& points, double xMax, double yMax);
    //多載
    void setData(const QVector<UICurveData>& curves, double xMax, double yMax);

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

    void drawCurves(QPainter &painter);
    void drawHintBox(QPainter *painter);
    void drawGridAndAxes(QPainter &painter);


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


};

#endif // PLOTCANVAS_H