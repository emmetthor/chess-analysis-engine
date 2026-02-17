#pragma once

#include "board/Board.h"
#include "move/Move.h"

extern bool startDebug;
extern bool exactDebug;

struct SearchResult {
    Move bestMove;
    int bestScore;
};

int negamax(
    Board &board,
    int depth,
    int alpha,
    int beta,
    Player player,
    int ply
);

SearchResult negamaxRoot(
    Board &board,
    int depth,
    Player player
);
