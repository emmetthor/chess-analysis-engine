#pragma once

#include "debug.h"

#include <cctype>

// 定義棋子，從 0 開始定義每個棋子編號
enum class Piece {
    EMPTY = 0,
    WPAWN, WKNIGHT, WBISHOP, WROOK, WQUEEN, WKING,
    BPAWN, BKNIGHT, BBISHOP, BROOK, BQUEEN, BKING
};

inline constexpr char PIECE_TO_CHAR[] {
    '.',
    'P', 'N', 'B', 'R', 'Q', 'K',
    'p', 'n', 'b', 'r', 'q', 'k'
};

inline constexpr Piece MAKE_PIECE_MAP[2][6] {
    {Piece::WPAWN, Piece::WKNIGHT, Piece::WBISHOP, Piece::WROOK, Piece::WQUEEN, Piece::WKING},
    {Piece::BPAWN, Piece::BKNIGHT, Piece::BBISHOP, Piece::BROOK, Piece::BQUEEN, Piece::BKING}
};

inline int charToPieceIndex(char c) {
    switch (std::toupper(c)) {
    case 'P': return 0;
    case 'N': return 1;
    case 'B': return 2;
    case 'R': return 3;
    case 'Q': return 4;
    case 'K': return 5;
    default:
        ENGINE_ASSERT(0);
    }
}

inline int pieceToIndex(Piece p) {
    return static_cast<int>(p);
}

inline char pieceToChar(Piece p) {
    return PIECE_TO_CHAR[pieceToIndex(p)];
}

inline bool isWhite(Piece p) {
    ENGINE_ASSERT(p != Piece::EMPTY);
    int 
        wp = pieceToIndex(Piece::WPAWN),
        wk = pieceToIndex(Piece::WKING),
        currentPiece = pieceToIndex(p);
    if (wp <= currentPiece && currentPiece <= wk) return true;
    else return false;
}

inline bool isBlack(Piece p) {
    ENGINE_ASSERT(p != Piece::EMPTY);
    int 
        bp = pieceToIndex(Piece::BPAWN),
        bk = pieceToIndex(Piece::BKING),
        currentPiece = pieceToIndex(p);
    if (bp <= currentPiece && currentPiece <= bk) return true;
    else return false;
}

inline bool isSameColor(Piece p1, Piece p2) {
    ENGINE_ASSERT(p1 != Piece::EMPTY);
    ENGINE_ASSERT(p2 != Piece::EMPTY);
    return isWhite(p1) == isWhite(p2);
}