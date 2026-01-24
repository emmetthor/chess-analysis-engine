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
};

struct SAN {
    Piece piece, promorePiece;
    bool isCapture = false;
    bool isPromote = false;
    Position toPos, fromPos;
};