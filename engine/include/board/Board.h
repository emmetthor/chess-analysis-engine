#pragma once

#include "Piece.h"

#include <string>
#include <set>
#include <vector>
#include <cstdint>

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

bool isPositionValid(Position p);

Player opponent(Player player);

std::string pngPosition(const Position pos);

struct Board {
    Board();

    void init();
    void debugPrint() const;

    Piece at(Position pos) const;
    void set(Position pos, Piece p);
    void change(std::vector<std::vector<Piece>> p);

    bool isInBoard(Position pos) const;
    bool whiteToMove;

    void updateMaterialScore(int d, Player player);
    void updatePSTScore(int d, Player player);

    int getMaterialScore() const;
    int getPSTScore() const;

    Piece board[8][8];
    int materialScore;
    int PSTScore;
    Player player;
    
    /*
    0001 黑方 queen side
    0010 黑方 king  side
    0100 白方 queen side
    1000 白方 king  side
    */
    int castleRights;
    uint64_t zobristKey;
};