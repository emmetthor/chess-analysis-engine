#include "board/Generate_Position.h"
#include "evaluate/Mobility.h"
#include "pgn/Pgn_Transformer.h"

static const int MOBILITY_WEIGHT = 4;

int evaluateKnightMobility (
    const Board &board,
    Player player
) {
    Piece knight = playerPieceCharToPiece(player, 'N');
    int cnt = 0;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board.at({r, c}) != knight) continue;

            Position knightPositions[32];
            int nKnightPositions = generatePiecePosFromPos(board, {r, c}, knight, knightPositions);

            cnt += nKnightPositions;
        }
    }

    return cnt * MOBILITY_WEIGHT;
}

int evaluateBishopMobility (
    const Board &board,
    Player player
) {
    Piece bishop = playerPieceCharToPiece(player, 'B');
    int cnt = 0;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board.at({r, c}) != bishop) continue;

            Position bishopPositions[32];
            int nBishopPositions = generatePiecePosFromPos(board, {r, c}, bishop, bishopPositions);

            cnt += nBishopPositions;
        }
    }

    return cnt * MOBILITY_WEIGHT;
}

int evaluateRookMobility (
    const Board &board,
    Player player
) {
    Piece rook = playerPieceCharToPiece(player, 'R');
    int cnt = 0;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board.at({r, c}) != rook) continue;

            Position rookPositions[32];
            int nRookPositions = generatePiecePosFromPos(board, {r, c}, rook, rookPositions);

            cnt += nRookPositions;
        }
    }

    return cnt * MOBILITY_WEIGHT;
}