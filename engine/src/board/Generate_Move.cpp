#pragma GCC optimize("O3,unroll-loops")

#include "board/Board.h"
#include "board/Piece.h"
#include "board/Move.h"
#include "board/Generate_Position.h"
#include "board/Check.h"
#include "board/Attack.h"
#include "pgn/Pgn_Transformer.h"
#include "debug.h"

#include <vector>

int generatePieceMoves(
    const Board &board,
    Player player,
    Piece movePiece,
    Move *buffer
) {
    int cnt = 0;

    Move move;
    move.player = player;
    move.movePiece = movePiece;

    Position posBuffer[30];
            
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Position pos = {r, c};
            Piece p = board.at(pos);

            if (p != movePiece) continue;

            move.from = pos;

            int n = generatePiecePosFromPos(board, pos, movePiece, posBuffer);

            for (int i = 0; i < n; i++) {
                move.to = posBuffer[i];
                move.capturePiece = board.at({move.to});

                buffer[cnt++] = move;
            }
        }
    }

    return cnt;
}

int generatePieceCapture(
    const Board &board,
    Player player,
    Piece movePiece,
    Move *buffer
) {
int cnt = 0;

    Move move;
    move.player = player;
    move.movePiece = movePiece;

    Position posBuffer[30];
            
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Position pos = {r, c};
            Piece p = board.at(pos);

            if (p != movePiece) continue;

            move.from = pos;

            int n = generatePieceCaptureFromPos(board, pos, movePiece, posBuffer);

            for (int i = 0; i < n; i++) {
                move.to = posBuffer[i];
                move.capturePiece = board.at({move.to});

                buffer[cnt++] = move;
            }
        }
    }

    return cnt;
}

int generateAllMoves(
    const Board &board,
    const Player player,
    Move *buffer
) {
    int cnt = 0;
    Piece 
        pawn =      playerPieceCharToPiece(player, 'P'),
        knight =    playerPieceCharToPiece(player, 'N'),
        bishop =    playerPieceCharToPiece(player, 'B'),
        rook =      playerPieceCharToPiece(player, 'R'),
        queen =     playerPieceCharToPiece(player, 'Q'),
        king =      playerPieceCharToPiece(player, 'K');

    cnt += generatePieceMoves(board, player, knight, buffer);
    cnt += generatePieceMoves(board, player, bishop, buffer + cnt);
    cnt += generatePieceMoves(board, player, rook, buffer + cnt);
    cnt += generatePieceMoves(board, player, queen, buffer + cnt);
    cnt += generatePieceMoves(board, player, king, buffer + cnt);

    // pawn
    int dr = (player == PLAYER_WHITE ? -1 : 1);
    int startRank = (player == PLAYER_WHITE ? 6 : 1);
    int promoteRank = (player == PLAYER_WHITE ? 0 : 7);

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board.at({r, c}) != pawn) continue;

            Move move;
            move.player = player;
            move.from = {r, c};
            move.movePiece = pawn;
            move.isPromotion = 0;

            // 一步
            move.to = {r + dr, c};
            move.capturePiece = board.at(move.to);

            if (board.isInBoard(move.to) && move.capturePiece == EMPTY) {
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

                if (board.isInBoard(move.to) && board.at(move.to) == EMPTY && board.at(mid) == EMPTY) {
                    move.capturePiece = EMPTY; // 兩步不能 capture
                    buffer[cnt++] = move;
                }
            }

            // capture
            for (int dc : {-1, 1}) {
                Position to = {r + dr, c + dc};
                if (!board.isInBoard(to)) continue;
                if (board.at(to) == EMPTY) continue;
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
    }

    // castle
    for (auto len: {SHORT_CASTLE, LONG_CASTLE}) {
        Move moveCastle;
        moveCastle.castle = len;
        moveCastle.player = player;
        buffer[cnt++] = moveCastle;
    }

    return cnt;
}

int generateCaptureMoves(
    const Board &board,
    const Player player,
    Move *buffer
) {
    int cnt = 0;
    Piece 
        pawn =      playerPieceCharToPiece(player, 'P'),
        knight =    playerPieceCharToPiece(player, 'N'),
        bishop =    playerPieceCharToPiece(player, 'B'),
        rook =      playerPieceCharToPiece(player, 'R'),
        queen =     playerPieceCharToPiece(player, 'Q'),
        king =      playerPieceCharToPiece(player, 'K');

    cnt += generatePieceCapture(board, player, knight, buffer);
    cnt += generatePieceCapture(board, player, bishop, buffer + cnt);
    cnt += generatePieceCapture(board, player, rook, buffer + cnt);
    cnt += generatePieceCapture(board, player, queen, buffer + cnt);
    cnt += generatePieceCapture(board, player, king, buffer + cnt);

    // pawn
    int dr = (player == PLAYER_WHITE ? -1 : 1);
    int startRank = (player == PLAYER_WHITE ? 6 : 1);
    int promoteRank = (player == PLAYER_WHITE ? 0 : 7);

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board.at({r, c}) != pawn) continue;

            Move move;
            move.player = player;
            move.from = {r, c};
            move.movePiece = pawn;
            move.isPromotion = 0;

            // capture
            for (int dc : {-1, 1}) {
                Position to = {r + dr, c + dc};
                if (!board.isInBoard(to)) continue;

                Piece atPiece = board.at(to);
                if (atPiece == EMPTY) continue;
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
    }

    return cnt;
}

int filterLegalMoves(
    const Board &board,
    const Player player,
    Move *allMoves,
    int nAllMoves,
    Move *buffer
) {
    int cnt = 0;
    Board copyBoard = board;

    for (int i = 0; i < nAllMoves; i++) {
        Move &move = allMoves[i];
        //if (!isMoveLegal(board, move)) continue; 已經是正確的

        if (move.castle == SHORT_CASTLE || move.castle == LONG_CASTLE) {
            if (!isCastleLegal(board, move)) continue;
        }

        makeMove(copyBoard, move);

        if (!isInCheck(copyBoard, player)) {
            buffer[cnt++] = move;
        }

        undoMove(copyBoard, move);
    }

    return cnt;
}

int generateAllLegalMoves(
    const Board &board,
    const Player player,
    Move *buffer
) {
    Move allMoves[2000];
    int nAll = generateAllMoves(board, player, allMoves);

    int nLegalMoves = filterLegalMoves(board, player, allMoves, nAll, buffer);

    return nLegalMoves;
}

int generateLegalCaptureMoves(
    const Board &board,
    const Player player,
    Move *buffer
) {
    Move captureMoves[2000];
    int ncaptureMoves = generateCaptureMoves(board, player, captureMoves);

    int nLegalMoves = filterLegalMoves(board, player, captureMoves, ncaptureMoves, buffer);

    return nLegalMoves;
}