#pragma once

#include "Board.h"
#include "Piece.h"

enum Castle {
    NOT, SHORT_CASTLE, LONG_CASTLE
};

enum Player {
    PLAYER_WHITE, PLAYER_BLACK
};

struct Move {
    Player player;

    Position from;
    Position to;
    Piece movePiece;

    bool isPromotion;
    Piece promotionPiece;

    Castle castle;

    bool isEnPassant;
};

void makeMove(Board &board, Move &move);

bool isPawnMoveLegal(const Board &board, const Move &move);

bool isKnightMoveLegal(const Board &board, const Move &move);

bool isBishopMoveLegal(const Board &board, const Move &move);

bool isRookMoveLegal(const Board &board, const Move &move);

bool isQueenMoveLegal(const Board &board, const Move &move);

bool isKingMoveLegal(const Board &board, const Move &move);