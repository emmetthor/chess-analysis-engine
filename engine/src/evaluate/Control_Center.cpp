#include "evaluate/Control_Center.h"
#include "board/Board.h"
#include "board/Piece.h"
#include "board/Attack.h"

#include <iostream>

Position center[] = {
    {3, 3},
    {3, 4},
    {4, 3},
    {4, 4}
};

int evaluateCenterControl(const Board &board) {
    return 0;

    int whitePawnControl = 0, blackPawnControl = 0;
    int whiteKnightControl = 0, blackKnightControl = 0;
    int whiteOccupy = 0, blackOccupy = 0;

    for (Position pos : center) {
        whitePawnControl += countPawnAttacks(board, pos, PLAYER_WHITE);
        whiteKnightControl += countKnightAttacks(board, pos, PLAYER_WHITE);
        blackPawnControl += countPawnAttacks(board, pos, PLAYER_BLACK);
        blackKnightControl += countKnightAttacks(board, pos, PLAYER_BLACK);

        Piece p = board.at(pos);

        if (p == EMPTY) continue;
        if (isWhite(p)) whiteOccupy++;
        else if (isBlack(p)) blackOccupy++;
    }

    //std::cout << whitePawnControl << ' ' << whiteKnightControl << ' ' << whiteOccupy << '\n';

    return whitePawnControl * 10 + whiteKnightControl * 8 + whiteOccupy * 8
        - blackPawnControl * 10 - blackKnightControl * 8 - blackOccupy * 8;
}