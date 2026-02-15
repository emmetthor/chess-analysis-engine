#include "move/Generate_Position.h"
#include "evaluate/Mobility.h"
#include "pgn/Pgn_Transformer.h"

static const int MOBILITY_WEIGHT = 4;

int evaluateKnightMobility (
    const Board &board,
    Player player
) {
    int cnt = 0;
    Piece knight = makePiece(player, 'N');
    int knightCount = board.getPieceCount(knight);
    const auto posArray = board.getPiecePos(knight);

    for (int i = 0; i < knightCount; i++) {
        Position knightPositions[32];
        int nKnightPositions = generatePiecePosFromPos(board, posArray[i], knight, knightPositions);

        cnt += nKnightPositions;
    }

    return cnt * MOBILITY_WEIGHT;
}

int evaluateBishopMobility (
    const Board &board,
    Player player
) {
    int cnt = 0;
    Piece bishop = makePiece(player, 'N');
    int bishopCount = board.getPieceCount(bishop);
    const auto posArray = board.getPiecePos(bishop);

    for (int i = 0; i < bishopCount; i++) {
        Position bishopPositions[32];
        int nBishopPositions = generatePiecePosFromPos(board, posArray[i], bishop, bishopPositions);

        cnt += nBishopPositions;
    }

    return cnt * MOBILITY_WEIGHT;
}

int evaluateRookMobility (
    const Board &board,
    Player player
) {
    int cnt = 0;
    Piece rook = makePiece(player, 'N');
    int rookCount = board.getPieceCount(rook);
    const auto posArray = board.getPiecePos(rook);

    for (int i = 0; i < rookCount; i++) {
        Position rookPositions[32];
        int nRookPositions = generatePiecePosFromPos(board, posArray[i], rook, rookPositions);

        cnt += nRookPositions;
    }

    return cnt * MOBILITY_WEIGHT;
}