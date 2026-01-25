#pragma once

#include "board/Board.h"

int countSquareAttacks(const Board &board, Position pos, const Player player);
bool isSquareAttacked(const Board &board, Position pos, const Player player);
void printAttackMap(const Board &board, const Player player);

int countPawnAttacks(const Board &board, Position pos, const Player player);
int countKnightAttacks(const Board &board, Position pos, const Player player);