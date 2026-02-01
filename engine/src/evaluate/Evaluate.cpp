#include "board/Check.h"
#include "evaluate/Evaluate.h"
#include "evaluate/Material_Point.h"
#include "evaluate/Control_Center.h"
#include "evaluate/Negamax.h"
#include "evaluate/Tempo.h"
#include "evaluate/PST.h"
#include "evaluate/King_Safety.h"

#include <iostream>

const int INF = 1e9;

int evaluate(Board &board, const Player player) {
}

int boardEvaluate(const Board &board, bool quick = false) {
    if (quick == 1) {
        return evaluateMaterial(board);
    }

    int res = 0;

    res += evaluateCenterControl(board);
    res += evaluateMaterial(board);
    res += evaluatePieceSquare(board);

    res += evaluateKingSafety(board, PLAYER_WHITE);
    res -= evaluateKingSafety(board, PLAYER_BLACK);
    
    return res;
}