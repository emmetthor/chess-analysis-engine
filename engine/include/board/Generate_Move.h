#pragma once

#include "board/Board.h"
#include "board/Piece.h"
#include "board/Move.h"
#include "board/Generate_Position.h"

#include <vector>

int generateAllLegalMoves(
    const Board &board,
    const Player player,
    Move *buffer
);

int generateCaptureMoves(
    const Board &board,
    const Player player,
    Move *buffer
);