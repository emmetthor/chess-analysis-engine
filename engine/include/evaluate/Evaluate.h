#pragma once

#include "board/Board.h"
#include "board/Attack.h"

int evaluate(Board &board, const Player player);

int boardEvaluate(const Board &board, bool quick);