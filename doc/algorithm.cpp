//#include "pch.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <stack>
#include <utility> // for std::pair 
#include <cstdint> // for uint64_t2.
#include <unordered_map>

#include "..\include\algorithm.h"
#include "..\include\Trigonometry.h"
#include "..\include\CalcLogger.h"


// 為了讓代碼更乾淨，我們在 .cpp 頂部定義一些短名稱
using Result = Calculator::CalcResult;
using Status = Calculator::CalcStatus;

Calculator::Calculator() noexcept : lastStatus(CalcStatus::Success) {}

// 判斷是否為右結合律 (目前只有指數 '^' 是右結合)
bool Calculator::isRightAssociative(char op) const noexcept {
    return op == '^';
}


std::vector<Calculator::Token> Calculator::tokenize(std::string expr) noexcept {
    std::vector<Token> tokens;
    bool expectOperand = true; // 負號判定標記

    for (size_t i = 0; i < expr.length(); ++i) {
        char ch = expr[i];
        if (isspace(ch)) continue;

        // --- 根據「詞類」派發任務 ---
        // A. 如果是數字類 (數字或小數點)
        if (isdigit(ch) || ch == '.') {
            tokens.push_back(scanNumber(expr, i));
            expectOperand = false;
        }
        // B. 如果是字母類 (常數或函數名)
        else if (isalpha(ch)) {
            tokens.push_back(scanWord(expr, i, expectOperand));
            // 注意：scanWord 內部會根據是「常數」還是「函數」來更新 expectOperand
        }
        // C. 如果是括號
        else if (ch == '(' || ch == ')') {
            tokens.push_back(scanParenthesis(ch, expectOperand));
        }
        // D. 剩下的是運算符 (包含負號)
        else {
            tokens.push_back(scanOperator(ch, expectOperand));
        }
    }
    return tokens;
}

// 數字專家
Calculator::Token Calculator::scanNumber(const std::string& expr, size_t& i) {
    std::string buf;
    buf += expr[i];
    while (i + 1 < expr.length() && (isdigit(expr[i + 1]) || expr[i + 1] == '.')) {
        buf += expr[++i];
    }
    return { TokenType::Number, std::stod(buf), '\0', 0, "" };
}

/*
bool isAlphabetic(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}
*/
// 單字專家 (常數/函數)
Calculator::Token Calculator::scanWord(const std::string& expr, size_t& i, bool& expectOp) {
    std::string word;
    while (i < expr.length() && isalpha(expr[i])) {
        word += expr[i++];
    }
    i--;

    // 轉小寫副本，方便比較與傳回給 funcTable（funcTable 使用小寫鍵）
    std::string lower = word;
    for (auto& c : lower) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

    double val = 0.0;
    // 特殊情況：單字 's' 應當視為二元運算子 (a s b) 表示 a 次方根運算符
    if (lower.size() == 1 && lower[0] == 's') {
        expectOp = true; // 運算符之後期待數字
        return { TokenType::Operator, 0.0, 's', getPrecedence('s'), "" };
    }

    // 把 "mod" 視為二元運算子 (對應到內部使用的 '%' 符號)
    if (lower == "mod") {
        expectOp = true;
        return { TokenType::Operator, 0.0, '%', getPrecedence('%'), "" };
    }

    // 常數辨識仍對大小寫敏感，但先嘗試原字串，再嘗試大寫（方便輸入 PI, pi, Pi）
    if (tryGetConstant(word, val) || tryGetConstant(lower, val)) {
        expectOp = false;
        //return { TokenType::Number, val, '\0', 0, word };
        return { TokenType::Constant, val, '\0', 0, word };
    }
    // 3. 重點：分辨是「函數」還是「變數」
    // 技巧：如果單字長度大於 1 (例如 sin, abs, log)，通常是函數
    // 如果單字長度只有 1 (例如 x, y, a)，通常是變數
    if (lower.length() > 1) {
        expectOp = true;
        return { TokenType::Function, 0.0, '\0', 4, lower };
    }
    else {
        expectOp = false;
        return { TokenType::Constant, 0.0, '\0', 0, word }; // 這就是你的變數 x
    }

    // 函數：回傳小寫名稱以符合 funcTable 的鍵值（例如 "Sin" -> "sin"）
    //expectOp = true; // 函數如 sin( 後面期待數字
    //return { TokenType::Function, 0.0, '\0', 4, lower };
}
// 符號專家 (處理負號與運算符)
Calculator::Token Calculator::scanOperator(char ch, bool& expectOp) {
    if (ch == '-' && expectOp) {
        return { TokenType::Function, 0.0, '\0', 4, "negate" };
    }
    expectOp = true;
    return { TokenType::Operator, 0.0, ch, getPrecedence(ch), "" };
}

