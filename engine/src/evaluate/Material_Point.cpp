#include "evaluate/Material_Point.h"

int pieceValue(const Piece p) {
    switch (p) {
    case WKING:
    case BKING:
        return 100000;

    case WQUEEN:
    case BQUEEN:
        return 900;
    
    case WKNIGHT:
    case BKNIGHT:
        return 320;

    case WBISHOP:
    case BBISHOP:
        return 330;

    case WROOK:
    case BROOK:
        return 500;

    case WPAWN:
    case BPAWN:
        return 100;
    }

    return 0;
}

int evaluateMaterial(const Board &board) {
    int res = 0;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece p = board.at({r, c});

            if (p == EMPTY) continue;

            if (isWhite(p)) res += pieceValue(p);
            else res -= pieceValue(p);
        }
    }

    return res;
}