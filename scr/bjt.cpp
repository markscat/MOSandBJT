/** File name:bjt.cpp */

#include "../include/bjt.h"
#include <cmath>
#include <algorithm>

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
    // 處理極性，轉成正的來計算
    double effectiveIb=std::abs(Ib);
    double effectiveVce=std::abs(Vce);

    // 如果輸入不合理，回傳 0
    if (effectiveIb <= 0.0 || effectiveVce < 0.0) {
        return 0.0;
    }

    // 主動區公式（用正值計算）
    double Ic = m_Beta * effectiveIb * (1.0 + effectiveVce / m_Va);

    // 限制電流大小
    if (Ic > m_Ic_max) {
        Ic = m_Ic_max;
    }

    // 根據極性回傳
    if (m_isNPN) {
        return Ic;
    } else {
        return -Ic;
    }
}


double BJT::calculateIbFromVbe(double Vbe) const
{
    // 處理極性
    double effectiveVbe = Vbe;
    if (!m_isNPN) {
        effectiveVbe = -Vbe;  // PNP 的 Vbe 是負的，轉成正的來算
    }

    // 如果 Vbe 小於導通電壓，Ib 非常小（近似為 0）
    if (effectiveVbe < m_Vbe_on) {
        return 0.0;
    }

    // 熱電壓 VT = kT/q，預設 25°C 時約 0.0257V
    const double VT = 0.0257;

    // 二極體公式：Ib = Is * (e^(Vbe/VT) - 1)
    // 這裡用 effectiveVbe - Vbe_on 是因為 Vbe_on 是導通電壓
    double exponent = (effectiveVbe - m_Vbe_on) / VT;

    // 防止指數爆炸（double 能處理約 e^700，但實際電路不會那麼大）
    if (exponent > 100.0) {
        exponent = 100.0;  // 限制最大值
    }

    double Ib_mag = m_Is * (exp(exponent) - 1.0);

    // 根據極性調整方向
    if (m_isNPN) {
        return Ib_mag;      // NPN：Ib 為正
    } else {
        return -Ib_mag;     // PNP：Ib 為負
    }
}

double BJT::calculateVbeFromIb(double Ib) const
{
    // 處理極性
    double effectiveIb = Ib;
    if (!m_isNPN) {
        effectiveIb = -Ib;  // PNP 的 Ib 是負的，轉成正的來算
    }

    // 如果 Ib 太小，Vbe 約等於 0
    if (effectiveIb <= 0.0) {
        return 0.0;
    }

    // 熱電壓 VT = kT/q，預設 25°C 時約 0.0257V
    const double VT = 0.0257;

    // 從二極體公式反推：Vbe = Vbe_on + VT * ln(Ib/Is + 1)
    double ratio = effectiveIb / m_Is + 1.0;

    // 防止對數參數過大
    if (ratio > 1e100) {
        ratio = 1e100;  // 限制最大值
    }

    double Vbe_mag = m_Vbe_on + VT * log(ratio);

    // 根據極性調整方向
    if (m_isNPN) {
        return Vbe_mag;     // NPN：Vbe 為正
    } else {
        return -Vbe_mag;    // PNP：Vbe 為負
    }
}


double BJT::calculateIc_saturation(double Ib, double Vce) const
{
    // 處理極性
    double effectiveIb;
    double effectiveVce;

    if (m_isNPN) {
        effectiveIb = Ib;
        effectiveVce = Vce;
    } else {
        effectiveIb = -Ib;
        effectiveVce = -Vce;
    }

    // 輸入檢查
    if (effectiveIb <= 0.0 || effectiveVce < 0.0) {
        return 0.0;
    }

    double Ic;

    if (effectiveVce >= m_Vce_sat) {
        // 如果 Vce 大於等於 Vce_sat，其實是在主動區邊界
        // 但為了連續性，用主動區公式
        Ic = m_Beta * effectiveIb * (1.0 + effectiveVce / m_Va);
    } else {
        // 飽和區：考慮兩個效應
        // 1. Vce 很小時，Ic 與 Vce 成正比
        // 2. 但不會超過 β×Ib

        double Ic_active_at_sat = m_Beta * effectiveIb * (1.0 + m_Vce_sat / m_Va);

        //double Ic_linear = Ic_active_at_sat * (effectiveVce / m_Vce_sat);

        double Ic_linear;
        if(m_Vce_sat > 0){
            Ic_linear=Ic_active_at_sat * (effectiveVce / m_Vce_sat);
        }else{
            throw std::runtime_error("BJT 參數錯誤：Vce_sat 必須大於 0");
        }

        // 取最小值（因為飽和區電流不會超過主動區邊界值）
        Ic = (Ic_linear < Ic_active_at_sat) ? Ic_linear : Ic_active_at_sat;
    }

    // 限制電流
    if (Ic > m_Ic_max) {
        Ic = m_Ic_max;
    }

    // 極性回傳
    if (m_isNPN) {
        return Ic;
    } else {
        return -Ic;
    }
}



