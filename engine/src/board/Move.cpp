#include <string>

#include "board/Board.h"
#include "board/Piece.h"
#include "board/Move.h"

void isMoveLegal(const Move &move) {

}

void makeMove(Board &board, Move &move) {
    board.set(move.from, EMPTY);
    board.set(move.to, move.movePiece);
}

void pgnToMove(std::string pgn) {

}