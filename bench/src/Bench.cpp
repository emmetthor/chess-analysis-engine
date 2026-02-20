#include "Bench.h"
#include "Mate_In_One.h"
#include "Mate_In_Two.h"

#include <iostream>

int main() {
    std::cout << "========== Mate In One ==========\n";
    testMateInOne(10000);

    // std::cout << "========== Mate In Two ==========\n";
    // testMateInTwo(10000);
}