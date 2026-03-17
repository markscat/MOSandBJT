#ifndef BJT_H
#define BJT_H

#include "../ui/cauverdrawable.h"
#include "transistor.h"
#include <string>
#include <vector>
/**
* @file bjt.h
* @brief BJT 類別，繼承自 Transistor
* @details
* BJT（雙極性接面晶體管）類別，實現了 Transistor 的虛擬方法，並提供了 BJT 特有的參數和方法。
* 主要功能包括：
* - 設定和讀取 BJT 參數（Beta、Vbe_on、Vce_sat、Va、Is、Ic_max、Vce_max）
* - 產生 BJT 的特性曲線（輸出特性、轉移特性、輸入特性）
* - 計算工作點（Bias Point）
* - 找點功能（從 Ic 找 Ib，從 Ib 找 Ic，從 Ic 和 Ib 找 Vce）
* 
* @see Transistor 
* @see MOSFET
* 
* @author Markscat
* @since 2024-06
* 
*/



class BJT : public Transistor,public CurveDrawable
{
public:

    /**@brief 回傳元件類型
        * @return std::string "BJT"
        * @details 回傳元件類型，固定為 "BJT"
        * @note 這個方法是 Transistor 類別的純虛擬方法，BJT 類別必須實作它以回傳正確的類型資訊。
        * @note BJT 類別還有一個 subtype() 方法，可以回傳 NPN 或 PNP 的子類型資訊。
        * @note 這個方法在使用者界面或其他需要識別元件類型的地方非常重要，可以幫助區分不同類型的晶體管（例如 BJT 和 MOSFET）。
        * @note 這個方法不接受任何參數，直接回傳固定的字串 "BJT"，表示這是一個雙極性接面晶體管。
     */
    BJT();
    ~BJT() override;

    // 型號與類型

    std::string type() const override;  // "BJT"
	/**@brief 回傳 BJT 的子類型
	* @return std::string "NPN" 或 "PNP"
    * 
	* @details 回傳 BJT 的子類型，根據內部的 m_isNPN 變數決定回傳 "NPN" 或 "PNP"
    */

    /**@brief 回傳 BJT 的子類型
* @return std::string "NPN" 或 "PNP"
*
* @details 根據內部的 m_isNPN 變數決定回傳 "NPN" 或 "PNP"
* @note 這個方法是 BJT 類別特有的方法，用於區分 NPN 和 PNP 兩種基本的 BJT 類型。NPN 和 PNP 的電流方向和偏壓條件不同，因此在模擬和分析電路時需要知道具體的子類型。
*/
    std::string subtype() const;        // NPN 或 PNP


    // 參數設定與讀取
    /** @brief setParameter(const std::string& name, double value) override;
      * @brief 設定 BJT 的參數
	  * @return void
      *
      * @param name 參數名稱，可以是 "Beta"、"Vbe_on"、"Vce_sat"、"Va"、"Is"、"Ic_max" 或 "Vce_max"
      * @param value 參數值，必須是正數（對於某些參數如 Vce_sat 可以是非負數）
      * @details 根據參數名稱設定對應的內部變數值，例如 "Beta" 對應 m_Beta，"Vbe_on" 對應 m_Vbe_on 等等。這個方法允許使用者靈活地設定 BJT 的各種參數，以模擬不同型號和特性的晶體管。
    */
    void setParameter(const std::string& name, double value) override;

    /**
	* @brief getParameter(const std::string& name) const override;
	* @brief 讀取 BJT 的參數值
	* @return double 參數值
	* @param name 參數名稱，可以是 "Beta"、"Vbe_on"、"Vce_sat"、"Va"、"Is"、"Ic_max" 或 "Vce_max"
	* @details 根據參數名稱讀取對應的內部變數值，例如 "Beta" 對應 m_Beta，"Vbe_on" 對應 m_Vbe_on 等等。這個方法允許使用者靈活地讀取 BJT 的各種參數，以便在模擬和分析電路時使用正確的參數值。
	* @note 這個方法是 Transistor 類別的純虛擬方法，BJT 類別必須實作它以提供對 BJT 參數的存取功能。使用者可以通過這個方法獲取當前設定的 BJT 參數值，以便在計算特性曲線或工作點時使用。
    */
    double getParameter(const std::string& name) const override;
    
