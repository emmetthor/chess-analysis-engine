#pragma once

#include "board/Board.h"

int evaluatePawnStructure(const Board &board, int doublePawnWeight, int isolatedPawnWeight, int passedPawnWeight, int *passedPawnRankWeight);
