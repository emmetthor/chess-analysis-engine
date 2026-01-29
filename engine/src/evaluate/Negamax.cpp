#include "board/Board.h"
#include "board/Move.h"
#include "board/Generate_Move.h"
#include "board/Check.h"
#include "evaluate/Negamax.h"
#include "evaluate/Evaluate.h"
#include "debug.h"

#include <vector>

static const int INF = 1e9;

int negamax(Board &board, int depth, int alpha, int beta, Player player) {
    if (depth == 0) {
        return (player == PLAYER_WHITE ? 1 : -1) * boardEvaluate(board);
    }

    std::vector<Move> moves = generateAllLegalMoves(board, player);

    if (moves.empty()) {
        if (isInCheck(board, player)) {
            // checkmate
            return -INF + depth;
        } else {
            // stalemate
            return 0;
        }
    }

    int best = -INF;

    for (auto &move : moves) {
        makeMove(board, move);
        int score = -negamax(board, depth - 1, -beta, -alpha, opponent(player));
        undoMove(board, move);

        best = std::max(best, score);
        alpha = std::max(alpha, score);

        if (alpha >= beta) break;
    }

    return best;
}

SearchResult negamaxRoot(Board &board, int depth, Player player) {
    std::vector<Move> moves = generateAllLegalMoves(board, player);

    SearchResult res;
    res.bestScore = -INF;

    for (auto &move : moves) {
        makeMove(board, move);
        int score = -negamax(board, depth - 1, -INF, INF, opponent(player));
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

    return res;
}