    /**
	* @brief paramList() const override;
	* @brief 回傳 BJT 的參數列表
	* @return std::vector<std::string> 包含 BJT 參數名稱的列表
	* @details 這個方法回傳一個包含 BJT 參數名稱的列表，例如 "Beta"、"Vbe_on"、"Vce_sat"、"Va"、"Is"、"Ic_max" 和 "Vce_max"。這個列表可以用於使用者界面或其他需要顯示或選擇參數的地方，幫助使用者了解可用的參數選項。
	* @note 這個方法是 Transistor 類別的純虛擬方法，BJT 類別必須實作它以提供對 BJT 參數的描述。使用者界面可以通過這個方法獲取 BJT 的參數列表，以便動態生成參數輸入框或顯示參數資訊。
    * 
    */
    std::vector<std::string> paramList() const override;

    // 參數驗證
    /**
	* @brief validateParameters(std::string& errorMsg) const override;
	* @brief 驗證 BJT 的參數設定是否合理
	* @param errorMsg 如果參數無效，這個參數會被設置為錯誤訊息的內容
	* @return bool 參數是否有效
	* @param errorMsg 如果參數無效，這個參數會被設置為錯誤訊息的內容
	* @details 這個方法檢查 BJT 的參數是否在合理的範圍內，例如 Beta 必須大於 0，Vbe_on 必須大於 0，Vce_sat 不能為負等等。如果有任何參數不合理，方法會回傳 false 並設置 errorMsg 
    *          為對應的錯誤訊息。這個方法可以在使用者設定參數後調用，以確保模擬和分析使用的是有效的參數值。
    */
    bool validateParameters(std::string& errorMsg) const;

    // 特性曲線
    /**
	* @brief outputCurve(double Ib) const override;
	* @brief 產生 BJT 的輸出特性曲線
	* @param Ib 基極電流，這個參數用於生成特定 Ib 下的 Ic vs Vce 曲線
	* @return std::vector<Point> 包含輸出特性曲線點的列表
	* @param Ib 基極電流，這個參數用於生成特定 Ib 下的 Ic vs Vce 曲線
	* @details 這個方法根據給定的基極電流 Ib 產生 BJT 的輸出特性曲線，返回一個包含 Point 結構的列表，每個 Point 包含 Vce 和對應的 Ic 值。
    *          這個曲線可以用於分析 BJT 在不同 Ib 下的工作狀態，例如主動區、飽和區和截止區。
    */
    std::vector<Point> outputCurve(double Ib) const override;      // Ic vs Vce, 給定 Ib

    /**
	* @brief transferCurve() const override;
	* @brief 產生 BJT 的轉移特性曲線
	* @return std::vector<Point> 包含轉移特性曲線點的列表
	* @details 這個方法產生 BJT 的轉移特性曲線，返回一個包含 Point 結構的列表，每個 Point 包含 Ib 和對應的 Ic 值。這個曲線通常是在固定 Vce 的條件下生成的，表示 Ic 隨 Ib 變化的關係。
	* @example 例如，可以使用 transferCurve() 方法來生成 Ic vs Ib 的曲線，這對於分析 BJT 的增益和工作點非常有用。
    */
    std::vector<Point> transferCurve() const override;              // Ic vs Ib (固定 Vce)
    
    // BJT 特有的方法
    /**
	* @brief transferCurve(double Vce) const;
	* @brief 產生 BJT 的轉移特性曲線，給定 Vce
	* @param Vce 集極-射極電壓，這個參數用於生成特定 Vce 下的 Ib vs Ic 曲線
	* @return std::vector<Point> 包含轉移特性曲線點的列表
	* @param Vce 集極-射極電壓，這個參數用於生成特定 Vce 下的 Ib vs Ic 曲線
	* @details 這個方法根據給定的集極-射極電壓 Vce 產生 BJT 的轉移特性曲線，返回一個包含 Point 結構的列表，每個 Point 包含 Ib 和對應的 Ic 值。
    *          這個曲線可以用於分析 BJT 在不同 Vce 下的增益和工作狀態，例如在主動區和飽和區之間的轉換。
    */
    std::vector<Point> transferCurve(double Vce) const;  // 給定 Vce 的轉移曲線

    /**
	* @brief inputCurve() const;
	* @brief 產生 BJT 的輸入特性曲線
	* @return std::vector<Point> 包含輸入特性曲線點的列表
	* @details 這個方法產生 BJT 的輸入特性曲線，返回一個包含 Point 結構的列表，每個 Point 包含 Vbe 和對應的 Ib 值。
    * 這個曲線表示基極-射極電壓 Vbe 與基極電流 Ib 之間的關係，通常呈現二極體特性。
    */
    std::vector<Point> inputCurve() const;               // Ib vs Vbe (輸入特性)

