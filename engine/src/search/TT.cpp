#include "search/TT.h"

#include <cstdint>
#include <assert.h>

TTEntry TT[TT_SIZE];

bool probeTT (
    uint64_t key,
    int depth,
    int alpha, 
    int beta,
    TTEntry &TTOut
) {
    TTEntry &tt = TT[TTIndex(key)];

    //如果 collide 直接跳掉
    if (tt.key != key) return false;
    TTOut = tt;
    return true;
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