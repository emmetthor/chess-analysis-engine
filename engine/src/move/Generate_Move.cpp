#pragma GCC optimize("O3,unroll-loops")

#include "board/Board.h"
#include "board/Piece.h"
#include "move/Move.h"
#include "move/Make_Move.h"
#include "move/Generate_Position.h"
#include "board/Check.h"
#include "debug.h"

int generatePieceMoves(
    const Board &board,
    Piece movePiece,
    Move *buffer
) {
    Player player = board.player;
    ENGINE_ASSERT(isPlayerValid(player));
    ENGINE_ASSERT(movePiece != Piece::EMPTY);

    int cnt = 0;
    Move move;
    move.player = player;
    move.movePiece = movePiece;
    int pieceCount = board.getPieceCount(movePiece);
    const auto *posArray = board.getPiecePos(movePiece);

    Position posBuffer[30];
            
    for (int i = 0; i < pieceCount; i++) {
        Position pos = posArray[i];
        move.from = pos;

        int n = generatePiecePosFromPos(board, pos, movePiece, posBuffer);

        for (int i = 0; i < n; i++) {
            move.to = posBuffer[i];
            move.capturePiece = board.at({move.to});

            buffer[cnt++] = move;
        }
    }

    return cnt;
}

int generatePieceCapture(
    const Board &board,
    Piece movePiece,
    Move *buffer
) {
    Player player = board.player;
    ENGINE_ASSERT(isPlayerValid(player));
    ENGINE_ASSERT(movePiece != Piece::EMPTY);

    int cnt = 0;
    Move move;
    move.player = player;
    move.movePiece = movePiece;
    int pieceCount = board.getPieceCount(movePiece);
    const auto *posArray = board.getPiecePos(movePiece);

    Position posBuffer[30];
            
    for (int i = 0; i < pieceCount; i++) {
        Position pos = posArray[i];
        move.from = pos;

        int n = generatePieceCaptureFromPos(board, pos, movePiece, posBuffer);

        for (int i = 0; i < n; i++) {
            move.to = posBuffer[i];
            move.capturePiece = board.at({move.to});

            buffer[cnt++] = move;
        }
    }

    return cnt;
}

int generateAllMoves(
    const Board &board,
    Move *buffer
) {
    Player player = board.player;
    ENGINE_ASSERT(isPlayerValid(player));
    ENGINE_ASSERT(validatePiecePos(board));

    int cnt = 0;
    Piece 
        pawn    = makePiece(player, 'P'),
        knight  = makePiece(player, 'N'),
        bishop  = makePiece(player, 'B'),
        rook    = makePiece(player, 'R'),
        queen   = makePiece(player, 'Q'),
        king    = makePiece(player, 'K');

    cnt += generatePieceMoves(board, knight, buffer);
    cnt += generatePieceMoves(board, bishop, buffer + cnt);
    cnt += generatePieceMoves(board, rook, buffer + cnt);
    cnt += generatePieceMoves(board, queen, buffer + cnt);
    cnt += generatePieceMoves(board, king, buffer + cnt);

    int dr = (player == Player::WHITE ? -1 : 1);
    int startRank = (player == Player::WHITE ? 6 : 1);
    int promoteRank = (player == Player::WHITE ? 0 : 7);
    int pawnCount = board.getPieceCount(pawn);
    const auto *posArray = board.getPiecePos(pawn);

    for (int i = 0; i < pawnCount; i++) {
        auto [r, c] = posArray[i];
        Move move;
        move.player = player;
        move.from = posArray[i];
        move.movePiece = pawn;
        move.isPromotion = 0;

        // 一步
        move.to = {r + dr, c};
        move.capturePiece = board.at(move.to);

        if (isInBoard(move.to) && move.capturePiece == Piece::EMPTY) {
            if (move.to.row == promoteRank) {
                for (auto promo : {knight, bishop, rook, queen}) {
                    move.isPromotion = 1;
                    move.promotionPiece = promo;
                    buffer[cnt++] = move;
                }
                move.isPromotion = 0;
            } else {
                buffer[cnt++] = move;
            }
        }

        // 兩步
        if (r == startRank) {
            move.to = {r + 2 * dr, c};
            Position mid = {r + dr, c};

            if (isInBoard(move.to) && board.at(move.to) == Piece::EMPTY && board.at(mid) == Piece::EMPTY) {
                move.capturePiece = Piece::EMPTY; // 兩步不能 capture
                buffer[cnt++] = move;
            }
        }

        // capture
        for (int dc : {-1, 1}) {
            Position to = {r + dr, c + dc};
            if (!isInBoard(to)) continue;
            if (board.at(to) == Piece::EMPTY) continue;
            if (isSameColor(board.at(to), pawn)) continue;

            move.to = to;
            move.capturePiece = board.at(to);
            if (to.row == promoteRank) {
                for (auto promo : {knight, bishop, rook, queen}) {
                    move.isPromotion = 1;
                    move.promotionPiece = promo;
                    buffer[cnt++] = move;
                }
                move.isPromotion = 0;
            } else {
                buffer[cnt++] = move;
            }
        }
    }

    // castle
    for (auto len: {SHORT_CASTLE, LONG_CASTLE}) {
        Move moveCastle;
        moveCastle.castle = len;
        moveCastle.player = player;
        moveCastle.capturePiece = Piece::EMPTY;

        CastleMove m = getCastleMove(moveCastle);
        moveCastle.from = m.kingFrom;
        moveCastle.to = m.kingTo;

        buffer[cnt++] = moveCastle;
    }

    return cnt;
}

