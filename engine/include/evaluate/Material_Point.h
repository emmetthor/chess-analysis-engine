#pragma once

#include "board/Piece.h"
#include "board/Board.h"

int pieceValue(const Piece p);

int evaluateMaterial(const Board &board);