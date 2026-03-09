#ifndef CURVEEXPORTER_H
#define CURVEEXPORTER_H

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



};

#endif
