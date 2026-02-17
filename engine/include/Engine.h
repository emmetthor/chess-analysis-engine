#pragma once

#include "board/Board.h"
#include "move/Move.h"
#include "evaluate/Evaluate.h"

class Engine {
public: 
    Engine();

    Evaluate eval;

    void setStartPosition();
    void setPositionWithFen(const std::string &fen);
    void checkReady();

    Move goDepth(int depth, Evaluate eval);
    Board getBoard();
    void move(Move move);
    void quit();
    void changePlayer();
    void setPlayer(Player player);

private:
    Board board;
    const int INF = 1e9;
};