Calculator::Token Calculator::scanParenthesis(char ch, bool& expectOperand) noexcept {
    if (ch == '(') {
        expectOperand = true; // 左括號後，期待數字或負號
        return { TokenType::Parenthesis, 0.0, '(', 0, "" };
    }
    else {
        expectOperand = false; // 右括號後，期待運算符
        return { TokenType::Parenthesis, 0.0, ')', 0, "" };
    }
}

/**
* @brief getPrecedence 取得運算符的優先級
 * @param op 運算符字元
 * @return 優先級數值 (數字為0, + -為1, * /為2, ^ s為3)
 * @note 括號的優先級特例：在 Shunting Yard 演算法中，括號不直接參與比較，所以給予最低優先級 0
*/
int Calculator::getPrecedence(char op) const noexcept {
    if (op == '(' || op == ')') return 0; // 括號權力最低
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    if (op == '^' || op == 's') return 3;
    return 0;
}

/**
* @brief 中序轉後序的核心函式：實作 Shunting Yard 演算法
 * @param tokens 已經切好的 Token 清單 (中序)
 * @return 轉換完成的 Token 清單 (後序)
 * @note 這裡不處理函數的參數分隔（逗號），因為目前的函數都是一元的
 * @param tokens 已經切好的 Token 清單 (中序)
 * 
 */

