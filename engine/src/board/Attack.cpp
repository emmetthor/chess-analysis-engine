#include "board/Attack.h"
#include "board/Board.h"
#include "board/Move.h"
#include "board/Generate_Position.h"
#include "pgn/Pgn_Transformer.h"
#include "debug.h"

#include <iostream>

int countSquareAttacks(const Board &board, Position pos, const Player player) {
    int cnt = 0;

    // pawn
    int pdr = (player == PLAYER_WHITE ? -1 : 1);
    for (int pdc : {-1, 1}) {
        Position p = {pos.row - pdr, pos.col + pdc};
        if (!board.isInBoard(p)) continue;

        if (board.at(p) == playerPieceCharToPiece(player, 'P')) cnt++;
    }

    // knight
    for (Position nPos : generatePosFromPosWithKnight(board, pos)) {
        if (board.at(nPos) == playerPieceCharToPiece(player, 'N')) cnt++;
    }

    // 直線
    for (Position straightPos : generatePosFromPosWithStraight(board, pos)) {
        if (board.at(straightPos)== playerPieceCharToPiece(player, 'R')) cnt++;
        if (board.at(straightPos)== playerPieceCharToPiece(player, 'Q')) cnt++;
    }

    // 斜線
    for (Position diagonalPos : generatePosFromPosWithDiagonal(board, pos)) {
        if (board.at(diagonalPos)== playerPieceCharToPiece(player, 'B')) cnt++;
        if (board.at(diagonalPos)== playerPieceCharToPiece(player, 'Q')) cnt++;
    }

    // king
    for (Position kPos : generatePosFromPosWithKing(board, pos)) {
        if (board.at(kPos) == playerPieceCharToPiece(player, 'K')) cnt++;
    }

    return cnt;
}

bool isSquareAttacked(const Board &board, Position pos, const Player player) {
    if (countSquareAttacks(board, pos, player)) return true;
    else return false;
}

void printAttackMap(const Board &board, const Player player) {
    for (int r = 0; r < 9; r++) {
        for (int c = -1; c < 8; c++) {
            if (r == 8 && c == -1) {
                debug::log("_ ");
                continue;
            }

            if (c == -1) {
                debug::log(rowToPgn[r], ' ');
                continue;
            }

            if (r == 8) {
                debug::log(colToPgn[c], ' ');
                continue;
            }

            debug::log(countSquareAttacks(board, {r, c}, player), " \n"[c == 8 - 1]);
        }
    }

    debug::log('\n');
}

int countPawnAttacks(const Board &board, Position pos, const Player player) {
    int cnt = 0;
    // pawn
    int pdr = (player == PLAYER_WHITE ? -1 : 1);
    for (int pdc : {-1, 1}) {
        Position p = {pos.row - pdr, pos.col + pdc};
        if (!board.isInBoard(p)) continue;

        if (board.at(p) == playerPieceCharToPiece(player, 'P')) cnt++;
    }

    return cnt;
}

int countKnightAttacks(const Board &board, Position pos, const Player player) {
    int cnt = 0;
    
    for (Position nPos : generatePosFromPosWithKnight(board, pos)) {
        if (board.at(nPos) == playerPieceCharToPiece(player, 'N')) cnt++;
    }

    return cnt;
}
