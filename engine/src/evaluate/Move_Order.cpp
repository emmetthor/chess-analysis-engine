#include "evaluate/Move_Order.h"
#include "evaluate/Material_Point.h"

#include <algorithm>

int evaluateMoveScore(Move &move) {
    int score = 0;

    if (move.isPromotion) {
        // 最先看
        score += 100000;
        score += pieceValue(move.promotionPiece);
    }

    if (move.capturePiece != EMPTY) {
        // 次要看
        score += 5000;
        score += pieceValue(move.capturePiece) * 10 - pieceValue(move.movePiece);
    }

    return score;
}

void sortMove(Move *moves, int nMoves) {
    ScoreMove tmp[256];

    for (int i = 0; i < nMoves; i++) {
        tmp[i].move = moves[i];
        tmp[i].score = evaluateMoveScore(moves[i]);
    }

    std::sort(tmp, tmp + nMoves,
        [](const ScoreMove &a, const ScoreMove &b) {
            return a.score > b.score;
        });

    for (int i = 0; i < nMoves; i++) {
        moves[i] = tmp[i].move;
    }
}