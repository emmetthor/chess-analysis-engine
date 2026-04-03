#pragma once
#include "board/Board.h"
#include "move/Move.h"
#include <board/Piece.h>
#include <cstdint>

struct UndoState
{
    uint8_t castleRights;

    int materialScore;
    int PSTScore;

    uint64_t zobristKey;

    Piece capturedPiece;
};

// intergret move infos
// placedPiece: The piece that actually placed at Position to
struct MoveState
{
    Position from, to;

    Piece movePiece, capturedPiece, placedPiece;

    bool isCastle, isPromotion;

    MoveState(const Board &board, const BitMove move)
    {
        from = squareToPosition(getFromSquare(move));
        to = squareToPosition(getToSquare(move));

        movePiece = board.at(from);
        capturedPiece = board.at(to);
        
        isCastle = getCastle(move);
        isPromotion = getPromotion(move);

        if (isPromotion)
        {
            placedPiece = getPromotePiece(move);
        }
        else
        {
            placedPiece = movePiece;
        }
    }
};

void doBitMove(Board &board, const BitMove move, UndoState &undo);

void undoBitMove(Board &board, const BitMove move, const UndoState &undo);