#include "Debug.h"

#if DEBUG_ENABLED
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdarg>
#endif

//======================
// Static members
//======================
DebugLevel Debug::level = DebugLevel::DEBUG;
uint32_t Debug::categoryMask = 0xFFFFFFFF;
bool Debug::consoleEnabled = true;
bool Debug::fileEnabled = false;

#if DEBUG_ENABLED
static std::ofstream g_logFile;
#endif

//======================
// Helpers
//======================
#if DEBUG_ENABLED
static const char* levelToStr(DebugLevel lvl) {
    switch (lvl) {
        case DebugLevel::INFO:  return "INFO";
        case DebugLevel::DEBUG: return "DEBUG";
        case DebugLevel::WARN:  return "WARN";
        case DebugLevel::ERROR: return "ERROR";
    }
    return "UNK";
}

static const char* categoryToStr(DebugCategory cat) {
    switch (cat) {
        case DebugCategory::TT:     return "TT";
        case DebugCategory::MOVE:   return "MOVE";
        case DebugCategory::QS:     return "QS";
        case DebugCategory::EVAL:   return "EVAL";
        case DebugCategory::BOARD:  return "BOARD";
        case DebugCategory::SEARCH: return "SEARCH";
        case DebugCategory::ATK:    return "ATK";
    }
    return "UNK";
}
#endif

//======================
// Init / Shutdown
//======================
void Debug::init(const char* file) {
#if DEBUG_ENABLED
    if (file) {
        g_logFile.open(file);
        fileEnabled = g_logFile.is_open();
    }
#endif
}

void Debug::shutdown() {
#if DEBUG_ENABLED
    if (g_logFile.is_open())
        g_logFile.close();
#endif
}

//======================
// Core log
//======================
#if DEBUG_ENABLED
void Debug::log(
    DebugCategory cat,
    DebugLevel lvl,
    const char* file,
    int line,
    const char* func,
    const char* fmt,
    ...
) {
    if (lvl < level) return;
    if (!(categoryMask & (1u << int(cat)))) return;

    char msgBuf[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msgBuf, sizeof(msgBuf), fmt, args);
    va_end(args);

    std::ostringstream oss;
    oss << "["
        << std::left << std::setw(7) << categoryToStr(cat)
        << "]["
        << std::setw(5) << levelToStr(lvl)
        << "]["
        << std::setw(20) << (std::string(file) + ":" + std::to_string(line))
        << "]["
        << std::setw(12) << func
        << "] "
        << msgBuf;

    if (consoleEnabled)
        std::cout << oss.str() << '\n';

    if (fileEnabled && g_logFile.is_open())
        g_logFile << oss.str() << '\n';
}
#endif
