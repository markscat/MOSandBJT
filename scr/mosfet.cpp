#include "../include/mosfet.h"
#include "../include/transistor.h"
#include "../ui/cauverdrawable.h"

#include <cmath>      // 用 std::sqrt 取代 qSqrt
#include <string>
#include <vector>

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
    , m_Cgs(0)      // 1pF 典型值
    , m_Cgd(0)      // 1pF
    , m_Cds(0)      // 1pF

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
    points.reserve(m_curvePoints + 1);

    // 處理 P-channel 的極性反轉
    double effectiveVgs = Vgs;
    double effectiveVth = m_Vth;
    double effectiveVds_max = m_Vds_max;

    if (!m_isNChannel) {
        // P-channel：電壓極性相反
        effectiveVgs = -Vgs;
        effectiveVth = m_Vth;  // Vth 在 P-channel 通常是負值，但我們用正值表示
        // 對於 P-channel，Vds 是負的，所以掃描範圍從 -m_Vds_max 到 0
        effectiveVds_max = -m_Vds_max;
    }

    // 如果 Vgs < Vth，電晶體截止，Id = 0
    if (effectiveVgs <= effectiveVth) {
        if (m_isNChannel) {
            points.push_back(Point(0, 0));
            points.push_back(Point(effectiveVds_max, 0));
            return points;
        }
    }

    double Vgs_eff = effectiveVgs - effectiveVth;  // 過驅動電壓 (Vgs - Vth)
    double Vds_sat = Vgs_eff;  // 飽和區起始電壓 = Vgs - Vth

    for (int i = 0; i <= m_curvePoints; i++) {

        double progress = i / (double)m_curvePoints;

        double Vds = progress * effectiveVds_max;  // ← 使用 effectiveVds_max

        double Id = 0;

        // 使用 effectiveVds 來計算 Id（正值）
        double absVds = std::abs(Vds);

        if (absVds < Vds_sat) {
            // 三極管區 (線性區)
            // 公式：Id = Kn * [2(Vgs - Vth)Vds - Vds²]
            Id = m_Kn * (2.0 * Vgs_eff * absVds - absVds * absVds);

            // 在非常小的 Vds 時，可以用 Rds_on 來近似（如果有的話）
            if (absVds < 1e-6 && m_Rds_on > 0) {
                Id = absVds / m_Rds_on;
            }
        } else {
            // 飽和區
            // 公式：Id = Kn * (Vgs - Vth)² * (1 + λ * Vds)
            Id = m_Kn * Vgs_eff * Vgs_eff * (1.0 + m_lambda * absVds);
        }

        // 限制電流不超過 Id_max
        if (Id > m_Id_max) Id = m_Id_max;

        // 對於 P-channel，電流方向相反
        if (!m_isNChannel) {
            Id = -Id;
        }

        points.push_back(Point(Vds, Id));
    }
    return points;
}

// 無參數版本 - 根據通道類型選擇合理的預設 Vds
std::vector<Point> MOSFET::transferCurve() const
{
    if (m_isNChannel) {
        return transferCurve(5.0);      // N-channel：Vds = +5V
    } else {
        return transferCurve(-5.0);     // P-channel：Vds = -5V
    }
}

std::vector<Point> MOSFET::transferCurve(double Vds) const
{
    std::vector<Point> points;
    points.reserve(m_curvePoints +1);

    // 根據通道類型決定 Vgs 掃描範圍
    double Vgs_start, Vgs_end;

    if (m_isNChannel) {
        // N-channel: Vgs 從 0 掃到某個最大值（例如 5V 或 Vth + 3V）
        Vgs_start = 0.0;
        Vgs_end = m_Vth + 5.0;  // 或直接用 5.0
    } else {
        // P-channel: Vgs 從某個負值掃到 0
        Vgs_start = -5.0;        // 或 m_Vth - 5.0（因為 m_Vth 是負的）
        Vgs_end = 0.0;
    }

    for (int i = 0; i <= m_curvePoints; i++) {
        double Vgs = Vgs_start + (i / (double)m_curvePoints) * (Vgs_end - Vgs_start);
        double Id = findIdFromVgs(Vgs, Vds);
        points.push_back(Point(Vgs, Id));
    }

    return points;
}

