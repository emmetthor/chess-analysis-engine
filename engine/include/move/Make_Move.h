#pragma once

#include "board/Board.h"
#include "move/Move.h"

void makeMove(Board &board, Move &move);

void undoMove(Board &board, Move &move);