/**
 *  File name:curve_utils.cpp
*/

#include "../ui/curve_utils.h"

//"curve_utils.h"
#include "../include/mosfet.h"
#include "../include/bjt.h"

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

QVector<UICurveData> generateOutputCurves(
    const MOSFET& mos,
    const QVector<double>& Vgs_list,
    const QString& label_prefix)
{
    QVector<UICurveData> curves;
    curves.reserve(Vgs_list.size());

    for (double Vgs : Vgs_list) {
        auto stlPoints = mos.outputCurve(Vgs);
        QVector<QPointF> qtPoints = toQtPoints(stlPoints);
        QString label = label_prefix + QString::number(Vgs) + "V";
        curves.append(UICurveData(qtPoints, label));
    }

    return curves;
}

QVector<double> generateVgsList(double start, double end, int count)
{
    QVector<double> list;
    list.reserve(count);

    double step = (end - start) / (count - 1);
    for (int i = 0; i < count; ++i) {
        list.append(start + i * step);
    }
    return list;
}

QVector<UICurveData> generateOutputCurves(
    const MOSFET& mos,
    double Vgs_start,
    double Vgs_end,
    int count,
    const QString& label_prefix)
{
    QVector<double> Vgs_list = generateVgsList(Vgs_start, Vgs_end, count);
    return generateOutputCurves(mos, Vgs_list, label_prefix);
}

#endif // UI_Curve_Drawing
