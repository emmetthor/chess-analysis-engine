#pragma GCC optimize("O3,unroll-loops")

#include "board/Board.h"
#include "move/Move.h"
#include "move/Make_Move.h"
#include "move/Generate_Move.h"
#include "board/Check.h"
#include "search/Negamax.h"
#include "evaluate/Evaluate.h"
#include "move/Move_Order.h"
#include "debug.h"

#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

static const int INF = 1e9;

int quietscenceNodes = 0;
int quietscence(Board &board, int alpha, int beta, Player player) {
    quietscenceNodes++;
    int standerdPoint = (player == PLAYER_WHITE ? 1 : -1) * boardEvaluate(board, 1);
    if (standerdPoint >= beta) return beta;
    if (standerdPoint > alpha) alpha = standerdPoint;

    Move captureMoves[256];
    int nCaptureMoves = generateLegalCaptureMoves(board, player, captureMoves);

    sortMove(board, captureMoves, nCaptureMoves);

    for (int i = 0; i < nCaptureMoves; i++) {
        Move move = captureMoves[i];
        
        // debug::set(1);
        // printMove(move);
        // board.debugPrint();
        // debug::set(0);
        // std::this_thread::sleep_for(std::chrono::seconds(1));

        makeMove(board, move);

        int score = 0;
        if (!isInCheck(board, player)) {
            score = -quietscence(board, -beta, -alpha, opponent(player));
        }
        undoMove(board, move);

        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }

    return alpha;
}

int negamaxNodes = 0;
int negamax(Board &board, int depth, int alpha, int beta, Player player) {
    negamaxNodes++;
    if (depth == 0) {
        return quietscence(board, alpha, beta, player);
    }

    int standerdPoint = (player == PLAYER_WHITE ? 1 : -1) * boardEvaluate(board, 0);
    if (standerdPoint >= beta) return beta;
    if (standerdPoint > alpha) alpha = standerdPoint;

    Move moves[256];
    int nMoves = generateAllLegalMoves(board, player, moves);

    if (nMoves == 0) {
        if (isInCheck(board, player)) return -INF + depth;
        else return 0;
    }

    sortMove(board, moves, nMoves);

    for (int i = 0; i < nMoves; i++) {
        Move move = moves[i];

        // debug::set(1);
        // debug::log("negamax move: ");
        // printMove(move);
        // board.debugPrint();
        // debug::set(0);
        // std::this_thread::sleep_for(std::chrono::seconds(1));

        makeMove(board, move);
        int score = -negamax(board, depth - 1, -beta, -alpha, opponent(player));
        undoMove(board, move);

        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }

    return alpha;
}

int lstNegamaxNodes = 0;
int lstQuietscenceNodes = 0;
SearchResult negamaxRoot(Board &board, int depth, Player player) {
    SearchResult finalRes;
    finalRes.bestScore = -INF;

    Move moves[256];
    int nMoves = generateAllLegalMoves(board, player, moves);

    sortMove(board, moves, nMoves);

    for (int d = 1; d <= depth; d++) {
        SearchResult res;
        res.bestScore = -INF;

        for (int i = 0; i < nMoves; i++) {
            Move move = moves[i];

            // if (d == 1) {
            //     debug::set(1);
            //     debug::log("negamaxRoot move: ");
            //     printMove(move);
            //     debug::set(0);
            // }

            //std::this_thread::sleep_for(std::chrono::seconds(1));

            makeMove(board, move);
            int score = -negamax(board, d - 1, -INF, INF, opponent(player));
            undoMove(board, move);

            if (score > res.bestScore) {
                res.bestMove = move;
                res.bestScore = score;
            }
        }

        std::cout << "depth = " << d << '\n';
        std::cout << "negamax nodes = " << negamaxNodes - lstNegamaxNodes << '\n';
        std::cout << "quietscence nodes = " << quietscenceNodes - lstQuietscenceNodes<< '\n';

        lstNegamaxNodes = negamaxNodes;
        lstQuietscenceNodes = quietscenceNodes;

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

