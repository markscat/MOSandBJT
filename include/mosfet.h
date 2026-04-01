#ifndef MOSFET_H
#define MOSFET_H

//1. 輸出特性（Output Characteristics）：Id vs Vds
//
//
/**
 * 1. 直流分析公式:
 * Id =Kn 2*((Vgs-Vth)Vds_eff-Vds_eff^2)(1+λVds)
 * Kn= 1/2*μn*Cox*W/L
 * μn: 電子遷移率 (n-channel)
 * Cox:單位面積閘氧化層電容
 * W:通道寬度
 * L:通道長度
 *
 * 2. 在飽和區中：
 * Vdes_eff =(Vgs-Vth)
 * Id = Kn (2*(Vgs-Vth)Vds_eff-Vds_eff^2)(1+λVds)
 *    = Kn (2* Vds_eff^2- Vds_eff^2)(1+λVds)
 *    = Kn*(Vds_eff^2)(1+λVds)
 *    = 2*Kn[(Vgs-Vth)^2]*(1+λVds)
 *
 * 3. 在線性區中:
 * 1.  **電壓極小：**     Vds_eff = V_ds（因為還沒達到飽和門檻）。
 * 2.  **忽略高次方：**    V_ds^2 項太小了，直接丟掉。
 * 3.  **忽略微小修正：**  lambda*Vds 遠小於 1，所以 1 + lambda*V_ds 趨近於 1
 * Id =Kn 2*((Vgs-Vth)Vds)
 *
 * 4. λ（通道調變係數）怎麼來？
 * λ 影響飽和區曲線的斜率，規格書通常沒有，但有幾種方式：
 * 方法 A：用輸出電阻 ro 反推
 * ro = 1 / (λ × Id)
 * 如果規格書有給輸出電阻（很少），可以反推
 *
 * 方法 B：直接忽略（λ = 0）
 * 初學者或粗略計算，λ 設為 0 也沒關係
 * 曲線在飽和區就是平的
 *
 * 方法 C：用經驗值
 * 小訊號 MOSFET：λ ≈ 0.01 ~ 0.1
 * 功率 MOSFET：λ ≈ 0.001 ~ 0.01
 *
 * 5. Rds(on)
 *   動態電阻
 *   Id =Kn 2*((Vgs-Vth)Vds)
 *   =>
 *   Id/Vds = Kn 2*((Vgs-Vth)Vds)/Vds
 *   ==>分母的Vds，和分子的Vds約分
 *   ==>因為Rds= Vds/Id
 *   所以：
 *   Rds = Vds/Id = 1/{Kn 2*((Vgs-Vth)}
 *
 *   線性區中:
 *   Vgs >> Vth
 *   Vds 趨近於零
 *   Rds(on)= Rds
 *
 *  Letex語法
   \begin{array}
   $I_{d}=2K_{n}*((V_{gs}-V_{th})*V_{ds-eff}-V_{ds-eff}^2)(1+λV_{ds})\\
   Kn= \frac{\mu_{n}*C_{ox}*W}{2L} \\
   \mu_{n}\text{:電子遷移率 (n-channel)/電洞遷移率 (p-channel) }\\
   C_{ox}\text{:單位面積閘氧化層電容}\\
   \text{W:通道寬度}\\
   \text{L:通道長度}\end{array}


 */


#include "transistor.h"

#include "../ui/cauverdrawable.h"
#include <string>
#include <vector>

/**
* @file mosfet.h
* @brief MOSFET 類別，繼承自 Transistor
*   @主要功能包括：
*   @ - 設定和讀取 MOSFET 參數（Vth、Kn、λ、Rds_on、gfs、Id_max、Vds_max）
*   @ - 產生 MOSFET 的特性曲線（輸出特性、轉移特性）
*   @ - 計算工作點（Bias Point）
*   @ - 找點功能（從 Id 找 Vgs，從 Vgs 算 Id，從 Id 找 Vds）
*   @ - 電容參數設定與計算（Cgs、Cgd、Cds）
* 
* @author Markscat
* @copyright Copyright (c) 2024 Markscat. All rights reserved.
* 本專案使用 GPLv3 授權，詳情請見 LICENSE 檔案。
* 
* @since 2024-06
* @see Transistor
* @see BJT
* */
/**
 * Transistor       -+
 * CurveDrawable     | MOSFET
 */

