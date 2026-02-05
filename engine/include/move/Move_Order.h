#pragma once

#include "move/Move.h"

#include <vector>
#include <string>

struct ScoreMove {
    Move move;
    int score;
};

void sortMove(
    const Board &board,
    Move *moves,
    int nMoves,
    Move &TTMove
);