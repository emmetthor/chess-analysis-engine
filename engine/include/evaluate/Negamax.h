#pragma once

#include "board/Board.h"
#include "board/Move.h"

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
    AttackMap &attackMap
);
SearchResult negamaxRoot(
    Board &board,
    int depth,
    Player player
);