    // 工作點計算
    /**
	* @brief calculateQPoint(double Vcc, double Rc, double Rb) const override;
	* @brief 計算 BJT 的工作點（Bias Point）
	* @param Vcc 電源電壓
	* @param Rc 集極電阻
	* @param Rb 基極電阻
	* @return BiasPoint 包含工作點資訊的結構
	* @details 這個方法根據給定的電源電壓 Vcc、集極電阻 Rc 和基極電阻 Rb 計算 BJT 的工作點，返回一個 BiasPoint 結構，
    *          其中包含 Vce、Ic、Ib 等資訊。這個方法會考慮 BJT 的特性來確定工作點是否在主動區、飽和區或截止區，並相應地計算 Ib 和 Ic 的值。
    */
    //BiasPoint calculateQPoint(double Vcc, double Rc, double Rb) const override;

    // 找點功能
    /**
	* @brief findIbFromIc(double Ic, double Vce) const;
	* @brief findIcFromIb(double Ib, double Vce) const;
	* @brief findVceFromIc(double Ic, double Ib) const;
	* @brief 給定 Ic 和 Vce 找 Ib，給定 Ib 和 Vce 找 Ic，給定 Ic 和 Ib 找 Vce
	* @param Ic 集極電流
	* @param Ib 基極電流
	* @param Vce 集極-射極電壓
	* @return double 對應的 Ib、Ic 或 Vce 值
	* @details 這些方法提供了從已知的 Ic 和 Vce 計算對應的 Ib，從已知的 Ib 和 Vce 計算對應的 Ic，以及從已知的 Ic 和 Ib 計算對應的 Vce 的功能。
    *          這些方法可以用於分析特定工作點或特性曲線上的點，幫助使用者更深入地理解 BJT 的行為和特性。
    */
    double findIbFromIc(double Ic, double Vce) const;   // 給 Ic 找 Ib
    double findIcFromIb(double Ib, double Vce) const;   // 給 Ib 算 Ic
    double findVceFromIc(double Ic, double Ib) const;   // 給 Ic 找 Vce

    // 設定曲線取點數量
    /**
    * @brief setCurvePoints(int points);
	* @brief 設定特性曲線的取點數量
	* @param points 取點數量，必須是正整數
	* @details 這個方法用於設定在生成特性曲線時使用的取點數量，影響曲線的平滑程度和計算時間。較多的取點數量可以產生更平滑的曲線，
    但也會增加計算負擔。使用者可以根據需要調整這個參數，以在精確度和效率之間取得平衡。
    */
    void setCurvePoints(int points) { m_curvePoints = points; }

    /**
	* @brief getCurvePoints() const;
	* @brief 讀取特性曲線的取點數量
	* @Param points 取點數量，必須是正整數
	* @return int 取點數量
	* @details 這個方法用於讀取當前設定的特性曲線取點數量，幫助使用者了解生成曲線時將使用多少點來描述特性曲線。這個資訊可以用於調整取點數量以獲得更好的模擬結果。
    * 
    */
    int getCurvePoints() const { return m_curvePoints; }


    // 新增：從 CurveDrawable 繼承來的函式
    std::vector<Point> generateCurve(double inputParam) const override;

    QString deviceType() const override;

    QString inputUnit() const override;


private:
    // 模型參數
    /**
	* @brief BJT 模型參數
	* @details 這些參數是 BJT 模型的核心參數，用於描述 BJT 的電氣特性和行為。這些參數包括：
	* @param m_Beta 電流增益 (hFE)，描述集極電流與基極電流之間的增益關係
	* @param m_Vbe_on 導通電壓 (V)，描述基極-射極二極體的導通電壓
	* @param m_Vce_sat 飽和電壓 (V)，描述 BJT 飽和區的特性 
	* @param m_Va Early voltage (V)，描述 BJT 的 Early 效應，影響 Ic 與 Vce 之間的關係
	* @param m_Is 飽和電流 (A)，描述基極-射極二極體的飽和電流，影響 Ib 與 Vbe 之間的關係
	* @param m_Ic_max 最大集極電流 (A)，限制 BJT 的最大集極電流，防止超出安全範圍
	* @param m_Vce_max 最大集極-射極電壓 (V)，限制 BJT 的最大集極-射極電壓，防止超出安全範圍
	* @note 這些參數可以通過 setParameter() 方法進行設定，並且在模擬和分析 BJT 的特性曲線和工作點時使用。合理的參數設定對於獲得準確的模擬結果非常重要。
    */
    double m_Beta;       // 電流增益 (hFE)
    double m_Vbe_on;     // 導通電壓 (V)
    double m_Vce_sat;    // 飽和電壓 (V)
    double m_Va;         // Early voltage (V)
    double m_Is;         // 飽和電流 (A)
    double m_Ic_max;     // 最大集極電流 (A)
    double m_Vce_max;    // 最大集極-射極電壓 (V)

