#include <iostream>
#include <string>

#include "board/Board.h"
#include "board/Piece.h"
#include "search/Zobrist.h"
#include "pgn/Pgn_Transformer.h"
#include "debug.h"

Board::Board() {
    init();
}

void Board::init() {
    player = PLAYER_WHITE;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            board[r][c] = EMPTY;
        }
    }

    //黑方棋子
    board[0][0] = board[0][7] = BROOK;
    board[0][1] = board[0][6] = BKNIGHT;
    board[0][2] = board[0][5] = BBISHOP;
    board[0][3] = BQUEEN;
    board[0][4] = BKING;
    for (int c = 0; c < 8; c++) {
        board[1][c] = BPAWN;
    }

    //白方棋子
    board[7][0] = board[7][7] = WROOK;
    board[7][1] = board[7][6] = WKNIGHT;
    board[7][2] = board[7][5] = WBISHOP;
    board[7][3] = WQUEEN;
    board[7][4] = WKING;
    for (int c = 0; c < 8; c++) {
        board[6][c] = WPAWN;
    }

    materialScore = 0;
    PSTScore = 0;
    // 所有 castle 都可能
    castleRights = 0b1111;

    initZobrist();
    zobristKey = computeZobrist(*this, PLAYER_BLACK);
}

// 回傳位於 pos 的 Piece
Piece Board::at(Position pos) const {
    return board[pos.row][pos.col];
}

// 設定 Piece 在 pos 裡
void Board::set(Position pos, Piece p) {
    board[pos.row][pos.col] = p;
}

// 檢查 pos 是否合法
bool isInBoard(Position pos) {
    return 0 <= pos.row && pos.row < 8 && 0 <= pos.col && pos.col < 8;
}

// 檢查 player 是否合法
bool isPlayerValid(Player player) {
    return player == PLAYER_WHITE || player == PLAYER_BLACK;
}

// 輸出 player 的對手顏色
Player opponent(Player player) {
    return player == PLAYER_WHITE ? PLAYER_BLACK : PLAYER_WHITE;
}

void Board::change(std::vector<std::vector<Piece>> p) {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            board[r][c] = p[r][c];
        }
    }
}