#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
public:
    virtual ~Logger() = default;

    // 純虛擬介面，由子類別決定要記錄什麼
    virtual void log(const std::string& message) = 0;
    virtual void logError(const std::string& error) = 0;

    // 設定/取得檔名
    virtual void setFilename(const std::string& filename) = 0;
    virtual std::string getFilename() const = 0;
};

#endif
