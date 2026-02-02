#pragma once

#include "board/Board.h"

int SEE(
    const Board &board,
    Position capturePos,
    Piece movePiece,
    Player player
);