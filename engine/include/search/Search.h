#pragma once

#include "Killer_Move.h"
#include "PV_Table.h"
#include "board/Board.h"
#include "evaluate/Evaluate.h"
#include "move/Move.h"
#include <chrono>

constexpr int MATE_SCORE = 1e6;
constexpr int TIMEOUT_SCORE = 123456789;
constexpr int MAX_SCORE = 1e9;

struct SearchResult
{
    bool isValid = 0;
    Move bestMove;
    int bestScore;
    BitMove bestBitMove = INVALID_BITMOVE;
};

struct SearchLimits
{
    int maxDepth = -1;
    int64_t maxTimeMs = -1;
};

struct SearchInfo
{
    int64_t depth = 0, score = 0, nodes = 0, qsnodes = 0, timeMs = 0, nps = 0;
    int64_t pvsFullSearch = 0, pvsNullSearch = 0, pvsResearch = 0, pvsRootFullSearch = 0, pvsRootNullSearch = 0, pvsRootResearch = 0;
    int64_t ttCuts = 0, betaCuts = 0;
    PVTable pv;
};

class Search
{
public:
    Search(const Evaluate& _eval, const SearchLimits _limits);
    SearchResult findBestMove(const Board& board);

private:
    Evaluate eval;

    SearchResult
    chooseMove(Board& board, int depth, int alpha, int beta, int ply, const BitMove PVMove);

    int negamax(Board& board, int depth, int alpha, int beta, int ply);

    int quietscence(Board& board, int alpha, int beta, int ply);

    SearchLimits limits;

    struct SearchState
    {
        bool stopped = false;
        bool timeout = false;

        uint64_t negamaxNodes = 0;
        uint64_t qsNodes = 0;

        std::chrono::steady_clock::time_point startTime;

        PVTable pv, prevPv;

        killerMove kill;

        int prevScore = 0;

        SearchInfo info;
    } state;

    bool shouldStop();
};