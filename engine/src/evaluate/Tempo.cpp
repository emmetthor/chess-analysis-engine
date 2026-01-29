#include "evaluate/Tempo.h"

const int TEMPO = 10;

int evaluateTempo(const Board &board, const Player player) {
    if (player == PLAYER_WHITE) return TEMPO;
    else return -TEMPO;
}