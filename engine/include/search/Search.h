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

struct SearchInfo
{
    int64_t depth, score, nodes, qsnodes, timeMs, nps;
    PVTable pv;
};

struct SearchResult
{
    bool isValid = 0;
    Move bestMove;
    int bestScore;
    BitMove bestBitMove = INVALID_BITMOVE;
    SearchInfo info;
};

struct SearchLimits
{
    int maxDepth = -1;
    int64_t maxTimeMs = -1;
};

class Search
{
public:
    SearchResult findBestMove(const Board& board);

    inline void setEval(const Evaluate &_eval)
    {
        eval = _eval;
    }

    inline void setLimits(const SearchLimits &_limits)
    {
        limits = _limits;
    }

    inline void newSearch(const Evaluate &_eval, const SearchLimits &_limits)
    {
        setEval(_eval);
        setLimits(_limits);
        state.reset();
    }

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

        inline void reset()
        {
            stopped = false;
            timeout = false;

            negamaxNodes = 0;
            qsNodes = 0;
        }
    } state;

    bool shouldStop();
};