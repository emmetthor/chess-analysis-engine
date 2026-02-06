#include "board/Board.h"
#include "board/Piece.h"
#include "move/Move.h"
#include "board/Attack.h"
#include "move/Generate_Move.h"
#include "move/Make_Move.h"
#include "board/Check.h"
#include "pgn/Pgn_Parser.h"
#include "evaluate/Evaluate.h"
#include "search/Negamax.h"
#include "debug.h"

#include <iostream>
#include <chrono>
#include <algorithm>

int main() {
    Debug::init();
    Debug::level = DebugLevel::INFO;
    LOG_INFO(DebugCategory::BOARD, "Engine start");
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

    // int maxDuration = 0;
    // int cnt = 1;
    // for (auto &move : moves) {
    //     Player player = move.player;

    //     debug::set(1);
    //     board.debugPrint();
    //     debug::set(0);

    //     auto start = std::chrono::high_resolution_clock::now();
    //     SearchResult res = negamaxRoot(board, 4, player);
    //     auto searchEnd = std::chrono::high_resolution_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(searchEnd - start).count();
        
    //     if (duration > maxDuration) maxDuration = duration;

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

    // std::cout << "max duration: " << maxDuration << "ms\n";

    Position p = {-1, 123};
    LOG_ERROR(DebugCategory::BOARD, p);

    // int cnt = 1;
    // PGN pgn;
    // Board board;
    // Player player = PLAYER_WHITE;
    // // debug::set(1);
    // // attackMap.debugPrint();
    
    // std::string s = "";
    // while (s != "end") {
    //     auto start = std::chrono::high_resolution_clock::now();
    //     SearchResult res = negamaxRoot(board, 6, player);
    //     auto searchEnd = std::chrono::high_resolution_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(searchEnd - start).count();

    //     Move best = res.bestMove;
    //     int score = res.bestScore;

    //     makeMove(board, best);

    //     std::cin >> s;
    //     Move oppoMove = pgn.SantoMoveSingle(board, s, opponent(player));
    // }

    // return 0;
}