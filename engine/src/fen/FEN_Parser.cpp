#include "fen/FEN_Parser.h"
#include "debug.h"
#include "Structure_IO.h"
#include "search/Zobrist.h"
#include "evaluate/Material_Point.h"
#include "evaluate/PST.h"
#include <iostream>

Board cinFenToBoard() {
    Board board;

    std::string boardStr, player, castling, enpass, halfmove, fullmove;
    if (!(std::cin >> boardStr >> player >> castling >> enpass >> halfmove >> fullmove)) {
        LOG_ERROR(DebugCategory::BOARD, "FEN is incomplete.");
        ENGINE_ASSERT(0);
    }

    int row = 0, col = 0;
    for (auto c : boardStr) {
        if (c == '/') {
            row++;
            col = 0;
        }
        
        else if (isdigit(c)) {
            for (int i = 0; i < c - '0'; i++) {
                board.board[row][col + i] = Piece::EMPTY;
            }
            col += c - '0';
        }

        else {
            board.board[row][col++] = makePiece((isupper(c) ? Player::WHITE : Player::BLACK), c);
        }
    }
    ENGINE_ASSERT(row == 7 && col == 8);

    LOG_DEBUG(DebugCategory::BOARD, "board: \n", board);

    ENGINE_ASSERT(player == "w" || player == "b");

    board.player = (player == "w" ? Player::WHITE : Player::BLACK);

    for (char c : castling) {
        switch (c) {
        case 'K': board.castleRights |= 0b1000; break;
        case 'Q': board.castleRights |= 0b0100; break;
        case 'k': board.castleRights |= 0b0010; break;
        case 'q': board.castleRights |= 0b0001; break;
        case '-': break;
        default:  ENGINE_ASSERT(c == 'K' || c == 'Q' || c == 'k' || c == 'q'); break;
        }
    }

    // 初始化分析參數
    board.materialScore = computePieceValue(board);
    board.PSTScore = computePST(board);
    board.zobristKey = computeZobrist(board, board.player);

    return board;
}