#include "Bench.h"
#include "Mate_In_One.h"
#include "Mate_In_Two.h"

#include <iostream>

int main() {
    testResult res;

    std::cout << "========== Mate In One ==========\n";
    res += testMateInOne(10000);

    std::cout << "========== Mate In Two ==========\n";
    res += testMateInTwo(10000);

    std::cout << "========== Result ==========\n";
    std::cout << "Failed test cases: " << res.failedTests << '/' << res.totalTests << '\n';
    std::cout << "Accuracy: " << (res.failedTests * 100 / res.totalTests) << "%\n";

    if (res.failedTests != 0) {
        return 1;
    } else {
        return 0;
    }
}