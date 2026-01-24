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
    Piece movePiece;

    bool isPromotion = false;
    Piece promotionPiece;

    Castle castle = NOT;

    bool isEnPassant;
};

void printMove(const Move &move);

void makeMove(Board &board, Move &move);

void moveDebugPrint(const Move &move);

bool isMoveLegal(const Board &board, const Move &move);

bool isPawnMoveLegal(const Board &board, const Move &move);

bool isKnightMoveLegal(const Board &board, const Move &move);

bool isBishopMoveLegal(const Board &board, const Move &move);

bool isRookMoveLegal(const Board &board, const Move &move);

bool isQueenMoveLegal(const Board &board, const Move &move);

bool isKingMoveLegal(const Board &board, const Move &move);