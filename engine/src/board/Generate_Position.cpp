#include "board/Generate_Position.h"
#include "board/Board.h"
#include "board/Move.h"
#include "pgn/Pgn_Transformer.h"

static const int ndr[] = {1, 2, 2, 1, -1, -2, -2, -1}, ndc[] = {2, 1, -1, -2, -2, -1, 1, 2};
static const int rqdr[] = {1, -1, 0 , 0}, rqdc[] = {0, 0, 1, -1};
static const int bqdr[] = {1, 1, -1 , -1}, bqdc[] = {1, -1, 1, -1};
static const int kdr[] = {1, 1, 1, 0, -1, -1, -1, 0}, kdc[] = {1, 0, -1, -1, -1, 0, 1, 1};

int generatePosFromPosWithJumpPiece(
    const Board &board,
    const Position &pos,
    Piece movePiece,
    const int dr[],
    const int dc[],
    Position *buffer
) {
    int cnt = 0;

    for (int i = 0; i < 8; i++) {
        Position p = {pos.row + dr[i], pos.col + dc[i]};
        Piece atPiece = board.at(p);

        if (!board.isInBoard(p)) continue;
        if (atPiece != EMPTY && isSameColor(atPiece, movePiece)) continue;

        buffer[cnt++] = p;
    }

    return cnt;
}

int generatePosFromPosWithSlidePiece(
    const Board &board,
    const Position &pos,
    Piece movePiece,
    const int dr[],
    const int dc[],
    Position *buffer
) {
    int cnt = 0;

    for (int i = 0; i < 4; i++) {
        Position p = {pos.row + dr[i], pos.col + dc[i]};

        while (true) {
            if (!board.isInBoard(p)) break;
            Piece atPiece = board.at(p);
            if (atPiece != EMPTY && isSameColor(atPiece, movePiece)) break;

            buffer[cnt++] = p;
            if (atPiece != EMPTY) break;

            p.row += dr[i];
            p.col += dc[i];
        }
    }

    return cnt;
}

int generateCaptureFromPosWithJumpPiece(
    const Board &board,
    const Position &pos,
    Piece movePiece,
    const int dr[],
    const int dc[],
    Position *buffer
) {
    int cnt = 0;

    for (int i = 0; i < 8; i++) {
        Position p = {pos.row + dr[i], pos.col + dc[i]};
        Piece atPiece = board.at(p);

        if (!board.isInBoard(p)) continue;
        if (atPiece == EMPTY) continue;
        if (isSameColor(atPiece, movePiece)) continue;

        buffer[cnt++] = p;
    }

    return cnt;
}

int generateCaptureFromPosWithSlidePiece(
    const Board &board,
    const Position &pos,
    Piece movePiece,
    const int dr[],
    const int dc[],
    Position *buffer
) {
    int cnt = 0;

    for (int i = 0; i < 4; i++) {
        Position p = {pos.row + dr[i], pos.col + dc[i]};

        while (true) {
            if (!board.isInBoard(p)) break;
            Piece atPiece = board.at(p);

            if (atPiece != EMPTY && isSameColor(atPiece, movePiece)) break;

            if (atPiece != EMPTY) {
                buffer[cnt++] = p;
                break;
            }

            p.row += dr[i];
            p.col += dc[i];
        }
    }

    return cnt;
}

int generatePiecePosFromPos(
    const Board &board,
    const Position &pos,
    Piece p,
    Position *buffer
) {
    switch (p) {
    case WKNIGHT:
    case BKNIGHT:
        return generatePosFromPosWithJumpPiece(board, pos, p, ndr, ndc, buffer);

    case WBISHOP:
    case BBISHOP:
        return generatePosFromPosWithSlidePiece(board, pos, p, bqdr, bqdc, buffer);

    case WROOK:
    case BROOK:
        return generatePosFromPosWithSlidePiece(board, pos, p, rqdr, rqdc, buffer);

    case WQUEEN:
    case BQUEEN: {
        int n1 = generatePosFromPosWithSlidePiece(board, pos, p, rqdr, rqdc, buffer);
        int n2 = generatePosFromPosWithSlidePiece(board, pos, p, bqdr, bqdc, buffer + n1);
        return n1 + n2;
    }

    case WKING:
    case BKING:
        return generatePosFromPosWithJumpPiece(board, pos, p, kdr, kdc, buffer);
    }

    return 0;
}

int generatePieceCaptureFromPos(
    const Board &board,
    const Position &pos,
    Piece p,
    Position *buffer
) {
    switch (p) {
    case WKNIGHT:
    case BKNIGHT:
        return generateCaptureFromPosWithJumpPiece(board, pos, p, ndr, ndc, buffer);

    case WBISHOP:
    case BBISHOP:
        return generateCaptureFromPosWithSlidePiece(board, pos, p, bqdr, bqdc, buffer);

    case WROOK:
    case BROOK:
        return generateCaptureFromPosWithSlidePiece(board, pos, p, rqdr, rqdc, buffer);

    case WQUEEN:
    case BQUEEN: {
        int n1 = generateCaptureFromPosWithSlidePiece(board, pos, p, rqdr, rqdc, buffer);
        int n2 = generateCaptureFromPosWithSlidePiece(board, pos, p, bqdr, bqdc, buffer + n1);
        return n1 + n2;
    }

    case WKING:
    case BKING:
        return generateCaptureFromPosWithJumpPiece(board, pos, p, kdr, kdc, buffer);
    }

    return 0;
}