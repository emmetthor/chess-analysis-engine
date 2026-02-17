#pragma once

#include "move/Move.h"

constexpr int MAX_PLY = 128;
extern Move killerMove[2][MAX_PLY];

inline void addKillerMove(const Move &move, int ply) {
    if (killerMove[0][ply] == move) return;
    killerMove[1][ply] = killerMove[0][ply];
    killerMove[0][ply] = move;
}