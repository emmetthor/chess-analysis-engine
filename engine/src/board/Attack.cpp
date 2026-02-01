#include "board/Attack.h"
#include "board/Board.h"
#include "board/Move.h"
#include "board/Generate_Position.h"
#include "pgn/Pgn_Transformer.h"
#include "debug.h"

#include <iostream>
#include <vector>

std::vector<Position> generatePawnAttackPosition(const Board &board, const Position pos, Player player) {
    std::vector<Position> res;

    int dr = (player == PLAYER_WHITE ? -1 : 1);
    for (auto dc : {1, -1}) {
        Position p = {pos.row + dr, pos.col + dc};
        
        if (!board.isInBoard(p)) continue;

        res.emplace_back(p);
    }

    return res;
}

void AttackMap::clear() {
    for (auto player : {PLAYER_WHITE, PLAYER_BLACK}) {
        for (int r = 0; r < 8; r++) for (int c = 0; c < 8; c++) {
            attackMap[player][r][c] = 0;
        }
    }
}

void AttackMap::calculate(const Board &board) {
    clear();

    // for (int r = 0; r < 8; r++) {
    //     for (int c = 0; c < 8; c++) {
    //         Piece p = board.at({r, c});
    //         if (p == EMPTY) continue;

    //         Player player = (isWhite(p) ? PLAYER_WHITE : PLAYER_BLACK);

    //         switch (p) {
    //         case WPAWN:
    //         case BPAWN:
    //             for (auto pos : generatePawnAttackPosition(board, {r, c}, player)) {
    //                 if (!board.isInBoard(pos)) continue;

    //                 attackMap[player][pos.row][pos.col]++;
    //             }
    //             break;

    //         default:
    //             for (Position pp : generatePiecePosFromPos(board, {r, c}, p)) {
    //                 attackMap[player][pp.row][pp.col]++;
    //             }
    //             break;
    //         }
    //     }
    // }
}

void updateAttackMap(const Board &board, const Move &move, bool add) {
    debug::log("undefined function\n");
    return;
    // int dd = (add == 1 ? 1 : -1);

    // Player player = move.player;

    // // ====== CASTLING 特判 ======
    // if (move.castle != NOT) {
    //     int row = (player == PLAYER_WHITE ? 7 : 0);

    //     // --- King ---
    //     for (auto &pos : generatePiecePosFromPos(board, move.from, move.movePiece))
    //         attackMap[player][pos.row][pos.col] -= dd;

    //     for (auto &pos : generatePiecePosFromPos(board, move.to, move.movePiece))
    //         attackMap[player][pos.row][pos.col] += dd;

    //     // --- Rook ---
    //     Position rookFrom, rookTo;
    //     if (move.castle == SHORT_CASTLE) {
    //         rookFrom = {row, 7};
    //         rookTo   = {row, 5};
    //     } else {
    //         rookFrom = {row, 0};
    //         rookTo   = {row, 3};
    //     }

    //     Piece rook = (player == PLAYER_WHITE ? WROOK : BROOK);

    //     for (auto &pos : generatePiecePosFromPos(board, rookFrom, rook))
    //         attackMap[player][pos.row][pos.col] -= dd;

    //     for (auto &pos : generatePiecePosFromPos(board, rookTo, rook))
    //         attackMap[player][pos.row][pos.col] += dd;

    //     return;
    // }

    // Piece movePiece = move.movePiece;

    // std::vector<Position> oldAttacks, newAttacks, captureAttacks;

    // if (movePiece == WPAWN || movePiece == BPAWN) {
    //     oldAttacks = generatePawnAttackPosition(board, move.from, player);
    // } else {
    //     oldAttacks = generatePiecePosFromPos(board, move.from, movePiece);
    // }

    // Piece moveToPiece = (move.isPromotion ? move.promotionPiece : movePiece);

    // if (moveToPiece == WPAWN || moveToPiece == BPAWN) {
    //     newAttacks = generatePawnAttackPosition(board, move.to, player);
    // } else {
    //     newAttacks = generatePiecePosFromPos(board, move.to, moveToPiece);
    // }

    // if (move.capturePiece != EMPTY) {
    //     if (move.capturePiece == WPAWN || move.capturePiece == BPAWN) {
    //         captureAttacks = generatePawnAttackPosition(board, move.to, opponent(player));
    //     } else {
    //         captureAttacks = generatePiecePosFromPos(board, move.to, move.capturePiece);
    //     }
    // }

    // for (const auto &pos : oldAttacks) attackMap[player][pos.row][pos.col] -= dd;
    // for (const auto &pos : newAttacks) attackMap[player][pos.row][pos.col] += dd;
    // for (const auto &pos : captureAttacks) attackMap[opponent(player)][pos.row][pos.col] -= dd;
}

bool AttackMap::isSquareAttacked(Position pos, const Player player) {
    return attackMap[player][pos.row][pos.col];
}
int AttackMap::countSquareAttacks(Position pos, const Player player) {
    return attackMap[player][pos.row][pos.col];
}

void AttackMap::debugPrint() {
    for (auto player : {PLAYER_WHITE, PLAYER_BLACK}) {
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                debug::log(attackMap[player][r][c], " \n"[c == 8 - 1]);
            }
        }
    }
}

static const int ndr[] = {1, 2, 2, 1, -1, -2, -2, -1}, ndc[] = {2, 1, -1, -2, -2, -1, 1, 2};
static const int rqdr[] = {1, -1, 0 , 0}, rqdc[] = {0, 0, 1, -1};
static const int bqdr[] = {1, 1, -1 , -1}, bqdc[] = {1, -1, 1, -1};
static const int kdr[] = {1, 1, 1, 0, -1, -1, -1, 0}, kdc[] = {1, 0, -1, -1, -1, 0, 1, 1};

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

int countSquareAttacks(const Board &board, Position pos, const Player player) {
    int cnt = 0;
    cnt += countPawnAttacks(board, pos, player);
    cnt += countKnightAttacks(board, pos, player);
    cnt += countDiagnalAttacks(board, pos, player);
    cnt += countStraightAttacks(board, pos, player);

    return cnt;
}

bool isSquareAttacked(const Board &board, Position pos, const Player player) {
    return (countSquareAttacks(board, pos, player) > 0 ? 1 : 0);
}