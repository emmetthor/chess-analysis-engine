#include "evaluate/Tempo.h"



int evaluateTempo(const Board &board, const Player player, const int tempo) {
    if (player == Player::WHITE) return tempo;
    else return -tempo;
}