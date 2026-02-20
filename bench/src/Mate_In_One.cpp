#include "../../engine/include/Engine.h"
#include "../../engine/include/UCI/UCI.h"
#include "Mate_In_One.h"
#include "Progress_Bar.h"
#include "Bench.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <iostream>

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

testResult testMateInOne(int testCnt) {
    auto testData = readMateInOneFile("../bench/Mate_In_One.txt");

    testCnt = std::min(testCnt, (int)testData.size());

    int failCnt = 0;
    progressBar bar(testCnt, 1);
    std::vector<std::array<std::string, 3>> failed;
    for (int i = 0; i < testCnt; i++) {
        auto [fen, bestMove] = testData[i];

        Engine engine;
        engine.setPositionWithFen(fen);
        
        std::string retMove = UCIMoveToString(engine.goDepth(2, 0));
        // 引擎應在兩半步內確認一步將殺

        if (retMove != bestMove) {
            failCnt++;
            failed.push_back({fen, bestMove, retMove});
        }

        bar.update(i + 1);
    }

    bar.finish();

    std::cout << "success test cases: " << testCnt - failCnt << " / " << testCnt << '\n';
    std::cout << "failed test cases:\n";
    for (auto [a, b, c] : failed) {
        std::cout << a << " ; expected: " << b << " ; result: " << c << '\n';
    }

    return {testCnt, failCnt};
    return {0, 0};
}