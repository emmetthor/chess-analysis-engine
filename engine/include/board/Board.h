#pragma once

// WARN player 放這裡怪怪的，之後要將這些 enum class 整合在一起
enum class Player {
    WHITE, BLACK
};

#include "Piece.h"

#include <string>
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

inline bool isInBoard(Position pos) {
    return 0 <= pos.row && pos.row < 8 && 0 <= pos.col && pos.col < 8;
}

inline bool isPlayerValid(Player player) {
    return player == Player::WHITE || player == Player::BLACK;
}

inline Player opponent(Player player) {
    return (player == Player::WHITE ? Player::BLACK : Player::BLACK);
}

inline Piece makePiece(Player player, char pieceChar) {
    return MAKE_PIECE_MAP[static_cast<int>(player)][charToPieceIndex(pieceChar)];
}

inline int playerToIndex(Player player) {
    return static_cast<int>(player);
}

struct Board {
    Board();

    void init();

    Piece at(Position pos) const;
    void set(Position pos, Piece p);
    
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