#include "../include/mosfet.h"
#include "../include/transistor.h"

#include <cmath>      // 用 std::sqrt 取代 qSqrt
#include <string>
#include <vector>

#ifdef Qt_version
#include <QtMath>  // 如果需要 qPow 之類的
#endif

// 建構子：設定預設值
MOSFET::MOSFET()
    : m_Vth(2.0)        // 典型值 2V
    , m_Kn(0.1)         // 典型值 0.1 A/V²
    , m_lambda(0.01)    // 典型值 0.01
    , m_Rds_on(0.1)     // 典型值 0.1Ω
    , m_gfs(1.0)        // 典型值 1S
    , m_Id_max(10.0)    // 典型值 10A
    , m_Vds_max(100.0)  // 典型值 100V
    , m_isNChannel(true)
{
}

MOSFET::~MOSFET()
{
}

// 型號與類型
std::string MOSFET::type() const
{
    return "MOSFET";
}

std::string MOSFET::subtype() const
{
    return m_isNChannel ? "N-channel" : "P-channel";
}

// 參數設定與讀取
void MOSFET::setParameter(const std::string& name, double value)
{
    if (name == "Vth") m_Vth = value;
    else if (name == "Kn") m_Kn = value;
    else if (name == "lambda") m_lambda = value;
    else if (name == "Rds_on") m_Rds_on = value;
    else if (name == "gfs") m_gfs = value;
    else if (name == "Id_max") m_Id_max = value;
    else if (name == "Vds_max") m_Vds_max = value;
    // 忽略不認識的參數
}

double MOSFET::getParameter(const std::string& name) const
{
    if (name == "Vth") return m_Vth;
    if (name == "Kn") return m_Kn;
    if (name == "lambda") return m_lambda;
    if (name == "Rds_on") return m_Rds_on;
    if (name == "gfs") return m_gfs;
    if (name == "Id_max") return m_Id_max;
    if (name == "Vds_max") return m_Vds_max;
    return 0.0;
}

std::vector<std::string> MOSFET::paramList() const
{
    return {"Vth", "Kn", "lambda", "Rds_on", "gfs", "Id_max", "Vds_max"};
}

// 參數驗證
bool MOSFET::validateParameters(std::string& errorMsg) const
{
    if (m_Vth <= 0) {
        errorMsg = "Vth 必須大於 0";
        return false;
    }
    if (m_Kn <= 0) {
        errorMsg = "Kn 必須大於 0";
        return false;
    }
    if (m_lambda < 0) {
        errorMsg = "lambda 不能為負";
        return false;
    }
    return true;
}

// 特性曲線計算
std::vector<Point> MOSFET::outputCurve(double Vgs) const
{
    std::vector<Point> points;
    points.reserve(101);  // 預留空間

    // 如果 Vgs < Vth，電晶體截止，Id = 0
    if (Vgs <= m_Vth) {
        points.push_back(Point(0, 0));
        points.push_back(Point(m_Vds_max, 0));
        return points;
    }

    double Vds_sat = Vgs - m_Vth;  // 飽和區起始電壓

    // 從 0 到 Vds_max 取 100 個點
    for (int i = 0; i <= 100; i++) {
        double Vds = (i / 100.0) * m_Vds_max;
        double Id = 0;

        if (Vds < Vds_sat) {
            // 三極管區
            Id = m_Kn * (2 * (Vgs - m_Vth) * Vds - Vds * Vds);
        } else {
            // 飽和區
            Id = m_Kn * (Vgs - m_Vth) * (Vgs - m_Vth) * (1 + m_lambda * Vds);
        }

        // 限制電流不超過 Id_max
        if (Id > m_Id_max) Id = m_Id_max;

        points.push_back(Point(Vds, Id));

    }

    return points;
}

// 特性曲線計算 - 轉移曲線 (Id vs Vgs)，無參數版本（使用預設 Vds）
std::vector<Point> MOSFET::transferCurve() const
{
    // 轉移曲線：Id vs Vgs，固定 Vds = 5V
    return transferCurve(5.0);
}