// 特性曲線：輸出特性 (Ic vs Vce)

std::vector<Point> BJT::outputCurve(double Ib) const
{
    std::vector<Point> points;
    points.reserve(m_curvePoints + 1);

    // 處理 PNP 的極性
    double effectiveIb = std::abs(Ib);
    double effectiveVce_max = std::abs(m_Vce_max);



    double Vce_sat = calculateVce_saturation(effectiveIb);

    for (int i = 0; i <= m_curvePoints; i++) {
        double progress = i / (double)m_curvePoints;
        double Vce = progress * effectiveVce_max;

        // 使用絕對值來判斷工作區域
        double absVce = std::abs(Vce);
        double Ic = 0;

        if (absVce < Vce_sat) {
            // 飽和區（用絕對值計算）
            Ic = calculateIc_saturation(effectiveIb, absVce);
        } else {
            // 主動區（用絕對值計算）
            Ic = calculateIc_active(effectiveIb, absVce);
        }

        // 根據極性調整 Ic 的符號
        if (!m_isNPN) {
            Ic = -Ic;    // PNP 電流為負
        }

        points.push_back(Point(Vce, Ic));
    }

    return points;
}


// 無參數版本：根據通道類型選擇合理的預設 Vce
std::vector<Point> BJT::transferCurve() const
{
    if (m_isNPN) {
        return transferCurve(5.0);      // NPN：Vce = +5V
    } else {
        return transferCurve(-5.0);     // PNP：Vce = -5V
    }
}

// 給定 Vce 的版本
std::vector<Point> BJT::transferCurve(double Vce) const
{
    std::vector<Point> points;
    points.reserve(m_curvePoints + 1);

    // 處理 PNP 的極性
    double effectiveVce = Vce;
    const double effectiveVce_sat = m_Vce_sat;
    //double sign = 1.0;

    if (!m_isNPN) {
        effectiveVce = -Vce;            // PNP：轉成正的來計算
        // Vce_sat 本來就是正的，不用轉
        //sign = -1.0;                    // 最後 Ic 要變負
    }

    // 計算最大 Ib（讓 Ic 能到 Ic_max 的兩倍左右）
    double max_Ib = m_Ic_max / m_Beta * 2.0;

    for (int i = 0; i <= m_curvePoints; i++) {
        double Ib = (i / (double)m_curvePoints) * max_Ib;

        // 根據極性調整 Ib 的符號
        double effectiveIb = Ib;
        if (!m_isNPN) {
            effectiveIb = -Ib;  // PNP 的 Ib 為負
        }

        double Ic = 0;

        if (effectiveVce > effectiveVce_sat) {
            // 主動區（用 effectiveVce 計算，已經是正的）
            Ic = m_Beta * effectiveIb * (1.0 + effectiveVce / m_Va);

        } else {
            // 飽和區
            Ic = m_Beta * effectiveIb * (effectiveVce / effectiveVce_sat);
        }

        // 限制電流大小
        if (Ic > m_Ic_max) Ic = m_Ic_max;

        // PNP 的 Ic 為負
        if (!m_isNPN) {
            Ic = -Ic;
        }

        points.push_back(Point(effectiveIb, Ic));
    }

    return points;
}

std::vector<Point> BJT::transferCurve(double Vce, double Ib_start, double Ib_end) const
{
    std::vector<Point> points;
    points.reserve(m_curvePoints + 1);

    // 處理 PNP 的極性
    double effectiveVce = Vce;
    double effectiveStart = Ib_start;
    double effectiveEnd = Ib_end;

    if (!m_isNPN) {
        effectiveVce = -Vce;
        effectiveStart = -Ib_start;
        effectiveEnd = -Ib_end;
    }

    // 確保掃描方向正確
    if (effectiveStart > effectiveEnd) {
        std::swap(effectiveStart, effectiveEnd);
    }

    for (int i = 0; i <= m_curvePoints; i++) {
        double effectiveIb = effectiveStart + (i / (double)m_curvePoints) * (effectiveEnd - effectiveStart);
        double Ic = 0;

        if (effectiveVce > m_Vce_sat) {
            Ic = m_Beta * std::abs(effectiveIb) * (1.0 + effectiveVce / m_Va);
        } else {
            Ic = m_Beta * std::abs(effectiveIb) * (effectiveVce / m_Vce_sat);
        }

        if (Ic > m_Ic_max) Ic = m_Ic_max;

        // PNP 的 Ib 和 Ic 都為負
        if (!m_isNPN) {
            effectiveIb = -effectiveIb;
            Ic = -Ic;
        }

        points.push_back(Point(effectiveIb, Ic));
    }


    return points;
}

