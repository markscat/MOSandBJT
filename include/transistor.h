#ifndef TRANSISTOR_H
#define TRANSISTOR_H


#include <string>
#include <vector>

struct Point {
    double x;
    double y;

    Point(double x = 0, double y = 0) : x(x), y(y) {}
};

struct BiasPoint {
    double Vce;     // 8 bytes
    double Ic;      // 8 bytes
    double Ib;      // 8 bytes
    double Vds;     // 8 bytes
    double Id;      // 8 bytes
    double Vgs;     // 8 bytes
    bool valid;     // 1 byte (+ padding)

    BiasPoint() : Vce(0), Ic(0), Ib(0), Vds(0), Id(0), Vgs(0), valid(false) {}

};

class Transistor
{
public:
    virtual ~Transistor() = default;

    // 型號與類型
    virtual std::string type() const = 0;  // "BJT" 或 "MOSFET"

    // 參數存取（用字串是因為每家參數名稱可能不同）
    virtual void setParameter(const std::string& name, double value) = 0;
    virtual double getParameter(const std::string& name) const = 0;

    virtual std::vector<std::string> paramList() const = 0;

    // 特性曲線
    virtual std::vector<Point> outputCurve(double inputParam) const = 0;
    virtual std::vector<Point> transferCurve() const = 0;

    // 工作點
    virtual BiasPoint calculateQPoint(double Vcc, double Rc, double Rb) const = 0;
};


#endif // TRANSISTOR_H
