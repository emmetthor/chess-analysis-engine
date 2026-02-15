#include "board/Attack.h"
#include "evaluate/King_Safety.h"
#include "pgn/Pgn_Transformer.h"
#include "Structure_IO.h"

#include <iostream>

Position findKing(const Board &board, Player player) {
    Piece king = makePiece(player, 'K');
    int kingIndex = pieceToIndex(king);
    int kingCount = board.pieceCount[kingIndex];
    const Position *posArray = board.piecePos[kingIndex];

    return posArray[0];
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