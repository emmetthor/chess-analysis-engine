#include "evaluate/Evaluate.h"
#include "evaluate/Material_Point.h"
#include "evaluate/Control_Center.h"

int evaluate(const Board &board) {
    int res = 0;
    res += evaluateMaterial(board);
    res += evaluateCenterControl(board);
    return res;
}