class MOSFET : public Transistor,public CurveDrawable{

public:
    /**
	* @brief MOSFET 類別建構子
	* @details 建構子會設定 MOSFET 的預設參數值，例如 Vth、Kn、λ、Rds_on、gfs、Id_max、Vds_max 等等。
               這些預設值可以根據常見的 MOSFET 型號來設定，或者使用一些典型的值。建構子還會初始化電容參數為 0，並設定曲線點數的預設值。
	* @note 這個建構子不接受任何參數，使用者需要在創建 MOSFET 物件後通過 setParameter() 方法來設定具體的參數值，以模擬不同型號和特性的 MOSFET。
    * 
    */
    MOSFET();
    ~MOSFET() override;

    /**
	* @brief 電容結構體
	* @details 這個結構體用於存儲 MOSFET 的電容參數，包括 Cgs（閘極-源極電容）、Cgd（閘極-汲極電容）和 Cds（汲極-源極電容）。
               這些電容參數對於模擬 MOSFET 的高頻特性和開關行為非常重要。
	* @param Cgs 閘極-源極電容，單位為法拉（F）
	* @param Cgd 閘極-汲極電容，單位為法拉（F），也稱為米勒電容，對於開關特性影響很大
	* @param Cds 汲極-源極電容，單位為法拉（F），通常較小，但在高頻下也會影響性能
	* @note 這個結構體可以用於一次性返回所有電容參數，或者在內部存儲電容相關的數據。使用者可以通過 setCapacitances() 
            方法來設定這些電容參數，或者從規格書的 Ciss、Coss、Crss 反推來設定。這些電容參數也可以隨著 Vgs 和 Vds 的變化而變化，模擬更真實的 MOSFET 行為。
    * 
    */
    struct Capacitances {
        double Cgs;  // 閘極-源極電容 (F)
        double Cgd;  // 閘極-汲極電容 (F) - 米勒電容
        double Cds;  // 汲極-源極電容 (F)

        Capacitances() : Cgs(0), Cgd(0), Cds(0) {}
    };


    /**
	* @brief setCapacitances(double Cgs, double Cgd, double Cds);
	* @brief 設定電容參數
	* @param Cgs 閘極-源極電容，單位為法拉（F）
	* @param Cgd 閘極-汲極電容，單位為法拉（F），也稱為米勒電容，對於開關特性影響很大
	* @param Cds 汲極-源極電容，單位為法拉（F），通常較小，但在高頻下也會影響性能
	* @details 這個方法用於設定 MOSFET 的電容參數，這些參數對於模擬 MOSFET 的高頻特性和開關行為非常重要。
               使用者可以直接設定 Cgs、Cgd 和 Cds 的值，或者從規格書的 Ciss、Coss、Crss 反推來設定。這些電容參數也可以隨著 Vgs 和 Vds 的變化而變化，模擬更真實的 MOSFET 行為。
	* @note 這個方法允許使用者靈活地設定 MOSFET 的電容參數，以模擬不同型號和特性的 MOSFET。合理的電容設定對於獲得準確的模擬結果非常重要，特別是在高頻應用中。
    */
    void setCapacitances(double Cgs, double Cgd, double Cds);// 設定電容參數


    /**
	* @brief setCapacitancesFromDatasheet(double Ciss, double Coss, double Crss);
	* @brief 從規格書的 Ciss、Coss、Crss 設定電容參數
	* @param Ciss 輸入電容，通常在 Vds=0V 時測量，包含 Cgs 和 Cgd 的組合
	* @param Coss 輸出電容，通常在 Vds=0V 時測量，包含 Cds 和 Cgd 的組合
	* @param Crss 米勒電容，通常在 Vds=0V 時測量，等於 Cgd
    * @details 這個方法用於從規格書提供的 Ciss、Coss 和 Crss 參數來設定 MOSFET 的電容參數。通常，Ciss 包含 Cgs 和 Cgd 的組合，Coss 包含 Cds 和 Cgd 的組合，而 Crss 就是 Cgd。
			   使用者可以通過這個方法來快速設定電容參數，而不需要直接測量或計算 Cgs、Cgd 和 Cds 的值。這些電容參數也可以隨著 Vgs 和 Vds 的變化而變化，模擬更真實的 MOSFET 行為。
    */
    // 從規格書的 Ciss, Coss, Crss 設定電容
    void setCapacitancesFromDatasheet(double Ciss, double Coss, double Crss);



