#include "pgn/Pgn_Parser.h"
#include "board/Board.h"
#include "pgn/Valid_Piece.h"
#include "pgn/Pgn_Transformer.h"

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

std::map<char, bool> specialPiece = {
    {'K', 1},
    {'Q', 1},
    {'N', 1},
    {'B', 1},
    {'R', 1},
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

bool isCapture(std::string s) {
    bool res = 0;

    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] == 'x') res = 1;
    }
    
    return res;
}

SAN parsePieceSan(std::string strSan, Board &board, Player player) {
    SAN san;

    san.piece = playerPieceCharToPiece(player, strSan[0]);

    int sanSize = strSan.size();

    // skip strSan[0]
    int i = 1;

    if (sanSize > 3 && strSan[i] != 'x' && isalpha(strSan[i])) {
        san.fromPos.col = strSan[i] - 'a';
        i++;
    }

    if (strSan[i] != 'x' && isdigit(strSan[i])) {
        san.fromPos.col = 8 - (strSan[i] - '0');
        i++;
    }

    if (strSan[i] == 'x') {
        san.isCapture = true;
        i++;
    }

    san.toPos = pgnToPosition(strSan.substr(i, 2));

    return san;
}

Move parsePieceMove(SAN &san, Board &board) {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board.at({r, c}) != san.piece) continue;
            if (san.fromPos.row != -1 && r != san.fromPos.row) continue;
            if (san.fromPos.col != -1 && c != san.fromPos.col) continue;

            Move move;
            move.from = {r, c};
            move.movePiece = san.piece;
            move.player = isWhite(san.piece) ? PLAYER_WHITE : PLAYER_BLACK;
            move.to = san.toPos;

            if (isMoveLegal(board, move)) {
                return move;
            }
        }
    }

    std::cout << "No valid move\n";

    return Move();
}

void PGN::parsePawnSan(std::string san, Board &board, Player player) {
    // pawn
    Move move;

    Piece piece = playerPieceCharToPiece(player, 'P');
    Position toPos = pgnToPosition(san);

    if (isCapture(san)) {
        
    } else {
        std::vector<Position> validPieces = findValidPieceWithColume(board, piece, toPos.col);

        for (auto pos : validPieces) {
            Move _move;
            _move.from = pos;
            _move.to = toPos;
            _move.movePiece = piece;

            printMove(_move);

            if (isMoveLegal(board, _move)) {
                move.from = pos;
                move.to = toPos;
                move.movePiece = piece;
                break;
            }
        }

        moves.emplace_back(move);

        makeMove(board, move);
    }
}

Move parseCastleSan(std::string san, Player player) {
        Move move;
    move.player = player;

    if (san == "O-O") {
        move.castle = SHORT_CASTLE;
    }

    else if (san == "O-O-O") {
        move.castle = LONG_CASTLE;
    }

    return move;
}

void PGN::SantoMove() {
    Player player = PLAYER_WHITE;
    
    Board board;

    for (auto san : san_moves) {
        std::cout << san << '\n';
        
        if (specialPiece.find(san[0]) != specialPiece.end()) {
            SAN pieceSan = parsePieceSan(san, board, player);
            Move pieceMove = parsePieceMove(pieceSan, board);

            makeMove(board, pieceMove);

            moves.emplace_back(pieceMove);
        }

        else if (san[0] == 'O') {
            // castle
            Move castleMove = parseCastleSan(san, player);

            makeMove(board, castleMove);

            moves.emplace_back(castleMove);
        }

        else {
            // pawn
            parsePawnSan(san, board, player);
        }

        board.debugPrint();
        std::cout << '\n';

        player = (player == PLAYER_WHITE ? PLAYER_BLACK : PLAYER_WHITE);
    }
}