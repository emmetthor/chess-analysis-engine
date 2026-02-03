#pragma GCC optimize("O3,unroll-loops")

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

int cnt = 0;

int boardEvaluate(const Board &board, bool quick = false) {
    // std::cout << "here x" << cnt++ << " times.\n";
    int res = 0;

    res += board.getMaterialScore();
    res += board.getPSTScore();

    if (quick) return res;

    // slow
    res += evaluateKingSafety(board, PLAYER_WHITE);
    res -= evaluateKingSafety(board, PLAYER_BLACK);
    
    return res;
}