#include "pgn/Pgn_Parser.h"
#include "board/Board.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

enum Header {
    NONE, USELESS,
    WHITE, BLACK, 
    WHITEELO, BLACKELO,
    
};

std::map<std::string, Castle> castleTypeMap = {
    {"O-O", SHORT_CASTLE},
    {"O-O-O", LONG_CASTLE}
};

std::map<std::string, Header> headerTypeMap = {
    {"[White", WHITE},
    {"[Black", BLACK},
    {"[WhiteElo", WHITEELO},
    {"[BlackElo", BLACKELO}
};

void PGN::cinPgnToSan() {
    std::cout << "please input your game with PGN:\n";

    std::string input;
    int headerType = NONE;

    Board board;

    while (std::cin >> input) {
        Move move;

        // 查看 header [xxx 是否符合
        if (headerTypeMap.find(input) != headerTypeMap.end()) {
            headerType = headerTypeMap[input];
            continue;
        }

        if (input[0] == '[') {
            headerType = USELESS;
            std::cout << input << ' ' << "USELESS\n";
            continue;
        }

        // 接續 header
        if (headerType != NONE) {
            int sz = input.size();

            switch (headerType) {
            case WHITE: whiteName = input.substr(1, sz - 3); break;
            case BLACK: blackName = input.substr(1, sz - 3); break;
            case WHITEELO: whiteElo = input.substr(1, sz - 3); break;
            case BLACKELO: blackElo = input.substr(1, sz - 3); break;
            
            default:
                break;
            }

            if (input.back() == ']') {
                headerType = NONE;
            }

            continue;
        }
        
        // 1-0 or 0-1
        if (input.size() == 3 && isdigit(input[0]) && isdigit(input[2])) {
            break;
        }

        // 1. 2. 3. ...
        if (isdigit(input[0])) {
            continue;
        }

        san_moves.push_back(input);
    }

    std::cout << whiteName << ' ' << blackName << ' ' << whiteElo << ' ' << blackElo << '\n';
    for (auto s : san_moves) std::cout << s << '\n';
}