    /**
	* @brief setVoltageDependentCapacitances(double Ciss_0, double Coss_0, double Crss_0, double Vds_ref);
	* @brief 設定電壓相依電容的參考點
	* @param Ciss_0 輸入電容在 Vds=0V 時的值，包含 Cgs 和 Cgd 的組合
	* @param Coss_0 輸出電容在 Vds=0V 時的值，包含 Cds 和 Cgd 的組合
	* @param Crss_0 米勒電容在 Vds=0V 時的值，等於 Cgd
	* @param Vds_ref 參考電壓，通常是 25V，表示在這個電壓下的電容值作為參考點
    * @details 這個方法用於設定電壓相依電容的參考點，這些參數可以用於計算隨著 Vds 變化的電容值。通常，Ciss_0、Coss_0 和 Crss_0 是在 Vds=0V 時測量的電容值，而 Vds_ref 是一個參考電壓，表示在這個電壓下的電容值作為基準。
			   使用者可以通過這個方法來啟用電壓相依的電容模型，使得在模擬 MOSFET 的行為時能夠更真實地反映電容隨著 Vds 變化的特性。這對於高頻應用和開關特性的分析非常重要。 
    */
    // 設定電壓相依電容的參考點（可選）
    void setVoltageDependentCapacitances(
        double Ciss_0, double Coss_0, double Crss_0,  // Vds=0V 時的值
        double Vds_ref                                 // 參考電壓（通常 25V）
        );

    /**
	* @brief getCapacitances(double Vgs, double Vds) const;
	* @brief 取得電容值，根據 Vgs 和 Vds 的變化
	* @param Vgs 閘極-源極電壓，這個參數用於計算電容值，特別是當使用電壓相依模型時
	* @param Vds 汲極-源極電壓，這個參數用於計算電容值，特別是當使用電壓相依模型時
    * @details 這個方法用於根據給定的 Vgs 和 Vds 來計算和返回 MOSFET 的電容值。如果啟用了電壓相依的電容模型，則電容值會隨著 Vgs 和 Vds 的變化而變化。這些電容值對於模擬 MOSFET 的高頻特性和開關行為非常重要。
			   使用者可以通過這個方法來獲取在特定工作條件下的 Cgs、Cgd 和 Cds 的值，以便在模擬和分析中使用。這些電容值也可以用於計算米勒電荷和開關時間等相關特性。
    */
    // 取得電容值（可隨電壓變化）
    Capacitances getCapacitances(double Vgs, double Vds) const;

    /**
	* @brief calculateMillerCharge(double Vgs_start, double Vgs_end, double Vds) const;
	* @brief 計算米勒電荷
	* @param Vgs_start 起始的 Vgs 電壓，這個參數用於計算在 Vgs 從起始值變化到結束值過程中所涉及的電荷量
	* @param Vgs_end 結束的 Vgs 電壓，這個參數用於計算在 Vgs 從起始值變化到結束值過程中所涉及的電荷量
	* @param Vds 汲極-源極電壓，這個參數用於計算米勒電荷，特別是當使用電壓相依模型時
    * @details 這個方法用於計算米勒電荷，米勒電荷是由 Cgd（閘極-汲極電容）引起的電荷量，當 Vgs 從一個值變化到另一個值時，
               會有一部分電荷需要通過 Cgd 來充放電。這個電荷量對於開關時間的計算非常重要，特別是在高頻應用中。
			   使用者可以通過這個方法來獲取在特定 Vgs 變化過程中所涉及的米勒電荷量，以便在模擬和分析中使用。這些電荷量可以用於計算開關時間、損耗等相關特性。
    */
    // 計算米勒電荷
    double calculateMillerCharge(double Vgs_start, double Vgs_end, double Vds) const;


