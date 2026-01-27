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
    Piece knight = playerPieceCharToPiece(player, 'N');
    for (Position knightPos : generatePiecePosFromPos(board, pos, knight)) {
        if (board.at(knightPos) == knight) cnt++;
    }

    // bishop
    Piece bishop = playerPieceCharToPiece(player, 'B');
    for (Position bishopPos : generatePiecePosFromPos(board, pos, bishop)) {
        if (board.at(bishopPos) == bishop) cnt++;
    }

    // rook
    Piece rook = playerPieceCharToPiece(player, 'R');
    for (Position rookPos : generatePiecePosFromPos(board, pos, rook)) {
        if (board.at(rookPos) == rook) cnt++;
    }

    // queen
    Piece queen = playerPieceCharToPiece(player, 'Q');
    for (Position queenPos : generatePiecePosFromPos(board, pos, queen)) {
        if (board.at(queenPos) == queen) cnt++;
    }

    // king
    Piece king = playerPieceCharToPiece(player, 'K');
    for (Position kingPos : generatePiecePosFromPos(board, pos, king)) {
        if (board.at(kingPos) == king) cnt++;
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
    
    for (Position knightPos : generatePiecePosFromPos(board, pos, WKNIGHT)) {
        if (board.at(knightPos) == playerPieceCharToPiece(player, 'N')) cnt++;
    }

    return cnt;
}
