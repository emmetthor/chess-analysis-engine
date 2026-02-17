#pragma once

#include "move/Move.h"

#include <vector>
#include <string>

struct ScoreMove {
    Move move;
    int score;
};

struct advanceMoves {
    Move TTMove, killer1, killer2;
};

void sortMove(
    const Board &board,
    Move *moves,
    int nMoves,
    advanceMoves &advMove
);