// 新增一個更完整的版本，讓使用者可以指定 Vgs 範圍
std::vector<Point> MOSFET::transferCurve(double Vds, double Vgs_start, double Vgs_end) const
{
    std::vector<Point> points;
    points.reserve(m_curvePoints + 1);

    // 確保掃描方向正確（start < end）
    double start = Vgs_start;
    double end = Vgs_end;
    if (start > end) {
        std::swap(start, end);
    }

    for (int i = 0; i <= m_curvePoints; i++) {
        double Vgs = start + (i / (double)m_curvePoints) * (end - start);
        // findIdFromVgs 內部會處理極性，直接傳入原始值即可
        double Id = findIdFromVgs(Vgs, Vds);
        points.push_back(Point(Vgs, Id));
    }

    return points;
}


// 工作點計算

// 找點功能
//#define findVgsFromId_short_Version
//#define findVdsFromId_short_Version


#ifdef findVdsFromId_short_Version
double MOSFET::findVdsFromId(double Id, double Vgs) const
{
    // TODO: 需要完整測試 N-channel 和 P-channel 的各種情況

    // 處理 P-channel 的極性
    double effectiveId = Id;
    double effectiveVgs = Vgs;
    double effectiveVth = m_Vth;

    if (!m_isNChannel) {
        effectiveId = -Id;           // P-channel 電流為負，轉成正的
        effectiveVgs = -Vgs;         // P-channel Vgs 為負，轉成正的
        effectiveVth = -m_Vth;       // P-channel Vth 為負，轉成正的
    }

    // 檢查是否截止
    if (effectiveId <= 0.0 || effectiveVgs <= effectiveVth) {
        return 0.0;
    }

    // 限制電流不超過 Id_max
    if (effectiveId > m_Id_max) {
        effectiveId = m_Id_max;
    }

    // 計算飽和區電流
    double Id_sat = m_Kn * (effectiveVgs - effectiveVth) * (effectiveVgs - effectiveVth);
    double Vds = 0.0;

    if (effectiveId >= Id_sat) {
        // 在飽和區：Id = Id_sat * (1 + λ * Vds)
        // => Vds = (Id / Id_sat - 1) / λ
        if (m_lambda > 0.0) {
            Vds = (effectiveId / Id_sat - 1.0) / m_lambda;
        } else {
            // λ = 0 時，飽和區電流不隨 Vds 變化，回傳一個合理的值
            Vds = effectiveVgs - effectiveVth;  // 至少大於飽和區起始電壓
        }
    } else {
        // 在三極管區：解二次方程式
        // Id = Kn * [2*(Vgs-Vth)*Vds - Vds^2]
        // => Vds^2 - 2*(Vgs-Vth)*Vds + Id/Kn = 0

        double a = 1.0;
        double b = -2.0 * (effectiveVgs - effectiveVth);
        double c = effectiveId / m_Kn;

        double discriminant = b * b - 4.0 * a * c;
        if (discriminant < 0.0) {
            return 0.0;
        }

        // 取較小的根（三極管區的 Vds 較小）
        Vds = (-b - std::sqrt(discriminant)) / (2.0 * a);

        // 確保 Vds 不超過飽和區起始電壓
        double Vds_sat = effectiveVgs - effectiveVth;
        if (Vds > Vds_sat) {
            Vds = Vds_sat;
        }
        if (Vds < 0.0) {
            Vds = 0.0;
        }
    }

    // 限制不超過 Vds_max
    if (Vds > m_Vds_max) {
        Vds = m_Vds_max;
    }

    // 根據通道類型返回正確極性
    if (m_isNChannel) {
        return Vds;
    } else {
        return -Vds;
    }
}
#else

