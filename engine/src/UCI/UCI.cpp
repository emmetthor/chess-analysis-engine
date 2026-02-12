#include "UCI/UCI.h"
#include "move/Move.h"
#include "pgn/Pgn_Transformer.h"
#include "UCI/UCI_Move_Parcer.h"
#include "Structure_IO.h"

#include <string>
#include <sstream>
#include <iostream>

std::string castleMap[2][3] {
    {"invalid", "e1g1", "e1c1"},
    {"invalid", "e8g8", "e8c8"}
};

void printUCIMove(const Move& move) {
    if (move.castle == SHORT_CASTLE || move.castle == LONG_CASTLE) {
        std::cout << castleMap[playerToIndex(move.player)][static_cast<int>(move.castle)];
    } else {
        std::cout << positionToPgn(move.from) << positionToPgn(move.to);
    }

    if (move.isPromotion) {
        std::cout << std::tolower(pieceToChar(move.promotionPiece));
    }
}


void handleGo(std::istringstream &iss, Engine &engine) {
    int depth = 6;
    std::string token;

    while (iss >> token) {
        if (token == "depth") {
            iss >> depth;
        }
    }

    Move move = engine.goDepth(depth);

    std::cout << "bestmove ";
    printUCIMove(move);
    std::cout << '\n' << std::flush;
}

void handlePosition(std::istringstream &iss, Engine &engine) {
    std::string token, fen;
    iss >> token;

    if (token == "startpos") {
        engine.setStartPosition();
        iss >> token;
    }
    if (token == "fen") {
        for (int t = 0; t < 6; t++) {
            iss >> token;
            if (!fen.empty()) fen += " ";
            fen += token;
        }
        engine.setPositionWithFen(fen);
        iss >> token;
    }

    if (token == "moves") {
        engine.setPlayer(Player::WHITE);
        std::string strMove;
        while (iss >> strMove) {
            Move move = parseUCIMove(strMove, engine.getBoard());
            engine.move(move);
            engine.changePlayer();
        }
    }
}

void uciLoop(Engine &engine) {
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "uci") {
            std::cout << "id name engine1\nid author EmmetThor\n";
            std::cout << "uciok\n" << std::flush;
        }
        else if (token == "isready") {
            std::cout << "readyok\n" << std::flush;
        }
        else if (token == "position") {
            handlePosition(iss, engine);
        }
        else if (token == "go") {
            handleGo(iss, engine);
        }
        else if (token == "quit" || token == "stop") {
            break;
        }
        else if (token == "PRINTBOARD") {
            std::cout << engine.getBoard() << '\n';
        }
        else {
            std::cerr << "Unrecognized token.\n" << std::flush;
        }
    }
}