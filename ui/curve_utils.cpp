/**
 *  File name:curve_utils.cpp
*/

#include "../ui/curve_utils.h"
#include "../include/mosfet.h"
#include "../include/bjt.h"

#define UI_Curve_Drawing
//------------------------------------------------------------------------------
// UI 層專用：用 Qt 型態的輔助函式實作
//------------------------------------------------------------------------------
#ifdef UI_Curve_Drawing

#include <QVector>
#include <QPointF>
#include <QString>

QVector<QPointF> toQtPoints(const std::vector<Point>& points) {
    QVector<QPointF> qtPoints;
    qtPoints.reserve(points.size());
    for (const auto& p : points) {
        qtPoints.append(QPointF(p.x, p.y));
    }
    return qtPoints;
}

//------------------------------------------------------------------------------
// 產生參數列表
//------------------------------------------------------------------------------

QVector<double> generateParamList(double start, double end, int count)
{
    QVector<double> list;

    if (count <= 1) {
        if (count == 1) list.append(start);
        return list;
    }

    list.reserve(count);
    double step = (end - start) / (count - 1);

    for (int i = 0; i < count; ++i) {
        list.append(start + i * step);
    }

    return list;
}


//------------------------------------------------------------------------------
// Template 函式實作注意事項
//------------------------------------------------------------------------------
//
// 注意：generateOutputCurves 的 template 版本已經在 .h 檔中完整實作，
//       因為 template 函式通常需要將實作放在標頭檔中，編譯器才能根據
//       不同型別產生對應的程式碼。
//
//       如果你堅持要把 template 實作放在 .cpp 中，需要使用 explicit instantiation，
//       但這會讓程式碼變得複雜且不易維護。
//
//       因此，generateOutputCurves 的實作保留在 curve_utils.h 中。
//
//------------------------------------------------------------------------------



#endif // UI_Curve_Drawing
