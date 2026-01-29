#pragma once

// 定義棋子，從 0 開始定義每個棋子編號
enum Piece {
    EMPTY = 0,
    WPAWN, WKNIGHT, WBISHOP, WROOK, WQUEEN, WKING,
    BPAWN, BKNIGHT, BBISHOP, BROOK, BQUEEN, BKING
};

char pieceToChar(Piece p);

int isWhite (Piece p);
int isBlack (Piece p);
int isSameColor (Piece p1, Piece p2);
