#include "Engine.h"
#include "UCI/UCI.h"

int main() {
    Debug::level = DebugLevel::INFO;
    LOG_INFO(DebugCategory::BOARD, "engine start");
    Engine engine;

    uciLoop(engine);

    return 0;
}