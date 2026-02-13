#include "board/Attack.h"
#include "evaluate/King_Safety.h"
#include "pgn/Pgn_Transformer.h"

Position findKing(const Board &board, Player player) {
    Piece king = makePiece(player, 'K');
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board.at({r, c}) == king) return {r, c};
        }
    }
    return {-1,-1}; // 沒找到
}

int evaluateKingSafety(const Board &board, Player player) {
    Position kingPos = findKing(board, player);

    int danger = 0;

    int dr[] = {-1, -1, -1, 0, 1, 1, 1, 0};
    int dc[] = {-1, 0, 1, 1, -1, 0, 1, -1};

    for (int i = 0; i < 8; i++) {
        Position p = {kingPos.row + dr[i], kingPos.col + dc[i]};
        if (!isInBoard(p)) continue;

        danger += countSquareAttacks(board, p, opponent(player));
    }


    return -danger * 8;
}