double MOSFET::findVdsFromId(double Id, double Vgs) const
{
    //判斷是哪一類型的MOS，以及後續處理
    // 處理 P-channel 的極性
    double effectiveId = Id;
    double effectiveVgs = Vgs;
    double effectiveVth = m_Vth;
    double effectiveVds_max = m_Vds_max;

    if (!m_isNChannel) {
        effectiveId = -Id;           // P-channel 電流為負，轉成正的
        effectiveVgs = -Vgs;         // P-channel Vgs 為負，轉成正的
        effectiveVth = -m_Vth;       // P-channel Vth 為負，轉成正的
        effectiveVds_max = -m_Vds_max; // P-channel Vds_max 為負，轉成正的
    }

    //判斷目前狀況
    // 檢查輸入是否合理
    if (effectiveId < 0.0) {
        // 經過極性轉換後，Id 不該是負的
        return 0.0;
    }

    // 檢查是否截止
    if (effectiveId == 0.0 || effectiveVgs <= effectiveVth) {
        return 0.0;
    }

    // 限制電流不超過 Id_max
    double limitedId = effectiveId;
    if (limitedId > m_Id_max) {
        limitedId = m_Id_max;
    }

    // 計算過驅動電壓和飽和區起始電壓
    double Vgs_eff = effectiveVgs - effectiveVth;
    double Vds_sat = Vgs_eff;

    // 計算飽和區電流（不考慮 λ）
    double Id_sat_base = m_Kn * Vgs_eff * Vgs_eff;

    // 先假設在飽和區，考慮 λ 的影響
    // Id = Id_sat_base * (1 + λ * Vds)
    // 但我們不知道 Vds，所以先假設 Vds = Vds_sat

    double Vds = 0.0;
    double Id_sat_at_Vds_sat = Id_sat_base * (1.0 + m_lambda * Vds_sat);

    if (limitedId >= Id_sat_at_Vds_sat) {
        // 在飽和區，且 Vds 大於 Vds_sat
        // Id = Id_sat_base * (1 + λ * Vds)
        // => Vds = (Id / Id_sat_base - 1) / λ

        if (m_lambda > 0.0) {
            Vds = (limitedId / Id_sat_base - 1.0) / m_lambda;

            // 確保 Vds 至少是 Vds_sat
            if (Vds < Vds_sat) {
                Vds = Vds_sat;
            }
        } else {
            // λ = 0 時，飽和區電流不隨 Vds 變化
            // 無法從 Id 反推 Vds，回傳 Vds_sat 作為最小可能值
            Vds = Vds_sat;
        }
    } else {
        // 可能在三極管區或飽和區邊界
        // 先試試看是否在飽和區（Vds = Vds_sat 時，Id 應該等於 Id_sat_at_Vds_sat）
        if (std::abs(limitedId - Id_sat_at_Vds_sat) < 1e-12) {
            // 剛好在飽和區邊界
            Vds = Vds_sat;
        } else {
            // 在三極管區，解二次方程式
            // Id = Kn * [2*(Vgs-Vth)*Vds - Vds^2]
            // => Vds^2 - 2*(Vgs-Vth)*Vds + Id/Kn = 0

            double a = 1.0;
            double b = -2.0 * Vgs_eff;
            double c = limitedId / m_Kn;

            double discriminant = b * b - 4.0 * a * c;

            // 檢查判別式
            if (discriminant < -1e-12) {
                // 判別式為負，無實數解
                return 0.0;
            }
            if (discriminant < 0.0) {
                discriminant = 0.0;
            }

            double sqrt_disc = std::sqrt(discriminant);

            // 兩個根：(-b ± sqrt_disc) / (2a)
            double root1 = (-b + sqrt_disc) / (2.0 * a);
            double root2 = (-b - sqrt_disc) / (2.0 * a);

            // 在三極管區，Vds 應該小於 Vds_sat，且為正
            // 通常 root2 較小，root1 較大
            if (root2 >= 0.0 && root2 <= Vds_sat) {
                Vds = root2;
            } else if (root1 >= 0.0 && root1 <= Vds_sat) {
                Vds = root1;
            } else {
                // 兩個根都不在合理範圍內
                Vds = Vds_sat;
            }
        }
    }

    // 套用 Vds_max 限制
    if (Vds > effectiveVds_max) {
        Vds = effectiveVds_max;
    }
    if (Vds < 0.0) {
        Vds = 0.0;
    }

    // 檢查計算結果是否合理：用算出的 Vds 反推 Id 應該接近輸入的 Id
    double calculatedId = 0.0;
    if (Vds < Vds_sat) {
        // 三極管區
        calculatedId = m_Kn * (2.0 * Vgs_eff * Vds - Vds * Vds);
    } else {
        // 飽和區
        calculatedId = Id_sat_base * (1.0 + m_lambda * Vds);
    }

    // 如果誤差太大，可能是數值問題，回傳 0
    if (std::abs(calculatedId - limitedId) > 0.1 * limitedId) {
        return 0.0;
    }

    // 根據通道類型返回正確極性
    if (m_isNChannel) {
        return Vds;
    } else {
        return -Vds;
    }
}
#endif

