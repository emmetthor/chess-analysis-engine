#include "search/TT.h"

#include <cstdint>
#include <assert.h>

TTEntry TT[TT_SIZE];

bool probeTT (
    uint64_t key,
    int depth,
    int alpha, 
    int beta,
    int &outValue,
    Move &outMove
) {
    TTEntry &tt = TT[TTIndex(key)];

    //如果 collide 直接跳掉
    if (tt.key != key) return false;
    //如果比較淺直接跳掉
    if (tt.depth < depth) return false;

    outMove = tt.bestMove;

    assert(outMove.from.row >= 0 && outMove.from.row < 8);
    assert(outMove.from.col >= 0 && outMove.from.col < 8);
    assert(outMove.to.row >= 0 && outMove.to.row < 8);
    assert(outMove.to.col >= 0 && outMove.to.col < 8);

    if (tt.flag == EXACT) {
        outValue = tt.score;
        return true;
    }

    else if (tt.flag == LOWER && tt.score >= beta) {
        outValue = tt.score;
        return true;
    }

    else if (tt.flag == UPPER && tt.score <= alpha) {
        outValue = tt.score;
        return true;
    }

    return false;
}

void storeTT(
    uint64_t key,
    int depth,
    int score,
    TTFlag flag,
    Move bestMove
) {
    TTEntry &tt = TT[TTIndex(key)];
    // depth check，保留較深的 entry
    if (tt.key != key || depth >= tt.depth) {
        tt.key = key;
        tt.depth = depth;
        tt.score = score;
        tt.flag = flag;
        tt.bestMove = bestMove;
    }
}