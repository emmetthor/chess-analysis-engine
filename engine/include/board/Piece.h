#pragma once

// 定義棋子，從 0 開始定義每個棋子編號
enum Piece {
    EMPTY = 0,
    WPAWN, WKNIGHT, WBISHOP, WROOK, WQUEEN, WKING,
    BPAWN, BKNIGHT, BBISHOP, BROOK, BQUEEN, BKING
};

char pieceToChar(Piece p);


inline bool isWhite (Piece p) {
    return p >= WPAWN && p <= WKING;
}

inline bool isBlack (Piece p) {
    return p >= BPAWN && p <= BKING;
}