#include "pgn/Valid_Piece.h"

#include <iostream>
#include <vector>
#include <map>



std::map<char, Piece> pieceCharToPieceType[2] = {
    {
        {'K', WKING},
        {'Q', WQUEEN},
        {'N', WKNIGHT},
        {'B', WBISHOP},
        {'R', WROOK},
        {'P', WPAWN}
    },
    {
        {'K', BKING},
        {'Q', BQUEEN},
        {'N', BKNIGHT},
        {'B', BBISHOP},
        {'R', BROOK},
        {'P', BPAWN}
    }
};

Piece playerPieceCharToPiece(Player player, char piece) {
    return pieceCharToPieceType[player][piece];
}

Position findValidPieceWithColume(Board &board, Piece p, int col) {
    Position res = invalidPosition;
    for (int r = 0; r < 8; r++) {
        if (board.at({r, col}) == p) {
            if (!samePosition(res, invalidPosition)) {
                std::cout << "Multiple pieces at the colume\n";
                return invalidPosition;
            }
            res = {r, col};
        }
    }

    if (samePosition(res, invalidPosition)) {
        std::cout << "Can't find a valid piece\n";
        return invalidPosition;
    }

    return res;
}

Position findValidPieceWithRow(Board &board, Piece p, int row) {
    Position res = invalidPosition;
    for (int c = 0; c < 8; c++) {
        if (board.at({row, c}) == p) {
            if (!samePosition(res, invalidPosition)) {
                std::cout << "Multiple pieces at the colume\n";
                return invalidPosition;
            }
            res = {row, c};
        }
    }

    if (samePosition(res, invalidPosition)) {
        std::cout << "Can't find a valid piece\n";
        return invalidPosition;
    }

    return res;
}