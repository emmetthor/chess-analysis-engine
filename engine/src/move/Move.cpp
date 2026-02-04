#include "board/Board.h"
#include "board/Piece.h"
#include "move/Move.h"
#include "board/Attack.h"
#include "pgn/Pgn_Transformer.h"
#include "evaluate/Material_Point.h"
#include "evaluate/PST.h"
#include "debug.h"

#include <iostream>
#include <string>
#include <algorithm>
// 檢查兵走子
bool isPawnMoveLegal(const Board &board, const Move &move) {
    int moveForWard = move.from.row - move.to.row;
    int moveSideward = abs(move.from.col - move.to.col);

    if (move.player == PLAYER_WHITE) {
        if (moveForWard < 0) {
            debug::log("isPawnMoveLegal: Pawn can't move backward\n");
            return false;
        }
    }

    if (move.player == PLAYER_BLACK) {
        if (moveForWard > 0) {
            debug::log("isPawnMoveLegal: Pawn can't move backward\n");
            return false;
        }

        moveForWard *= -1;
    }

    // 直走
    if (moveSideward == 0) {
        if (moveForWard > 2) {
            debug::log("isPawnMoveLegal: Pawn can't move three or more steps at one time\n");
            return false;
        }

        int midPos = -1;
        if (moveForWard == 2) {
            // 白方兵走兩格
            if (isWhite(move.movePiece) && move.from.row != 6) {
                debug::log("isPawnMoveLegal: Pawn can move two steps only when stated from the original position\n");
                return false;
            }

            // 黑方兵走兩格
            if (isBlack(move.movePiece) && move.from.row != 1) {
                debug::log("isPawnMoveLegal: Pawn can move two steps only when stated from the original position\n");
                return false;
            }

            // 兩格中間那格
            midPos = (move.from.row + move.to.row) / 2;

            if (board.at(move.to) != EMPTY || board.at({midPos, move.to.col}) != EMPTY) {
                debug::log("isPawnMoveLegal: Pawn can't capture a piece in front of it\n");
                return false;
            }

            return true;
        }

        if (moveForWard == 1) {
            if (board.at(move.to) != EMPTY) {
                debug::log("isPawnMoveLegal: Pawn can't capture a piece in front of it\n");
                return false;
            }

            return true;
        }

        debug::log("Pawn should move\n");
        return false;
    }

    // 斜吃
    else if (moveSideward == 1) {
        if (board.at(move.to) == EMPTY) {
            debug::log("isPawnMoveLegal: Pawn can't capure nothing\n");
            return false;
        }

        if (moveForWard != 1) {
            debug::log("isPawnMoveLegal: Pawn can only take the piece a block diagonally\n");
            return false;
        }

        return true;
    }

    // 亂走
    else {
        debug::log("isPawnMoveLegal: Pawn can't move like that\n");
        return false;
    }

    debug::log("isPawnMoveLegal: HOW DID YOU GET HERE?\n");
    return false;
}

// 檢查騎士走子
bool isKnightMoveLegal(const Board &board, const Move &move) {
    int moveForWard = abs(move.from.row - move.to.row);
    int moveSideward = abs(move.from.col - move.to.col);

    if (moveForWard == 1 && moveSideward == 2) {
        return true;
    }

    if (moveForWard == 2 && moveSideward == 1) {
        return true;
    }

    debug::log("isKnightMoveLegal: knight can't move like that\n");

    return false;
}

// 檢查主教走子
bool isBishopMoveLegal(const Board &board, const Move &move) {
    int moveForWard = abs(move.from.row - move.to.row);
    int moveSideward = abs(move.from.col - move.to.col);

    debug::log("isBishopMoveLegal: from: ", move.from.row, ", ", move.from.col, " to ", move.to.row, ", ", move.to.col, '\n');

    if (moveForWard != moveSideward) {
        debug::log("isBishopMoveLegal: bishop can't move like that\n");
        return false;
    }

    int fromRow = move.from.row, fromCol = move.from.col, toRow = move.to.row, toCol = move.to.col;
    int dr = (toRow - fromRow > 0 ? 1 : -1);
    int dc = (toCol - fromCol > 0 ? 1 : -1);

    bool allEmpty = true;
    for (int r = fromRow + dr, c = fromCol + dc; r != toRow, c != toCol; r += dr, c += dc) {
        debug::log(r, ", ", c, '\n');
        if (board.at({r, c}) != EMPTY) {
            allEmpty = false;
            break;
        }
    }

    if (allEmpty) return true;
    else {
        debug::log("isBishopMoveLegal: a piece blocked the way during the move\n");
        return false;
    }
}