// 輸入特性 (Ib vs Vbe)
std::vector<Point> BJT::inputCurve() const
{
    std::vector<Point> points;
    points.reserve(m_curvePoints + 1);


    // 熱電壓 VT = kT/q，
    // 想用哪個溫度，就把對應的 #define 解除註解
    // #define TEMP_125C
    // #define TEMP_0C
    // #define TEMP_M40C
    //......

#ifdef TEMP_125C
    //125度
    const double VT = 0.0344;
#elif TEMP_80C
    //80度
    const double VT = 0.0305;
#elif TEMP_40C
    //40度
    const double VT = 0.0270;
#elif defined(TEMP_0C)
    //0度
    const double VT = 0.0235;
#elif defined(TEMP_M20C)
    //-20度
    const double VT = 0.0218;
#elif defined(TEMP_M40C)
    //-40度
    const double VT = 0.0201;
#else
    //25度
    const double VT = 0.0257;  // 預設 25℃
#endif

    // 根據極性決定掃描範圍
    double Vbe_start, Vbe_end;

    if (m_isNPN) {
        Vbe_start = 0.0;
        Vbe_end = 1.0;
    } else {
        Vbe_start = -1.0;
        Vbe_end = 0.0;
    }

    for (int i = 0; i <= m_curvePoints; i++) {
        double progress = i / (double)m_curvePoints;
        double Vbe = Vbe_start + progress * (Vbe_end - Vbe_start);

        // 使用封裝好的函式計算 Ib
        // 注意：calculateIbFromVbe 內部會處理極性，所以直接傳入 Vbe
        double Ib = calculateIbFromVbe(Vbe);

        points.push_back(Point(Vbe, Ib));
    }
    return points;
}

// 工作點計算

/**
 * @brief 計算 BJT 的工作點（Q Point）
 * @param params 偏壓參數結構，包含：
 *               - config: 偏壓組態（FixedIb, VoltageDivider, FourResistor...）
 *               - Vcc: 電源電壓
 *               - Rc: 集極電阻
 *               - R1, R2: 分壓電阻（用於 VoltageDivider、FourResistor 組態）
 *               - Ib: 基極電流（用於 FixedIb 組態）
 * @return BiasPoint 工作點結構，包含 Vce、Ic、Ib 及有效性標誌
 */
BiasPoint BJT::calculateQPoint(const BiasParams& params) const
{
    BiasPoint bp;
    bp.valid = false;

    // 處理 PNP 的極性
    double effectiveVcc = params.Vcc;
    //double sign = 1.0;

    if (!m_isNPN) {
        // PNP: Vcc 通常是負的，轉成正的來計算
        effectiveVcc = -params.Vcc;
        //sign = -1.0;
    }

    // 檢查基本條件
    if (effectiveVcc <= 0.0) {
        return bp;
    }

    switch(params.config) {
    case BiasConfig::FixedIb:
        // 固定基極電流
        return calculateQPoint_FixedIb(effectiveVcc, params.Rc, params.Ib);

    case BiasConfig::VoltageDivider:
        // 電阻分壓偏壓（無 Re）
        return calculateQPoint_VoltageDivider(effectiveVcc, params.Rc, 0.0, params.R1, params.R2);

    case BiasConfig::FourResistor:
        // 四電阻偏壓（有 Re）
        if (params.Re > 0.0) {
            return calculateQPoint_FourResistor(effectiveVcc, params.Rc, params.Re, params.R1, params.R2);
        }
        // 如果 Re=0，退化為 VoltageDivider
        return calculateQPoint_VoltageDivider(effectiveVcc, params.Rc, 0.0, params.R1, params.R2);

    default:
        return bp;
    }
}


/**
 * @brief 固定基極電流偏壓
 * @param Vcc 電源電壓（已轉為正值）
 * @param Rc 集極電阻
 * @param Ib 基極電流
 */
BiasPoint BJT::calculateQPoint_FixedIb(double Vcc, double Rc, double Ib) const
{
    BiasPoint bp;
    bp.valid = false;

    // 檢查電阻值
    if (Rc <= 0.0 || Ib <= 0.0) {
        return bp;

    }

    // 假設工作在主動區
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

    // 處理 PNP 極性
    if (!m_isNPN) {
        bp.Vce = -bp.Vce;
        bp.Ic = -bp.Ic;
        bp.Ib = -bp.Ib;
    }

    return bp;
}



