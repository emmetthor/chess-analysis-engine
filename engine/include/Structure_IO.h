#pragma once

#include <ostream>
#include "board/Board.h"
#include "board/Piece.h"
#include "move/Move.h"
#include "pgn/Pgn_Transformer.h"

inline std::ostream& operator<<(std::ostream& os, const Position& p) {
    return os << "(" << p.row << "," << p.col << ")";
}

inline std::ostream& operator<<(std::ostream& os, const Board& p) {
    os << '\n';
    for (int r = 0; r < 9; r++) {
        for (int c = -1; c < 8; c++) {
            if (r == 8 && c == -1) {
                os << "  ";
                continue;
            }

            if (c == -1) {
                os << rowToPgn[r] << ' ';
                continue;
            }

            if (r == 8) {
                os << colToPgn[c] << ' ';
                continue;
            }

            os << pieceToChar(p.board[r][c]) << " \n"[c == 8 - 1];
        }
    }

    return os;
}

inline std::ostream& operator<<(std::ostream& os, const Move& move) {
    if (move.castle == SHORT_CASTLE) os << "O-O";
    else if (move.castle == LONG_CASTLE) os << "O-O-O";
    else os << pieceToChar(move.movePiece) << positionToPgn(move.from) << positionToPgn(move.to);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const Player player) {
    return os << playerToIndex(player);
}

inline std::ostream& operator<<(std::ostream& os, const Piece p) {
    return os << pieceToChar(p);
}