// 檢查城堡走子
bool isRookMoveLegal(const Board &board, const Move &move) {
    int moveForWard = abs(move.from.row - move.to.row);
    int moveSideward = abs(move.from.col - move.to.col);

    if (moveForWard != 0 && moveSideward != 0) {
        debug::log("isRookMoveLegal: Rook can't move like that\n");
        return false;
    }

    if (moveForWard != 0) {
        int fromRow = move.from.row, toRow = move.to.row;
        if (move.from.row > move.to.row) std::swap(fromRow, toRow);

        bool allEmpty = 1;
        for (int r = fromRow + 1; r <= toRow - 1; r++) {
            if (board.at({r, move.from.col}) != EMPTY) {
                allEmpty = 0;
                break;
            }
        }

        if (allEmpty) return true;
        else {
            debug::log("isRookMoveLegal: a piece blocked the way during the move\n");
            return false;
        }
    }

    if (moveSideward != 0) {
        int fromCol = move.from.col, toCol = move.to.col;
        if (move.from.row > move.to.row) std::swap(fromCol, toCol);

        bool allEmpty = 1;
        for (int c = fromCol + 1; c <= toCol - 1; c++) {
            if (board.at({move.from.row, c}) != EMPTY) {
                allEmpty = 0;
                break;
            }
        }

        if (allEmpty) return true;
        else {
            debug::log("isRookMoveLegal: a piece blocked the way during the move\n");
            return false;
        }
    }

    debug::log("isRookMoveLegal: HOW DID YOU GET HERE?\n");

    return false;
}

// 檢查皇后走子
bool isQueenMoveLegal(const Board &board, const Move &move) {
    int moveForWard = abs(move.from.row - move.to.row);
    int moveSideward = abs(move.from.col - move.to.col);

    if (moveForWard == 0 || moveSideward == 0) {
        if (moveForWard != 0) {
            int fromRow = move.from.row, toRow = move.to.row;
            if (move.from.row > move.to.row) std::swap(fromRow, toRow);

            bool allEmpty = 1;
            for (int r = fromRow + 1; r <= toRow - 1; r++) {
                if (board.at({r, move.from.col}) != EMPTY) {
                    allEmpty = 0;
                    break;
                }
            }

            if (allEmpty) return true;
            else {
                debug::log("isQueenMoveLegal: a piece blocked the way during the move\n");
                return false;
            }
        }

        if (moveSideward != 0) {
            int fromCol = move.from.col, toCol = move.to.col;
            if (move.from.row > move.to.row) std::swap(fromCol, toCol);

            bool allEmpty = 1;
            for (int c = fromCol + 1; c <= toCol - 1; c++) {
                if (board.at({move.from.row, c}) != EMPTY) {
                    allEmpty = 0;
                    break;
                }
            }

            if (allEmpty) return true;
            else {
                debug::log("isQueenMoveLegal: a piece blocked the way during the move\n");
                return false;
            }
        }
    }

    if (moveForWard == moveSideward) {
        int fromRow = move.from.row, fromCol = move.from.col, toRow = move.to.row, toCol = move.to.col;
        int dr = (toRow - fromRow > 0 ? 1 : -1);
        int dc = (toCol - fromCol > 0 ? 1 : -1);

        bool allEmpty = true;
        for (int r = fromRow + dr, c = fromCol + dc; r != toRow, c != toCol; r += dr, c += dc) {
            if (board.at({r, c}) != EMPTY) {
                allEmpty = false;
                break;
            }
        }

        if (allEmpty) return true;
        else {
            debug::log("isQueenMoveLegal: a piece blocked the way during the move\n");
            return false;
        }
    }

    debug::log("isQueenMoveLegal: Queen can't move like that\n");
    return false;
}

// 檢查國王走子
bool isKingMoveLegal(const Board &board, const Move &move) {
    int moveForWard = abs(move.from.row - move.to.row);
    int moveSideward = abs(move.from.col - move.to.col);

    if (moveForWard > 1 || moveSideward > 1) {
        debug::log("King can't move two steps\n");
        return false;
    }

    if (isSquareAttacked(board, move.to, opponent(move.player))) {
        debug::log("King can't move to a attacked square\n");
        return false;
    }

    return true;
}

// 檢查入堡
bool isCastleLegal(const Board &board, const Move &move) {
    Player player = move.player;
    if (player == PLAYER_WHITE) {
        if (move.castle == SHORT_CASTLE) {
            if (
                board.at({7, 4}) == WKING &&
                board.at({7, 6}) == EMPTY &&
                board.at({7, 7}) == WROOK &&
                board.at({7, 5}) == EMPTY &&
                isSquareAttacked(board, {7, 4}, opponent(player)) == false &&
                isSquareAttacked(board, {7, 6}, opponent(player)) == false &&
                isSquareAttacked(board, {7, 5}, opponent(player)) == false &&
                (board.castleRights & 0b1000) == 1
            ) {
                return true;
            }
        } else {
            if (
                board.at({7, 4}) == WKING &&
                board.at({7, 2}) == EMPTY &&
                board.at({7, 0}) == WROOK &&
                board.at({7, 3}) == EMPTY &&
                board.at({7, 1}) == EMPTY &&
                isSquareAttacked(board, {7, 4}, opponent(player)) == false &&
                isSquareAttacked(board, {7, 2}, opponent(player)) == false &&
                isSquareAttacked(board, {7, 3}, opponent(player)) == false &&
                (board.castleRights & 0b0100) == 1
            ) {
                return true;
            }
        }
    } else {
        if (move.castle == SHORT_CASTLE) {
            if (
                board.at({0, 4}) == BKING &&
                board.at({0, 6}) == EMPTY &&
                board.at({0, 7}) == BROOK &&
                board.at({0, 5}) == EMPTY &&
                isSquareAttacked(board, {0, 4}, opponent(player)) == false &&
                isSquareAttacked(board, {0, 6}, opponent(player)) == false &&
                isSquareAttacked(board, {0, 5}, opponent(player)) == false &&
                (board.castleRights & 0b0010) == 1
            ) {
                return true;
            }
        } else {
            if (
                board.at({0, 4}) == BKING &&
                board.at({0, 2}) == EMPTY &&
                board.at({0, 0}) == BROOK &&
                board.at({0, 3}) == EMPTY &&
                board.at({0, 1}) == EMPTY &&
                isSquareAttacked(board, {0, 4}, opponent(player)) == false &&
                isSquareAttacked(board, {0, 2}, opponent(player)) == false &&
                isSquareAttacked(board, {0, 3}, opponent(player)) == false &&
                (board.castleRights & 0b0001) == 1
            ) {
                return true;
            }
        }
    }

    return false;
}

