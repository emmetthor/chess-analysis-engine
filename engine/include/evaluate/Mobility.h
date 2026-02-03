# pragma once

#include "board/Board.h"

int evaluateKnightMobility (
    const Board &board,
    Player player
);
int evaluateBishopMobility (
    const Board &board,
    Player player
);
int evaluateRookMobility (
    const Board &board,
    Player player
);