#include "board/Attack.h"
#include "evaluate/King_Safety.h"
#include "pgn/Pgn_Transformer.h"

int evaluateKingSafety(const Board &board, Player player) {
    Position kingPos;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board.at({r, c}) == playerPieceCharToPiece(player, 'K')) {
                kingPos = {r, c};
                goto findKing;
            }
        }
    }

    return 0;

    findKing:

    int danger = 0;

    for (auto dr : {-1, -1, -1, 0, 1, 1, 1, 0}) {
        for (auto dc : {-1, 0, 1, 1, -1, 0, 1, -1}) {
            Position p = {kingPos.row + dr, kingPos.col + dc};
            if (!board.isInBoard(p)) continue;

            danger += countSquareAttacks(board, p, opponent(player));
        }
    }

    return -danger * 8;
}