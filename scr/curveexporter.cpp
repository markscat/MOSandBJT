#include "../include/curveexporter.h"
#include <QFile>
#include <QTextStream>

bool CurveExporter::toCSV(const QVector<QPointF>& curve,
                          const QString& filename,
                          const QString& xLabel,
                          const QString& yLabel)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QTextStream stream(&file);
    stream << xLabel << "," << yLabel << "\n";

    for (const QPointF& p : curve) {
        stream << p.x() << "," << p.y() << "\n";
    }

    return true;
}

bool CurveExporter::toTXT(const QVector<QPointF>& curve,
                          const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QTextStream stream(&file);
    for (const QPointF& p : curve) {
        stream << p.x() << "\t" << p.y() << "\n";
    }

    return true;
}

bool CurveExporter::toCSVWithMetadata(const QVector<QPointF>& curve,
                                      const QString& filename,
                                      const QString& metadata)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QTextStream stream(&file);
    stream << "# " << metadata << "\n";
    stream << "Vds,Id\n";

    for (const QPointF& p : curve) {
        stream << p.x() << "," << p.y() << "\n";
    }

    return true;
}