    // 計算開關時間（粗略估算）
   /**
   * @brief calculateTurnOnDelay(double Vgs, double Rg) const;
   * @brief 計算 MOSFET 的開通延遲時間
   * @param Vgs 閘極-源極電壓，這個參數用於計算開通過程中所需的電荷量和電流
   * @param Rg 閘極電阻，這個參數用於計算充電時間常數，影響開通速度
   * @details 這個方法用於計算 MOSFET 的開通延遲時間，這是從 Vgs 開始上升到達臨界電壓 Vth 所需的時間。
   *          計算過程中會考慮到米勒電荷和閘極電阻對充電過程的影響。這個方法提供了一個粗略的估算，實際的開通時間可能會受到其他因素（如溫度、製程變異等）的影響。
   */
    double calculateTurnOnDelay(double Vgs, double Rg) const;

    /**
	* @brief calculateTurnOffDelay(double Vgs, double Rg) const;
	* @brief 計算 MOSFET 的關斷延遲時間
	* @param Vgs 閘極-源極電壓，這個參數用於計算關斷過程中所需的電荷量和電流
	* @param Rg 閘極電阻，這個參數用於計算放電時間常數，影響關斷速度
    * @details 這個方法用於計算 MOSFET 的關斷延遲時間，這是從 Vgs 開始下降到達臨界電壓 Vth 所需的時間。
			   計算過程中會考慮到米勒電荷和閘極電阻對放電過程的影響。這個方法提供了一個粗略的估算，實際的關斷時間可能會受到其他因素（如溫度、製程變異等）的影響。
    */
    double calculateTurnOffDelay(double Vgs, double Rg) const;


    // 型號與類型
    /**
	* @brief type() const override;
	* @brief 回傳元件類型
	* @return std::string "MOSFET"
	* @details 這個方法是 Transistor 類別的純虛擬方法，MOSFET 類別必須實作它以回傳正確的類型資訊。
               這個方法在使用者界面或其他需要識別元件類型的地方非常重要，可以幫助區分不同類型的晶體管（例如 BJT 和 MOSFET）。
               這個方法不接受任何參數，直接回傳固定的字串 "MOSFET"，表示這是一個金氧半場效電晶體。
    */
    std::string type() const override;

    /**
	* @brief subtype() const;
	* @brief 回傳 MOSFET 的子類型
	* @return std::string "N-channel" 或 "P-channel"
    * @details 這個方法是 MOSFET 類別特有的方法，用於區分 N-channel 和 P-channel 兩種基本的 MOSFET 類型。N-channel 和 P-channel 的電流方向和偏壓條件不同，因此在模擬和分析電路時需要知道具體的子類型。
			   根據內部的 m_isNChannel 變數決定回傳 "N-channel" 或 "P-channel"。
    */
    std::string subtype() const;  // N-channel 或 P-channel


    // 參數設定與讀取使用 std::string）
    /**
	* @brief setParameter(const std::string& name, double value) override;
	* @brief 設定 MOSFET 的參數
	* @return void
	* @param name 參數名稱，可以是 "Vth"、"Kn"、"lambda"、"Rds_on"、"gfs"、"Id_max" 或 "Vds_max"
	* @details 根據參數名稱設定對應的內部變數值，例如 "Vth" 對應 m_Vth，"Kn" 對應 m_Kn 等等。這個方法允許使用者靈活地設定 MOSFET 的各種參數，以模擬不同型號和特性的 MOSFET。
    */
    virtual void setParameter(const std::string& name, double value) override;

    /**
	* @brief getParameter(const std::string& name) const override;
	* @brief 讀取 MOSFET 的參數
	* @param name 參數名稱，可以是 "Vth"、"Kn"、"lambda"、"Rds_on"、"gfs"、"Id_max" 或 "Vds_max"
	* @return double 參數值，如果參數名稱無效則回傳 NaN
    * @details 根據參數名稱讀取對應的內部變數值，例如 "Vth" 對應 m_Vth，"Kn" 對應 m_Kn 等等。這個方法允許使用者靈活地讀取 MOSFET 的各種參數，以便在模擬和分析中使用。
			   如果參數名稱無效，則回傳 NaN（Not a Number），表示沒有對應的參數。使用者可以通過這個方法來獲取 MOSFET 的具體參數值，以便在模擬和分析中使用。
    */
    virtual double getParameter(const std::string& name) const override;

