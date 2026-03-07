#ifndef TRANSISTOR_H
#define TRANSISTOR_H


#include <QString>
#include <QVector>
#include <QPointF>

struct BiasPoint {
    double Vce;     // 8 bytes
    double Ic;      // 8 bytes
    double Ib;      // 8 bytes
    double Vds;     // 8 bytes
    double Id;      // 8 bytes
    double Vgs;     // 8 bytes
    bool valid;     // 1 byte (+ padding)
};

class Transistor
{
public:
    virtual ~Transistor() = default;

    // 型號與類型
    virtual QString type() const = 0;  // "BJT" 或 "MOSFET"

    // 參數存取（用字串是因為每家參數名稱可能不同）
    virtual void setParameter(const QString& name, double value) = 0;
    virtual double getParameter(const QString& name) const = 0;
    virtual QStringList paramList() const = 0;  // 這顆電晶體有哪些參數

    // 特性曲線
    virtual QVector<QPointF> outputCurve(double inputParam) const = 0;
    virtual QVector<QPointF> transferCurve() const = 0;

    // 工作點

    virtual BiasPoint calculateQPoint(double Vcc, double Rc, double Rb) const = 0;
};


#endif // TRANSISTOR_H
