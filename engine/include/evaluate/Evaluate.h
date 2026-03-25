#pragma once

#include "board/Board.h"
#include "Type.h"
#include "Evaluate_Weight.h"

class Evaluate {
public:
    int evaluateBoard(const Board &board, EVALUATE_MODE m) const;
    Evaluate_Weights evalWeight;
};

int boardEvaluate(const Board &board, EVALUATE_MODE m);