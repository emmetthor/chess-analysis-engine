#include "board/Check.h"
#include "evaluate/Evaluate.h"
#include "evaluate/Material_Point.h"
#include "evaluate/Control_Center.h"
#include "evaluate/Negamax.h"
#include "evaluate/Tempo.h"
#include "evaluate/PST.h"

#include <iostream>

int evaluate(Board &board, const Player player) {
    return negamax(board, 2, 0, 0, player);
}

int boardEvaluate(const Board &board, const Player player) {
    int res = 0;

    //res += evaluateCenterControl(board);
    res += evaluateMaterial(board);
    res += evaluateTempo(board, player);
    res += evaluatePieceSquare(board);
    
    return res;
}