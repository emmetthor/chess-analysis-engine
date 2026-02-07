#pragma GCC optimize("O3,unroll-loops")

#include "board/Check.h"
#include "evaluate/Evaluate.h"
#include "evaluate/Material_Point.h"
#include "evaluate/Control_Center.h"
#include "search/Negamax.h"
#include "evaluate/Tempo.h"
#include "evaluate/PST.h"
#include "evaluate/King_Safety.h"
#include "evaluate/Mobility.h"

#include <iostream>

const int INF = 1e9;

int evaluate(Board &board, const Player player) {
}

int boardEvaluate(const Board &board, bool quick = false) {
    int res = 0;

    res += board.materialScore;
    res += board.PSTScore;

    if (quick) return res;

    // slow
    res += evaluateKingSafety(board, PLAYER_WHITE);
    res -= evaluateKingSafety(board, PLAYER_BLACK);

    res += evaluateKnightMobility(board, PLAYER_WHITE);
    res -= evaluateKnightMobility(board, PLAYER_BLACK);
    res += evaluateBishopMobility(board, PLAYER_WHITE);
    res -= evaluateBishopMobility(board, PLAYER_BLACK);
    res += evaluateRookMobility(board, PLAYER_WHITE);
    res -= evaluateRookMobility(board, PLAYER_BLACK);
    
    return res;
}