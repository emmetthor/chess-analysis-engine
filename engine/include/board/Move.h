#pragma once

#include "Board.h"
#include "Piece.h"

struct Move {
    Position from;
    Position to;
    Piece movePiece;
    Piece capturedPiece;

    bool isPromotion;
    Piece promotionPiece;

    bool isCastling;

    bool isEnPassant;
};

void makeMove(Board &board, Move &move);