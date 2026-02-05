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

    //黑方棋子
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
    castleRights = 0b1111;

    initZobrist();
    zobristKey = computeZobrist(*this, PLAYER_BLACK);
}

void Board::debugPrint() const {
    for (int r = 0; r < 9; r++) {
        for (int c = -1; c < 8; c++) {
            if (r == 8 && c == -1) {
                //debug::log("_ ");
                continue;
            }

            if (c == -1) {
                //debug::log(rowToPgn[r], ' ');
                continue;
            }

            if (r == 8) {
                //debug::log(colToPgn[c], ' ');
                continue;
            }

            //debug::log(pieceToChar(board[r][c]), " \n"[c == 8 - 1]);
        }
    }

    //debug::log('\n');
}

Piece Board::at(Position pos) const {
    return board[pos.row][pos.col];
}

void Board::set(Position pos, Piece p) {
    board[pos.row][pos.col] = p;
}

bool Board::isInBoard(Position pos) const {
    return 0 <= pos.row && pos.row < 8 && 0 <= pos.col && pos.col < 8;
}

std::string pngPosition(const Position pos) {
    std::string result = "";
    result += colToPgn[pos.col];
    result += rowToPgn[pos.row];
    return result;
}

bool samePosition(Position a, Position b) {
    if (a.row == b.row && a.col == b.col) return true;
    else return false;
}

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

void Board::updateMaterialScore(int d, Player player) {
    materialScore += d * (player == PLAYER_WHITE ? 1 : -1);
}

void Board::updatePSTScore(int d, Player player) {
    PSTScore += d * (player == PLAYER_WHITE ? 1 : -1);
}

int Board::getMaterialScore() const { return materialScore; }

int Board::getPSTScore() const { return PSTScore; }