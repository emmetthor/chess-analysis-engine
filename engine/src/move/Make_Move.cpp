#include "move/Make_Move.h"
#include "board/Board.h"
#include "pgn/Valid_Piece.h"
#include "evaluate/Material_Point.h"
#include "evaluate/PST.h"

// 入堡走子
void castleMove(Board &board, Move &move) {
    CastleMove c = getCastleMove(move);
    Player player = move.player;

    board.set(c.kingFrom,   EMPTY);
    board.set(c.kingTo,     c.kingPiece);
    board.set(c.rookFrom,   EMPTY);
    board.set(c.rookTo,     c.rookPiece);

    board.updatePSTScore(-1 * evaluatePieceSquare(c.kingPiece, c.kingFrom), player);
    board.updatePSTScore(+1 * evaluatePieceSquare(c.kingPiece, c.kingTo), player);

    board.updatePSTScore(-1 * evaluatePieceSquare(c.rookPiece, c.rookFrom), player);
    board.updatePSTScore(+1 * evaluatePieceSquare(c.rookPiece, c.rookTo), player);
}

// 還原入堡
void undoCastleMove(Board &board, Move &move) {
    CastleMove c = getCastleMove(move);
    Player player = move.player;

    board.set(c.kingFrom,   c.kingPiece);
    board.set(c.kingTo,     EMPTY);
    board.set(c.rookFrom,   c.rookPiece);
    board.set(c.rookTo,     EMPTY);

    board.updatePSTScore(+1 * evaluatePieceSquare(c.kingPiece, c.kingFrom), player);
    board.updatePSTScore(-1 * evaluatePieceSquare(c.kingPiece, c.kingTo), player);

    board.updatePSTScore(+1 * evaluatePieceSquare(c.rookPiece, c.rookFrom), player);
    board.updatePSTScore(-1 * evaluatePieceSquare(c.rookPiece, c.rookTo), player);
}

int updateCastleRights(int castleRights, const Move &move) {
    Player player = move.player;
    int fromCol = move.from.col;

    // 動 king → 清掉該方所有 castle bit
    if (move.movePiece == playerPieceCharToPiece(player, 'K')) {
        if (player == PLAYER_WHITE) {
            castleRights &= ~0b1100; // 清 bit2 bit3 → 白方 king + queen side
        } else {
            castleRights &= ~0b0011; // 清 bit0 bit1 → 黑方 king + queen side
        }
    }

    // 動 rook → 清掉對應側
    else if (move.movePiece == playerPieceCharToPiece(player, 'R')) {
        if (player == PLAYER_WHITE) {
            if (fromCol == 0) castleRights &= ~0b0100; // WQ
            else if (fromCol == 7) castleRights &= ~0b1000; // WK
        } else {
            if (fromCol == 0) castleRights &= ~0b0001; // BQ
            else if (fromCol == 7) castleRights &= ~0b0010; // BK
        }
    }

    return castleRights;
}

// 執行 move
void makeMove(Board &board, Move &move) {
    printMove(move);

    // 控制 castleRights
    move.prevCastleRights = board.castleRights;
    board.castleRights = updateCastleRights(board.castleRights, move);

    //std::cout << board.castleRights << '\n';

    // 執行 move
    Piece captured = board.at(move.to);
    Piece moved = move.movePiece;
    Player player = move.player;

    move.capturePiece = captured;

    if (move.castle == SHORT_CASTLE || move.castle == LONG_CASTLE) {
        castleMove(board, move);
    }

    else if (move.isPromotion) {
        board.set(move.from, EMPTY);
        board.set(move.to, moved);
    }

    else {
        board.set(move.from, EMPTY);
        board.set(move.to, moved);
    }

    // 更新 material score
    if (captured != EMPTY) {
        board.updateMaterialScore(pieceValue(captured), player);
    }

    if (move.isPromotion) {
        board.updateMaterialScore(pieceValue(move.promotionPiece) - pieceValue(moved), player);
    }

    // 更新 PST

    if (move.castle == SHORT_CASTLE || move.castle == LONG_CASTLE) {
        // castleMove 完成
    }

    else if (move.isPromotion) {
        board.updatePSTScore(-1 * evaluatePieceSquare(moved, move.from), player);
        board.updatePSTScore(+1 * evaluatePieceSquare(move.promotionPiece, move.to), player);
    }

    else {
        board.updatePSTScore(-1 * evaluatePieceSquare(moved, move.from), player);
        board.updatePSTScore(+1 * evaluatePieceSquare(moved, move.to), player);
    }
}

void undoMove(Board &board, Move &move) {
    // 控制 castleRights
    board.castleRights = move.prevCastleRights;

    // 執行 undo
    Piece captured = move.capturePiece;
    Piece moved = move.movePiece;
    Player player = move.player;

    move.capturePiece = captured;

    if (move.castle == SHORT_CASTLE || move.castle == LONG_CASTLE) {
        undoCastleMove(board, move);
    }

    else if (move.isPromotion) {
        board.set(move.from, moved);
        board.set(move.to, captured);
    }

    else {
        board.set(move.from, moved);
        board.set(move.to, captured);
    }

    // 更新 material score
    if (captured != EMPTY) {
        board.updateMaterialScore(-1 * pieceValue(captured), player);
    }

    if (move.isPromotion) {
        board.updateMaterialScore(-1 * (pieceValue(move.promotionPiece) - pieceValue(moved)), player);
    }

    // 更新 PST

    if (move.castle == SHORT_CASTLE || move.castle == LONG_CASTLE) {
        // undoCastleMove 完成
    }

    else if (move.isPromotion) {
        board.updatePSTScore(+1 * evaluatePieceSquare(moved, move.from), player);
        board.updatePSTScore(-1 * evaluatePieceSquare(move.promotionPiece, move.to), player);
    }

    else {
        board.updatePSTScore(+1 * evaluatePieceSquare(moved, move.from), player);
        board.updatePSTScore(-1 * evaluatePieceSquare(moved, move.to), player);
    }
}
