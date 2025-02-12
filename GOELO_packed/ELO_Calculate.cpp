#include <iostream>
#include <cmath>

// 计算期望胜率E
float calculateExpectedScore(int playerRating, int opponentRating) {
    return 1.0 / (1.0 + pow(10.0, (opponentRating - playerRating) / 400.0));
}

// 更新两个棋手的等级分
float updateGoRating(float& playerRating, float& opponentRating, double score, int K = 20) {
    // 计算期望胜率
    double expectedScorePlayer = calculateExpectedScore(playerRating, opponentRating);
    double expectedScoreOpponent = calculateExpectedScore(opponentRating, playerRating);

    // 计算等级分变化
    float ratingChangePlayer = (K * (score - expectedScorePlayer));
    float ratingChangeOpponent = (K * ((1.0 - score) - expectedScoreOpponent));

    // 更新棋手的等级分
    playerRating += ratingChangePlayer;
    opponentRating += ratingChangeOpponent;

	return ratingChangePlayer;
}


//
//设置1d的等级分为1000
//每段差200分
//9d准入2600分