#include "board/Board.h"
#include "board/Piece.h"
#include "board/Move.h"
#include "pgn/Pgn_Parser.h"

void aMove(Board &board, int fromX, int fromY, int toX, int toY, Piece piece) {
    Move m;
    m.from = {fromX, fromY};
    m.to = {toX, toY};
    m.movePiece = piece;

    makeMove(board, m);

    board.debugPrint();
}

int main() {
    PGN pgn;
    pgn.cinPgnToMove();

    return 0;
}