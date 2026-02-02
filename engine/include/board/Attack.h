#pragma once

#include "board/Board.h"

struct Move;

class AttackMap {
private:
    int attackMap[2][8][8];
public:
    void clear();
    void calculate(const Board &board);
    bool isSquareAttacked(Position pos, const Player player);
    int countSquareAttacks(Position pos, const Player player);
    void debugPrint();
};

int countPawnAttacks(const Board &board, Position pos, const Player player);
int countKnightAttacks(const Board &board, Position pos, const Player player);

int countSquareAttacks(const Board &board, Position pos, const Player player);
bool isSquareAttacked(const Board &board, Position pos, const Player player);

int getAttackers(
    const Board &board,
    Position pos,
    Player player,
    Position* buffer
);