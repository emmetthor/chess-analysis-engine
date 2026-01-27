#include "board/Generate_Position.h"
#include "board/Board.h"
#include "board/Move.h"
#include "pgn/Pgn_Transformer.h"

static const std::vector<int> ndr = {1, 2, 2, 1, -1, -2, -2, -1}, ndc = {2, 1, -1, -2, -2, -1, 1, 2};
static const std::vector<int> rqdr = {1, -1, 0 , 0}, rqdc = {0, 0, 1, -1};
static const std::vector<int> bqdr = {1, 1, -1 , -1}, bqdc = {1, -1, 1, -1};
static const std::vector<int> kdr = {1, 1, 1, 0, -1, -1, -1, 0}, kdc = {1, 0, -1, -1, -1, 0, 1, 1};

std::vector<Position> generatePosFromPosWithJumpPiece(
    const Board &board,
    const Position &pos,
    const std::vector<int> &dr,
    const std::vector<int> &dc
) {
    std::vector<Position> res;

    for (int i = 0; i < 8; i++) {
        Position p = {pos.row + dr[i], pos.col + dc[i]};
        if (!board.isInBoard(p)) continue;

        res.emplace_back(p);
    }

    return res;
}

std::vector<Position> generatePosFromPosWithSlidePiece(
    const Board &board,
    const Position &pos,
    const std::vector<int> &dr,
    const std::vector<int> &dc
) {
    std::vector<Position> res;

    for (int i = 0; i < 4; i++) {
        Position p = {pos.row + dr[i], pos.col + dc[i]};

        while (true) {
            if (!board.isInBoard(p)) break;

            res.emplace_back(p);

            if (board.at(p) != EMPTY) break;

            p.row += dr[i];
            p.col += dc[i];
        }
    }

    return res;
}

std::vector<Position> generatePiecePosFromPos(
    const Board &board,
    const Position &pos,
    Piece p
) {
    std::vector<Position> res, straight, diagonal;
    switch (p) {
    case WKNIGHT:
    case BKNIGHT:
        res = generatePosFromPosWithJumpPiece(board, pos, ndr, ndc);
        break;

    case WBISHOP:
    case BBISHOP:
        res = generatePosFromPosWithSlidePiece(board, pos, bqdr, bqdc);
        break;

    case WROOK:
    case BROOK:
        res = generatePosFromPosWithSlidePiece(board, pos, rqdr, rqdc);
        break;

    case WQUEEN:
    case BQUEEN:
        straight = generatePosFromPosWithSlidePiece(board, pos, rqdr, rqdc),
        diagonal = generatePosFromPosWithSlidePiece(board, pos, bqdr, bqdc);

        std::copy(diagonal.begin(), diagonal.end(), std::back_inserter(res));
        std::copy(straight.begin(), straight.end(), std::back_inserter(res));
        break;

    case WKING:
    case BKING:
        res = generatePosFromPosWithJumpPiece(board, pos, kdr, kdc);
        break;
    }

    return res;
}