#include "evaluate/Material_Point.h"

int pieceValue(const Piece p) {
    switch (p) {
    case WKING:
    case BKING:
        break;

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