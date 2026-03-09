#ifndef CURVEEXPORTER_H
#define CURVEEXPORTER_H

#ifdef Qt_version
#include <QString>
#include <QVector>
#include <QPointF>
#include <QFile>
#include <QTextStream>
#endif

#include <string>
#include <vector>
#include <fstream>  // 標準 C++ 檔案操作
#include "transistor.h"  // 需要 Point 的定義

class CurveExporter
{
public:

public:
    static bool toCSV(const std::vector<Point>& curve,
                      const std::string& filename,
                      const std::string& xLabel = "Vds",
                      const std::string& yLabel = "Id");

    static bool toTXT(const std::vector<Point>& curve,
                      const std::string& filename);

    // 輸出為 CSV 並加上中繼資料（註解）
    static bool toCSVWithMetadata(const std::vector<Point>& curve,
                                  const std::string& filename,
                                  const std::string& metadata,
                                  const std::string& xLabel = "Vds",
                                  const std::string& yLabel = "Id");

#ifdef Qt_version
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

#endif

};

#endif
