#include "Engine.h"

#include "board/Board.h"
#include "fen/FEN_Parser.h"
#include "search/Search.h"
#include "move/Move.h"
#include "move/Make_Move.h"

Engine::Engine() {

}

void Engine::setStartPosition() {
    board.init();
}

void Engine::setPositionWithFen(const std::string &fen) {
    //board = cinFenToBoard(fen);
}

void Engine::checkReady() {

}

Board Engine::getBoard() {
    return board;
}

void Engine::move(Move move) {
    makeMove(board, move);
}

void Engine::changePlayer() {
    board.player = opponent(board.player);
}

void Engine::setPlayer(Player player) {
    board.player = player;
}

Move Engine::goDepth(int depth, bool isPrintInfo) {
    Search search(eval);
    auto res = search.findBestMove(board, depth);

    if (isPrintInfo) {
        printInfo(res.info);
    }

    return res.bestMove;
}

SearchResult Engine::fullInfoSearch(int depth) {
    Search search(eval);
    return search.findBestMove(board, depth);
}

void Engine::quit() {

}