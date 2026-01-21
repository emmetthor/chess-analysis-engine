#include "pgn/Pgn_Parser.h"
#include "board/Board.h"
#include "pgn/Valid_Piece.h"

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

std::map<char, bool> specialPiece = {
    {'K', 1},
    {'Q', 1},
    {'N', 1},
    {'B', 1},
    {'R', 1},
};

bool isStringAPosition(std::string s) {
    if (s.size() != 2) {
        return 0;
    }

    if (0 <= s[0] - 'a' && s[0] - 'a' <= 26 && 0 <= s[1] - '0' && s[1] - '0' <= 9) {
        return 1;
    } else 
        return 0;
}

bool isCapture(std::string s) {
    bool res = 0;

    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] == 'x') res = 1;
    }
    
    return res;
}

Position pgnToPosition(std::string s) {
    if (!isStringAPosition(s)) {
        std::cout << "Invalid input: " << s << '\n';
        return {-1, -1};
    }

    int col = 8 - (s[1] - '0');
    int rol = s[0] - 'a';

    return {rol, col};
}

void PGN::SantoMove() {
    Player player = PLAYER_WHITE;
    
    Board board;

    for (auto san : san_moves) {
        Move move;

        if (specialPiece.find(san[0]) != specialPiece.end()) {
            // 存在 special piece

        }

        else if (san[0] == 'O') {
            // castle
        }

        else {
            // pawn

            if (isCapture(san)) {
                int col = san[0] - 'a';
                Piece piece = playerPieceCharToPiece(player, 'P');
                Position fromPosition = findValidPieceWithColume(board, piece, col);
            } else {

            }
        }
    }
}