#ifdef findVgsFromId_short_Version
double MOSFET::findVgsFromId(double Id, double Vds) const
{
    // 處理 P-channel 的極性
    double effectiveId = Id;
    double effectiveVth = m_Vth;

    if (!m_isNChannel) {
        effectiveId = -Id;      // P-channel 電流為負，轉成正的來算
        effectiveVth = -m_Vth;  // P-channel Vth 為負，轉成正的
    }

    // 檢查電流是否合理
    if (effectiveId <= 0.0) {
        return 0.0;
    }

    // 限制不超過 Id_max
    if (effectiveId > m_Id_max) {
        effectiveId = m_Id_max;
    }

    // 從飽和區公式反推 Vgs
    // Id = Kn * (Vgs - Vth)^2
    double Vgs = effectiveVth + std::sqrt(effectiveId / m_Kn);

    // 根據通道類型返回正確極性
    if (m_isNChannel) {
        return Vgs;
    } else {
        return -Vgs;
    }
}
#else
double MOSFET::findVgsFromId(double Id, double Vds) const
{
    // 處理極性
    double effectiveId = Id;
    double effectiveVds = Vds;
    double effectiveVth = m_Vth;

    if (!m_isNChannel) {
        effectiveId = -Id;
        effectiveVds = -Vds;
        effectiveVth = -m_Vth;
    }

    // 檢查電流是否合理
    if (effectiveId <= 0.0) {
        return 0.0;
    }

    // 檢查是否超過 Id_max
    if (effectiveId > m_Id_max) {
        effectiveId = m_Id_max;
    }

    // 先假設在飽和區，計算 Vgs
    double Vgs = effectiveVth + std::sqrt(effectiveId / m_Kn);

    // 檢查這個 Vgs 下是否真的在飽和區
    double Vds_sat = Vgs - effectiveVth;

    // 確保不超過合理範圍

    if (effectiveVds >= Vds_sat) {
        // 確實在飽和區，直接使用
        double result = Vgs;

        double Vgs_max = effectiveVth + std::sqrt(m_Id_max / m_Kn);

        // 確保不超過合理範圍
        if (result  > Vgs_max) {
            result  = Vgs_max;
        }
        if (result  < effectiveVth) {
            result  = effectiveVth;
        }

        if (m_isNChannel) {
            return result;
        } else {
            return -result;
        }

    }else {
        // 在三極管區，需要解二次方程式
        // Id = Kn * (2*(Vgs-Vth)*Vds - Vds^2)
        // => 2*Kn*Vds*(Vgs-Vth) = Id + Kn*Vds^2
        // => Vgs = (Id + Kn*Vds^2) / (2*Kn*Vds) + Vth

        // 避免除以零
        if (effectiveVds <= 0.0) {
            return 0.0;
        }

        double Vgs_triode = (effectiveId + m_Kn * effectiveVds * effectiveVds)
                                / (2.0 * m_Kn * effectiveVds) + effectiveVth;

        double result = Vgs_triode;


        // 確保不超過合理範圍
        double Vgs_max = effectiveVth + std::sqrt(m_Id_max / m_Kn);

        if (result  > Vgs_max) {
            result  = Vgs_max;
        }
        if (result  < effectiveVth) {
            result  = effectiveVth;
        }

        if (m_isNChannel) {
            return result;
        } else {
            return -result;
        }
    }

}
#endif

