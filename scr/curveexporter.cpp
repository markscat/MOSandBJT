#include "../include/curveexporter.h"
//#include <QFile>
//#include <QTextStream>
#include <fstream>
#include <string>
bool CurveExporter::toCSV(const std::vector<Point>& curve,
                          const std::string& filename,
                          const std::string& xLabel,
                          const std::string& yLabel)
{
    std::ofstream file(filename);
    if (!file.is_open())
        return false;

    file << xLabel << "," << yLabel << "\n";

    for (const auto& p : curve) {
        file << p.x << "," << p.y << "\n";
    }

    file.close();
    return true;
}

bool CurveExporter::toTXT(const std::vector<Point>& curve,
                          const std::string& filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
        return false;

    for (const auto& p : curve) {
        file << p.x << "\t" << p.y << "\n";
    }

    file.close();
    return true;
}

bool CurveExporter::toCSVWithMetadata(const std::vector<Point>& curve,
                                      const std::string& filename,
                                      const std::string& metadata,
                                      const std::string& xLabel,
                                      const std::string& yLabel)
{
    std::ofstream file(filename);
    if (!file.is_open())
        return false;

    file << "# " << metadata << "\n";
    file << xLabel << "," << yLabel << "\n";

    for (const auto& p : curve) {
        file << p.x << "," << p.y << "\n";
    }

    file.close();
    return true;
}
