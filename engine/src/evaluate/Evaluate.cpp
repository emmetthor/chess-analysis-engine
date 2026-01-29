#include "board/Check.h"
#include "evaluate/Evaluate.h"
#include "evaluate/Material_Point.h"
#include "evaluate/Control_Center.h"
#include "evaluate/Negamax.h"

int evaluate(Board &board, const Player player) {
    return negamax(board, 2, 0, 0, player);
}

int boardEvaluate(const Board &board) {
    int res = 0;

    res += evaluateCenterControl(board);
    res += evaluateMaterial(board);

    return res;
}