    /**
	* @brief paramList() const override;
	* @brief 取得參數列表
	* @return std::vector<std::string> 包含所有可用參數名稱的列表
	* @details 這個方法回傳一個包含所有可用參數名稱的列表，這些參數名稱可以用於 setParameter() 和 getParameter() 方法。
               這個列表有助於使用者了解可以設定和讀取哪些參數，以便在模擬和分析中使用正確的參數名稱
    */
    std::vector<std::string> paramList() const override;

    // 參數驗證（檢查是否合理）
    /**
	* @brief validateParameters(std::string& errorMsg) const;
	* @brief 驗證參數的合理性
	* @param errorMsg 用於返回錯誤訊息的參考變數，如果參數不合理，會在這裡填入具體的錯誤描述
	* @return bool 如果參數合理則回傳 true，否則回傳 false 並在 errorMsg 中提供錯誤訊息
	* @details 這個方法檢查 MOSFET 的參數是否在合理的範圍內，例如 Vth 必須大於 0，Kn 必須大於 0，λ 不能為負等等。
               如果有任何參數不合理，方法會回傳 false 並設置 errorMsg 為具體的錯誤描述，幫助使用者了解哪個參數有問題以及為什麼不合理。
    */
    bool validateParameters(std::string& errorMsg) const;


    // 特性曲線
    /**
	* @brief outputCurve(double Vgs) const override;
	* @brief 產生輸出特性曲線（Id vs Vds）
	* @param Vgs 閘極-源極電壓，這個參數用於生成特定 Vgs 下的 Id vs Vds 曲線
    * @details 這個方法用於生成 MOSFET 的輸出特性曲線，即在給定 Vgs 的條件下，Id 隨 Vds 變化的曲線。方法會根據 MOSFET 的工作區域（截止區、三極管區、飽和區）來計算對應的 Id 值，並返回一個包含 (Vds, Id) 點的向量。
			   使用者可以通過這個方法來獲取在特定 Vgs 下的輸出特性曲線，以便在模擬和分析中使用。這些曲線對於理解 MOSFET 的行為和設計電路非常重要。
    */
    std::vector<Point> outputCurve(double Vgs) const override;      // Id vs Vds
    
    /**
	* @brief transferCurve() const override;
	* @brief 產生轉移特性曲線（Id vs Vgs）
    * @details 這個方法用於生成 MOSFET 的轉移特性曲線，即 Id 隨 Vgs 變化的曲線。方法會根據 MOSFET 的工作區域（截止區、三極管區、飽和區）來計算對應的 Id 值，並返回一個包含 (Vgs, Id) 點的向量。
			   使用者可以通過這個方法來獲取 MOSFET 的轉移特性曲線，以便在模擬和分析中使用。這些曲線對於理解 MOSFET 的行為和設計電路非常重要。
    */
    std::vector<Point> transferCurve() const override;              // Id vs Vgs


    // MOSFET 特有的方法（可選）
    /**
	* @brief transferCurve(double Vds) const;
	* @brief 產生轉移特性曲線（Id vs Vgs）給定 Vds
	* @param Vds 汲極-源極電壓，這個參數用於生成特定 Vds 下的 Id vs Vgs 曲線
    * @details 這個方法是 MOSFET 類別特有的方法，用於生成在給定 Vds 的條件下，Id 隨 Vgs 變化的轉移特性曲線。這個方法允許使用者在特定的 Vds 條件下分析 MOSFET 的轉移特性，這對於設計和模擬電路非常有用。
			   使用者可以通過這個方法來獲取在特定 Vds 下的轉移特性曲線，以便在模擬和分析中使用。這些曲線對於理解 MOSFET 的行為和設計電路非常重要，特別是在開關應用中。
    */
    std::vector<Point> transferCurve(double Vds) const;  // 給定 Vds 的轉移曲線


