#include "board/Check.h"
#include "board/Board.h"
#include "board/Attack.h"
#include "move/Generate_Move.h"
#include "pgn/Pgn_Transformer.h"

#include "Structure_IO.h"
#include "debug.h"

// Check whether the king of the player is attacked.
bool isInCheck (
    const Board &board,
    const Player player
) {
    Piece king = makePiece(player, 'K');
    int kingCount = board.getPieceCount(king);
    const auto *posArray = board.getPiecePos(king);
    for (int i = 0; i < kingCount; i++) {
        if (isSquareAttacked(board, posArray[i], opponent(player))) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}