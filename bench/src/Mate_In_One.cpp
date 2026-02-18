#include "../../engine/include/Engine.h"
#include "../../engine/include/UCI/UCI.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <iostream>

struct TestData {
    std::string fen;
    std::string bestMove;
};

std::vector<TestData> readMateInOneFile(const std::string &filename) {
    std::vector<TestData> res;

    std::ifstream infile(filename);
    if (infile.fail()) {
        std::cout << "failed to open file: " << filename << '\n';
    }

    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty()) continue;

        std::istringstream ss(line);
        std::string fen, bestMove;

        std::getline(ss, fen, ',');
        std::getline(ss, bestMove, ',');

        res.push_back({fen, bestMove});
    }
    return res;
}

int main() {
    std::cout.tie(0);
    std::ios::sync_with_stdio(0);
    auto testData = readMateInOneFile("../bench/Mate_In_One.txt");

    int testCnt = testData.size();
    int failCnt = 0;
    std::vector<std::array<std::string, 3>> failed;
    for (int i = 0; i < testCnt; i++) {
        auto [fen, bestMove] = testData[i];

        Engine engine;
        engine.setPositionWithFen(fen);
        
        Evaluate eval;
        std::string retMove = UCIMoveToString(engine.goDepth(3, eval));
        // 引擎應在三半步內確認一步將殺

        if (retMove != bestMove) {
            failCnt++;
            failed.push_back({fen, bestMove, retMove});
        }
    }

    std::cout << "test complete.\n";
    std::cout << "success test cases: " << testCnt - failCnt << " / " << testCnt << '\n';
    std::cout << "failed test cases:\n";
    for (auto [a, b, c] : failed) {
        std::cout << a << " ; expected: " << b << " ; result: " << c << '\n';
    }

    return 0;
}