/**
 * @brief 計算 MOSFET 的工作點（Q Point）
 * @param params 偏壓參數結構，包含：
 *               - config: 偏壓組態（FixedVgs, VoltageDivider, CurrentSource...）
 *               - Vcc: 電源電壓
 *               - Rd: 汲極電阻
 *               - Rs: 源極電阻（可選）
 *               - R1, R2: 分壓電阻（用於 VoltageDivider 組態）
 *               - Ib: 電流源值（用於 CurrentSource 組態）
 *               - Vgs: 閘極電壓（用於 FixedVgs 組態）
 * @return BiasPoint 工作點結構，包含 Vds、Id、Vgs 及有效性標誌
 *
 * @note 使用 BiasParams 結構而非多個獨立參數的原因：
 *       - 不同偏壓組態需要的參數不同（FixedVgs 只需 Vgs，VoltageDivider 需 R1/R2 等）
 *       - 避免函數宣告過長且參數意義不明（如 extra1, extra2）
 *       - 防止參數傳遞順序錯誤
 *       - 便於未來擴充新參數（如溫度）而不需修改函數介面
 *       - 提高程式碼可讀性（params.R1 一看就知道是分壓電阻）
 */
BiasPoint MOSFET::calculateQPoint(const BiasParams& params) const
{
    BiasPoint bp;
    bp.valid = false;

    // 根據不同的 bias config 處理
    switch(params.config) {

    case BiasConfig::FixedVgs:
        return calculateQPoint_FixedVgs(params.Vcc, params.Rd, params.Vgs);

    case BiasConfig::VoltageDivider:
        // Vgs = Vcc * R2/(R1+R2)
        {
            double Vgs = params.Vcc * params.R2 / (params.R1 + params.R2);
            return calculateQPoint_FixedVgs(params.Vcc, params.Rc, Vgs);
        }

    case BiasConfig::CurrentSource:
        // 從電流反推 Vgs
        {
            double Vgs = m_Vth + std::sqrt(params.Ib / m_Kn);  // 假設 Ib 是電流
            if (!m_isNChannel) Vgs = -Vgs;
            return calculateQPoint_FixedVgs(params.Vcc, params.Rc, Vgs);
        }


    default:
        return bp;
    }
}

