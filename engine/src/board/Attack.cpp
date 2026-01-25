#include "board/Attack.h"
#include "board/Board.h"
#include "board/Move.h"
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
    static const int ndr[] = {1, 2, 2, 1, -1, -2, -2, -1}, ndc[] = {2, 1, -1, -2, -2, -1, 1, 2};
    for (int i = 0; i < 8; i++) {
        Position p = {pos.row + ndr[i], pos.col + ndc[i]};
        if (!board.isInBoard(p)) continue;

        if (board.at(p) == playerPieceCharToPiece(player, 'N')) cnt++;
    }

    // 直線
    static const int rqdr[] = {1, -1, 0 , 0}, rqdc[] = {0, 0, 1, -1};
    for (int i = 0; i < 4; i++) {
        Position p = {pos.row + rqdr[i], pos.col + rqdc[i]};

        while (true) {
            if (!board.isInBoard(p)) break;

            if (board.at(p) == playerPieceCharToPiece(player, 'R')) cnt++;
            if (board.at(p) == playerPieceCharToPiece(player, 'Q')) cnt++;

            if (board.at(p) != EMPTY) break;

            p.row += rqdr[i];
            p.col += rqdc[i];
        }
    } 

    // 斜線
    static const int bqdr[] = {1, 1, -1 , -1}, bqdc[] = {1, -1, 1, -1};
    for (int i = 0; i < 4; i++) {
        Position p = {pos.row + bqdr[i], pos.col + bqdc[i]};

        while (true) {
            if (!board.isInBoard(p)) break;

            if (board.at(p) == playerPieceCharToPiece(player, 'B')) cnt++;
            if (board.at(p) == playerPieceCharToPiece(player, 'Q')) cnt++;

            if (board.at(p) != EMPTY) break;    

            p.row += bqdr[i];
            p.col += bqdc[i];
        }
    }

    // king
    static const int kdr[] = {1, 1, 1, 0, -1, -1, -1, 0}, kdc[] = {1, 0, -1, -1, -1, 0, 1, 1};
    for (int i = 0; i < 8; i++) {
        Position p = {pos.row + kdr[i], pos.col + kdc[i]};
        if (!board.isInBoard(p)) continue;

        if (board.at(p) == playerPieceCharToPiece(player, 'K')) cnt++;
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