/**
 * @brief 電阻分壓偏壓（無射極電阻）
 * @param Vcc 電源電壓（已轉為正值）
 * @param Rc 集極電阻
 * @param Re 射極電阻（此處為 0）
 * @param R1, R2 分壓電阻
 */
BiasPoint BJT::calculateQPoint_VoltageDivider(double Vcc, double Rc, double Re, double R1, double R2) const
{
    BiasPoint bp;
    bp.valid = false;

    // 檢查電阻值
    if (Rc <= 0.0 || R1 <= 0.0 || R2 <= 0.0) {
        return bp;
    }

    // 計算戴維寧等效電路
    double Rth = (R1 * R2) / (R1 + R2);
    double Vth = Vcc * R2 / (R1 + R2);

    // 假設 Vbe = 0.7V（固定值近似）
    double Vbe = m_Vbe_on;

    // 從基極迴路求 Ib
    // Vth = Ib * Rth + Vbe + Ie * Re
    // Ie ≈ Ic = β * Ib
    double Ib = (Vth - Vbe) / (Rth + (m_Beta + 1.0) * Re);

    if (Ib <= 0.0) {
        return bp;
    }

    double Ic = m_Beta * Ib;
    double Vce = Vcc - Ic * Rc - (m_Beta + 1.0) * Ib * Re;

    // 檢查是否在主動區
    if (Vce > m_Vce_sat) {
        bp.Ib = Ib;
        bp.Ic = Ic;
        bp.Vce = Vce;
        bp.valid = true;
    } else {
        // 飽和區（需要更複雜的計算，先回傳無效）
        return bp;
    }

    // 處理 PNP 極性
    if (!m_isNPN) {
        bp.Vce = -bp.Vce;
        bp.Ic = -bp.Ic;
        bp.Ib = -bp.Ib;
    }

    return bp;
}

/**
 * @brief 四電阻偏壓（有射極電阻）
 * @param Vcc 電源電壓（已轉為正值）
 * @param Rc 集極電阻
 * @param Re 射極電阻
 * @param R1, R2 分壓電阻
 */
BiasPoint BJT::calculateQPoint_FourResistor(double Vcc, double Rc, double Re, double R1, double R2) const
{
    // 四電阻偏壓其實就是有 Re 的電阻分壓
    // 直接呼叫 VoltageDivider 版本，傳入 Re
    return calculateQPoint_VoltageDivider(Vcc, Rc, Re, R1, R2);
}

// 找點功能



double BJT::findIcFromIb(double Ib, double Vce) const
{
    // 如果 Ib 為 0，截止區
    if (Ib == 0.0) {
        return 0.0;
    }

    // 根據極性判斷是否導通
    if (m_isNPN && Ib <= 0) return 0.0;    // NPN：Ib 要 > 0
    if (!m_isNPN && Ib >= 0) return 0.0;   // PNP：Ib 要 < 0

    // 處理 PNP 的極性（轉成正的來計算）
    double effectiveIb;
    double effectiveVce;

    if (m_isNPN) {
        effectiveIb = Ib;
        effectiveVce = Vce;
    } else {
        effectiveIb = -Ib;    // PNP：負的 Ib 轉成正的
        effectiveVce = -Vce;  // PNP：負的 Vce 轉成正的
    }

    double Ic = 0.0;

    if (effectiveVce > m_Vce_sat) {
        // 主動區
        /*
         * Ic = β × Ib × (1 + Vce / Va)
         * 各項意義：
         * β (Beta)：直流電流增益（hFE）
         * Ib：基極電流
         * Vce：集極-射極電壓
         * Va：Early voltage（爾利電壓）
          */
        Ic = m_Beta * effectiveIb * (1.0 + effectiveVce / m_Va);
    } else {
        // 飽和區
        Ic = m_Beta * effectiveIb * (effectiveVce / m_Vce_sat);
    }

    // 限制電流不超過 Ic_max
    if (Ic > m_Ic_max) {
        Ic = m_Ic_max;
    }

    // 根據極性調整 Ic 的符號
    if (m_isNPN) {
        return Ic;
    } else {
        return -Ic;  // PNP 電流為負
    }
}


