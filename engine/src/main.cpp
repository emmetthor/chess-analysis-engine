#include "Engine.h"
#include "command/UCI.h"

int main()
{
    Engine engine;

    uciLoop(engine);

    return 0;
}