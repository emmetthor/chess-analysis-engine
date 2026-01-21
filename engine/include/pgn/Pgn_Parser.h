#pragma once

#include "board/Move.h"
#include <string>
#include <vector>

class PGN {
public:
    void cinPgnToSan();
    void SantoMove();

private:
    std::string whiteName, blackName, result, whiteElo, blackElo;

    std::vector<std::string> san_moves;
};