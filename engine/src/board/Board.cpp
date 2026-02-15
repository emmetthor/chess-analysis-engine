#include <iostream>
#include <string>

#include "board/Board.h"
#include "board/Piece.h"
#include "search/Zobrist.h"
#include "pgn/Pgn_Transformer.h"
#include "debug.h"
#include "Structure_IO.h"

Board::Board() {
    init();
}

void Board::init() {
    player = Player::WHITE;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            board[r][c] = Piece::EMPTY;
        }
    }

    //黑方棋子
    board[0][0] = board[0][7] = Piece::BROOK;
    board[0][1] = board[0][6] = Piece::BKNIGHT;
    board[0][2] = board[0][5] = Piece::BBISHOP;
    board[0][3] = Piece::BQUEEN;
    board[0][4] = Piece::BKING;
    for (int c = 0; c < 8; c++) {
        board[1][c] = Piece::BPAWN;
    }

    //白方棋子
    board[7][0] = board[7][7] = Piece::WROOK;
    board[7][1] = board[7][6] = Piece::WKNIGHT;
    board[7][2] = board[7][5] = Piece::WBISHOP;
    board[7][3] = Piece::WQUEEN;
    board[7][4] = Piece::WKING;
    for (int c = 0; c < 8; c++) {
        board[6][c] = Piece::WPAWN;
    }

    for (int i = 1; i <= 12; i++) {
        pieceCount[i] = 0;
        for (int j = 0; j < 10; j++) {
            piecePos[i][j] = {-1, -1};
        }
    }
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece p = board[r][c];
            if (p == Piece::EMPTY) continue;
            piecePos[pieceToIndex(p)][pieceCount[pieceToIndex(p)]++] = {r, c};
        }
    }

    materialScore = 0;
    PSTScore = 0;
    // 所有 castle 都可能
    castleRights = 0b1111;

    initZobrist();
    zobristKey = computeZobrist(*this, Player::WHITE);
}

// 回傳位於 pos 的 Piece
Piece Board::at(Position pos) const {
    return board[pos.row][pos.col];
}

// 設定 Piece 在 pos 裡
void Board::set(Position pos, Piece p) {
    board[pos.row][pos.col] = p;
}

bool validatePiecePos(const Board &b) {
    for (int i=0;i<8;i++)
        for (int j=0;j<8;j++) {
            Piece p = b.board[i][j];
            if (p==Piece::EMPTY) continue;
            int idx = pieceToIndex(p);
            bool found = false;
            for (int k=0;k<b.pieceCount[idx];k++)
                if (b.piecePos[idx][k] == Position{i,j})
                    found = true;
            if (!found) return false;
        }
    return true;
}