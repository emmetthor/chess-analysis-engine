#pragma GCC optimize("O3,unroll-loops")

#include "board/AttackMap.h"
#include "board/Board.h"
#include "move/Move.h"
#include "move/Generate_Position.h"
#include "pgn/Pgn_Transformer.h"
#include "debug.h"

#include <iostream>
#include <vector>

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
    // debug::log("undefined function\n");
    // return;
    int dd = (add == 1 ? 1 : -1);
    Player player = move.player;
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