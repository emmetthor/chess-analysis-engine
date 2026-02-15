#pragma GCC optimize("O3,unroll-loops")

#include "board/Board.h"
#include "move/Move.h"
#include "move/Make_Move.h"
#include "move/Generate_Move.h"
#include "board/Check.h"
#include "search/Negamax.h"
#include "search/TT.h"
#include "evaluate/Evaluate.h"
#include "evaluate/Material_Point.h"
#include "move/Move_Order.h"
#include "debug.h"
#include "Structure_IO.h"

#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

static const int INF = 1e9;
static const int MAX_QS_DEPTH = 15;

bool startDebug = 0;
bool exactDebug = 0;

int totalMoves = 0;
int betaCutAtMove[5] = {};

int quietscenceNodes = 0;
int quietscence(Board &board, int alpha, int beta, Player player, int depth) {
    quietscenceNodes++;
    int standerdPoint = (player == Player::WHITE ? 1 : -1) * boardEvaluate(board, EVALUATE_MODE::FULL);
    if (standerdPoint >= beta) return beta;
    if (standerdPoint > alpha) alpha = standerdPoint;

    Move captureMoves[256];
    int nCaptureMoves = generateLegalCaptureMoves(board, player, captureMoves);

    sortMove(board, captureMoves, nCaptureMoves, inValidMove);

    if (nCaptureMoves == 0) {
        return alpha;
    }

    for (int i = 0; i < nCaptureMoves; i++) {
        Move move = captureMoves[i];

        // delta pruning 如果吃子太糟就直接跳過
        Piece captured = move.capturePiece;
        ENGINE_ASSERT(!(!move.isPromotion && captured == Piece::EMPTY));
        if (!move.isPromotion && pieceValue(move.movePiece) >= pieceValue(captured) + 200) continue;

        makeMove(board, move);
        std::cout << depth << ' ' << move << '\n';

        int score = 0;
        ENGINE_ASSERT(!isInCheck(board, player));
        score = -quietscence(board, -beta, -alpha, opponent(player), depth + 1);

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
        return quietscence(board, alpha, beta, player, 0);
    }

    // 生成所有走法
    Move moves[256];
    int nMoves = generateAllLegalMoves(board, player, moves);
    totalMoves += nMoves;

    // 檢查 checkmate / stalemate
    if (nMoves == 0) {
        //LOG_DEBUG(DebugCategory::SEARCH, "no move!");
        if (isInCheck(board, player)) return -INF + depth;
        else return 0;
    }

    sortMove(board, moves, nMoves, TTMove);

    for (int i = 0; i < nMoves; i++) {
        int searchDepth = depth - 1;
        Move move = moves[i];

        if (!move.isPromotion && move.capturePiece == Piece::EMPTY && depth >= 3) {
            if (i >= 6) {
                searchDepth = depth - 2;
            }
        }

        int score = 0;
        makeMove(board, move);

        std::cout << "nega move: " << move << '\n';
        std::cout << board << '\n';
        if (i == 0) {
            // 第一步全搜
            score = -negamax(board, depth - 1, -beta, -alpha, opponent(player));
        } else {
            score = -negamax(board, searchDepth, -alpha - 1, -alpha, opponent(player));
            if (score > alpha && score < beta) {
                score = -negamax(board, depth - 1, -beta, -alpha, opponent(player));
            }
        }
        undoMove(board, move);

        if (score >= beta) {
            // cutoff 存 LOWER
            storeTT(board.zobristKey, depth, beta, LOWER, move);

            betaCutAtMove[(i >= 4 ? 4 : i)]++;
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

SearchResult searchRootCore(Board &board, int depth, int alpha, int beta, Player player, Move iterativeMove) {
    SearchResult res;
    res.bestScore = -INF;

    // 生成所有走法
    Move moves[256];
    int nMoves = generateAllLegalMoves(board, player, moves);

    // 排序
    sortMove(board, moves, nMoves, iterativeMove);

    for (int i = 0; i < nMoves; i++) {
        Move move = moves[i];

        // 遞迴下一層
        makeMove(board, move);
        std::cout << "root move: " << move << '\n';
        ENGINE_ASSERT(!isInCheck(board, player));
        int score = -negamax(board, depth - 1, -beta, -alpha, opponent(player));
        undoMove(board, move);

        if (score > res.bestScore) {
            res.bestMove = move;
            res.bestScore = score;
        }

        if (score > alpha) {
            alpha = score;
        }
        if (alpha >= beta) {
            break; // root cut-off
        }
    }

    return res;
}

SearchResult negamaxRoot(Board &board, int depth, Player player) {
    auto startTime = std::chrono::steady_clock::now();

    SearchResult finalRes = {inValidMove, -INF};
    int lastScore = 0;

    for (int d = 1; d <= depth; d++) {
        SearchResult currentRes = {inValidMove, -INF};


        if (d == 1) {
            currentRes = searchRootCore(board, d, -INF, INF, player, finalRes.bestMove);
        } else {
            // aspiration window
            int window = 30;
            int alpha = lastScore - window;
            int beta = lastScore + window;

            while (true) {
                currentRes = searchRootCore(board, d, alpha, beta, player, finalRes.bestMove);

                int score = currentRes.bestScore;

                if (score <= alpha) {
                    alpha -= window;
                }
                else if (score >= beta) {
                    beta += window;
                } else {
                    break; // success
                }

                window *= 2; // 指數成長

                if (window > 2000) {
                    alpha = -INF;
                    beta = INF;
                    // 太多次還沒找到，就給他最大窗口
                }
            }
        }

        lastScore = currentRes.bestScore;
        finalRes = currentRes;
    }

    auto endTime = std::chrono::steady_clock::now();

    Table<std::string> outputTable;
    outputTable.colWidth = 25;
    outputTable.setTitles({"item", "result"});
    outputTable.addRow({"time used", std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()) + "ms"});
    outputTable.addRow({"Negamax nodes", std::to_string(negamaxNodes)});
    outputTable.addRow({"QuietScence nodes", std::to_string(quietscenceNodes)});
    outputTable.addRow({"tt Cut times / node", std::to_string((double)ttCut / negamaxNodes)});
    outputTable.addRow({"tt Hit times / node", std::to_string((double)ttHit / negamaxNodes)});
    outputTable.addRow({"average move / node", std::to_string((double)totalMoves / negamaxNodes)});
    outputTable.addRow({"betaCut at move1", std::to_string(betaCutAtMove[0])});
    outputTable.addRow({"betaCut at move2", std::to_string(betaCutAtMove[1])});
    outputTable.addRow({"betaCut at move3", std::to_string(betaCutAtMove[2])});
    outputTable.addRow({"betaCut at move4", std::to_string(betaCutAtMove[3])});
    outputTable.addRow({"betaCut after move5", std::to_string(betaCutAtMove[4])});
    outputTable.addRow({"score", std::to_string(finalRes.bestScore)});

    LOG_INFO(DebugCategory::SEARCH, "search result: \n", outputTable);

    return finalRes;
}

