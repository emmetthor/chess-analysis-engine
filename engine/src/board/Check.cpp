#include "board/Check.h"
#include "board/Board.h"
#include "board/Attack.h"
#include "move/Generate_Move.h"
#include "pgn/Pgn_Transformer.h"

#include "Structure_IO.h"
#include "debug.h"

// 確認 player 方王有沒有被將軍
// WARN 每次都要 O(64) 掃一遍太慢，建議用陣列紀錄棋子位置
bool isInCheck (
    const Board &board,
    const Player player
) {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece king = makePiece(player, 'K');

            if (board.at({r, c}) != king) continue;

            if (isSquareAttacked(board, {r, c}, opponent(player))) {
                return true;
            } else {
                return false;
            }
        }
    }

    return false;
}