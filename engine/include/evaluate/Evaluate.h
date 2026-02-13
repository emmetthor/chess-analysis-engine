#pragma once

#include "board/Board.h"
#include "board/Attack.h"

enum class EVALUATE_MODE {
    QUICK, POSITION, FULL
};

int evaluate(Board &board, const Player player);

int boardEvaluate(const Board &board, EVALUATE_MODE m);