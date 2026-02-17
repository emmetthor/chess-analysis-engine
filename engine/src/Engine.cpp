#include "Engine.h"

#include "board/Board.h"
#include "fen/FEN_Parser.h"
#include "search/Negamax.h"
#include "move/Move.h"
#include "move/Make_Move.h"

Engine::Engine() {

}

void Engine::setStartPosition() {
    board.init();
}

void Engine::setPositionWithFen(const std::string &fen) {
    board = cinFenToBoard(fen);
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

Move Engine::goDepth(int depth, Evaluate eval) {
    SearchResult result = negamaxRoot(board, depth, board.player, eval);
    return result.bestMove;
}

void Engine::quit() {

}