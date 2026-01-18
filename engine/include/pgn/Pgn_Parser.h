#pragma once

#include "board/Move.h"
#include <string>
#include <vector>

class PGN {
public:
    void cinPgnToMove();

private:
    std::string whiteName, blackName, result;
    int whiteElo, blackElo;

    std::vector<Move> moves;
};