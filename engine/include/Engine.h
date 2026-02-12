#pragma once

#include "board/Board.h"
#include "move/Move.h"

class Engine {
public: 
    Engine();

    void setStartPosition();
    void setPositionWithFen(const std::string &fen);
    void checkReady();

    Move goDepth(int depth);
    Board getBoard();
    void move(Move move);
    void quit();
    void changePlayer();
    void setPlayer(Player player);

private:
    Board board;
    const int INF = 1e9;
};