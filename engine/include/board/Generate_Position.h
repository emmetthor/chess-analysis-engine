#pragma once

#include "board/Board.h"
#include "board/Move.h"

#include <vector>

std::vector<Position> generatePiecePosFromPos(
    const Board &board,
    const Position &pos,
    Piece p
);