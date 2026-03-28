#include "evaluate/SEE.h"
#include "board/Attack.h"
#include "evaluate/Material_Point.h"

int SEE(const Board& board, Position capturePos, Piece movePiece, Player player)
{
    if (isSquareAttacked(board, capturePos, opponent(player)))
        return pieceValue(board.at(capturePos)) - pieceValue(movePiece);
    else
        return pieceValue(board.at(capturePos));
}