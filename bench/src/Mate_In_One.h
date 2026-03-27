#pragma once

#include "Bench.h"
#include <string>

struct TestData
{
    std::string fen;
    std::string bestMove;
};

testResult testMateInOne(int testCnt);