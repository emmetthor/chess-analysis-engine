#include "board/Board.h"
#include "board/Piece.h"
#include "board/Move.h"
#include "board/Generate_Position.h"
#include "pgn/Pgn_Transformer.h"

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

                if (!isMoveLegal(board, move)) continue;

                toVec.emplace_back(move);
            }
        }
    }
}

std::vector<Move> generateAllLegalMoves(
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
}