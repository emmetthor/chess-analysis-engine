#pragma once

#include "board/Board.h"
#include "move/Move.h"
#include "evaluate/Evaluate.h"

extern bool startDebug;
extern bool exactDebug;

struct SearchResult {
    Move bestMove;
    int bestScore;
};

SearchResult negamaxRoot(
    Board &board,
    int depth,
    Player player,
    const Evaluate &eval
);
