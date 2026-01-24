#pragma once

#include "board/Board.h"

#include <string>
#include <map>

extern const char colToPgn[8];
extern const char rowToPgn[8];
extern std::map<char, Piece> pieceCharToPieceType[2];

Piece playerPieceCharToPiece(Player player, char piece);

bool isStringAPosition(std::string s);

Position pgnToPosition(std::string s);

std::string positionToPgn(Position pos);