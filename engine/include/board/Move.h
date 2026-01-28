#pragma once

#include "Board.h"
#include "Piece.h"

enum Castle {
    NOT, SHORT_CASTLE, LONG_CASTLE
};

struct Move {
    Player player;

    Position from;
    Position to;
    Piece movePiece, capturePiece = EMPTY;

    bool isPromotion = false;
    Piece promotionPiece = EMPTY;

    Castle castle = NOT;

    bool isEnPassant;
};

void printMove(const Move &move);

void makeMove(Board &board, Move &move);

void undoMove(Board &board, Move &move);

void moveDebugPrint(const Move &move);

bool isMoveLegal(const Board &board, const Move &move);