BiasPoint MOSFET::calculateQPoint_FixedVgs(double Vdd, double Rd, double Vgs) const
{
    BiasPoint bp;
    bp.valid = false;

    // 處理 P-channel 的極性
    double effectiveVdd = Vdd;
    double effectiveVgs = Vgs;
    double effectiveVth = m_Vth;
    double effectiveVds_max = m_Vds_max;
    double effectiveId_max = m_Id_max;

    if (!m_isNChannel) {
        effectiveVdd = -Vdd;
        effectiveVgs = -Vgs;
        effectiveVth = -m_Vth;
        effectiveVds_max = -m_Vds_max;
    }

    // 檢查基本條件
    if (effectiveVdd <= 0.0 || Rd <= 0.0) {
        return bp;
    }


    // 限制 Vgs 在合理範圍
    if (effectiveVgs < 0.0) effectiveVgs = 0.0;
    if (effectiveVgs > effectiveVdd) effectiveVgs = effectiveVdd;

    // 檢查是否截止
    if (effectiveVgs <= effectiveVth) {
        bp.Vds = effectiveVdd;
        bp.Id = 0.0;
        bp.Vgs = effectiveVgs;

        if (!m_isNChannel) {
            bp.Vds = -bp.Vds;
            bp.Vgs = -bp.Vgs;
        }
        bp.valid = true;
        return bp;
    }


    // 計算過驅動電壓
    double Vgs_eff = effectiveVgs - effectiveVth;

    // 用疊代法求工作點
    // 負載線：Id = (Vdd - Vds) / Rd
    // MOSFET 特性：Id = f(Vgs, Vds)

    double Vds = effectiveVdd / 2.0;  // 初始猜測值
    double Id = 0.0;
    int maxIter = 100;
    double tol = 1e-9;

    for (int iter = 0; iter < maxIter; iter++) {
        // 根據目前的 Vds 計算 Id
        if (Vds < Vgs_eff) {
            // 三極管區
            Id = m_Kn * (2.0 * Vgs_eff * Vds - Vds * Vds);
        } else {
            // 飽和區
            Id = m_Kn * Vgs_eff * Vgs_eff * (1.0 + m_lambda * Vds);
        }

        // 限制 Id 不超過 Id_max
        if (Id > effectiveId_max) {
            Id = effectiveId_max;
        }

        // 從負載線計算新的 Vds
        double newVds = effectiveVdd - Id * Rd;

        // 檢查收斂
        if (std::abs(newVds - Vds) < tol) {
            Vds = newVds;
            break;
        }

        // 更新 Vds（用加權平均避免震盪）
        Vds = 0.5 * Vds + 0.5 * newVds;

        // 確保 Vds 在合理範圍內
        if (Vds < 0.0) Vds = 0.0;
        if (Vds > effectiveVdd) Vds = effectiveVdd;
    }

    // 限制 Vds 不超過 Vds_max
    if (Vds > effectiveVds_max) {
        Vds = effectiveVds_max;
        //Id = (effectiveVdd - Vds) / Rd;
    }

    // 最後一次計算 Id（確保一致性）
    if (Vds < Vgs_eff) {
        Id = m_Kn * (2.0 * Vgs_eff * Vds - Vds * Vds);
    } else {
        Id = m_Kn * Vgs_eff * Vgs_eff * (1.0 + m_lambda * Vds);
    }

    // 填寫 BiasPoint
    bp.Vds = Vds;
    bp.Id = Id;
    bp.Vgs = effectiveVgs;

    // 檢查是否在合理範圍內
    if (Id >= 0.0 && Id <= effectiveId_max &&
        Vds >= 0.0 && Vds <= effectiveVdd &&
        effectiveVgs >= effectiveVth) {
        bp.valid = true;
    }

    // 處理 P-channel 的極性
    if (!m_isNChannel) {
        bp.Vds = -bp.Vds;
        bp.Id = -bp.Id;
        bp.Vgs = -bp.Vgs;
    }

    return bp;

}

double MOSFET::findIdFromVgs(double Vgs, double Vds) const
{
    // 處理 P-channel 的極性反轉
    double effectiveVgs = Vgs;
    double effectiveVds = Vds;
    double effectiveVth = m_Vth;

    if (!m_isNChannel) {
        // P-channel：輸入的 Vgs 和 Vds 都是負的
        // 我們轉成正的來做計算
        effectiveVgs = -Vgs;
        effectiveVds = -Vds;
        // Vth 在 P-channel 應該是負的，但我們用正值表示
        effectiveVth = -m_Vth;  // 假設 m_Vth 存的是負值，轉成正的
    }

    // 檢查是否截止
    if (effectiveVgs <= effectiveVth) {
        return 0.0;
    }

    double Vgs_eff = effectiveVgs - effectiveVth;  // 過驅動電壓
    double Vds_sat = Vgs_eff;  // 飽和區起始電壓
    double Id = 0.0;

    if (effectiveVds < Vds_sat) {
        // 三極管區
        // Id = Kn * [2(Vgs - Vth)Vds - Vds²]
        Id = m_Kn * (2.0 * Vgs_eff * effectiveVds - effectiveVds * effectiveVds);
                // 在非常小的 Vds 時，用 Rds_on 修正
        if (effectiveVds < 1e-6 && m_Rds_on > 0) {
            double Id_linear = effectiveVds / m_Rds_on;
            // 取較小的值（避免過大）
            if (Id_linear < Id) Id = Id_linear;
        }
    } else {
        // 飽和區
        // Id = Kn * (Vgs - Vth)² * (1 + λ * Vds)
        Id = m_Kn * Vgs_eff * Vgs_eff * (1.0 + m_lambda * effectiveVds);
    }
    // 限制電流不超過 Id_max
    if (Id > m_Id_max) Id = m_Id_max;
    if (Id < -m_Id_max) Id = -m_Id_max;

    // 根據通道類型調整電流方向
    if (m_isNChannel) {
        return Id;      // N-channel：電流為正
    } else {
        return -Id;     // P-channel：電流為負
    }
}


