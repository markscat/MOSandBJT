/** File name:bjt.cpp
 */


#include "../include/bjt.h"
#include <cmath>
//#include <algorithm>

// 建構子：設定預設值
BJT::BJT()
    : m_Beta(100.0)        // 典型值 100
    , m_Vbe_on(0.65)       // 典型值 0.65V
    , m_Vce_sat(0.2)       // 典型值 0.2V
    , m_Va(50.0)           // Early voltage 50V
    , m_Is(1e-14)          // 飽和電流 1e-14A
    , m_Ic_max(0.1)        // 最大電流 100mA
    , m_Vce_max(40.0)      // 最大電壓 40V
    , m_isNPN(true)
    , m_curvePoints(1000)
{
}

BJT::~BJT()
{
}

// 型號與類型
std::string BJT::type() const
{
    return "BJT";
}

std::string BJT::subtype() const
{
    return m_isNPN ? "NPN" : "PNP";
}

// 參數設定與讀取
void BJT::setParameter(const std::string& name, double value)
{
    if (name == "Beta" || name == "hFE") m_Beta = value;
    else if (name == "Vbe_on") m_Vbe_on = value;
    else if (name == "Vce_sat") m_Vce_sat = value;
    else if (name == "Va" || name == "Early") m_Va = value;
    else if (name == "Is") m_Is = value;
    else if (name == "Ic_max") m_Ic_max = value;
    else if (name == "Vce_max") m_Vce_max = value;
    // 忽略不認識的參數
}

double BJT::getParameter(const std::string& name) const
{
    if (name == "Beta" || name == "hFE") return m_Beta;
    if (name == "Vbe_on") return m_Vbe_on;
    if (name == "Vce_sat") return m_Vce_sat;
    if (name == "Va" || name == "Early") return m_Va;
    if (name == "Is") return m_Is;
    if (name == "Ic_max") return m_Ic_max;
    if (name == "Vce_max") return m_Vce_max;
    return 0.0;
}

std::vector<std::string> BJT::paramList() const
{
    return {"Beta", "Vbe_on", "Vce_sat", "Va", "Is", "Ic_max", "Vce_max"};
}

// 參數驗證
bool BJT::validateParameters(std::string& errorMsg) const
{
    if (m_Beta <= 0) {
        errorMsg = "Beta 必須大於 0";
        return false;
    }
    if (m_Vbe_on <= 0) {
        errorMsg = "Vbe_on 必須大於 0";
        return false;
    }
    if (m_Vce_sat < 0) {
        errorMsg = "Vce_sat 不能為負";
        return false;
    }
    if (m_Va < 0) {
        errorMsg = "Va 不能為負";
        return false;
    }
    return true;
}

// 內部計算函式
double BJT::calculateVce_saturation(double Ib) const
{
    // 飽和區起始電壓，簡單用 Vce_sat 就好
    return m_Vce_sat;
}

double BJT::calculateIc_active(double Ib, double Vce) const
{
    // 主動區：Ic = Beta * Ib * (1 + Vce / Va)
    double Ic = m_Beta * Ib * (1.0 + Vce / m_Va);

    // 限制電流不超過 Ic_max
    if (Ic > m_Ic_max) Ic = m_Ic_max;

    return Ic;
}

double BJT::calculateIc_saturation(double Ib, double Vce) const
{
    // 飽和區：簡單線性近似，從 0 到 Beta*Ib
    // 當 Vce = Vce_sat 時，Ic = Beta * Ib
    // 當 Vce = 0 時，Ic = 0
    double Ic_max = m_Beta * Ib;
    double Ic = Ic_max * (Vce / m_Vce_sat);

    // 限制電流不超過 Ic_max
    if (Ic > Ic_max) Ic = Ic_max;
    if (Ic > m_Ic_max) Ic = m_Ic_max;

    return Ic;
}

// 特性曲線：輸出特性 (Ic vs Vce)
std::vector<Point> BJT::outputCurve(double Ib) const
{
    std::vector<Point> points;
    points.reserve(m_curvePoints + 1);

    // 如果 Ib <= 0，截止區
    if (Ib <= 0) {
        points.push_back(Point(0, 0));
        points.push_back(Point(m_Vce_max, 0));
        return points;
    }

    double Vce_sat = calculateVce_saturation(Ib);

    for (int i = 0; i <= m_curvePoints; i++) {
        double Vce = (i / (double)m_curvePoints) * m_Vce_max;
        double Ic = 0;

        if (Vce < Vce_sat) {
            // 飽和區
            Ic = calculateIc_saturation(Ib, Vce);
        } else {
            // 主動區
            Ic = calculateIc_active(Ib, Vce);
        }

        points.push_back(Point(Vce, Ic));
    }

    return points;
}

