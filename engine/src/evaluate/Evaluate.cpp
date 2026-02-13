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
#include "evaluate/Castle_Rights.h"

#include "debug.h"

#include <iostream>

const int INF = 1e9;

int evaluate(Board &board, const Player player) {
}

int boardEvaluate(const Board &board, EVALUATE_MODE m) {
    int res = 0;
    switch (m) {
    case EVALUATE_MODE::POSITION:
        res += evaluateKingSafety(board, Player::WHITE);
        res -= evaluateKingSafety(board, Player::BLACK);

        res += evaluateKnightMobility(board, Player::WHITE);
        res -= evaluateKnightMobility(board, Player::BLACK);
        res += evaluateBishopMobility(board, Player::WHITE);
        res -= evaluateBishopMobility(board, Player::BLACK);
        res += evaluateRookMobility(board, Player::WHITE);
        res -= evaluateRookMobility(board, Player::BLACK);

        res += evaluateCastleRights(board);

        break;
    
    case EVALUATE_MODE::QUICK:
        res += board.materialScore;
        res += board.PSTScore;

        break;

    case EVALUATE_MODE::FULL:
        res += evaluateKingSafety(board, Player::WHITE);
        res -= evaluateKingSafety(board, Player::BLACK);

        res += evaluateKnightMobility(board, Player::WHITE);
        res -= evaluateKnightMobility(board, Player::BLACK);
        res += evaluateBishopMobility(board, Player::WHITE);
        res -= evaluateBishopMobility(board, Player::BLACK);
        res += evaluateRookMobility(board, Player::WHITE);
        res -= evaluateRookMobility(board, Player::BLACK);

        res += evaluateCastleRights(board);
        
        res += board.materialScore;
        res += board.PSTScore;
    }

    return res;
}