#pragma once

#include <string>
#include "Bench.h"

struct TestData {
    std::string fen;
    std::string bestMove;
};

testResult testMateInOne(int testCnt);