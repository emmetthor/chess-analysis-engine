#include "pgn/Valid_Piece.h"

#include <iostream>
#include <vector>
#include <map>

std::vector<Position> findValidPieceWithColume(Board &board, Piece p, int col) {
    std::vector<Position> res;
    for (int r = 0; r < 8; r++) {
        if (board.at({r, col}) == p) {
            res.push_back({r, col});
        }
    }

    if (res.empty()) {
        std::cout << "No valid piece\n";
    }

    return res;
}

std::vector<Position> findValidPieceWithRow(Board &board, Piece p, int row) {
    std::vector<Position> res;
    for (int c = 0; c < 8; c++) {
        if (board.at({row, c}) == p) {
            res.push_back({row, c});
        }
    }

    return res;
}