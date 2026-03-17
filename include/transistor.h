#ifndef TRANSISTOR_H
#define TRANSISTOR_H

/**
* @file transistor.h
* @brief
*   @Transistor 類別，作為 BJT 和 MOSFET 的基類，定義了晶體管的基本接口和功能。
* @details
* @Transistor 類別是一個抽象基類，定義了晶體管的基本接口和功能。BJT 和 MOSFET 類別將繼承自 Transistor 並實現其純虛擬方法，以提供具體的行為和特性。
*
*
* @author Markscat
* @copyright Copyright (c) 2024 Markscat. All rights reserved.
* 本專案使用 GPLv3 授權，詳情請見 LICENSE 檔案。
*
* @since 2024-06
* @see Transistor
* @see BJT
* */

#include <string>
#include <vector>


/**
* @brief Point 結構，用於表示特性曲線上的點
* @param x 特性曲線上的 x 值，通常表示電壓（例如 Vce 或 Vds）
* @param y 特性曲線上的 y 值，通常表示電流（例如 Ic 或 Id）
* @return Point 結構，包含 x 和 y 的值
* @details Point 結構包含兩個 double 類型的成員 x 和 y，分別表示特性曲線上的 x 和 y 值。這個結構用於存儲特性曲線上的點，
                 例如在輸出特性曲線中，x 可能表示 Vce 或 Vds，而 y 可能表示 Ic 或 Id。
				 使用 Point 結構可以方便地管理和傳遞特性曲線上的數據，並且在生成特性曲線時可以使用 std::vector<Point> 來存儲多個點，形成完整的曲線。

*/
struct Point {
    double x;
    double y;

    Point(double x = 0, double y = 0) : x(x), y(y) {}
};


/**
* @brief BiasPoint 結構，用於表示晶體管的工作點（Q Point）
* @param Vce BJT 的集電極-發射極電壓，或 MOSFET 的汲極-源極電壓
* @param Ic BJT 的集電極電流，或 MOSFET 的漏極電流
* @param Ib BJT 的基極電流，MOSFET 沒有基極電流，這裡可以設為 0
* @param Vds MOSFET 的汲極-源極電壓，BJT 沒有這個參數，可以設為 0
* @param Id MOSFET 的漏極電流，BJT 沒有這個參數，可以設為 0
* @param Vgs MOSFET 的閘極-源極電壓，BJT 沒有這個參數，可以設為 0
* @param valid 工作點是否有效，當計算失敗或參數不合理時設為 false
* @return BiasPoint 結構，包含工作點的電壓和電流值，以及有效性標誌
* @details BiasPoint 結構包含了晶體管的工作點（Q Point）相關的電壓和電流值，以及一個有效性標誌。對於 BJT，Vce、Ic 和 Ib 是主要的參數，
                     而對於 MOSFET，Vds、Id 和 Vgs 是主要的參數。當計算工作點時，如果參數不合理或計算失敗，valid 會被設置為 false，以便使用者知道工作點無效。
*/
struct BiasPoint {
    //bjt
    double Vce;     // 8 bytes
    double Ic;      // 8 bytes
    double Ib;      // 8 bytes

    //mos
    double Vds;     // 8 bytes
    double Id;      // 8 bytes
    double Vgs;     // 8 bytes
    bool valid;     // 1 byte (+ padding)

    BiasPoint() : Vce(0), Ic(0), Ib(0), Vds(0), Id(0), Vgs(0), valid(false) {}
};

enum class BiasConfig {
    FixedVgs,        // MOSFET: 固定 Vgs
    FixedIb,         // BJT: 固定 Ib
    VoltageDivider,  // 電阻分壓
    CurrentSource,   // 電流源偏壓
    FourResistor,    // BJT 四電阻偏壓
    Unknown,         //未知選項
    // ... 其他組態
};


struct BiasParams {
    BiasConfig config;
    double Vcc;      // 電源電壓

    double Rc;       // 集極(C)電阻 (Bjt用)
    double Re;       // 射極(E)電阻 (Bjt用)
    double Rd;       // 汲極(D)電阻（MOSFET用）
    double Rs;       // 源極(S)電阻（MOSFET用）
    //Rb or Rg可以由R1或R2來決定
    double R1, R2;   // 分壓電阻（可選）
    double Ib;       // 基極電流（BJT用，可選）
    double Vgs;      // 閘極電壓（MOSFET用，可選）
    BiasParams() : config(BiasConfig::Unknown), Vcc(0), Rc(0),Re(0),Rd(0),Rs(0),
        R1(0), R2(0), Ib(0), Vgs(0) {}
};

class Transistor
{
public:

    virtual ~Transistor() = default;

    // 型號與類型
    /**
	* @brief type() const = 0;
	* @brief 回傳元件類型
	* @return std::string 元件類型，例如 "BJT" 或 "MOSFET"
    * @details 這個方法是 Transistor 類別的純虛擬方法，BJT 和 MOSFET 類別必須實作它以回傳正確的類型資訊。
               這個方法在使用者界面或其他需要識別元件類型的地方非常重要，可以幫助區分不同類型的晶體管（例如 BJT 和 MOSFET）。
			   這個方法不接受任何參數，直接回傳固定的字串 "BJT" 或 "MOSFET"，表示具體的晶體管類型。
    */
    virtual std::string type() const = 0;  // "BJT" 或 "MOSFET"

