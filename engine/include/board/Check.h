#pragma once

#include "board/Board.h"

bool isInCheck(
    const Board &board,
    const Player player
);

bool isCheckmate(
    const Board &board,
    const Player player
);