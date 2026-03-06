// CalcLogger.h
#pragma once

#include <string>
#include <vector>
#include "algorithm.h" // 必須包含，因為需要 Calculator::Token 和 Calculator::CalcResult

class CalcLogger {
public:

    static std::string generateTimestampFilename();


    /**
     * @brief 將計算過程與結果寫入文字檔
     * @param expr 使用者輸入的原始算式
     * @param postfix 經過 Shunting Yard 處理後的後序表達式
     * @param res 計算結果
     */

     // 修改為不需從外部傳入檔名，或讓檔名有預設值
    static void writeLog(const std::string& expr,
        const std::vector<Calculator::Token>& postfix,
        const Calculator::CalcResult& res);

    // 新增的函式（專門給錄製功能使用，接收外部指定的檔名）
    static void writeLog(const std::string& filename,
                         const std::string& expr,
                         const std::vector<Calculator::Token>& postfix,
                         const Calculator::CalcResult& res);

};
