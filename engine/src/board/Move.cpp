#include <iostream>
#include <string>

#include "board/Board.h"
#include "board/Piece.h"
#include "board/Move.h"

void printMove(const Move &move) {
    std::cout << pieceToChar(move.movePiece) << pngPosition(move.from) << pngPosition(move.to) << '\n';
}

// 通用檢查走子
bool isMoveLegal(const Board &board, const Move &move) {
    // 檢查是否超出棋盤
    if (!board.isInBoard(move.from)) {
        std::cout << "There are no pieces outside the board\n";
        return false;
    }

    if (!board.isInBoard(move.to)) {
        std::cout << "You can't move the piece outside the board\n";
        return false;
    }

    Piece fromPiece = board.at(move.from);

    // 檢查移動棋子是否為空
    if (fromPiece == EMPTY) {
        std::cout << "You are moving nothing\n";
        return false;
    }

    // 檢查移動棋子是否正確
    if (fromPiece != move.movePiece) {
        std::cout << "Not moving the correct piece: " << pieceToChar(fromPiece) << " != " << pieceToChar(move.movePiece) << '\n';
        return false;
    }

    Piece toPiece = board.at(move.to);

    //檢查是否吃同色棋子
    if (toPiece != EMPTY && isSameColor(move.movePiece, board.at(move.to))) {
        std::cout << "You can't capture your piece: " << pieceToChar(fromPiece) << " !# " << pieceToChar(move.movePiece) << '\n';
        return false;
    }

    switch (move.movePiece) {
        case WPAWN: return isPawnMoveLegal(board, move);
        case BPAWN: return isPawnMoveLegal(board, move);

        default:
        break;
    }

    return true;
}

// 檢查兵走子
bool isPawnMoveLegal(const Board &board, const Move &move) {
    if (move.movePiece != WPAWN && move.movePiece != BPAWN) {
        std::cout << "This is not a pawn, this is a " << pieceToChar(move.movePiece) << '\n';
        return false;
    }

    int moveForWard = abs(move.from.row - move.to.row);
    int moveSideward = abs(move.from.col - move.to.col);

    // 直走
    if (moveSideward == 0) {
        if (moveForWard > 2) {
            std::cout << "Pawn can't move three or more steps at one time\n";
            return false;
        }

        int midPos = -1;
        if (moveForWard == 2) {
            // 白方兵走兩格
            if (isWhite(move.movePiece) && move.from.row != 6) {
                std::cout << "Pawn can move two steps only when stated from the original position\n";
                return false;
            }

            // 黑方兵走兩格
            if (isBlack(move.movePiece) && move.from.row != 1) {
                std::cout << "Pawn can move two steps only when stated from the original position\n";
                return false;
            }

            // 兩格中間那格
            midPos = (move.from.row + move.to.row) / 2;

            if (board.at(move.to) != EMPTY || board.at({move.to.row, midPos}) != EMPTY) {
                std::cout << "Pawn can't capture a piece in front of it\n";
                return false;
            }

            return true;
        }

        if (moveForWard == 1) {
            if (board.at(move.to) != EMPTY) {
                std::cout << "Pawn can't capture a piece in front of it\n";
                return false;
            }

            return true;
        }
    }

    // 斜吃
    else if (moveSideward == 1) {
        if (board.at(move.to) == EMPTY) {
            std::cout << "Pawn can't capure nothing\n";
            return false;
        }

        return true;
    }

    // 亂走
    else {
        std::cout << "Pawn can't move sidewards like that\n";
        return false;
    }

    std::cout << "HOW DID YOU GET HERE\n";
    return false;
}

// 執行 move
void makeMove(Board &board, Move &move) {
    std::cout << "making move:\n";
    printMove(move);

    if (!isMoveLegal(board, move)) {
        return;
    }

    board.set(move.from, EMPTY);
    board.set(move.to, move.movePiece);
}

void pgnToMove(std::string pgn) {

}