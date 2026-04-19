#pragma once

#include "Time_Management.h"
#include "board/Board.h"
#include "evaluate/Evaluate.h"
#include "move/Move.h"
#include "search/Search.h"

class Engine
{
public:
    Engine();

    Evaluate eval;
    Board board;

    void setStartPosition();
    void setPositionWithFen(const std::string& fen);
    void checkReady();

    Move goDepth(int depth);
    Move goClock(const TimeManage& tm);
    SearchResult fullInfoSearch(int depth);
    void move(Move move);
    void quit();
    void changePlayer();
    void setPlayer(Player player);

    inline int ping() {
        std::cout << "pong from Hynobius engine" << std::endl;
        return 42;
    }
};