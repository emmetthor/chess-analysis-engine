#include "board/Board.h"
#include "board/Move.h"
#include "board/Generate_Move.h"
#include "board/Check.h"
#include "evaluate/Negamax.h"
#include "evaluate/Evaluate.h"
#include "evaluate/Move_Order.h"
#include "debug.h"

#include <vector>
#include <algorithm>

static const int INF = 1e9;

int quietscence(Board &board, int alpha, int beta, Player player) {
    int standerdPoint = (player == PLAYER_WHITE ? 1 : -1) * boardEvaluate(board, 1);
    if (standerdPoint >= beta) return beta;
    if (standerdPoint > alpha) alpha = standerdPoint;

    Move captureMoves[256];
    int nCaptureMoves = generateCaptureMoves(board, player, captureMoves);

    sortMove(captureMoves, nCaptureMoves);

    for (int i = 0; i < nCaptureMoves; i++) {
        Move move = captureMoves[i];

        makeMove(board, move);
        int score = -quietscence(board, -beta, -alpha, opponent(player));
        undoMove(board, move);

        if (score >= beta) return beta;
        if (score > alpha) alpha = score;

    }

    return alpha;
}

int negamax(Board &board, int depth, int alpha, int beta, Player player) {
    int standerdPoint = (player == PLAYER_WHITE ? 1 : -1) * boardEvaluate(board, (depth == 0 ? 1 : 0));
    if (standerdPoint >= beta) return beta;
    if (standerdPoint > alpha) alpha = standerdPoint;

    if (depth == 0) {
        return quietscence(board, alpha, beta, player);
    }

    Move moves[256];
    int nMoves = generateAllLegalMoves(board, player, moves);

    if (nMoves == 0) {
        if (isInCheck(board, player)) return -INF + depth;
        else return 0;
    }

    sortMove(moves, nMoves);

    for (int i = 0; i < nMoves; i++) {
        Move move = moves[i];

        makeMove(board, move);
        int score = -negamax(board, depth - 1, -beta, -alpha, opponent(player));
        undoMove(board, move);

        if (score > alpha) alpha = score;
        if (alpha >= beta) break;
    }

    return alpha;
}

SearchResult negamaxRoot(Board &board, int depth, Player player) {
    SearchResult finalRes;
    finalRes.bestScore = -INF;

    Move moves[256];
    int nMoves = generateAllLegalMoves(board, player, moves);

    sortMove(moves, nMoves);

    for (int d = 1; d <= depth; d++) {
        SearchResult res;
        res.bestScore = -INF;

        for (int i = 0; i < nMoves; i++) {
            Move move = moves[i];

            makeMove(board, move);
            int score = -negamax(board, d - 1, -INF, INF, opponent(player));
            undoMove(board, move);

            if (score > res.bestScore) {
                res.bestMove = move;
                res.bestScore = score;
            }

            // debug::set(1);
            // printMove(move);
            // debug::log("score: ", score, '\n');
            // debug::set(0);
        }

        for (int i = 0; i < nMoves; i++) {
            if (moves[i] == res.bestMove) {
                std::swap(moves[0], moves[i]);
                break;
            }
        }

        finalRes = res;
    }

    return finalRes;
}

