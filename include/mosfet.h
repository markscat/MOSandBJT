#ifndef MOSFET_H
#define MOSFET_H

//1. 輸出特性（Output Characteristics）：Id vs Vds
//Id = Kn × [2(Vgs - Vth) × Vds - Vds²]
//Id = Kn × (Vgs - Vth)² × (1 + λ × Vds)

/*
 * 這兩個公式需要的參數：
 * Vth：臨界電壓（規格書一定有，通常給 min/typ/max）
 * Kn：導電參數（規格書通常沒有直接給）
 *
 * 2. 轉移特性（Transfer Characteristics）：Id vs Vgs
 * Id = Kn × (Vgs - Vth)²
 * 問題：Kn 從哪裡來？
 * 規格書不會直接給你 Kn，但可以用有給的參數反推：
 *
 * 方法 A：用 Rds(on) 反推
 * Rds(on) 是在某個 Vgs 下（通常是 10V）測的
 * 在三極管區，當 Vds 很小時，Rds(on) ≈ 1 / [2 × Kn × (Vgs - Vth)]
 * 所以 Kn ≈ 1 / [2 × Rds(on) × (Vgs - Vth)]
 *
 * 方法 B：用 gfs 反推
 * gfs（轉導）= ∂Id/∂Vgs = 2 × Kn × (Vgs - Vth)
 * 所以 Kn = gfs / [2 × (Vgs - Vth)]
 *
 * 方法 C：用 Id(on) 反推
 * 有些規格書會給在某個 Vgs 和 Vds 下的 Id
 * 用 Id = Kn × (Vgs - Vth)² 反推 Kn
 *
 *
 */


#include "transistor.h"

class MOSFET : public Transistor
{
public:
    MOSFET();
    ~MOSFET() override;

    // 型號與類型
    QString type() const override;
    QString subtype() const;  // N-channel 或 P-channel

    // 參數設定與讀取
    void setParameter(const QString& name, double value) override;
    double getParameter(const QString& name) const override;
    QStringList paramList() const override;

    // 參數驗證（檢查是否合理）
    bool validateParameters(QString& errorMsg) const;

    // 特性曲線計算
    QVector<QPointF> outputCurve(double Vgs) const override;
      // Id vs Vds, 給定 Vgs
    QVector<QPointF> transferCurve(double Vds) const;    // Id vs Vgs, 給定 Vds

    // 工作點計算
    //回傳型別 函式名稱(參數列表) [const] [override] [final] = 0;
    BiasPoint calculateQPoint(double Vdd, double Rd, double Rg = 0) const override;


    // 找點功能
    double findVgsFromId(double Id, double Vds) const;   // 給 Id 找 Vgs
    double findIdFromVgs(double Vgs, double Vds) const;  // 給 Vgs 算 Id
    double findVdsFromId(double Id, double Vgs) const;   // 給 Id 找 Vds

    QVector<QPointF> transferCurve() const override;


private:
    // 模型參數
    double m_Vth;       // 臨界電壓 (V)
    double m_Kn;        // 導電參數 (A/V²)
    double m_lambda;    // 通道調變係數 (1/V)
    double m_Rds_on;    // 導通電阻 (Ω) - 可選
    double m_gfs;       // 轉導 (S) - 可選
    double m_Id_max;    // 最大電流 (A)
    double m_Vds_max;   // 最大電壓 (V)

    bool m_isNChannel;  // true: N-channel, false: P-channel

    // 內部計算函式
    double calculateId_saturation(double Vgs, double Vds) const;
    double calculateId_triode(double Vgs, double Vds) const;
    double calculateVds_saturation(double Vgs) const;  // 飽和區起始電壓
};


#endif // MOSFET_H