std::vector<Calculator::Token> Calculator::shunt(const std::vector<Token>& tokens) noexcept {
    std::vector<Token> output; // 輸出佇列
    std::stack<Token> opStack; // 符號維修站

    for (const auto& token : tokens) {
        //if (token.type == TokenType::Number) {
        if (token.type == TokenType::Number || token.type == TokenType::Constant) {
            output.push_back(token); // 數字直接通過
        }
        // 如果是函數 (abs, sin, negate...) 直接進符號站
        else if (token.type == TokenType::Function) {
            opStack.push(token);
        }
        else if (token.symbol == '(') {
            // 規則 1：左括號直接進站
            opStack.push(token);
        }
        else if (token.symbol == ')') {
            // 規則 2：右括號出現，把站內符號通通倒出來，直到遇到左括號
            while (!opStack.empty() && opStack.top().symbol != '(') {
                output.push_back(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop(); // 把左括號從站內丟棄，不進入輸出隊列
            // 如果括號前是函數，也要彈出
            if (!opStack.empty() && opStack.top().type == TokenType::Function) {
                output.push_back(opStack.top());
                opStack.pop();
            }
        }
        else {
            while (!opStack.empty() && opStack.top().symbol != '(') {
                // --- 修正 2：處理右結合律 (讓 ^ 不會被立刻踢出來) ---
                bool isRight = isRightAssociative(token.symbol);
                if ((!isRight && opStack.top().precedence >= token.precedence) ||
                    (isRight && opStack.top().precedence > token.precedence)) {
                    output.push_back(opStack.top());
                    opStack.pop();
                }
                else {
                    break;
                }
            }
            opStack.push(token); // 自己進站等待
        }
    }
    // 最後把維修站裡剩餘的符號全部清空到輸出佇列
    while (!opStack.empty()) {
        output.push_back(opStack.top());
        opStack.pop();
    }
    return output;
}

bool Calculator::tryExecuteFunction(const std::string& name, double x, double& outRes, Calculator::CalcStatus& outStatus) const noexcept {
    // 定義一個處理函式的表格
    // 鍵：字串 (如 "sin")
    // 值：一個接受 double 並回傳結果與狀態的動作
    using FuncAction = std::function<void(double, double&, Status&)>;

    static const std::unordered_map<std::string, FuncAction> funcTable = {
        // 新增一元負號支援
        {"negate", [](double v, double& r, Status& s) { r = -v; s = Status::Success; }},

        {"abs", [](double v, double& r, Status& s) { r = std::abs(v); s = Status::Success; }},

        {"log", [](double v, double& r, Status& s) {
            if (v <= 0) s = Status::UnknownError;
            else { r = std::log10(v); s = Status::Success; }
        }},

        {"ln", [](double v, double& r, Status& s) {
            if (v <= 0) s = Status::UnknownError;
            else { r = std::log(v); s = Status::Success; }
        }},

        {"sin", [this](double v, double& r, Status& s) {
            r = trig.sin(v); s = Status::Success;
        }},

        {"cos", [this](double v, double& r, Status& s) {
            r = trig.cos(v); s = Status::Success;
        }},

        {"tan", [this](double v, double& r, Status& s) {
            r = trig.tan(v); s = Status::Success;
        }},

        { "cot", [this](double v, double& r, Status& s) { 
            r = trig.cot(v); s = Status::Success; 
        }},

        {"sec", [this](double v, double& r, Status& s) {
            r = trig.sec(v); s = Status::Success; 
        }},

        {"csc", [this](double v, double& r, Status& s) {
            r = trig.csc(v); s = Status::Success; 
        }},

        {"asin", [this](double v, double& r, Status& s) {
          if (v < -1.0 || v > 1.0) s = Status::UnknownError;
          else{r = trig.asin(v); s = Status::Success;    } 
        }},
        {"acos", [this](double v, double& r, Status& s) {
           if (v < -1.0 || v > 1.0) s = Status::UnknownError;
           else {r = trig.acos(v); s = Status::Success;}
        }},
        {"atan", [this](double v, double& r, Status& s) {
           if (v < -1.0 || v > 1.0) s = Status::UnknownError;
           else{ r = trig.atan(v); s = Status::Success; }
        }},
        {"sqrt", [](double v, double& r, Status& s) {
            if (v < 0) {
            s = Status::NegativeRoot;
            }
            else {
                r = std::sqrt(v);
                s = Status::Success;
            }
        }},
        {"exp", [](double v, double& r, Status& s) {
            r = std::exp(v);
            s = Status::Success;
        }},
        {"sinh", [](double v, double& r, Status& s) {
            r = std::sinh(v); s = Status::Success;
        }},
        {"cosh", [](double v, double& r, Status& s) {
            r = std::cosh(v); s = Status::Success;
        }},
        {"tanh", [](double v, double& r, Status& s) {
            r = std::tanh(v); s = Status::Success;
        }},
        {"asinh", [](double v, double& r, Status& s) {
            r = std::asinh(v); s = Status::Success;
        }},
        {"acosh", [](double v, double& r, Status& s) {
            if (v < 1.0) s = Status::UnknownError; // acosh 定義域限制
            else { r = std::acosh(v); s = Status::Success; }
        }},
        {"atanh", [](double v, double& r, Status& s) {
            if (v <= -1.0 || v >= 1.0) s = Status::UnknownError; // atanh 定義域限制
            else { r = std::atanh(v); s = Status::Success; }
        }},
        {"mod", [](double v, double& r, Status& s) {
            // 這裡可以設計成一元函數，或保留二元運算符
            // 如果要支援 mod(x,y)，就要改 evaluate 支援二元函數
            r = std::fmod(v, 1.0); // 只是示範
            s = Status::Success;
        }},

        {"fact", [](double v, double& r, Status& s) {
            if (v < 0) {
                s = Status::UnknownError; // 階乘不定義在負數
                return;
            }
            // 檢查是否為整數
            if (std::floor(v) == v && v <= 20) {
                unsigned long long result = 1;
                for (int i = 2; i <= static_cast<int>(v); ++i) {
                    result *= i;
                }
                r = static_cast<double>(result);
                s = Status::Success;
            } else {
                // 切換到 Gamma 函數近似
                r = std::tgamma(v + 1);
                s = Status::Success;
            }
            }},
        // 以後要加 tan, sqrt, exp... 就在這裡加一行就好！
    };

    auto it = funcTable.find(name);
    if (it != funcTable.end()) {
        it->second(x, outRes, outStatus);
        return true;
    }
    return false;
}


Calculator::CalcResult Calculator::evaluate(const std::vector<Token>& postfix) noexcept {
    
    if (postfix.empty()) return { 0.0, CalcStatus::Success };

    std::stack<double> valStack; // 用來存放數字的堆疊

    for (const auto& token : postfix) {
        if (token.type == TokenType::Number) {
            valStack.push(token.value); // 遇到數字，丟進堆疊
        }
        else if (token.type == TokenType::Constant) {
            // --- 處理動態變數 (Constant) ---
            double latestVal = 0.0;
            // 每次計算都現場查表，這會抓到微積分 setVariable 設定的最新 x 值
            if (tryGetConstant(token.name, latestVal)) {
                valStack.push(latestVal);
            }
            else {
                // 如果真的查不到，就用 Token 裡面存的舊值 (保險用)
                valStack.push(token.value);
            }
        }
        else if (token.type == TokenType::Operator) {
            // 遇到符號，抓出最後兩個數字來運算
            if (valStack.size() < 2) return { 0.0, CalcStatus::InvalidOperator };

            double b = valStack.top(); valStack.pop();
            double a = valStack.top(); valStack.pop();

            // 呼叫你原本寫好的核心 calculate 函式
            CalcResult res = calculate(a, token.symbol, b);

            if (res.status != CalcStatus::Success) return res;
            valStack.push(res.value); // 把算完的結果丟回堆疊，供下一個符號使用
        }

        // 3. 處理一元函數 (abs, log, ln, 以及未來的 sin/cos)
        else if (token.type == TokenType::Function) {
            // 安全檢查：一元函數需要至少 1 個數字
            if (valStack.empty()) return { 0.0, CalcStatus::InvalidOperator };
            double x = valStack.top(); valStack.pop();

            double resultVal = 0.0;
            Status resStatus = Status::Success;

            // 一行搞定：去表裡找這個名字，找到了就執行
            if (tryExecuteFunction(token.name, x, resultVal, resStatus)) {
                if (resStatus != Status::Success) return { 0.0, resStatus };
                // 檢查函數結果是否溢位 (在 push 之前檢查)
                if (std::isinf(resultVal) || std::isnan(resultVal)) {
                    return { 0.0, CalcStatus::Overflow };
                }
                valStack.push(resultVal);
            }
            else {
                return { 0.0, CalcStatus::InvalidOperator };
            }
        }
    }
    // 最終檢查：算完後，堆疊裡必須「剛好剩下一個」數字
    if (valStack.size() != 1) {
        return { 0.0, CalcStatus::InvalidOperator };
    }
    return { valStack.top(), CalcStatus::Success };
}

// --- 總入口：串聯三個階段 ---
Calculator::CalcResult Calculator::execute(std::string expression) noexcept {
    // 1. 分詞
    auto tokens = tokenize(expression);
    if (tokens.empty()) return { 0.0, CalcStatus::Success };

    // 2. 重排 (中序轉後序)
    auto postfix = shunt(tokens);

    // 3. 計算
    CalcResult res = evaluate(postfix);

    // 修改這裡：
    if (this->isLogging && !currentSessionFile.empty()) {
        // 呼叫帶有 4 個參數的新版本 (filename, expr, postfix, res)
        // 這樣它就會一直寫入 currentSessionFile 指向的那個檔案
        CalcLogger::writeLog(this->currentSessionFile, expression, postfix, res);
    }

    /*
    // 如果正在錄製中，就持續「附加」到同一個檔案
    if (this->isLogging && !currentSessionFile.empty()) {
        CalcLogger::writeLog(expression, postfix, res);
    }*/
    
    lastStatus = res.status;
    return res;
}

//把守衛程式寫在函數裡頭,我個人覺得這樣比較容易讀
Calculator::CalcResult Calculator::calculateSequence(DoubleList nums, CharList ops) noexcept {
    // --- 所有的判斷都進來函數後才做 ---
    // 1. 基本安全檢查
    if (nums.empty()) return { 0.0, CalcStatus::Success };
    if (ops.empty())  return { nums[0], CalcStatus::Success };
    if (nums.size() != ops.size() + 1) {
        lastStatus = CalcStatus::InvalidOperator; // 數量不匹配
        return { 0.0, lastStatus };
    }
    // 2. 初始化結果
    CalcResult result = { 0.0, CalcStatus::Success };
    // 3. 判斷結合律並直接計算
    // 如果第一個符號是 '^'，就走「右結合」邏輯
    if (ops[0] == '^') {
        double currentVal = nums.back();
        for (int i = (int)ops.size() - 1; i >= 0; --i) {
            result = calculate(nums[i], ops[i], currentVal);
            currentVal = result.value;
            if (result.status != CalcStatus::Success) break;
        }
    }
    // 否則，一律走「左結合」邏輯 (加減乘除)
    else {
        result.value = nums[0];
        for (size_t i = 0; i < ops.size(); ++i) {
            result = calculate(result.value, ops[i], nums[i + 1]);
            if (result.status != CalcStatus::Success) break;
        }
    }
    return result;
}

Calculator::CalcResult Calculator::calculate(double a, char op, double b) noexcept {
    //成員函式 "Calculator::calculate" 不能在其類別之外重新宣告
    CalcResult result = { 0.0, CalcStatus::Success };
    switch (op) {
    case '+':
        result.value = a + b;
        break;
    case '-':
        result.value = a - b;
        break;
    case '*':
        result.value = a * b;
        break;
    case '/':
        if (b == 0.0) {
            result.status = CalcStatus::DivideByZero;
            result.value = 0.0;
        } else {
            result.value = a / b;
        }
        break;
        // --- 新增：指數運算 (a 的 b 次方) ---
    case '^':
        result.value = std::pow(a, b);
        break;

    // --- 新增：方根運算 (b 的 a 次方根) ---
    // 例如 a=2, b=16，結果就是 4
    case 's':
        // a 是根次 (例如 2 s 16 表示 2 次方根 16 -> 4)
        // 若被設計為 a = 根次, b = 被開之數
        if (b < 0 && ((int)a % 2) == 0) {
            // 偶數次根的被開數為負數，無解 (實數域)
            result.status = CalcStatus::NegativeRoot;
        }
        else {
            // 開 a 次方根等於 b^(1/a)
            result.value = std::pow(b, 1.0 / a);
        }
        break;
    case '%': // 整數取餘數 
        result.value = std::fmod(a, b); 
        break;
    default:
        result.status = CalcStatus::InvalidOperator;
        result.value = 0.0;
        break;
    }
    // 檢查運算結果是否溢位 (太大或太小到電腦無法表示)
    if (std::isinf(result.value)) {
        result.status = CalcStatus::Overflow;
    }
    lastStatus = result.status; // 更新最後一次狀態
    return result;
}

// 輔助函式：將字串轉為常數數值
// 使用 std::optional 或回傳 bool 來判斷是否找到
bool Calculator::tryGetConstant(const std::string& name, double& outValue) noexcept {
    using namespace CalcConstants;

    auto itVar = variables.find(name);
    //variables 未定義
    if (itVar != variables.end()) {
        outValue = itVar->second;
        return true;
    }

    // 建立一個靜態的查表 (只會初始化一次)
    static const std::unordered_map<std::string, double> constTable = {
        // 數學
        {"PI", PI}, {"Expr", expr}, {"SQRT2", SQRT2},

        // 物理 (Physics)
        {"Planck", Physics::Planck},
        {"LightSpeed", Physics::LightSpeed},
        {"Gravity", Physics::Gravity},
        {"ElementaryCharge", Physics::ElementaryCharge},

        // 化學 (Chemistry)
        {"Avogadro", Chemistry::Avogadro},
        {"GasR", Chemistry::GasConstant}, // 簡化名稱方便輸入
        {"Boltzmann", Chemistry::Boltzmann},
        {"Faraday", Chemistry::Faraday},
        {"AMU", Chemistry::AtomicMassUnit},

        // 統計 (Statistics)
        {"EulerGamma", Statistics::EulerGamma},
        {"InvSqrt2PI", Statistics::InvSqrt2PI},
        {"LN2", Statistics::LN2},
        {"LN10", Statistics::LN10}
    };

    auto it = constTable.find(name);
    if (it != constTable.end()) {
        outValue = it->second;
        return true;
    }
    return false;
}

// Fast Doubling 演算法
std::pair<long long, long long> Calculator::fibPair(long long n) {
    if (n < 0) {
        auto p = fibPair(-n); 
        long long fn = ((-n) % 2 == 0) ? -p.first : p.first;
        long long fn1 = (((-n - 1) % 2 == 0) ? -fibPair(-n - 1).first : fibPair(-n - 1).first); // F(-n+1)
        return { fn, fn1 };
    }
    else {
        if (n == 0) return { 0, 1 };
        auto p = fibPair(n >> 1);
        long long a = p.first;  // F(k)
        long long b = p.second; // F(k+1)
        long long c = a * (2 * b - a);   // F(2k)
        long long d = a * a + b * b;     // F(2k+1)
        if (n & 1) return { d, c + d };
        else return { c, d };
    }
}

// 封裝成單一函數
long long Calculator::fibonacci(long long n) {
    return fibPair(n).first;
}

std::vector<long long> Calculator::fibonacciSeries(long long n) {
    std::vector<long long> seq;
    if (n >= 0) {
        seq.reserve(n + 1);
        for (long long i = 0; i <= n; ++i) {
            seq.push_back(fibPair(i).first);
        }
    }
    else {
        seq.reserve(-n + 1);
        for (long long i = n; i <= 0; ++i) {
            seq.push_back(fibPair(i).first);
        }
    }
    return seq;
}

/**
 * setVariable的設計是:
 * 提供一個簡單的介面讓使用者可以在計算機中定義自己的變數（例如 x=5），
*/
void Calculator::setVariable(const std::string& name, double value) noexcept {
    variables[name] = value;
}

/**
 *  @brief void clearVariables() noexcept;
 *   這個函式的設計是提供一個簡單的介面讓使用者可以清除所有已定義的變數，通常在重設計算機或開始新的計算時使用。
*/
void Calculator::clearVariables() noexcept {
    variables.clear();
}
/**
 * @brief std::vector<Calculator::Token> parseToPostfix(std::string expression) noexcept;
 * 負責把輸入字串轉成後序表達式 (RPN)，
 * executePostfix 負責直接執行已經是後序表達式的 Token 列表。
 * 這樣的分工讓微積分模組可以先呼叫 parseToPostfix 得到 RPN，
 * 然後重複呼叫 executePostfix 來計算不同的 x 值，而不需要每次都重新解析輸入字串。
*/

// 取得解析後的後序表達式 (RPN)，供微積分重複使用
std::vector<Calculator::Token> Calculator::parseToPostfix(std::string expression) noexcept {
    return shunt(tokenize(expression));
}

/**
*   executePostfix 的設計是：它接受一個已經是後序表達式的 Token 列表，
*   直接呼叫 evaluate 來計算結果。這樣的設計讓微積分模組可以先呼叫 parseToPostfix 得到 RPN，
*   然後重複呼叫 executePostfix 來計算不同的 x 值，而不需要每次都重新解析輸入字串。
*/
Calculator::CalcResult Calculator::executePostfix(const std::vector<Token>& postfix) noexcept {
    CalcResult res = evaluate(postfix);
    lastStatus = res.status;
    return res;
}

const char* Calculator::statusToText(Calculator::CalcStatus s) noexcept {
    switch (s) {
    case Calculator::CalcStatus::Success:         return u8"計算成功";
    case Calculator::CalcStatus::DivideByZero:    return u8"錯誤：不能除以零";
    case Calculator::CalcStatus::NegativeRoot:    return u8"錯誤：實數域中,負數無法開根號";
    case Calculator::CalcStatus::InvalidOperator: return u8"錯誤：無效的運算符";
    case Calculator::CalcStatus::Overflow:        return u8"錯誤：結果不是有限數 (溢位或 NaN)";
    default:                                      return u8"發生未知錯誤";
    }
}

void Calculator::startLogging() {
    // 只有在呼叫此函式時，才會正確產生帶有時間戳記的檔名
    this->currentSessionFile = CalcLogger::generateTimestampFilename();
    this->isLogging = true;
}

void Calculator::stopLogging() {
    this->isLogging = false;
    this->currentSessionFile = ""; // 清空檔名，結束會話
}


Calculator::CalcStatus Calculator::getLastStatus() const noexcept {
    return lastStatus;
}
