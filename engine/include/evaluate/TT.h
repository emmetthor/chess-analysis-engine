#pragma once

#include "board/Move.h"

#include <cstdint>

constexpr int TT_SIZE = 1 << 20;
constexpr uint64_t TT_MASK = TT_SIZE - 1;

enum TTFlag {
    EXACT, LOWER, UPPER
};

struct TTEntry {
    uint64_t key;
    int depth;
    int score;
    TTFlag flag;
    Move bestMove;
};