std::vector<Point> MOSFET::transferCurve(double Vds) const
{
    std::vector<Point> points;
    points.reserve(101);


    // 從 0 到 5V（假設 Vgs 範圍）
    for (int i = 0; i <= 100; i++) {
        double Vgs = (i / 100.0) * 5.0;
        double Id = 0;

        if (Vgs > m_Vth) {
            if (Vds >= Vgs - m_Vth) {
                // 飽和區
                Id = m_Kn * (Vgs - m_Vth) * (Vgs - m_Vth) * (1 + m_lambda * Vds);
            } else {
                // 三極管區
                Id = m_Kn * (2 * (Vgs - m_Vth) * Vds - Vds * Vds);
            }
        }

        if (Id > m_Id_max) Id = m_Id_max;
        points.push_back(Point(Vgs, Id));
    }

    return points;
}

// 工作點計算
BiasPoint MOSFET::calculateQPoint(double Vdd, double Rd, double Rg) const
{
    BiasPoint bp;
    bp.valid = false;

    // 簡化計算：假設工作在飽和區
    // Id = Kn * (Vgs - Vth)^2 * (1 + lambda * Vds)
    // Vds = Vdd - Id * Rd
    // Vgs = ? (取決於偏壓電路)

    // 這裡先簡單回傳一個值
    bp.Vds = Vdd / 2;
    bp.Id = (Vdd - bp.Vds) / Rd;
    bp.Vgs = m_Vth + std::sqrt(bp.Id / m_Kn);
    /*mosfet.cpp:175:22: Use of undeclared identifier 'qSqrt' (fixes available)*/
    bp.valid = true;

    return bp;
}

// 找點功能
double MOSFET::findVgsFromId(double Id, double Vds) const
{
    // 從 Id = Kn * (Vgs - Vth)^2 反推
    if (Id <= 0) return 0;
    return m_Vth + std::sqrt(Id / m_Kn);
}

double MOSFET::findIdFromVgs(double Vgs, double Vds) const
{
    if (Vgs <= m_Vth) return 0;

    if (Vds >= Vgs - m_Vth) {
        // 飽和區
        return m_Kn * (Vgs - m_Vth) * (Vgs - m_Vth) * (1 + m_lambda * Vds);
    } else {
        // 三極管區
        return m_Kn * (2 * (Vgs - m_Vth) * Vds - Vds * Vds);
    }
}

double MOSFET::findVdsFromId(double Id, double Vgs) const
{
    // 從飽和區公式反推
    if (Id <= 0 || Vgs <= m_Vth) return 0;

    double Id_sat = m_Kn * (Vgs - m_Vth) * (Vgs - m_Vth);
    if (Id >= Id_sat) {
        // 在飽和區
        return (Id / Id_sat - 1) / m_lambda;
    } else {
        // 在三極管區，解二次方程式
        // Id = Kn * (2*(Vgs-Vth)*Vds - Vds^2)
        // => Vds^2 - 2*(Vgs-Vth)*Vds + Id/Kn = 0
        double a = 1;
        double b = -2 * (Vgs - m_Vth);
        double c = Id / m_Kn;

        double discriminant = b * b - 4 * a * c;
        if (discriminant < 0) return 0;

        // 取較小的根（三極管區）
        return (-b - std::sqrt(discriminant)) / (2 * a);
    }
}
/*
// 在 mosfet.cpp 加入
QVector<QPointF> MOSFET::transferCurve() const
{
    // 轉移曲線：Id vs Vgs，固定 Vds
    QVector<QPointF> points;

    // 假設 Vds = 5V（或從參數來）
    double Vds = 5.0;

    // 從 0 到 5V 取 100 個點
    for (int i = 0; i <= 100; i++) {
        double Vgs = (i / 100.0) * 5.0;
        double Id = 0;

        if (Vgs > m_Vth) {
            if (Vds >= Vgs - m_Vth) {
                // 飽和區
                Id = m_Kn * (Vgs - m_Vth) * (Vgs - m_Vth) * (1 + m_lambda * Vds);
            } else {
                // 三極管區
                Id = m_Kn * (2 * (Vgs - m_Vth) * Vds - Vds * Vds);
            }
        }

        if (Id > m_Id_max) Id = m_Id_max;
        points.append(QPointF(Vgs, Id));
    }

    return points;
}
*/
