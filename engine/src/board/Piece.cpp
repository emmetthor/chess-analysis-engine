#include "board/Piece.h"

const char PIECE_TO_CHAR[] {
    '.',
    'P', 'N', 'B', 'R', 'Q', 'K',
    'p', 'n', 'b', 'r', 'q', 'k'
};

char pieceToChar(Piece p) {
    return PIECE_TO_CHAR[p];
}

int isWhite (Piece p) {
    if (p == EMPTY) return -1;
    return WPAWN <= p && p <= WKING;
}

int isBlack (Piece p) {
    if (p == EMPTY) return -1;
    return BPAWN <= p && p <= BKING;
}

int isSameColor (Piece p1, Piece p2) {
    if (p1 == EMPTY || p2 == EMPTY) return -1;
    return isWhite(p1) == isWhite(p2);
}
