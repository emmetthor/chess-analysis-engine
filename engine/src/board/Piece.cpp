#include "board/Piece.h"

const char PIECE_TO_CHAR[] {
    '.',
    'P', 'N', 'B', 'R', 'Q', 'K',
    'p', 'n', 'b', 'r', 'q', 'k'
};

char pieceToChar(Piece p) {
    return PIECE_TO_CHAR[p];
}