#ifndef CURVE_UTILS_H
#define CURVE_UTILS_H

#include "../include/mosfet.h"
#include <vector>
#include <string>

// 核心用的 Point 結構（已經在 transistor.h 裡）
// struct Point { double x; double y; };

// 核心用的 CurveData（如果需要的話）
struct CurveData {
    std::vector<Point> points;
    //curve_utils.h:12:17: Use of undeclared identifier 'Point'

    std::string label;
    CurveData(const std::vector<Point>& p, const std::string& lbl)
        : points(p), label(lbl) {}
};

//------------------------------------------------------------------------------
// UI 層專用：用 Qt 型態的輔助函式
// 只有在定義了 UI_Curve_Drawing 時才會編譯
//------------------------------------------------------------------------------
#ifdef UI_Curve_Drawing

#include <QVector>
#include <QPointF>
#include <QString>

// UI 層專用結構：包裝一條曲線和它的標籤
struct UICurveData {
    QVector<QPointF> points;
    QString label;

    UICurveData(const QVector<QPointF>& p, const QString& lbl)
        : points(p), label(lbl) {}
};

// 轉換函式
QVector<QPointF> toQtPoints(const std::vector<Point>& points);

// 產生一組 Vgs 曲線（回傳 Qt 版本）
QVector<UICurveData> generateOutputCurves(
    const MOSFET& mos,
    const QVector<double>& Vgs_list,
    const QString& label_prefix = "Vgs="
    );

// 從範圍產生 Vgs 列表
QVector<double> generateVgsList(double start, double end, int count);

// 合併版
QVector<UICurveData> generateOutputCurves(
    const MOSFET& mos,
    double Vgs_start,
    double Vgs_end,
    int count,
    const QString& label_prefix = "Vgs="
    );

#endif // UI_Curve_Drawing

#endif // CURVE_UTILS_H
