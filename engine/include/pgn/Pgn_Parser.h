#pragma once

#include "board/Move.h"
#include "board/Piece.h"

#include <string>
#include <vector>

class PGN {
public:
    void cinPgnToSan();
    void SantoMove();

private:
    std::string whiteName, blackName, result, whiteElo, blackElo;

    std::vector<std::string> san_moves;

    std::vector<Move> moves;

    void parsePawnSan(std::string san, Board &board, Player player);
};

struct SAN {
    Piece piece;
    bool isCapture = false;
    Position toPos, fromPos;
};