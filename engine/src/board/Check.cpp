#include "board/Check.h"
#include "board/Board.h"
#include "board/Attack.h"
#include "pgn/Pgn_Transformer.h"

bool isInCheck(
    const Board &board,
    const Player player
) {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece king = playerPieceCharToPiece(player, 'K');
            if (board.at({r, c}) != king) continue;

            if (isSquareAttacked(board, {r, c}, opponent(player))) {
                return true;
            }
        }
    }

    return false;
}