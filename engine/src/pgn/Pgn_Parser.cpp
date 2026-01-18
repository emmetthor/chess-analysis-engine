#include "pgn/Pgn_Parser.h"
#include <iostream>
#include <string>

enum Header {
    WHITE, BLACK, 
    WHITEELO, BLACKELO,
};

void PGN::cinPgnToMove() {
    std::cout << "please input your game with PGN:\n";

    std::string input;
    int headerType = -1;
    while (std::cin >> input) {
        if (input[0] == '[') {
            if (input.size() > 5 && input.substr(1, 5) == "White") {
                headerType = WHITE;
                continue;
            }

            if (input.size() > 5 && input.substr(1, 5) == "Black") {
                headerType = BLACK;
                continue;
            }

            if (input.size() > 8 && input.substr(1, 5) == "WhiteElo") {
                headerType = WHITEELO;
                continue;
            }

            if (input.size() > 8 && input.substr(1, 5) == "BlackElo") {
                headerType = BLACKELO;
                continue;
            }

            std::cout << "Don't need this: " << input << '\n';
            continue;
        }

        if (headerType != -1) {
            int sz = input.size();
            switch (headerType)
            {
            case WHITE: whiteName = input.substr(1, sz - 3);
            case BLACK: blackName = input.substr(1, sz - 3);
            
            default:
                break;
            }
        }
    }
}