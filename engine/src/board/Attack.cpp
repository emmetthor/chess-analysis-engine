#pragma GCC optimize("O3,unroll-loops")

#include "board/Attack.h"
#include "board/Board.h"
#include "move/Move.h"
#include "move/Generate_Position.h"
#include "pgn/Pgn_Transformer.h"
#include "debug.h"

#include <iostream>
#include <vector>

// 棋子移動的固定 map
// jump piece
static const int KNIGHT_DR[]        = {  1,  2,  2,  1, -1, -2, -2, -1};
static const int KNIGHT_DC[]        = {  2,  1, -1, -2, -2, -1,  1,  2};
static const int KING_DR[]          = {  1,  1,  1,  0, -1, -1, -1,  0};
static const int KING_DC[]          = {  1,  0, -1, -1, -1,  0,  1,  1};
// slide piece
static const int ROOK_QUEEN_DR[]    = {  1, -1,  0,  0};
static const int ROOK_QUEEN_DC[]    = {  0,  0,  1, -1};
static const int BISHOP_QUEEN_DR[]  = {  1,  1, -1, -1};
static const int BISHOP_QUEEN_DC[]  = {  1, -1,  1, -1};

int countPawnAttacks(
    const Board &board,
    Position pos,
    const Player player
) {
    // 假設 pos 與 player 合法
    // 枚舉所有兵的合法走法 O(2)
    int cnt = 0;
    int pdr = (player == PLAYER_WHITE ? -1 : 1);
    for (int pdc : {-1, 1}) {
        Position p = {pos.row - pdr, pos.col + pdc};
        if (!isInBoard(p)) continue;

        if (board.at(p) == playerPieceCharToPiece(player, 'P')) cnt++;
    }

    return cnt;
}

int countKingAttacks(
    const Board &board,
    Position pos,
    const Player player
) {
    // 假設 pos 與 player 合法
    // 枚舉所有王的合法走法 O(8)
    int cnt = 0;
    Piece king = playerPieceCharToPiece(player, 'K');
    
    for (int i = 0; i < 8; i++) {
        Position p = {pos.row + KING_DR[i], pos.col + KING_DC[i]};

        if (isInBoard(p) && board.at(p) == king) cnt++;
    }

    return cnt;
}

int countKnightAttacks(
    const Board &board,
    Position pos,
    const Player player
) {
    // 假設 pos 與 player 合法
    // 枚舉所有騎士的合法走法 O(8)
    int cnt = 0;
    Piece knight = playerPieceCharToPiece(player, 'N');
    
    for (int i = 0; i < 8; i++) {
        Position p = {pos.row + KNIGHT_DR[i], pos.col + KNIGHT_DC[i]};

        if (isInBoard(p) && board.at(p) == knight) cnt++;
    }

    return cnt;
}

int countDiagnalAttacks(
    const Board &board,
    Position pos,
    const Player player
) {
    // 假設 pos 與 player 合法
    // 枚舉所有斜向的合法走法 O(16)
    int cnt = 0;
    Piece bishop = playerPieceCharToPiece(player, 'B');
    Piece queen = playerPieceCharToPiece(player, 'Q');

    for (int i = 0; i < 4; i++) {
        Position p = {pos.row + BISHOP_QUEEN_DR[i], pos.col + BISHOP_QUEEN_DC[i]};

        while (true) {
            if (!isInBoard(p)) break;

            int pp = board.at(p);

            if (pp == bishop || pp == queen) cnt++;

            if (pp != EMPTY) break; 

            p.row += BISHOP_QUEEN_DR[i];
            p.col += BISHOP_QUEEN_DC[i];
        }
    }

    return cnt;
}

