#include "board/Board.h"
#include "board/Piece.h"
#include "board/Move.h"

void aMove(Board &board, int fromX, int fromY, int toX, int toY, Piece piece) {
    Move m;
    m.from = {fromX, fromY};
    m.to = {toX, toY};
    m.movePiece = piece;

    makeMove(board, m);

    board.debugPrint();
}

int main() {
    Board board;

    aMove(board, 6, 4, 4, 4, WPAWN);

    aMove(board, 1, 3, 3, 3, BPAWN);

    aMove(board, 4, 4, 3, 3, WPAWN);

    aMove(board, 1, 2, 2, 2, BPAWN);

    aMove(board, 3, 3, 2, 4, WPAWN);
    return 0;
}