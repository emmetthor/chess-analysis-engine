#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

enum class DebugLevel { INFO, DEBUG, WARN, ERROR };
enum class DebugCategory { TT, MOVE, QS, EVAL, BOARD, SEARCH };

// ---- 控制開關 ----
// 開發版定義 ENGINE_DEBUG
// 正式版未定義
#ifdef ENGINE_DEBUG
    #define DEBUG_ENABLED 1
#else
    #define DEBUG_ENABLED 0
#endif

class Debug {
public:
    static constexpr bool enabled = DEBUG_ENABLED;  // compile-time 控制
    static DebugLevel level;
    static std::ofstream logFile;

    // 初始化，可選檔案輸出
    static void init(const std::string &file = "") {
#if DEBUG_ENABLED
        if (!file.empty()) logFile.open(file);
#endif
    }

    // 關閉檔案
    static void shutdown() {
#if DEBUG_ENABLED
        if (logFile.is_open()) logFile.close();
#endif
    }

    // 主要 log 函數
    static void log(DebugCategory cat, DebugLevel msgLevel, const std::string &msg) {
#if DEBUG_ENABLED
        if (msgLevel < level) return;

        std::ostringstream oss;
        oss << "[" << categoryToString(cat) << "] " << msg;

        std::cout << oss.str() << std::endl;
        if (logFile.is_open()) logFile << oss.str() << std::endl;
#endif
    }

    // log 任意變數
    template<typename T>
    static void logVar(DebugCategory cat, DebugLevel msgLevel, const std::string &name, const T &val) {
#if DEBUG_ENABLED
        std::ostringstream oss;
        oss << name << " = " << val;
        log(cat, msgLevel, oss.str());
#endif
    }

private:
    static std::string categoryToString(DebugCategory cat) {
        switch(cat) {
            case DebugCategory::TT: return "TT";
            case DebugCategory::MOVE: return "MOVE";
            case DebugCategory::QS: return "QS";
            case DebugCategory::EVAL: return "EVAL";
            case DebugCategory::BOARD: return "BOARD";
            case DebugCategory::SEARCH: return "SEARCH";
        }
        return "UNKNOWN";
    }
};