    /**
	* @brief BJT 的子類型
	* @Param m_isNPN 布林值，true 表示 NPN 型 BJT，false 表示 PNP 型 BJT
	* @details 這個變數用於區分 BJT 的兩種基本類型：NPN 和 PNP。當 m_isNPN 為 true 時，表示這是一個 NPN 型 BJT；當 m_isNPN 為 false 時，表示這是一個 PNP 型 BJT。
	* @note NPN 和 PNP 的電流方向和偏壓條件不同，因此在模擬和分析電路時需要知道具體的子類型。這個變數可以通過 setParameter() 方法進行設定，或者在構造函式中指定。
	* @note 這個變數對於生成特性曲線和計算工作點非常重要，因為 NPN 和 PNP 的特性曲線和工作點計算方式會有所不同。使用者需要根據實際使用的 BJT 型號來設定這個參數，以獲得正確的模擬結果。
    */
    bool m_isNPN;        // true: NPN, false: PNP

    int m_curvePoints;   // 曲線取點數量

    // 內部計算函式
	/**@brief calculateIc_active(double Ib, double Vce) const;
	* @brief 計算主動區的集極電流
	* @param Ib 基極電流
	* @param Vce 集極-射極電壓
	* @return double 對應的集極電流值
	* @details 這個方法用於計算 BJT 在主動區的集極電流，根據公式 Ic = Beta * Ib * (1 + Vce / Va) 計算，其中 Beta 是電流增益，Va 是 Early voltage。
               這個方法會考慮 Early 效應對集極電流的影響，並且會限制計算出的集極電流不超過設定的最大值 Ic_max。
    */
    double calculateIc_active(double Ib, double Vce) const;   // 主動區

    /**
	* @brief calculateIc_saturation(double Ib, double Vce) const;
	* @brief 計算飽和區的集極電流
	* @param Ib 基極電流
	* @param Vce 集極-射極電壓
	* @return double 對應的集極電流值
	* @details 這個方法用於計算 BJT 在飽和區的集極電流，根據簡單的線性近似計算，其中當 Vce = Vce_sat 時，Ic = Beta * Ib；當 Vce = 0 時，Ic = 0。
               這個方法會限制計算出的集極電流不超過 Beta * Ib 和設定的最大值 Ic_max。
    */
    double calculateIc_saturation(double Ib, double Vce) const; // 飽和區

    /**
	* @brief calculateVce_saturation(double Ib) const;
	* @brief 計算飽和區起始電壓
	* @param Ib 基極電流
	* @return double 飽和區起始電壓值
	* @details 這個方法用於計算 BJT 飽和區的起始電壓，簡單地返回設定的 Vce_sat 作為飽和區的起始電壓。這個值表示當 Vce 小於或等於 Vce_sat 時，BJT 進入飽和區。
    * 
    */
    double calculateVce_saturation(double Ib) const;          // 飽和區起始電壓

    /**
	* @brief calculateIbFromVbe(double Vbe) const;
	* @brief 從 Vbe 計算 Ib，從 Ib 計算 Vbe
	* @param Vbe 基極-射極電壓
	* @return double 對應的基極電流值
    * @details 這個方法用於根據基極-射極電壓 Vbe 計算對應的基極電流 Ib，使用二極體方程式近似計算，其中 Ib = Is * (exp((Vbe - Vbe_on) / 0.026) - 1)，Is 是飽和電流，Vbe_on 是導通電壓。
			   這個方法也可以用於從已知的基極電流 Ib 計算對應的基極-射極電壓 Vbe，使用反向的二極體方程式計算，其中 Vbe = Vbe_on + 0.026 * ln(Ib / Is + 1)。
    */
    double calculateIbFromVbe(double Vbe) const;              // 從 Vbe 算 Ib

	/**
    * @brief calculateVbeFromIb(double Ib) const; 
	* @brief 從 Ib 計算 Vbe
	* @param Ib 基極電流
	* @return double 對應的基極-射極電壓值
    * @details 這個方法用於根據基極電流 Ib 計算對應的基極-射極電壓 Vbe，使用二極體方程式的反向計算，其中 Vbe = Vbe_on + 0.026 * ln(Ib / Is + 1)，Is 是飽和電流，Vbe_on 是導通電壓。
			   這個方法可以幫助使用者從已知的基極電流值推算出對應的基極-射極電壓，這在分析 BJT 的輸入特性和工作點時非常有用。
    */
    double calculateVbeFromIb(double Ib) const;               // 從 Ib 算 Vbe
};

#endif // BJT_H
