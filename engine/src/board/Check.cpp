#include "board/Check.h"
#include "board/Board.h"
#include "board/Attack.h"
#include "move/Generate_Move.h"
#include "pgn/Pgn_Transformer.h"

#include "Structure_IO.h"
#include "debug.h"

// 確認 player 方王有沒有被將軍
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