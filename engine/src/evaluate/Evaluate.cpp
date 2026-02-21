#pragma GCC optimize("O3,unroll-loops")

#include "board/Check.h"
#include "evaluate/Evaluate.h"
#include "evaluate/Material_Point.h"
#include "evaluate/Control_Center.h"
#include "evaluate/Tempo.h"
#include "evaluate/PST.h"
#include "evaluate/King_Safety.h"
#include "evaluate/Mobility.h"
#include "evaluate/Castle_Rights.h"
#include "evaluate/Pawn_Structure.h"

#include "debug.h"

#include <iostream>

const int INF = 1e9;

int Evaluate::evaluateBoard(const Board &board, EVALUATE_MODE m) const {
    int res = 0;
    switch (m) {
    case EVALUATE_MODE::FULL:
        res += evaluateKingSafety(board, Player::WHITE, kingSafetyWeight);
        res -= evaluateKingSafety(board, Player::BLACK, kingSafetyWeight);

        res += evaluateKnightMobility(board, Player::WHITE, knightMobilityWeight);
        res -= evaluateKnightMobility(board, Player::BLACK, knightMobilityWeight);
        res += evaluateBishopMobility(board, Player::WHITE, bishopMobilityWeight);
        res -= evaluateBishopMobility(board, Player::BLACK, bishopMobilityWeight);
        res += evaluateRookMobility(board, Player::WHITE, rookMobilityWeight);
        res -= evaluateRookMobility(board, Player::BLACK, rookMobilityWeight);

        res += evaluateCastleRights(board, castleWeight);
        
        res += board.materialScore;
        res += board.PSTScore;

        res += evaluatePawnStructure(board, doublePawnWeight, isolatedPawnWeight, passPawnWeight, passPawnRankWeight);
    }

    return res;
}

int boardEvaluate(const Board &board, EVALUATE_MODE m) {
    int res = 0;

    return res;
}