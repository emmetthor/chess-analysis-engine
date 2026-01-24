#include "debug.h"
#include <iostream>

namespace debug {
    bool isEnabled() {
        return debug::enable;
    }

    void set(bool on) {
        debug::enable = on;
    }
}