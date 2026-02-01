#pragma once

#include "board/Board.h"
#include "board/Move.h"

#include <vector>

int generatePiecePosFromPos(
    const Board &board,
    const Position &pos,
    Piece p,
    Position *buffer
);