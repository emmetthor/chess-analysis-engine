#pragma GCC optimize("O3,unroll-loops")

#include "move/Move_Order.h"
#include "evaluate/Material_Point.h"
#include "evaluate/SEE.h"

#include <algorithm>

int evaluateMoveScore(const Board &board, Move &move, Move &TTMove) {
    int score = 0;

    if (move == TTMove) score += 1000000;

    if (move.isPromotion) {
        // 最先看
        score += 100000;
        score += pieceValue(move.promotionPiece);
    }

    if (move.capturePiece != Piece::EMPTY) {
        score += pieceValue(move.capturePiece) * 10 - pieceValue(move.movePiece);
    }
    
    score += SEE(board, move.to, move.movePiece, move.player);

    return score;
}

void sortMove(const Board &board, Move *moves, int nMoves, Move &TTMove) {
    ScoreMove tmp[256];

    for (int i = 0; i < nMoves; i++) {
        tmp[i].move = moves[i];
        tmp[i].score = evaluateMoveScore(board, moves[i], TTMove);
    }

    std::sort(tmp, tmp + nMoves,
        [](const ScoreMove &a, const ScoreMove &b) {
            return a.score > b.score;
        });

    for (int i = 0; i < nMoves; i++) {
        moves[i] = tmp[i].move;
    }
}