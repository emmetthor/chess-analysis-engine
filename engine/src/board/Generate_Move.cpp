#include "board/Board.h"
#include "board/Piece.h"
#include "board/Move.h"
#include "board/Generate_Position.h"
#include "board/Check.h"
#include "pgn/Pgn_Transformer.h"
#include "debug.h"

#include <vector>

void generatePieceMoves(
    const Board &board,
    Player player,
    std::vector<Move> &toVec,
    Piece movePiece
) {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Position pos = {r, c};
            Piece p = board.at(pos);

            if (p != movePiece) continue;

            Move move;
            move.from = pos;
            move.movePiece = p;
            move.player = player;

            for (auto piecePos : generatePiecePosFromPos(board, pos, movePiece)) {
                move.to = piecePos;
                move.capturePiece = board.at({piecePos});

                if (!isMoveLegal(board, move)) continue;

                toVec.emplace_back(move);
            }
        }
    }
}

std::vector<Move> generateAllMoves(
    const Board &board,
    const Player player
) {
    std::vector<Move> moves;

    Piece 
        pawn = playerPieceCharToPiece(player, 'P'),
        knight = playerPieceCharToPiece(player, 'N'),
        bishop = playerPieceCharToPiece(player, 'B'),
        rook = playerPieceCharToPiece(player, 'R'),
        queen = playerPieceCharToPiece(player, 'Q'),
        king = playerPieceCharToPiece(player, 'K');

    generatePieceMoves(board, player, moves, knight);
    generatePieceMoves(board, player, moves, bishop);
    generatePieceMoves(board, player, moves, rook);
    generatePieceMoves(board, player, moves, queen);
    generatePieceMoves(board, player, moves, king);

    // pawn
    int dr = (player == PLAYER_WHITE ? -1 : 1);

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board.at({r, c}) != pawn) continue;

            // 一步
            Move moveAStep;
            moveAStep.player = player;
            moveAStep.from = {r, c};
            moveAStep.to = {r + dr, c};
            moveAStep.movePiece = pawn;
            moveAStep.capturePiece = board.at(moveAStep.to);

            moves.emplace_back(moveAStep);

            // promote
            for (auto promotePiece : {knight, bishop, rook, queen}) {
                moveAStep.isPromotion = 1;
                moveAStep.promotionPiece = promotePiece;

                moves.emplace_back(moveAStep);
            }

            // 兩步
            Move moveTwoStep;
            moveTwoStep.player = player;
            moveTwoStep.from = {r, c};
            moveTwoStep.to = {r + 2 * dr, c};
            moveTwoStep.movePiece = pawn;
            moveTwoStep.capturePiece = board.at(moveTwoStep.to);

            moves.emplace_back(moveTwoStep);

            // capture
            for (auto dc : {-1, 1}) {
                Move moveCapture;
                moveCapture.from = {r, c};
                moveCapture.to = {r + dr,  c + dc};
                moveCapture.movePiece = pawn;

                moves.emplace_back(moveCapture);

                // promote
                for (auto promotePiece : {knight, bishop, rook, queen}) {
                    moveCapture.isPromotion = 1;
                    moveCapture.promotionPiece = promotePiece;

                    moves.emplace_back(moveCapture);
                }
            }
        }
    }

    return moves;
}

std::vector<Move> filterMoves(
    const Board &board,
    const Player player,
    std::vector<Move> allMoves
) {
    std::vector<Move> res;
    Board copyBoard = board;
    for (auto move : allMoves) {
        if (!isMoveLegal(board, move)) continue;

        debug::log("here\n");

        makeMove(copyBoard, move);
        if (!isInCheck(copyBoard, player)) {
            res.emplace_back(move);
        } else {
            debug::log("check\n");
        }
        undoMove(copyBoard, move);
    }

    return res;
}

std::vector<Move> generateAllLegalMoves(
    const Board &board,
    const Player player
) {
    return filterMoves(board, player, generateAllMoves(board, player));
}