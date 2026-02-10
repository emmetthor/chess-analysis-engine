#include "evaluate/Material_Point.h"

int pieceValue(const Piece p) {
    switch (p) {
    case Piece::WKING:
    case Piece::BKING:
        break;

    case Piece::WQUEEN:
    case Piece::BQUEEN:
        return 900;
    
    case Piece::WKNIGHT:
    case Piece::BKNIGHT:
        return 320;

    case Piece::WBISHOP:
    case Piece::BBISHOP:
        return 330;

    case Piece::WROOK:
    case Piece::BROOK:
        return 500;

    case Piece::WPAWN:
    case Piece::BPAWN:
        return 100;
    }

    return 0;
}