    // MOSFET 特有的方法（可選）
    /**
    * @brief transferCurve(double Vds, double Vgs_start, double Vgs_end) const;
    * @brief 產生轉移特性曲線（Id vs Vgs）給定 Vds
    * @param Vds 汲極-源極電壓，這個參數用於生成特定 Vds 下的 Id vs Vgs 曲線
    * @param Vgs_start
    * @param Vgs_end
    * @details 這個方法是 MOSFET 類別特有的方法，用於生成在給定 Vds 的條件下，Id 隨 Vgs 變化的轉移特性曲線。這個方法允許使用者在特定的 Vds 條件下分析 MOSFET 的轉移特性，這對於設計和模擬電路非常有用。
               使用者可以通過這個方法來獲取在特定 Vds 下的轉移特性曲線，以便在模擬和分析中使用。這些曲線對於理解 MOSFET 的行為和設計電路非常重要，特別是在開關應用中。
    */
    std::vector<Point> transferCurve(double Vds, double Vgs_start, double Vgs_end) const;

    // 工作點計算
    //回傳型別 函式名稱(參數列表) [const] [override] [final] = 0;
    /**
	* @brief calculateQPoint(double Vdd, double Rd, double Rg = 0) const override;
	* @brief 計算 MOSFET 的工作點（Bias Point）
	* @param Vdd 電源電壓，這個參數用於計算工作點的電壓條件
	* @param Rd 汲極負載電阻，這個參數用於計算工作點的電流條件
	* @param Rg 閘極電阻，這個參數用於計算工作點的電壓條件，默認值為 0
    * @details 這個方法用於計算 MOSFET 的工作點（Bias Point），根據給定的電源電壓 Vdd、汲極負載電阻 Rd 和閘極電阻 Rg 來計算 MOSFET 在這些條件下的 Id 和 Vds。方法會考慮 MOSFET 的特性曲線和工作區域來確定最終的工作點。
			   使用者可以通過這個方法來獲取在特定電源和負載條件下的 MOSFET 工作點，以便在模擬和分析中使用。這些工作點對於理解 MOSFET 的行為和設計電路非常重要。
    */

    BiasPoint calculateQPoint(const BiasParams& params) const override;
    BiasPoint calculateQPoint_FixedVgs(double Vdd, double Rd, double Vgs) const;



    // 找點功能
    /**
	* @brief findVgsFromId(double Id, double Vds) const;
	* @brief 給定 Id 和 Vds 找 Vgs
	* @param Id 漏極電流，這個參數用於計算在特定 Vds 下對應的 Vgs
	* @param Vds 汲極-源極電壓，這個參數用於計算在特定 Id 下對應的 Vgs
    * @details 這個方法用於在給定 Id 和 Vds 的條件下計算對應的 Vgs。方法會根據 MOSFET 的特性曲線和工作區域來確定最終的 Vgs 值，並返回結果。
			   使用者可以通過這個方法來獲取在特定 Id 和 Vds 條件下的 Vgs，以便在模擬和分析中使用。這些值對於理解 MOSFET 的行為和設計電路非常重要。	
    */
    double findVgsFromId(double Id, double Vds) const;   // 給 Id 找 Vgs

    /**
	* @brief findIdFromVgs(double Vgs, double Vds) const;
	* @brief 給定 Vgs 和 Vds 算 Id
	* @param Vgs 閘極-源極電壓，這個參數用於計算在特定 Vds 下對應的 Id
	* @param Vds 汲極-源極電壓，這個參數用於計算在特定 Vgs 下對應的 Id
    * @details 這個方法用於在給定 Vgs 和 Vds 的條件下計算對應的 Id。方法會根據 MOSFET 的特性曲線和工作區域來確定最終的 Id 值，並返回結果。
			   使用者可以通過這個方法來獲取在特定 Vgs 和 Vds 條件下的 Id，以便在模擬和分析中使用。這些值對於理解 MOSFET 的行為和設計電路非常重要。
    */
    double findIdFromVgs(double Vgs, double Vds) const;  // 給 Vgs 算 Id

