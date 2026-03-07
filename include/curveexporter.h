#ifndef CURVEEXPORTER_H
#define CURVEEXPORTER_H

#include <QString>
#include <QVector>
#include <QPointF>
#include <QFile>
#include <QTextStream>

class CurveExporter
{
public:
    // 靜態方法，直接叫用
    static bool toCSV(const QVector<QPointF>& curve,
                      const QString& filename,
                      const QString& xLabel = "Vds",
                      const QString& yLabel = "Id");

    static bool toTXT(const QVector<QPointF>& curve,
                      const QString& filename);

    static bool toCSVWithMetadata(const QVector<QPointF>& curve,
                                  const QString& filename,
                                  const QString& metadata);
};

#endif
