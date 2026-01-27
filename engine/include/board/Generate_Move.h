#pragma once

#include "board/Board.h"
#include "board/Piece.h"
#include "board/Move.h"
#include "board/Generate_Position.h"

#include <vector>

std::vector<Move> generateAllLegalMoves(
    const Board &board,
    const Player Player
);