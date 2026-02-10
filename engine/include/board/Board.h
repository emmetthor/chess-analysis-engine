#pragma once

enum Player {
    PLAYER_WHITE, PLAYER_BLACK
};

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

struct Position {
    int row = -1;
    int col = -1;

    bool operator==(const Position &other) const {
        return
            row == other.row &&
            col == other.col;
    }
};

bool isInBoard(Position pos);
bool isPlayerValid(Player player);

Player opponent(Player player);

inline Piece makePiece(Player player, char pieceChar) {
    return MAKE_PIECE_MAP[static_cast<int>(player)][charToPieceIndex(pieceChar)];
}

struct Board {
    Board();

    void init();

    Piece at(Position pos) const;
    void set(Position pos, Piece p);
    void change(std::vector<std::vector<Piece>> p);
    
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