int countStraightAttacks(
    const Board &board,
    Position pos,
    const Player player
) {
    // 假設 pos 與 player 合法
    // 枚舉所有直向的合法走法 O(16)
    int cnt = 0;
    Piece rook = playerPieceCharToPiece(player, 'R');
    Piece queen = playerPieceCharToPiece(player, 'Q');

    for (int i = 0; i < 4; i++) {
        Position p = {pos.row + ROOK_QUEEN_DR[i], pos.col + ROOK_QUEEN_DC[i]};

        while (true) {
            if (!isInBoard(p)) break;

            int pp = board.at(p);

            if (pp == rook || pp == queen) cnt++;

            if (pp != EMPTY) break; 

            p.row += ROOK_QUEEN_DR[i];
            p.col += ROOK_QUEEN_DC[i];
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
    // 假設 pos 與 player 合法，且 buffer 空間足夠
    // 枚舉所有兵的合法走法 O(2)
    int cnt = 0;
    int pdr = (player == PLAYER_WHITE ? -1 : 1);
    for (int pdc : {-1, 1}) {
        Position p = {pos.row - pdr, pos.col + pdc};
        if (!isInBoard(p)) continue;

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
    // 假設 pos 與 player 合法，且 buffer 空間足夠
    // 枚舉所有騎士的合法走法 O(8)
    int cnt = 0;
    Piece knight = playerPieceCharToPiece(player, 'N');
    
    for (int i = 0; i < 8; i++) {
        Position p = {pos.row + KNIGHT_DR[i], pos.col + KNIGHT_DC[i]};

        if (isInBoard(p) && board.at(p) == knight) buffer[cnt++] = p;
    }

    return cnt;
}

int BishopAttackers(
    const Board &board,
    Position pos,
    const Player player,
    Position *buffer
) {
    // 假設 pos 與 player 合法，且 buffer 空間足夠
    // 枚舉所有主教的合法走法 O(16)
    int cnt = 0;
    Piece bishop = playerPieceCharToPiece(player, 'B');

    for (int i = 0; i < 4; i++) {
        Position p = {pos.row + BISHOP_QUEEN_DR[i], pos.col + BISHOP_QUEEN_DC[i]};

        while (true) {
            if (!isInBoard(p)) break;

            int pp = board.at(p);

            if (pp == bishop) buffer[cnt++] = p;

            if (pp != EMPTY) break; 

            p.row += BISHOP_QUEEN_DR[i];
            p.col += BISHOP_QUEEN_DC[i];
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
    // 假設 pos 與 player 合法，且 buffer 空間足夠
    // 枚舉所有城堡的合法走法 O(16)
    int cnt = 0;
    Piece rook = playerPieceCharToPiece(player, 'R');

    for (int i = 0; i < 4; i++) {
        Position p = {pos.row + ROOK_QUEEN_DR[i], pos.col + ROOK_QUEEN_DC[i]};

        while (true) {
            if (!isInBoard(p)) break;

            int pp = board.at(p);

            if (pp == rook) buffer[cnt++] = p;

            if (pp != EMPTY) break; 

            p.row += ROOK_QUEEN_DR[i];
            p.col += ROOK_QUEEN_DC[i];
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
    // 假設 pos 與 player 合法，且 buffer 空間足夠
    // 枚舉所有皇后的合法走法 O(32)
    int cnt = 0;
    Piece queen = playerPieceCharToPiece(player, 'Q');

    for (int i = 0; i < 4; i++) {
        Position p = {pos.row + BISHOP_QUEEN_DR[i], pos.col + BISHOP_QUEEN_DC[i]};

        while (true) {
            if (!isInBoard(p)) break;

            int pp = board.at(p);

            if (pp == queen) buffer[cnt++] = p;

            if (pp != EMPTY) break; 

            p.row += BISHOP_QUEEN_DR[i];
            p.col += BISHOP_QUEEN_DC[i];
        }
    }

    for (int i = 0; i < 4; i++) {
        Position p = {pos.row + ROOK_QUEEN_DR[i], pos.col + ROOK_QUEEN_DC[i]};

        while (true) {
            if (!isInBoard(p)) break;

            int pp = board.at(p);

            if (pp == queen) buffer[cnt++] = p;

            if (pp != EMPTY) break; 

            p.row += ROOK_QUEEN_DR[i];
            p.col += ROOK_QUEEN_DC[i];
        }
    }

    return cnt;
}

// 計算此格子中有多少攻擊
int countSquareAttacks(
    const Board &board,
    Position pos,
    const Player player
) {
    // 檢查位置合法性
    if (!isInBoard(pos)) {
        LOG_ERROR(DebugCategory::ATK, "position is invalid", pos);
        ENGINE_ASSERT(isInBoard(pos));
    }
    // 檢查玩家合法性
    if (!isPlayerValid(player)) {
        LOG_ERROR(DebugCategory::ATK, "player is invalid", player);
        ENGINE_ASSERT(isPlayerValid(player));
    }

    int cnt = 0;
    cnt += countPawnAttacks(board, pos, player);
    cnt += countKnightAttacks(board, pos, player);
    cnt += countDiagnalAttacks(board, pos, player);
    cnt += countStraightAttacks(board, pos, player);
    cnt += countKingAttacks(board, pos, player);
    return cnt;
}

// 給 buffer 填充此格子的攻擊者位置
// WARN 未寫 King 的 get attackers
int getAttackers(
    const Board &board,
    Position pos,
    Player player,
    Position* buffer
) {
    // 檢查位置合法性
    if (!isInBoard(pos)) {
        LOG_ERROR(DebugCategory::ATK, "position is invalid", pos);
        ENGINE_ASSERT(isInBoard(pos));
    }
    // 檢查玩家合法性
    if (!isPlayerValid(player)) {
        LOG_ERROR(DebugCategory::ATK, "player is invalid", player);
        ENGINE_ASSERT(isPlayerValid(player));
    }

    int cnt = 0;
    cnt += PawnAttackers(board, pos, player, buffer + cnt);
    cnt += KnightAttackers(board, pos, player, buffer + cnt);
    cnt += BishopAttackers(board, pos, player, buffer + cnt);
    cnt += RookAttackers(board, pos, player, buffer + cnt);
    return cnt;
}

// 判斷此格子是否被攻擊
bool isSquareAttacked(
    const Board &board,
    Position pos, const
    Player player
) {
    // 檢查位置合法性
    if (!isInBoard(pos)) {
        LOG_ERROR(DebugCategory::ATK, "position is invalid", pos);
        ENGINE_ASSERT(isInBoard(pos));
    }
    // 檢查玩家合法性
    if (!isPlayerValid(player)) {
        LOG_ERROR(DebugCategory::ATK, "player is invalid", player);
        ENGINE_ASSERT(isPlayerValid(player));
    }

    return (countSquareAttacks(board, pos, player) > 0 ? 1 : 0);
}

