#pragma once

#include "board/Board.h"
#include "board/Attack.h"

bool isInCheck(
    const Board &board,
    const Player player
);