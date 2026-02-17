#pragma once

#include "board/Board.h"
#include "board/Attack.h"

enum class EVALUATE_MODE {
    QUICK, POSITION, FULL
};

class Evaluate {
public:
    int evaluateBoard(const Board &board, EVALUATE_MODE m);
private:
    int kingSafetyWeight = 8;
    int knightMobilityWeight = 4;
    int bishopMobilityWeight = 4;
    int rookMobilityWeight = 4;
    int castleWeight = 20;
    int doublePawnWeight = 15;
    int isolatedPawnWeight = 15;
    int passPawnWeight = 30;
    int passPawnRankWeight[7] = {0, 150, 80, 40, 20, 10, 5};
};

int evaluate(Board &board, const Player player);

int boardEvaluate(const Board &board, EVALUATE_MODE m);