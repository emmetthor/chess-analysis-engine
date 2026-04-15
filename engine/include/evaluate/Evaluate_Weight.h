struct Evaluate_Weights
{
    int kingSafetyWeight = 0;
    int knightMobilityWeight = 0;
    int bishopMobilityWeight = 0;
    int rookMobilityWeight = 0;
    int castleWeight = 0;
    int doublePawnWeight = 0;
    int isolatedPawnWeight = 0;
    int passPawnWeight = 0;
    int passPawnRankWeight[7] = {0, 0, 0, 0, 0, 0, 0};
    int tempo = 0;
};