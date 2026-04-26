#pragma once

#include "board/Board.h"

int generatePiecePosFromPos(const Board& board, const Position& pos, const Piece p, Position* buffer);

int generatePieceCaptureFromPos(const Board& board, const Position& pos, const Piece p, Position* buffer);