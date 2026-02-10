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

static std::ofstream g_logFile;
static void logHeader(std::ostringstream& oss,
                        DebugCategory cat,
                        DebugLevel lvl,
                        const char* file,
                        int line,
                        const char* func) {
    oss << "[" << std::left << std::setw(7) << categoryToStr(cat)
        << "][" << std::setw(5) << levelToStr(lvl)
        << "][" << std::setw(20) << (std::string(file) + ":" + std::to_string(line))
        << "][" << std::setw(12) << func << "] ";
}

template <typename... Args>
static void Debug::logStream(DebugCategory cat,
                        DebugLevel lvl,
                        const char* file,
                        int line,
                        const char* func,
                        Args&&... args) {
    if (lvl < level) return;
    if (!(categoryMask & (1u << int(cat)))) return;

    std::ostringstream oss;
    logHeader(oss, cat, lvl, file, line, func);
    (oss << ... << std::forward<Args>(args)); // fold expression

    if (consoleEnabled) std::cout << oss.str() << '\n';
    if (fileEnabled && g_logFile.is_open()) g_logFile << oss.str() << '\n';
}
#endif