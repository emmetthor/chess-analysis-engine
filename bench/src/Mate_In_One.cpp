#include "../../engine/include/Engine.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

struct TestData {
    std::string fen;
    std::string bestMove;
};

std::vector<TestData> readMateInOneFile(const std::string &filename) {
    std::vector<TestData> res;
    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::string fen, bestMove;
        std::cin >> fen >> bestMove;
        std::cout << fen << ' ' << bestMove << '\n';
    }
    return res;
}

int main() {
    readMateInOneFile("../mateInOne.txt");
    std::cout << "WHAT\n";
    return 0;
}