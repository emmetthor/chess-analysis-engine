#pragma once

#include "board/Board.h"

#include <string>
#include <map>

extern const char colToPgn[8];
extern const char rowToPgn[8];

bool isStringAPosition(std::string s);

Position pgnToPosition(std::string s);

std::string positionToPgn(Position pos);