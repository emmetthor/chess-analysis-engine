#pragma once

#include "board/Board.h"
#include "board/Move.h"

#include <vector>

std::vector<Position> generatePosFromPosWithJumpPiece(
    const Board &board,
    const Position &pos,
    const std::vector<int> &dr,
    const std::vector<int> &dc
);

std::vector<Position> generatePosFromPosWithSlidePiece(
    const Board &board,
    const Position &pos,
    const std::vector<int> &dr,
    const std::vector<int> &dc
);

std::vector<Position> generatePosFromPosWithKnight(
    const Board &board,
    const Position &pos
);
std::vector<Position> generatePosFromPosWithStraight(
    const Board &board,
    const Position &pos
);
std::vector<Position> generatePosFromPosWithDiagonal(
    const Board &board,
    const Position &pos
);
std::vector<Position> generatePosFromPosWithKing(
    const Board &board,
    const Position &pos
);