    /**
	* @brief findVdsFromId(double Id, double Vgs) const;
	* @brief 給定 Id 和 Vgs 找 Vds
	* @param Id 漏極電流，這個參數用於計算在特定 Vgs 下對應的 Vds
	* @param Vgs 閘極-源極電壓，這個參數用於計算在特定 Id 下對應的 Vds
    * @details 這個方法用於在給定 Id 和 Vgs 的條件下計算對應的 Vds。方法會根據 MOSFET 的特性曲線和工作區域來確定最終的 Vds 值，並返回結果。
			   使用者可以通過這個方法來獲取在特定 Id 和 Vgs 條件下的 Vds，以便在模擬和分析中使用。這些值對於理解 MOSFET 的行為和設計電路非常重要。
    */
    double findVdsFromId(double Id, double Vgs) const;   // 給 Id 找 Vds

    /**
	* @brief setCurvePoints(int points);
	* @brief 設定特性曲線的點數
	* @param points 特性曲線中點的數量，這個參數用於控制在生成特性曲線時所使用的點的密度。較多的點可以提供更平滑和精確的曲線，但也會增加計算時間。
	* @details 這個方法用於設定在生成特性曲線（輸出特性和轉移特性）時所使用的點的數量。較多的點可以提供更平滑和精確的曲線，但也會增加計算時間。
               使用者可以根據需要來調整這個參數，以獲得適合的曲線品質和計算效率。
    */
    void setCurvePoints(int points);

    /**
	* @brief getCurvePoints() const;
	* @brief 取得特性曲線的點數
	* @return int 特性曲線中點的數量，這個參數用於控制在生成特性曲線時所使用的點的密度。較多的點可以提供更平滑和精確的曲線，但也會增加計算時間。
    */
    int getCurvePoints() const;



    // 新增：從 CurveDrawable 繼承來的函式
    std::vector<Point> generateCurve(double inputParam) const override;

    QString deviceType() const override;

    QString inputUnit() const override;



    // 根據規格書參數反推 Kn
    // rdsOn: 導通電阻 (Ohm)
    // vgsAtRds: 規格書測量該 Rds 時所用的 Vgs 電壓 (V)
    // vth: 臨界電壓 (V)
    double calculateKnFromRds(double rdsOn, double vgsAtRds, double vth);

    static double calculateGfsFromRds(double rdsOn_Ohm);


    double calculateGm(double Vgs, double Vds) const;
    double calculateRds(double Vgs, double Vds) const;

private:

    /**
	* @brief MOSFET 的內部參數
    * @details 這些參數是 MOSFET 模型的核心參數，用於計算特性曲線和工作點。這些參數包括：
               - m_Vth：臨界電壓，決定 MOSFET 開始導通的電壓
               - m_Kn：導電參數，影響 Id 的大小
               - m_lambda：通道調變係數，影響飽和區曲線的斜率
               - m_Rds_on：導通電阻，在特定 Vgs 下測量的 Rds(on) 用於反推 Kn
               - m_gfs：轉導，在特定 Vgs 下測量的 gfs 用於反推 Kn
               - m_Id_max：最大電流，限制 MOSFET 的最大 Id
               - m_Vds_max：最大電壓，限制 MOSFET 的最大 Vds
               - m_Cgs、m_Cgd、m_Cds：電容參數，用於模擬高頻特性和開關行為
			   這些參數可以通過 setParameter() 方法來設定，或者從規格書中反推來設定，以模擬不同型號和特性的 MOSFET。
    */
    // 模型參數
    double m_Vth;       // 臨界電壓 (V)
    double m_Kn;        // 導電參數 (A/V²)
    double m_lambda;    // 通道調變係數 (1/V)
    double m_Rds_on;    // 導通電阻 (Ω) - 可選
    double m_gfs;       // 轉導 (S) - 可選
    double m_Id_max;    // 最大電流 (A)
    double m_Vds_max;   // 最大電壓 (V)

    // 新增電容成員
    double m_Cgs;       // 閘極-源極電容 (F)
    double m_Cgd;       // 閘極-汲極電容 (F)
    double m_Cds;       // 汲極-源極電容 (F)

