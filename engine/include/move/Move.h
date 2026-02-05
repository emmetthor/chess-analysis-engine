#pragma once

#include "board/Board.h"
#include "move/Generate_Position.h"
#include "board/Attack.h"

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

    bool isEnPassant = false;

    int prevCastleRights;
    int prevMateralPoints;
    int prevPST;
    uint64_t prevZobrist;

    bool operator==(const Move &other) const {
        return
            player == other.player &&
            from == other.from &&
            to == other.to &&
            movePiece == other.movePiece &&
            capturePiece == other.capturePiece &&
            isPromotion == other.isPromotion && 
            promotionPiece == other.promotionPiece && 
            castle == other.castle && 
            isEnPassant == other.isEnPassant;
    }
};

struct CastleMove {
    Position kingFrom, kingTo;
    Position rookFrom, rookTo;
    Piece kingPiece;
    Piece rookPiece;
};

void printMove(const Move &move);

void moveDebugPrint(const Move &move);

bool isMoveLegal(const Board &board, const Move &move);

bool isCastleLegal(const Board &board, const Move &move);

CastleMove getCastleMove(Move &move);