#include "move/Make_Move.h"
#include "board/Board.h"
#include "pgn/Valid_Piece.h"
#include "search/Zobrist.h"
#include "search/Negamax.h"
#include "evaluate/Material_Point.h"
#include "evaluate/PST.h"
#include "debug.h"

#include <iostream>
#include <assert.h>

// 入堡走子
void castleMove(Board &board, Move &move) {
    CastleMove c = getCastleMove(move);
    Player player = move.player;

    // if (exactDebug) 
    //     std::cout << '(' << c.kingFrom.row << ' ' << c.kingFrom.col << ')' << " -> " << '(' << c.kingTo.row << ' ' << c.kingTo.col << ')'
    //     << " | " << '(' << c.rookFrom.row << ' ' << c.rookFrom.col << ')' << " -> " << '(' << c.rookTo.row << ' ' << c.rookTo.col << ')'
    //     << " | " << c.kingPiece << ' ' << c.rookPiece << '\n';

    board.set(c.kingFrom,   EMPTY);
    board.set(c.kingTo,     c.kingPiece);
    board.set(c.rookFrom,   EMPTY);
    board.set(c.rookTo,     c.rookPiece);

    board.updatePSTScore(-1 * evaluatePieceSquare(c.kingPiece, c.kingFrom), player);
    board.updatePSTScore(+1 * evaluatePieceSquare(c.kingPiece, c.kingTo), player);

    board.updatePSTScore(-1 * evaluatePieceSquare(c.rookPiece, c.rookFrom), player);
    board.updatePSTScore(+1 * evaluatePieceSquare(c.rookPiece, c.rookTo), player);

    board.zobristKey ^= zobPiece[c.kingPiece][zobBoardPosition(c.kingFrom)];
    board.zobristKey ^= zobPiece[c.kingPiece][zobBoardPosition(c.kingTo)];
    board.zobristKey ^= zobPiece[c.rookPiece][zobBoardPosition(c.rookFrom)];
    board.zobristKey ^= zobPiece[c.rookPiece][zobBoardPosition(c.rookTo)];
}

// 還原入堡
void undoCastleMove(Board &board, Move &move) {
    CastleMove c = getCastleMove(move);
    Player player = move.player;

    board.set(c.kingFrom,   c.kingPiece);
    board.set(c.kingTo,     EMPTY);
    board.set(c.rookFrom,   c.rookPiece);
    board.set(c.rookTo,     EMPTY);
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
    // if (!isPositionValid(move.from)) {
    //     std::cerr << "invalid from\n";
    //     std::cerr << move.from.row << ' ' << move.from.col << '\n';
    //     assert(0);
    // }
    // if (!isPositionValid(move.to)) {
    //     std::cerr << "invalid to\n";
    //     assert(0);
    // }

    move.prevCastleRights = board.castleRights;
    move.prevMateralPoints = board.materialScore;
    move.prevPST = board.PSTScore;
    move.prevZobrist = board.zobristKey;

    // 控制 castleRights
    
    board.zobristKey ^= zobCastle[board.castleRights];
    board.castleRights = updateCastleRights(board.castleRights, move);
    board.zobristKey ^= zobCastle[board.castleRights];

    //std::cout << board.castleRights << '\n';

    // 執行 move
    Piece captured = move.capturePiece;
    Piece moved = move.movePiece;
    Player player = move.player;

    if (move.castle == SHORT_CASTLE || move.castle == LONG_CASTLE) {
        castleMove(board, move);
    }

    else if (move.isPromotion) {
        board.set(move.from, EMPTY);
        board.set(move.to, move.promotionPiece);
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

    if (captured != EMPTY) {
        board.updatePSTScore(-1 * evaluatePieceSquare(captured, move.to), opponent(player));
    }

    // 更新Zobrist
    int fromZob = zobBoardPosition(move.from);
    int toZob = zobBoardPosition(move.to);

    board.zobristKey ^= zobPlayer;

    if (move.castle == SHORT_CASTLE || move.castle == LONG_CASTLE) {
        // castleMove 完成
    }

    else if (move.isPromotion) {
        board.zobristKey ^= zobPiece[moved][fromZob];
        board.zobristKey ^= zobPiece[move.promotionPiece][toZob];
    }

    else {
        board.zobristKey ^= zobPiece[moved][fromZob];
        board.zobristKey ^= zobPiece[moved][toZob];
    }

    if (captured != EMPTY) {
        board.zobristKey ^= zobPiece[captured][toZob];
    }

    if (computeZobrist(board, player) != board.zobristKey) {
        printMove(move);
        assert(move == inValidMove);
        assert(0);
    }
}

void undoMove(Board &board, Move &move) {
    board.castleRights = move.prevCastleRights;
    board.materialScore = move.prevMateralPoints;
    board.PSTScore = move.prevPST;
    board.zobristKey = move.prevZobrist;

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
}