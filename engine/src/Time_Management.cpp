#include "Time_Management.h"
#include <algorithm>

int64_t timeManageCore(int64_t timeLeft, int64_t increment)
{
    int64_t safeTime = std::max<int64_t>(0, timeLeft - MOVE_OVERHEAD);

    // 0.2 seconds
    if (safeTime <= std::max<int64_t>(2 * increment, 1000))
    {
        return std::max<int64_t>(MIN_THINK_TIME, increment / 4);
    }

    // 10 seconds
    else if (safeTime <= std::max<int64_t>(8 * increment, 5000))
    {
        return std::max<int64_t>(safeTime / 15, increment / 2);
    }

    // normal
    else
    {
        return safeTime / 30 + increment * 3 / 4;
    }
}

SearchLimits timeManager(const TimeManage& tm, Player player)
{
    if (tm.depth != -1)
        return {tm.depth, MAX_THINK_TIME};

    int64_t thinkTime = (player == Player::WHITE ? timeManageCore(tm.wtime, tm.winc)
                                                 : timeManageCore(tm.btime, tm.binc));

    return {-1, thinkTime};
}