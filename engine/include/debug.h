#pragma once

#include <iostream>

namespace debug {
    inline bool enable = false;

    bool isEnabled();
    void set(bool on);

    template<typename ... Args>
    void log(Args&&... args) {
        if (!debug::enable) return;

        (std::cerr << ... << args);
    }
    
    class ScopedEnable {
        bool old;

    public:
        ScopedEnable(bool value) : old(enable) {
            enable = value;
        }

        ~ScopedEnable() {
            enable = old;
        }
    };

}