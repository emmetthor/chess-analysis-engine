#pragma once
#include <fstream>
#include <cstdint>

//======================
// Compile-time switch
//======================
#ifdef ENGINE_DEBUG
    #define DEBUG_ENABLED 1
#else
    #define DEBUG_ENABLED 0
#endif

#ifdef ENGINE_DEBUG
    #include <assert.h>
    #define ENGINE_ASSERT(x) assert(x)
#else
    #define ENGINE_ASSERT(x) ((void)0)
#endif

//======================
// Enum
//======================
enum class DebugLevel { INFO, DEBUG, WARN, ERROR };
enum class DebugCategory { TT, MOVE, QS, EVAL, BOARD, SEARCH, ATK };

//======================
// Debug class
//======================
class Debug {
public:
    static DebugLevel level;
    static uint32_t categoryMask;
    static bool consoleEnabled;
    static bool fileEnabled;

    static void init(const char* file = nullptr);
    static void shutdown();

#if DEBUG_ENABLED
    static void log(
        DebugCategory cat,
        DebugLevel lvl,
        const char* file,
        int line,
        const char* func,
        const char* fmt,
        ...
    );
#else
    static inline void log(...) {}
#endif
};

//======================
// Macros (唯一使用入口)
//======================
#if DEBUG_ENABLED

#define LOG_DEBUG(cat, fmt, ...) \
    Debug::log(cat, DebugLevel::DEBUG, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#define LOG_INFO(cat, fmt, ...) \
    Debug::log(cat, DebugLevel::INFO, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#define LOG_WARN(cat, fmt, ...) \
    Debug::log(cat, DebugLevel::WARN, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#define LOG_ERROR(cat, fmt, ...) \
    Debug::log(cat, DebugLevel::ERROR, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#else

#define LOG_DEBUG(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)

#endif
