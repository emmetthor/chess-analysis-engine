#pragma GCC optimize("O3,unroll-loops")

#include "board/Attack.h"
#include "board/Board.h"
#include "move/Move.h"
#include "move/Generate_Position.h"
#include "pgn/Pgn_Transformer.h"
#include "debug.h"

#include <iostream>
#include <vector>

static const int ndr[] = {1, 2, 2, 1, -1, -2, -2, -1}, ndc[] = {2, 1, -1, -2, -2, -1, 1, 2};
static const int rqdr[] = {1, -1, 0 , 0}, rqdc[] = {0, 0, 1, -1};
static const int bqdr[] = {1, 1, -1 , -1}, bqdc[] = {1, -1, 1, -1};
static const int kdr[] = {1, 1, 1, 0, -1, -1, -1, 0}, kdc[] = {1, 0, -1, -1, -1, 0, 1, 1};

int countPawnAttacks(const Board &board, Position pos, const Player player) {
    if (isPositionValid(pos)) {
        ENGINE_ASSERT(isPositionValid(pos));
    }

    int cnt = 0;
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
    Piece knight = playerPieceCharToPiece(player, 'N');
    
    for (int i = 0; i < 8; i++) {
        Position p = {pos.row + ndr[i], pos.col + ndc[i]};

        if (board.isInBoard(p) && board.at(p) == knight) cnt++;
    }

    return cnt;
}

int countDiagnalAttacks(const Board &board, Position pos, const Player player) {
    int cnt = 0;
    Piece bishop = playerPieceCharToPiece(player, 'B');
    Piece queen = playerPieceCharToPiece(player, 'Q');

    for (int i = 0; i < 4; i++) {
        Position p = {pos.row + bqdr[i], pos.col + bqdc[i]};

        while (true) {
            if (!board.isInBoard(p)) break;

            int pp = board.at(p);

            if (pp == bishop || pp == queen) cnt++;

            if (pp != EMPTY) break; 

            p.row += bqdr[i];
            p.col += bqdc[i];
        }
    }

    return cnt;
}

int countStraightAttacks(const Board &board, Position pos, const Player player) {
    int cnt = 0;
    Piece rook = playerPieceCharToPiece(player, 'R');
    Piece queen = playerPieceCharToPiece(player, 'Q');

    for (int i = 0; i < 4; i++) {
        Position p = {pos.row + rqdr[i], pos.col + rqdc[i]};

        while (true) {
            if (!board.isInBoard(p)) break;

            int pp = board.at(p);

            if (pp == rook || pp == queen) cnt++;

            if (pp != EMPTY) break; 

            p.row += rqdr[i];
            p.col += rqdc[i];
        }
    }

    return cnt;
}

int PawnAttackers(
    const Board &board,
    Position pos,
    const Player player,
    Position *buffer
) {
    int cnt = 0;
    // pawn
    int pdr = (player == PLAYER_WHITE ? -1 : 1);
    for (int pdc : {-1, 1}) {
        Position p = {pos.row - pdr, pos.col + pdc};
        if (!board.isInBoard(p)) continue;

        if (board.at(p) == playerPieceCharToPiece(player, 'P')) buffer[cnt++] = p;
    }

    return cnt;
}

int KnightAttackers(
    const Board &board,
    Position pos,
    const Player player,
    Position *buffer
) {
    int cnt = 0;
    Piece knight = playerPieceCharToPiece(player, 'N');
    
    for (int i = 0; i < 8; i++) {
        Position p = {pos.row + ndr[i], pos.col + ndc[i]};

        if (board.isInBoard(p) && board.at(p) == knight) buffer[cnt++] = p;
    }

    return cnt;
}

int BishopAttackers(
    const Board &board,
    Position pos,
    const Player player,
    Position *buffer
) {
    int cnt = 0;
    Piece bishop = playerPieceCharToPiece(player, 'B');

    for (int i = 0; i < 4; i++) {
        Position p = {pos.row + bqdr[i], pos.col + bqdc[i]};

        while (true) {
            if (!board.isInBoard(p)) break;

            int pp = board.at(p);

            if (pp == bishop) buffer[cnt++] = p;

            if (pp != EMPTY) break; 

            p.row += bqdr[i];
            p.col += bqdc[i];
        }
    }

    return cnt;
}

int RookAttackers(
    const Board &board,
    Position pos,
    const Player player,
    Position *buffer
) {
    int cnt = 0;
    Piece rook = playerPieceCharToPiece(player, 'R');

    for (int i = 0; i < 4; i++) {
        Position p = {pos.row + rqdr[i], pos.col + rqdc[i]};

        while (true) {
            if (!board.isInBoard(p)) break;

            int pp = board.at(p);

            if (pp == rook) buffer[cnt++] = p;

            if (pp != EMPTY) break; 

            p.row += rqdr[i];
            p.col += rqdc[i];
        }
    }

    return cnt;
}

int QueenAttackers(
    const Board &board,
    Position pos,
    const Player player,
    Position *buffer
) {
    int cnt = 0;
    Piece queen = playerPieceCharToPiece(player, 'Q');

    for (int i = 0; i < 4; i++) {
        Position p = {pos.row + bqdr[i], pos.col + bqdc[i]};

        while (true) {
            if (!board.isInBoard(p)) break;

            int pp = board.at(p);

            if (pp == queen) buffer[cnt++] = p;

            if (pp != EMPTY) break; 

            p.row += bqdr[i];
            p.col += bqdc[i];
        }
    }

    for (int i = 0; i < 4; i++) {
        Position p = {pos.row + rqdr[i], pos.col + rqdc[i]};

        while (true) {
            if (!board.isInBoard(p)) break;

            int pp = board.at(p);

            if (pp == queen) buffer[cnt++] = p;

            if (pp != EMPTY) break; 

            p.row += rqdr[i];
            p.col += rqdc[i];
        }
    }

    return cnt;
}

int countSquareAttacks(const Board &board, Position pos, const Player player) {
    int cnt = 0;
    cnt += countPawnAttacks(board, pos, player);
    cnt += countKnightAttacks(board, pos, player);
    cnt += countDiagnalAttacks(board, pos, player);
    cnt += countStraightAttacks(board, pos, player);

    return cnt;
}

int getAttackers(
    const Board &board,
    Position pos,
    Player player,
    Position* buffer
) {
    int cnt = 0;
    cnt += PawnAttackers(board, pos, player, buffer + cnt);
    cnt += KnightAttackers(board, pos, player, buffer + cnt);
    cnt += BishopAttackers(board, pos, player, buffer + cnt);
    cnt += RookAttackers(board, pos, player, buffer + cnt);
    return cnt;
}

bool isSquareAttacked(const Board &board, Position pos, const Player player) {
    return (countSquareAttacks(board, pos, player) > 0 ? 1 : 0);
}

int attackers(
    const Board &board,
    Position pos,
    const Player player,
    Position *buffer
) {

}