bool isPromoteLegal(const Board &board, const Move &move) {
    if (!isPawnMoveLegal(board, move)) return false;

    if (move.promotionPiece == EMPTY) return false;

    if (move.player == PLAYER_WHITE && move.to.row == 0) return true;
    if (move.player == PLAYER_BLACK && move.to.row == 7) return true;

    return false;
}

const Position castleMoveMap[2][3][4] {
    // white
    {
        {},
        // short
        {{7, 4}, {7, 6}, {7, 7}, {7, 5}},
        // long
        {{7, 4}, {7, 2}, {7, 0}, {7, 3}},
    },
    // black
    {
        {},
        // short
        {{0, 4}, {0, 6}, {0, 7}, {0, 5}},
        // long
        {{0, 4}, {0, 2}, {0, 0}, {0, 3}},
    }
};

CastleMove getCastleMove(Move &move) {
    Player player = move.player;
    CastleMove res;

    res.kingFrom    = castleMoveMap[player][move.castle][0];
    res.kingTo      = castleMoveMap[player][move.castle][1];
    res.rookFrom    = castleMoveMap[player][move.castle][2];
    res.rookTo      = castleMoveMap[player][move.castle][3];

    res.kingPiece = playerPieceCharToPiece(player, 'K');
    res.rookPiece = playerPieceCharToPiece(player, 'R');

    return res;
}

void printMove(const Move &move) {
    if (move.castle == SHORT_CASTLE) debug::log("move: O-O\n");
    else if (move.castle == LONG_CASTLE) debug::log("move: O-O-O\n");
    else {
        debug::log("move: ", pieceToChar(move.movePiece), pngPosition(move.from), pngPosition(move.to), '\n');
        // std::cerr
        //     << "move: "
        //     << pieceToChar(move.movePiece)
        //     << pngPosition(move.from)
        //     << pngPosition(move.to)
        //     << '\n';
    }
}

// 通用檢查走子
bool isMoveLegal(const Board &board, const Move &move) {
    // 檢查入堡
    if (move.castle == SHORT_CASTLE || move.castle == LONG_CASTLE) {
        return isCastleLegal(board, move);
    }

    // 檢查升變
    if (move.isPromotion) {
        return isPromoteLegal(board, move);
    }

    // 檢查是否超出棋盤
    if (!board.isInBoard(move.from)) {
        debug::log("isMoveLegal: from postition: (", move.from.row, ", ", move.from.col, ") is out of board\n");
        return false;
    }

    if (!board.isInBoard(move.to)) {
        debug::log("isMoveLegal: to postition: (", move.to.row, ", ", move.to.col, ") is out of board\n");
        return false;
    }

    Piece fromPiece = board.at(move.from);

    // 檢查移動棋子是否為空
    if (fromPiece == EMPTY) {
        debug::log("isMoveLegal: moving EMPTY\n");
        return false;
    }

    // 檢查移動棋子是否正確
    if (fromPiece != move.movePiece) {
        debug::log("isMoveLegal: moving piece: ", pieceToChar(fromPiece), " is not correct. it should be: ", pieceToChar(move.movePiece), '\n');
        return false;
    }

    Piece toPiece = board.at(move.to);

    //檢查是否吃同色棋子
    if (toPiece != EMPTY && isSameColor(move.movePiece, board.at(move.to))) {
        debug::log("isMoveLegal: ", pieceToChar(move.movePiece), " can't capture the same color pieces: ", pieceToChar(toPiece), '\n');
        return false;
    }

    switch (move.movePiece) {
    case WPAWN:
    case BPAWN:
        return isPawnMoveLegal(board, move);

    case WKNIGHT:
    case BKNIGHT:
        return isKnightMoveLegal(board, move);

    case WBISHOP:
    case BBISHOP:
        return isBishopMoveLegal(board, move);

    case WROOK:
    case BROOK:
        return isRookMoveLegal(board, move);

    case WQUEEN:
    case BQUEEN:
        return isQueenMoveLegal(board, move);

    case WKING:
    case BKING:
        return isKingMoveLegal(board, move);

    default:
        break;
    }

    return true;
}
