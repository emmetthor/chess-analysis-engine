#include "board/Board.h"
#include "board/Piece.h"
#include "board/Move.h"

int main() {
    Board board;
    Move m;
    m.from = {1, 0};
    m.to = {3, 0};
    m.movePiece = Piece::BPAWN;

    board.debugPrint();

    makeMove(board, m);

    board.debugPrint();
    return 0;
}