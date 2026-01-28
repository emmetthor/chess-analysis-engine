#include "board/Board.h"
#include "board/Piece.h"
#include "board/Move.h"
#include "board/Attack.h"
#include "board/Generate_Move.h"
#include "pgn/Pgn_Parser.h"
#include "evaluate/Evaluate.h"
#include "debug.h"

void aMove(Board &board, int fromX, int fromY, int toX, int toY, Piece piece) {
    Move m;
    m.from = {fromX, fromY};
    m.to = {toX, toY};
    m.movePiece = piece;

    makeMove(board, m);

    board.debugPrint();
}

int main() {
    debug::set(1);
    debug::log("cinPgnToSan: please input your game with PGN:\n");
    debug::set(0);

    PGN pgn;
    pgn.cinPgnToSan();
    pgn.SantoMove();

    debug::set(1);
    debug::log("Pgn transform finished.\n");
    debug::set(0);

    Board board;
    std::vector<Move> moves = pgn.getMoves(), allLegalMoves;

    for (int i = 0; i < pgn.getMovesCount(); i++) {
        makeMove(board, moves[i]);

        std::cout << evaluate(board) << '\n';
    }

    return 0;
}