    // 參數存取（用字串是因為每家參數名稱可能不同）
    /**
	* @brief setParameter(const std::string& name, double value) = 0;
	* @brief 設定晶體管的參數
	* @return void
	* @param name 參數名稱，例如 "Beta"、"Vth"、"Kn" 等等，具體參數名稱取決於晶體管類型
	* @param value 參數值，必須是合理的數值，例如 Beta 必須大於 0，Vth 必須大於 0 等等
    */
    virtual void setParameter(const std::string& name, double value) = 0;

    /**
	* @brief getParameter(const std::string& name) const = 0;
	* @brief 讀取晶體管的參數值
	* @return double 參數值，如果參數名稱無效則回傳 NaN@
	* @param name 參數名稱，例如 "Beta"、"Vth"、"Kn" 等等，具體參數名稱取決於晶體管類型
    * @details 根據參數名稱讀取對應的內部變數值，例如 "Beta" 對應 m_Beta，"Vth" 對應 m_Vth 等等。這個方法允許使用者靈活地讀取晶體管的各種參數，以便在模擬和分析中使用。
			   如果參數名稱無效，則回傳 NaN（Not a Number），表示沒有對應的參數。使用者可以通過這個方法來獲取晶體管的具體參數值，以便在模擬和分析中使用。
    */
    virtual double getParameter(const std::string& name) const = 0;

    /**
	* @brief paramList() const = 0;
	* @brief 取得參數列表
	* @return std::vector<std::string> 包含所有可用參數名稱的列表
    * @details 這個方法回傳一個包含所有可用參數名稱的列表，這些參數名稱可以用於 setParameter() 和 getParameter() 方法。
			   這個列表有助於使用者了解可以設定和讀取哪些參數，以便在模擬和分析中使用正確的參數名稱

    */
    virtual std::vector<std::string> paramList() const = 0;

    // 特性曲線
    /**
	* @brief outputCurve(double inputParam) const = 0;
	* @brief 產生輸出特性曲線
	* @param inputParam 輸入參數，對於 BJT 可能是基極電流 Ib，對於 MOSFET 可能是閘極-源極電壓 Vgs
	* @return std::vector<Point> 包含特性曲線上點的列表，每個 Point 包含 x 和 y 的值，分別表示電壓和電流
    * @details 這個方法是 Transistor 類別的純虛擬方法，BJT 和 MOSFET 類別必須實作它以回傳特定輸入參數下的輸出特性曲線。對於 BJT，inputParam 可能是基極電流 Ib，方法會生成對應的 Ic vs Vce 曲線；對於 MOSFET，inputParam 可能是閘極-源極電壓 Vgs，方法會生成對應的 Id vs Vds 曲線。
			   使用者可以通過這個方法來獲取在特定輸入參數條件下的輸出特性曲線，以便在模擬和分析中使用。這些曲線對於理解晶體管的行為和設計電路非常重要。
    */
    virtual std::vector<Point> outputCurve(double inputParam) const = 0;

    /**
	* @brief transferCurve() const = 0;
	* @brief 產生轉移特性曲線
	* @return std::vector<Point> 包含轉移特性曲線上點的列表，每個 Point 包含 x 和 y 的值，分別表示輸入電壓和輸出電流
    * @details 這個方法是 Transistor 類別的純虛擬方法，BJT 和 MOSFET 類別必須實作它以回傳轉移特性曲線。對於 BJT，轉移特性曲線可能是 Ic vs Ib；對於 MOSFET，轉移特性曲線可能是 Id vs Vgs。
			   使用者可以通過這個方法來獲取晶體管的轉移特性曲線，以便在模擬和分析中使用。轉移特性曲線對於理解晶體管的增益和開關行為非常重要。
    */
    virtual std::vector<Point> transferCurve() const = 0;

    // 工作點
    /**
	* @brief calculateQPoint(double Vcc, double Rc, double Rb) const = 0;
	* @brief 計算工作點（Bias Point）
	* @param Vcc 電源電壓
	* @param Rc 集極電阻（對於 BJT）或汲極電阻（對於 MOSFET）
	* @param Rb 基極電阻（對於 BJT）或閘極電阻（對於 MOSFET）
	* @return BiasPoint 包含工作點資訊的結構，對於 BJT 包含 Vce、Ic、Ib，對於 MOSFET 包含 Vds、Id、Vgs，以及一個有效性標誌2
    * @details 這個方法是 Transistor 類別的純虛擬方法，BJT 和 MOSFET 類別必須實作它以計算工作點。對於 BJT，方法會根據給定的電源電壓 Vcc、集極電阻 Rc 和基極電阻 Rb 計算 Vce、Ic 和 Ib；對於 MOSFET，方法會根據給定的電源電壓 Vcc、汲極電阻 Rc 和閘極電阻 Rb 計算 Vds、Id 和 Vgs。
			   使用者可以通過這個方法來獲取晶體管在特定電路條件下的工作點資訊，以便在模擬和分析中使用。工作點資訊對於理解晶體管的運作狀態和設計穩定的放大器或開關非常重要。
    */

    virtual BiasPoint calculateQPoint(const BiasParams& params) const = 0;


    //virtual BiasPoint calculateQPoint(double Vcc, double Rc, double Rb) const = 0;

    //virtual BiasPoint calculateQPoint(double Vdd, double Rd,
    //                                  double biasParam,
    //                                  BiasConfig config) const=0;
};


#endif // TRANSISTOR_H
