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
#include "Structure_IO.h"

#include <iostream>
#include <chrono>
#include <algorithm>

int main() {
    Debug::level = DebugLevel::INFO;
    LOG_INFO(DebugCategory::BOARD, "Engine start");
    LOG_INFO(DebugCategory::BOARD, "input PGN:...");

//     PGN pgn;
//     pgn.cinPgnToSan();
//     pgn.SantoMove();

//     LOG_INFO(DebugCategory::BOARD, "PGN transform finished");

//     Board board;
//     std::vector<Move> moves = pgn.getMoves(), allLegalMoves;

//     int maxDuration = 0;
//     int cnt = 1;
//     for (auto &move : moves) {
//         Player player = move.player;

//         LOG_INFO(DebugCategory::BOARD, "current board: \n", board);

//         auto start = std::chrono::high_resolution_clock::now();
//         SearchResult res = negamaxRoot(board, 1, player);
//         auto searchEnd = std::chrono::high_resolution_clock::now();
//         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(searchEnd - start).count();
        
//         if (duration > maxDuration) maxDuration = duration;

//         Move best = res.bestMove;
//         int score = res.bestScore;

//         LOG_INFO(DebugCategory::BOARD, "best move: ", best);
//         LOG_INFO(DebugCategory::BOARD, "score: ", score);
//         LOG_INFO(DebugCategory::BOARD, "spent: ", duration, "ms");


//         makeMove(board, move);
//     }

    int cnt = 1;
    PGN pgn;
    Board board;
    Player player = Player::WHITE;
    // debug::set(1);
    // attackMap.debugPrint();
    
    std::string s = "";
    while (s != "end") {
        auto start = std::chrono::high_resolution_clock::now();
        SearchResult res = negamaxRoot(board, 6, player);
        auto searchEnd = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(searchEnd - start).count();

        Move best = res.bestMove;
        int score = res.bestScore;

        LOG_INFO(DebugCategory::BOARD, "best move: ", best);
        LOG_INFO(DebugCategory::BOARD, "score: ", score);
        LOG_INFO(DebugCategory::BOARD, "spent: ", duration, "ms");
        
        makeMove(board, best);

        LOG_INFO(DebugCategory::BOARD, "current board: ", '\n', board);

        std::cin >> s;
        Move oppoMove = pgn.SantoMoveSingle(board, s, opponent(player));

        LOG_INFO(DebugCategory::BOARD, "current board: ", '\n', board);
    }

    return 0;
}