// 設定電容參數
void MOSFET::setCapacitances(double Cgs, double Cgd, double Cds)
{
    m_Cgs = Cgs;
    m_Cgd = Cgd;
    m_Cds = Cds;

    // 簡單起見，先不設電壓相依性
    /*
     * m_Cgs_Vmax = Cgs;
     * m_Cgd_Vmax = Cgd;
     * m_Cds_Vmax = Cds;
     */
}

// 取得電容值（考慮電壓相依性）
MOSFET::Capacitances MOSFET::getCapacitances(double Vgs, double Vds) const
{
    Capacitances caps;

    // Cgs 主要受 Vgs 影響
    if (Vgs < m_Vth) {
        // 截止區：電容較小
        caps.Cgs = m_Cgs * 0.5;
    } else {
        // 導通區：電容較大
        caps.Cgs = m_Cgs;
    }

    // Cgd 是米勒電容，受 Vds 影響很大
    if (Vds < 1.0) {
        // 低 Vds：電容較大
        caps.Cgd = m_Cgd * 2.0;
    } else if (Vds > 10.0) {
        // 高 Vds：電容較小
        caps.Cgd = m_Cgd * 0.5;
    } else {
        // 中間線性變化
        double ratio = (Vds - 1.0) / 9.0;
        caps.Cgd = m_Cgd * (2.0 - ratio);
    }

    // Cds 主要受 Vds 影響
    caps.Cds = m_Cds * (1.0 / std::sqrt(1.0 + Vds));

    return caps;
}

// 計算米勒電荷
double MOSFET::calculateMillerCharge(double Vgs_start, double Vgs_end, double Vds) const
{
    //double m_Cgs_Vmax;
    // 米勒電荷 Qgd = ∫ Cgd dVgd
    // 簡化：用平均電容 × 電壓變化
    double Vgd_start = Vgs_start - Vds;
    double Vgd_end = Vgs_end - Vds;

    double Cgd_start = m_Cgd;  // 這裡可以用 getCapacitances 來算
    double Cgd_end = m_Cgd;

    double avg_Cgd = (Cgd_start + Cgd_end) / 2.0;
    return avg_Cgd * std::abs(Vgd_end - Vgd_start);
}

// 計算開通延遲
double MOSFET::calculateTurnOnDelay(double Vgs, double Rg) const
{
    // 簡單 RC 延遲：τ = Rg × Ciss
    // Ciss = Cgs + Cgd
    double Ciss = m_Cgs + m_Cgd;
    double tau = Rg * Ciss;

    // 充電到 Vth 的時間（粗略）
    if (Vgs > m_Vth) {
        return -tau * std::log(1.0 - m_Vth / Vgs);
    }
    return 0.0;
}

// 計算關斷延遲
double MOSFET::calculateTurnOffDelay(double Vgs, double Rg) const
{
    // 簡單 RC 放電
    double Ciss = m_Cgs + m_Cgd;
    double tau = Rg * Ciss;

    // 從 Vgs 放電到 Vth 的時間
    if (Vgs > m_Vth) {
        return tau * std::log(Vgs / m_Vth);
    }
    return 0.0;
}

void MOSFET::setCurvePoints(int points) {
    m_curvePoints = points;
}

int MOSFET::getCurvePoints() const {
    return m_curvePoints;
}