double BJT::findIbFromIc(double Ic, double Vce) const
{
    // 如果 Ic 為 0，截止區
    if (Ic == 0.0) {
        return 0.0;
    }

    // 根據極性判斷是否合理
    if (m_isNPN) {
        if (Ic <= 0) return 0.0;    // NPN：Ic 要 > 0
    } else {
        if (Ic >= 0) return 0.0;    // PNP：Ic 要 < 0
    }

    // 處理極性，轉成正的來計算
    double effectiveIc;
    double effectiveVce;

    if (m_isNPN) {
        effectiveIc = Ic;
        effectiveVce = Vce;
    } else {
        effectiveIc = -Ic;
        effectiveVce = -Vce;
    }

    double Ib = 0.0;

    // 先計算主動區所需的 Ib
    double Ib_active = effectiveIc / (m_Beta * (1.0 + effectiveVce / m_Va));

    // 判斷是否在飽和區
    if (effectiveVce <= m_Vce_sat) {
        // 飽和區處理

        // 方法一：根據 Vce 線性內插
        // Vce = 0 時，Ib 趨近無限大（實際上由外部電路限制）
        // Vce = Vce_sat 時，Ib = Ib_active
        if (effectiveVce <= 0.0) {
            // Vce 太小時，回傳一個很大的值（表示需要很大的 Ib）
            Ib = m_Ic_max / m_Beta * 10.0;  // 10 倍正常 Ib
        } else {
            // 線性內插：Vce 越小，需要的 Ib 越大
            double ratio = m_Vce_sat / effectiveVce;
            Ib = Ib_active * ratio;
        }

        // 方法二：使用飽和區公式（如果有的話）
        // Ib = Ic / (Beta_sat) ，其中 Beta_sat 通常比主動區小很多
        // 這裡用主動區 Beta 的 1/10 作為飽和區 Beta 的估計
        double Ib_sat_min = effectiveIc / (m_Beta * 0.1);  // Beta 剩 1/10
        if (Ib < Ib_sat_min) {
            Ib = Ib_sat_min;
        }

    } else {
        // 主動區
        Ib = Ib_active;
    }

    // 限制 Ib 不超過合理範圍
    double maxIb = m_Ic_max / m_Beta * 10.0;  // 允許 10 倍正常 Ib
    if (Ib > maxIb) {
        Ib = maxIb;
    }

    // 根據極性調整 Ib 的符號
    if (m_isNPN) {
        return Ib;
    } else {
        return -Ib;
    }
}

/*
 * Ic / (β × Ib) = 1 + Vce / Va
 * Vce / Va = (Ic / (β × Ib)) - 1
 * Vce = ((Ic / (β × Ib)) - 1) × Va
 */

double BJT::findVceFromIc(double Ic, double Ib) const
{
    if (Ib == 0.0) {
        return 0.0;
    }

    double effectiveIc;
    double effectiveIb;

    if (m_isNPN) {
        if (Ic <= 0.0 || Ib <= 0.0) return 0.0;
        effectiveIc = Ic;
        effectiveIb = Ib;
    } else {
        if (Ic >= 0.0 || Ib >= 0.0) return 0.0;
        effectiveIc = -Ic;
        effectiveIb = -Ib;
    }

    double ratio = effectiveIc / (m_Beta * effectiveIb);

    if (ratio < 1.0) {
        double Vce = m_Vce_sat * ratio;
        if (m_isNPN) {
            return Vce;
        } else {
            return -Vce;
        }

    } else {
        double Vce = (ratio - 1.0) * m_Va;
        if (Vce > m_Vce_max) {
            Vce = m_Vce_max;
        }

        if (m_isNPN) {
            return Vce;
        } else {
            return -Vce;
        }
    }
}


// bjt.cpp 裡實作插值
double BJT::getDynamicBeta(double currentIc) const {
    if (m_betaPoints.empty()) return m_Beta;

    // 取絕對值計算，不論 NPN/PNP
    double absIc = std::abs(currentIc);

    // 如果只有一點或電流太小/太大，直接回傳邊界值
    if (absIc <= m_betaPoints.front().ic) return m_betaPoints.front().hfe;
    if (absIc >= m_betaPoints.back().ic) return m_betaPoints.back().hfe;

    // 對數線性插值 (Log-Log Interpolation)
    for (size_t i = 0; i < m_betaPoints.size() - 1; ++i) {
        if (absIc >= m_betaPoints[i].ic && absIc <= m_betaPoints[i+1].ic) {
            double x0 = std::log10(m_betaPoints[i].ic);
            double x1 = std::log10(m_betaPoints[i+1].ic);
            double y0 = std::log10(m_betaPoints[i].hfe);
            double y1 = std::log10(m_betaPoints[i+1].hfe);

            double y = y0 + (y1 - y0) * (std::log10(absIc) - x0) / (x1 - x0);
            return std::pow(10, y);
        }
    }
    return m_Beta;
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
