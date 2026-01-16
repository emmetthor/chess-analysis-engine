#include <iostream>

#include "board/Board.h"
#include "board/Piece.h"

Board::Board() {
    init();
}

void Board::init() {
    whiteToMove = true; //白方先手

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

    //黑方棋子
    board[7][0] = board[7][7] = WROOK;
    board[7][1] = board[7][6] = WKNIGHT;
    board[7][2] = board[7][5] = WBISHOP;
    board[7][3] = WQUEEN;
    board[7][4] = WKING;
    for (int c = 0; c < 8; c++) {
        board[6][c] = BPAWN;
    }
}

void Board::debugPrint() const {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            std::cout << pieceToChar(board[r][c]) << " \n"[c == 8 - 1];
        }
    }
}

Piece Board::at(Position pos) const {
    return board[pos.row][pos.col];
}

void Board::set(Position pos, Piece p) {
    board[pos.row][pos.col] = p;
}