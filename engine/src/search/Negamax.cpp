#pragma GCC optimize("O3,unroll-loops")

#include "board/Board.h"
#include "move/Move.h"
#include "move/Make_Move.h"
#include "move/Generate_Move.h"
#include "board/Check.h"
#include "search/Negamax.h"
#include "search/TT.h"
#include "evaluate/Evaluate.h"
#include "move/Move_Order.h"
#include "debug.h"
#include "Structure_IO.h"

#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

static const int INF = 1e9;

bool startDebug = 0;
bool exactDebug = 0;

int quietscenceNodes = 0;
int quietscence(Board &board, int alpha, int beta, Player player) {
    quietscenceNodes++;
    int standerdPoint = (player == Player::WHITE ? 1 : -1) * boardEvaluate(board, 1);
    if (standerdPoint >= beta) return beta;
    if (standerdPoint > alpha) alpha = standerdPoint;

    Move captureMoves[256];
    int nCaptureMoves = generateLegalCaptureMoves(board, player, captureMoves);

    sortMove(board, captureMoves, nCaptureMoves, inValidMove);

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
int ttProbe = 0;
int ttHit = 0;
int ttCut = 0;
int negamax(Board &board, int depth, int alpha, int beta, Player player) {
    negamaxNodes++;

    //TT表 記憶化搜索
    ttProbe++;
    TTEntry tt;
    Move TTMove;
    if (probeTT(board.zobristKey, depth, alpha, beta, tt)) {
        ttHit++;

        if (tt.depth >= depth) {
            if (tt.flag == EXACT) {
                ttCut++;
                return tt.score;
            }
            if (tt.flag == LOWER && tt.score >= beta) {
                ttCut++;
                return beta;
            }
            if (tt.flag == UPPER && tt.score <= alpha) {
                ttCut++;
                return alpha;
            }
        }
        
        if (isMoveLegal(board, tt.bestMove)) TTMove = tt.bestMove;
    }

    int oriAlpha = alpha;
    int bestScore = -INF;
    Move bestMove;
    bool hasMove = 0;

    // depth = 0 進入QS
    if (depth == 0) {
        return quietscence(board, alpha, beta, player);
    }

    // 生成所有走法
    Move moves[256];
    int nMoves = generateAllLegalMoves(board, player, moves);

    // 檢查 checkmate / stalemate
    if (nMoves == 0) {
        //LOG_DEBUG(DebugCategory::SEARCH, "no move!");
        if (isInCheck(board, player)) return -INF + depth;
        else return 0;
    }

    sortMove(board, moves, nMoves, TTMove);

    for (int i = 0; i < nMoves; i++) {
        Move move = moves[i];

        makeMove(board, move);
        int score = -negamax(board, depth - 1, -beta, -alpha, opponent(player));
        undoMove(board, move);

        if (score >= beta) {
            // cutoff 存 LOWER
            storeTT(board.zobristKey, depth, beta, LOWER, move);

            return beta;
        }
        if (score > alpha) alpha = score;
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
            hasMove = 1;
        }
    }

    TTFlag flag;
    if (bestScore <= oriAlpha) flag = UPPER;
    else if (bestScore >= beta) flag = LOWER;
    else flag = EXACT;

    storeTT(board.zobristKey, depth, bestScore, flag, (hasMove ? bestMove : inValidMove));

    return bestScore;
}

int lstNegamaxNodes = 0;
int lstQuietscenceNodes = 0;
int lstPorbeTTTimes = 0;
int lstttProbe = 0;
int lstttHit = 0;
int lstttCut = 0;
SearchResult negamaxRoot(Board &board, int depth, Player player) {
    SearchResult finalRes;
    finalRes.bestScore = -INF;

    // 生成所有走法
    Move moves[256];
    int nMoves = generateAllLegalMoves(board, player, moves);

    // 排序
    sortMove(board, moves, nMoves, inValidMove);

    for (int d = 1; d <= depth; d++) {
        SearchResult res;
        res.bestScore = -INF;

        for (int i = 0; i < nMoves; i++) {
            Move move = moves[i];

            // 遞迴下一層
            makeMove(board, move);
            int score = -negamax(board, d - 1, -INF, INF, opponent(player));
            undoMove(board, move);

            if (score > res.bestScore) {
                res.bestMove = move;
                res.bestScore = score;
            }
        }

        // 輸出節點數
        if (d == depth) {
            LOG_INFO(DebugCategory::SEARCH, "negamaxNodes = ", negamaxNodes);
            LOG_INFO(DebugCategory::SEARCH, "qquietscenceNodes ", quietscenceNodes);
            LOG_INFO(DebugCategory::SEARCH, "ttProbe = ", ttProbe);
            LOG_INFO(DebugCategory::SEARCH, "ttCut = ", ttCut);
            LOG_INFO(DebugCategory::SEARCH, "ttHit = ", ttHit);
        }
        lstNegamaxNodes = negamaxNodes;
        lstQuietscenceNodes = quietscenceNodes;
        lstttProbe = ttProbe;
        lstttCut = ttCut;
        lstttHit = ttHit;

        // iterative
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

