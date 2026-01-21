#pragma once

#include "board/Board.h"
#include "board/Piece.h"

Piece playerPieceCharToPiece(Player player, char piece);

Position findValidPieceWithColume(Board &board, Piece p, int col);

Position findValidPieceWithRow(Board &board, Piece p, int row);
