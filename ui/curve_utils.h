#ifndef CURVE_UTILS_H
#define CURVE_UTILS_H

#include "../include/mosfet.h"
#include "../include/bjt.h"
#include <vector>
#include <string>
#include <QVector>
#include <QPointF>
#include <QString>

//------------------------------------------------------------------------------
// 核心用的結構（不依賴 Qt）
//------------------------------------------------------------------------------

// 核心用的 CurveData
struct CurveData {
    std::vector<Point> points;
    std::string label;

    CurveData(const std::vector<Point>& p, const std::string& lbl)
        : points(p), label(lbl) {}
};

//------------------------------------------------------------------------------
// UI 層專用：用 Qt 型態的輔助函式
//------------------------------------------------------------------------------


// UI 層專用結構：包裝一條曲線和它的標籤
struct UICurveData {
    QVector<QPointF> points;
    QString label;

    UICurveData(const QVector<QPointF>& p, const QString& lbl)
        : points(p), label(lbl) {}
};

// 轉換函式
QVector<QPointF> toQtPoints(const std::vector<Point>& points);


//------------------------------------------------------------------------------
// Template 函式：產生一組輸出特性曲線（適用於 MOSFET 和 BJT）
//------------------------------------------------------------------------------


/**
 * 產生一組輸出特性曲線
 * @param device     MOSFET 或 BJT 物件
 * @param param_list 輸入參數列表（MOSFET 為 Vgs，BJT 為 Ib）
 * @param label_prefix 標籤前綴（預設 "Input="）
 * @return 一組 UICurveData，每條曲線包含點和標籤
 */

template<typename DeviceType>
QVector<UICurveData> generateOutputCurves(
    const DeviceType& device,
    const QVector<double>& param_list,
    const QString& label_prefix = "Input=")
{
    // 檢查 DeviceType 是否繼承 CurveDrawable（編譯期檢查）
    static_assert(std::is_base_of_v<CurveDrawable, DeviceType>,
                  "DeviceType must inherit from CurveDrawable");


    QVector<UICurveData> curves;
    curves.reserve(param_list.size());

    for (double param : param_list) {


        // 呼叫 device 的 outputCurve()，回傳 std::vector<Point>
        auto stlPoints = device.outputCurve(param);

        // 轉成 Qt 用的 QVector<QPointF>
        QVector<QPointF> qtPoints = toQtPoints(stlPoints);


        // 直接用 device.inputUnit() 取得單位
        QString label = label_prefix + QString::number(param) + device.inputUnit();
        curves.append(UICurveData(qtPoints, label));
        /*
        // 根據 device 型別決定單位
        QString unit;
        if constexpr (std::is_same_v<DeviceType, MOSFET>) {
            unit = "V";
        } else if constexpr (std::is_same_v<DeviceType, BJT>) {
            unit = "A";
        } else {
            unit = "";
        }
        label = label_prefix + QString::number(param) + unit;
        */
        curves.append(UICurveData(qtPoints, label));
    }

    return curves;
}

//------------------------------------------------------------------------------
// 輔助函式：產生參數列表
//------------------------------------------------------------------------------

/**
 * 從 start 到 end 產生 count 個等間距的數值
 */
QVector<double> generateParamList(double start, double end, int count);

/**
 * 合併版：直接給範圍和數量，產生曲線
 */
template<typename DeviceType>
QVector<UICurveData> generateOutputCurves(
    const DeviceType& device,
    double param_start,
    double param_end,
    int count,
    const QString& label_prefix = "Input=")
{
    QVector<double> param_list = generateParamList(param_start, param_end, count);
    return generateOutputCurves(device, param_list, label_prefix);
}


#endif // CURVE_UTILS_H
