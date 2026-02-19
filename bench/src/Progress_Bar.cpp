#include "Progress_Bar.h"

#include <iostream>
#include <iomanip>

void flushProgress(int total, int showCnt) {
    int percent = showCnt * 100 / total;
    std::cout << '\r' << std::setw(3) << percent << "% " << showCnt << " / " << total << std::flush;
}