std::vector<Point> MOSFET::generateCurve(double inputParam) const {
    return outputCurve(inputParam);  // 直接呼叫原本的函式
    /*mosfet.h:429:12: Use of undeclared identifier 'outputCurve'
     */
}

// 在 mosfet.cpp 中加入

void MOSFET::setCapacitancesFromDatasheet(double Ciss, double Coss, double Crss)
{
    // 從規格書參數反推個別電容
    // Ciss = Cgs + Cgd
    // Coss = Cds + Cgd
    // Crss = Cgd

    m_Cgd = Crss;                    // 米勒電容直接是 Crss
    m_Cgs = Ciss - Crss;              // Cgs = Ciss - Crss
    m_Cds = Coss - Crss;              // Cds = Coss - Crss

    // 確保不為負（數值誤差保護）
    if (m_Cgs < 0.0) {
        m_Cgs = 0.0;
    }
    if (m_Cds < 0.0) {
        m_Cds = 0.0;
    }
}

void MOSFET::setVoltageDependentCapacitances(
    double Ciss_0, double Coss_0, double Crss_0,
    double Vds_ref)
{
    // 先用基本公式設定
    setCapacitancesFromDatasheet(Ciss_0, Coss_0, Crss_0);

    // 這裡可以儲存參考電壓和電容值，供 getCapacitances 使用
    // 但目前 getCapacitances 用的是簡化模型
    // 如果需要精確的電壓相依性，需要擴充 getCapacitances

    // 簡單起見，先不做複雜處理
    // 未來可以擴充成員變數：
    // m_Ciss_0 = Ciss_0;
    // m_Coss_0 = Coss_0;
    // m_Crss_0 = Crss_0;
    // m_Vds_ref = Vds_ref;
}

// 飽和區 Id 計算
double MOSFET::calculateId_saturation(double Vgs, double Vds) const
{
    // 處理極性
    double effectiveVgs = Vgs;
    double effectiveVds = Vds;
    double effectiveVth = m_Vth;

    if (!m_isNChannel) {
        effectiveVgs = -Vgs;
        effectiveVds = -Vds;
        effectiveVth = -m_Vth;
    }

    // 檢查是否截止
    if (effectiveVgs <= effectiveVth) return 0.0;

    double Vgs_eff = effectiveVgs - effectiveVth;
    double Id = m_Kn * Vgs_eff * Vgs_eff * (1.0 + m_lambda * effectiveVds);

    // 限制電流
    if (Id > m_Id_max) Id = m_Id_max;

    return m_isNChannel ? Id : -Id;
}

// 三極管區 Id 計算
double MOSFET::calculateId_triode(double Vgs, double Vds) const
{
    // 處理極性
    double effectiveVgs = Vgs;
    double effectiveVds = Vds;
    double effectiveVth = m_Vth;

    if (!m_isNChannel) {
        effectiveVgs = -Vgs;
        effectiveVds = -Vds;
        effectiveVth = -m_Vth;
    }

    // 檢查是否截止
    if (effectiveVgs <= effectiveVth) return 0.0;
    if (effectiveVds <= 0.0) return 0.0;

    double Vgs_eff = effectiveVgs - effectiveVth;
    double Id = m_Kn * (2.0 * Vgs_eff * effectiveVds - effectiveVds * effectiveVds);

    // 限制電流
    if (Id > m_Id_max) Id = m_Id_max;

    return m_isNChannel ? Id : -Id;
}

// 飽和區起始電壓計算
double MOSFET::calculateVds_saturation(double Vgs) const
{
    // 處理極性
    double effectiveVgs = Vgs;
    double effectiveVth = m_Vth;

    if (!m_isNChannel) {
        effectiveVgs = -Vgs;
        effectiveVth = -m_Vth;
    }

    if (effectiveVgs <= effectiveVth) return 0.0;

    double Vds_sat = effectiveVgs - effectiveVth;

    return m_isNChannel ? Vds_sat : -Vds_sat;
}


QString MOSFET::deviceType() const {
    return "MOSFET";
}

QString MOSFET::inputUnit() const {
    return "V";  // Vgs 的單位
}