    /**
	* @brief m_isNChannel
	* @details 這個布林變數用於區分 MOSFET 的子類型，是 N-channel 還是 P-channel。N-channel MOSFET 的電流方向是從漏極流向源極，
               而 P-channel MOSFET 的電流方向是從源極流向漏極。這個變數在計算特性曲線和工作點時會影響電流的計算方式和偏壓條件。
    */
    bool m_isNChannel;  // true: N-channel, false: P-channel

    /**
	* @brief m_curvePoints
	* @return int 特性曲線中點的數量，這個參數用於控制在生成特性曲線時所使用的點的密度。較多的點可以提供更平滑和精確的曲線，但也會增加計算時間。
	* @details 這個整數變數用於設定在生成特性曲線（輸出特性和轉移特性）時所使用的點的數量。較多的點可以提供更平滑和精確的曲線，
    *          但也會增加計算時間。使用者可以通過 setCurvePoints() 方法來調整這個參數，以獲得適合的曲線品質和計算效率。
    */
    int m_curvePoints = 1000;  // 預設 1000 點

    // 內部計算函式
    /**
	* @brief calculateId_saturation(double Vgs, double Vds) const;
	* @brief 計算飽和區的 Id
	* @param Vgs 閘極-源極電壓，這個參數用於計算在飽和區條件下的 Id
	* @param Vds 汲極-源極電壓，這個參數用於計算在飽和區條件下的 Id
	* @return double 飽和區的 Id 值，根據 MOSFET 的特性曲線和工作區域來確定最終的 Id 值
    * @details 這個方法用於計算在飽和區條件下的 Id，根據 MOSFET 的特性曲線和工作區域來確定最終的 Id 值。方法會考慮到 Vgs 和 Vds 的關係，以及 MOSFET 的參數（如 Vth、Kn、λ 等）來計算 Id。
			   使用者可以通過這個方法來獲取在特定 Vgs 和 Vds 條件下的飽和區 Id，以便在模擬和分析中使用。這些值對於理解 MOSFET 的行為和設計電路非常重要。
    */
    double calculateId_saturation(double Vgs, double Vds) const;

    /**
	* @brief calculateId_triode(double Vgs, double Vds) const;
	* @brief 計算三極管區的 Id
	* @param Vgs 閘極-源極電壓，這個參數用於計算在三極管區條件下的 Id
	* @param Vds 汲極-源極電壓，這個參數用於計算在三極管區條件下的 Id
	* @return double 三極管區的 Id 值，根據 MOSFET 的特性曲線和工作區域來確定最終的 Id 值
    * @details 這個方法用於計算在三極管區條件下的 Id，根據 MOSFET 的特性曲線和工作區域來確定最終的 Id 值。方法會考慮到 Vgs 和 Vds 的關係，以及 MOSFET 的參數（如 Vth、Kn、λ 等）來計算 Id。
			   使用者可以通過這個方法來獲取在特定 Vgs 和 Vds 條件下的三極管區 Id，以便在模擬和分析中使用。這些值對於理解 MOSFET 的行為和設計電路非常重要。
    */
    double calculateId_triode(double Vgs, double Vds) const;

    /**
	* @brief calculateVds_saturation(double Vgs) const;
	* @brief 計算飽和區的 Vds 起始電壓
	* @param Vgs 閘極-源極電壓，這個參數用於計算在飽和區條件下的 Vds 起始電壓
	* @return double 飽和區的 Vds 起始電壓，根據 MOSFET 的特性曲線和工作區域來確定最終的 Vds 起始電壓
    * @details 這個方法用於計算在飽和區條件下的 Vds 起始電壓，根據 MOSFET 的特性曲線和工作區域來確定最終的 Vds 起始電壓。方法會考慮到 Vgs 的值，以及 MOSFET 的參數（如 Vth、Kn、λ 等）來計算 Vds 起始電壓。
			   使用者可以通過這個方法來獲取在特定 Vgs 條件下的飽和區 Vds 起始電壓，以便在模擬和分析中使用。這些值對於理解 MOSFET 的行為和設計電路非常重要。
    */
    double calculateVds_saturation(double Vgs) const;  // 飽和區起始電壓


};



#endif // MOSFET_H
