#include "pgn/Pgn_Transformer.h"
#include "debug.h"

#include <iostream>
#include <map>

const char colToPgn[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
const char rowToPgn[] = {'8', '7', '6', '5', '4', '3', '2', '1'};

bool isStringAPosition(std::string s) {
    if (s.size() != 2) {
        return 0;
    }

    if (0 <= s[0] - 'a' && s[0] - 'a' <= 26 && 0 <= s[1] - '0' && s[1] - '0' <= 9) {
        return 1;
    } else 
        return 0;
}

Position pgnToPosition(std::string s) {
    if (!isStringAPosition(s)) {
        //debug::log("pgnToPosition: Invalid input: ", s, '\n');
        return {-1, -1};
    }

    int col = s[0] - 'a';
    int rol = 8 - (s[1] - '0');

    return {rol, col};
}

std::string positionToPgn(Position pos) {
    std::string res = "";
    res += colToPgn[pos.col];
    res += rowToPgn[pos.row];
    return res;
}