int generateCaptureMoves(
    const Board &board,
    Move *buffer
) {
    Player player = board.player;
    ENGINE_ASSERT(isPlayerValid(player));
    ENGINE_ASSERT(validatePiecePos(board));
    
    int cnt = 0;
    Piece 
        pawn =      makePiece(player, 'P'),
        knight =    makePiece(player, 'N'),
        bishop =    makePiece(player, 'B'),
        rook =      makePiece(player, 'R'),
        queen =     makePiece(player, 'Q'),
        king =      makePiece(player, 'K');

    cnt += generatePieceCapture(board, knight, buffer);
    cnt += generatePieceCapture(board, bishop, buffer + cnt);
    cnt += generatePieceCapture(board, rook, buffer + cnt);
    cnt += generatePieceCapture(board, queen, buffer + cnt);
    cnt += generatePieceCapture(board, king, buffer + cnt);

    // pawn
    int dr = (player == Player::WHITE ? -1 : 1);
    int startRank = (player == Player::WHITE ? 6 : 1);
    int promoteRank = (player == Player::WHITE ? 0 : 7);
    int pawnCount = board.getPieceCount(pawn);
    const auto *posArray = board.getPiecePos(pawn);

    for (int i = 0; i < pawnCount; i++) {
        auto [r, c] = posArray[i];
        Move move;
        move.player = player;
        move.from = {r, c};
        move.movePiece = pawn;
        move.isPromotion = 0;

        // capture
        for (int dc : {-1, 1}) {
            Position to = {r + dr, c + dc};
            if (!isInBoard(to)) continue;

            Piece atPiece = board.at(to);
            if (atPiece == Piece::EMPTY) continue;
            if (isSameColor(atPiece, pawn)) continue;

            move.to = to;
            move.capturePiece = atPiece;
            if (to.row == promoteRank) {
                for (auto promo : {knight, bishop, rook, queen}) {
                    move.isPromotion = 1;
                    move.promotionPiece = promo;
                    buffer[cnt++] = move;
                }
                move.isPromotion = 0;
            } else {
                buffer[cnt++] = move;
            }
        }
    }

    return cnt;
}

int filterLegalMoves(
    const Board &board,
    Move *allMoves,
    int nAllMoves,
    Move *buffer
) {
    ENGINE_ASSERT(isPlayerValid(player));

    int cnt = 0;
    Board copyBoard = board;

    ENGINE_ASSERT(validatePiecePos(board));

    for (int i = 0; i < nAllMoves; i++) {
        Move &move = allMoves[i];
        //if (!isMoveLegal(board, move)) continue; 已經是正確的

        if (move.castle == SHORT_CASTLE || move.castle == LONG_CASTLE) {
            if (!isCastleLegal(board, move)) {
                continue;
            }
        }

        makeMove(copyBoard, move);

        if (!isInCheck(copyBoard, opponent(copyBoard.player))) {
            buffer[cnt++] = move;
        }

        undoMove(copyBoard, move);
    }

    return cnt;
}

int generateAllLegalMoves(
    const Board &board,
    Move *buffer
) {
    ENGINE_ASSERT(isPlayerValid(board.player));

    Move allMoves[2000];
    int nAll = generateAllMoves(board, allMoves);

    int nLegalMoves = filterLegalMoves(board, allMoves, nAll, buffer);

    return nLegalMoves;
}

int generateLegalCaptureMoves(
    const Board &board,
    Move *buffer
) {
    ENGINE_ASSERT(isPlayerValid(player));
    
    Move captureMoves[2000];
    int ncaptureMoves = generateCaptureMoves(board, captureMoves);

    int nLegalMoves = filterLegalMoves(board, captureMoves, ncaptureMoves, buffer);

    return nLegalMoves;
}