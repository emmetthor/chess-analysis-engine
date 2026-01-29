#include "board/Board.h"
#include "board/Piece.h"
#include "board/Move.h"
#include "board/Attack.h"
#include "board/Generate_Move.h"
#include "board/Check.h"
#include "pgn/Pgn_Parser.h"
#include "evaluate/Evaluate.h"
#include "evaluate/Negamax.h"
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
    // debug::set(1);
    // debug::log("cinPgnToSan: please input your game with PGN:\n");
    // debug::set(0);

    // PGN pgn;
    // pgn.cinPgnToSan();
    // pgn.SantoMove();

    // debug::set(1);
    // debug::log("Pgn transform finished.\n");
    // debug::set(0);

    // Board board;
    // std::vector<Move> moves = pgn.getMoves(), allLegalMoves;

    // int cnt = 1;
    // for (auto &move : moves) {
    //     Player player = move.player;

    //     debug::set(1);
    //     board.debugPrint();
    //     debug::set(0);

    //     SearchResult res = negamaxRoot(board, 3, player);
    //     Move best = res.bestMove;
    //     int score = res.bestScore;

    //     std::cerr
    //         << cnt++
    //         << ". "
    //         << "current move: "
    //         << pieceToChar(move.movePiece)
    //         << pngPosition(move.from)
    //         << pngPosition(move.to)
    //         << '\n';

    //     std::cerr
    //         << "best move: "
    //         << pieceToChar(best.movePiece)
    //         << pngPosition(best.from)
    //         << pngPosition(best.to)
    //         << " score: "
    //         << score
    //         <<  '\n';

    //     makeMove(board, move);
    // }

    int cnt = 1;
    PGN pgn;
    Board board;
    Player player = PLAYER_WHITE;
    
    std::string s;
    while (s != "end") {
        debug::set(1);
        board.debugPrint();
        debug::set(0);

        SearchResult res = negamaxRoot(board, 4, player);
        Move best = res.bestMove;
        int score = res.bestScore;

        std::cerr
            << "best move: "
            << pieceToChar(best.movePiece)
            << pngPosition(best.from)
            << pngPosition(best.to)
            << " score: "
            << score
            <<  '\n';

        makeMove(board, best);


        std::cin >> s;
        Move oppoMove = pgn.SantoMoveSingle(board, s, opponent(player));

        debug::set(1);
        printMove(oppoMove);
        debug::set(0);
    }

    return 0;
}