// 特性曲線：轉移特性 (Ic vs Ib)，固定 Vce
std::vector<Point> BJT::transferCurve(double Vce) const
{
    std::vector<Point> points;
    points.reserve(m_curvePoints + 1);

    // 從 0 到某個最大 Ib（例如 1mA）
    double max_Ib = m_Ic_max / m_Beta * 2.0;  // 讓 Ic 能到 Ic_max 的兩倍左右

    for (int i = 0; i <= m_curvePoints; i++) {
        double Ib = (i / (double)m_curvePoints) * max_Ib;
        double Ic = 0;

        if (Vce > m_Vce_sat) {
            // 主動區
            Ic = m_Beta * Ib * (1.0 + Vce / m_Va);
        } else {
            // 飽和區
            Ic = m_Beta * Ib * (Vce / m_Vce_sat);
        }

        if (Ic > m_Ic_max) Ic = m_Ic_max;
        points.push_back(Point(Ib, Ic));
    }

    return points;
}

// 無參數版本：用預設 Vce
std::vector<Point> BJT::transferCurve() const
{
    return transferCurve(5.0);  // 預設 Vce = 5V
}

// 輸入特性 (Ib vs Vbe)
std::vector<Point> BJT::inputCurve() const
{
    std::vector<Point> points;
    points.reserve(m_curvePoints + 1);

    // 從 0 到 1V
    for (int i = 0; i <= m_curvePoints; i++) {
        double Vbe = (i / (double)m_curvePoints) * 1.0;
        double Ib = 0;

        if (Vbe > m_Vbe_on) {
            // 二極體公式近似
            Ib = m_Is * (exp((Vbe - m_Vbe_on) / 0.026) - 1.0);
        }

        points.push_back(Point(Vbe, Ib));
    }

    return points;
}

// 工作點計算
/*
BiasPoint BJT::calculateQPoint(double Vcc, double Rc, double Rb) const
{
    BiasPoint bp;
    bp.valid = false;

    // 簡化計算：假設工作在主動區
    // Ib = (Vcc - Vbe) / Rb
    // Ic = Beta * Ib
    // Vce = Vcc - Ic * Rc

    double Ib = (Vcc - m_Vbe_on) / Rb;
    if (Ib < 0) return bp;

    double Ic = m_Beta * Ib;
    double Vce = Vcc - Ic * Rc;

    // 檢查是否在主動區
    if (Vce > m_Vce_sat) {
        bp.Ib = Ib;
        bp.Ic = Ic;
        bp.Vce = Vce;
        bp.valid = true;
    } else {
        // 飽和區
        bp.Ib = Ib;
        bp.Ic = (Vcc - m_Vce_sat) / Rc;
        bp.Vce = m_Vce_sat;
        bp.valid = true;
    }

    return bp;
}
*/
// 找點功能
double BJT::findIbFromIc(double Ic, double Vce) const
{
    if (Ic <= 0) return 0;

    if (Vce > m_Vce_sat) {
        // 主動區
        return Ic / (m_Beta * (1.0 + Vce / m_Va));
    } else {
        // 飽和區
        return Ic / (m_Beta * (Vce / m_Vce_sat));
    }
}

double BJT::findIcFromIb(double Ib, double Vce) const
{
    if (Ib <= 0) return 0;

    if (Vce > m_Vce_sat) {
        // 主動區
        return m_Beta * Ib * (1.0 + Vce / m_Va);
    } else {
        // 飽和區
        return m_Beta * Ib * (Vce / m_Vce_sat);
    }
}

double BJT::findVceFromIc(double Ic, double Ib) const
{
    if (Ib <= 0 || Ic <= 0) return 0;

    // 從主動區公式反推
    // Ic = Beta * Ib * (1 + Vce / Va)
    // => Vce = (Ic / (Beta * Ib) - 1) * Va

    double ratio = Ic / (m_Beta * Ib);
    if (ratio < 1.0) {
        // 可能在飽和區
        return m_Vce_sat * (Ic / (m_Beta * Ib));
    } else {
        // 主動區
        return (ratio - 1.0) * m_Va;
    }
}


std::vector<Point> BJT::generateCurve(double inputParam) const
{
    // 直接呼叫 BJT 自己的 outputCurve
    return outputCurve(inputParam);
}

QString BJT::deviceType() const
{
    return "BJT";
}

QString BJT::inputUnit() const
{
    return "A";  // Ib 的單位是安培
}
