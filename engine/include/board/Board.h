#pragma once

#include "Piece.h"

#include <string>
#include <set>
#include <vector>

/*
初始化
輸出棋盤

取的 (r, c) 棋子
放置棋子 p 於 (r, c)

是否白方先手
*/

enum Player {
    PLAYER_WHITE, PLAYER_BLACK
};

struct Position {
    int row = -1;
    int col = -1;

    bool operator==(const Position &other) const {
        return
            row == other.row &&
            col == other.col;
    }
};

bool samePosition(Position a, Position b);

Player opponent(Player player);

std::string pngPosition(const Position pos);

class Board {
public: 
    Board();

    void init();
    void debugPrint() const;

    Piece at(Position pos) const;
    void set(Position pos, Piece p);
    void change(std::vector<std::vector<Piece>> p);

    bool isInBoard(Position pos) const;
    bool whiteToMove;

    void updateMaterialScore(int d);
    
private:

    Piece board[8][8];
    int materialScore = 0;
    int PSTScore = 0;
};