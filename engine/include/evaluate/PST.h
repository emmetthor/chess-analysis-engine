#pragma once

#include "board/Board.h"
#include "move/Move.h"

int evaluatePieceSquare(Piece p, Position pp);

int computePST(const Board board);