#pragma once

#include "Piece.h"

#include <string>

/*
初始化
輸出棋盤

取的 (r, c) 棋子
放置棋子 p 於 (r, c)

是否白方先手
*/

struct Position {
    int row;
    int col;
};

std::string pngPosition(const Position pos);

class Board {
public: 
    Board();

    void init();
    void debugPrint() const;

    Piece at(Position pos) const;
    void set(Position pos, Piece p);

    bool isInBoard(Position pos) const;

    bool whiteToMove;
    
private:

    Piece board[8][8];
};