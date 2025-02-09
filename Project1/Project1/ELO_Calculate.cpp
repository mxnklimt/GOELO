#include <iostream>
#include <cmath>

// ��������ʤ��E
float calculateExpectedScore(int playerRating, int opponentRating) {
    return 1.0 / (1.0 + pow(10.0, (opponentRating - playerRating) / 400.0));
}

// �����������ֵĵȼ���
float updateGoRating(float& playerRating, float& opponentRating, double score, int K = 20) {
    // ��������ʤ��
    double expectedScorePlayer = calculateExpectedScore(playerRating, opponentRating);
    double expectedScoreOpponent = calculateExpectedScore(opponentRating, playerRating);

    // ����ȼ��ֱ仯
    float ratingChangePlayer = (K * (score - expectedScorePlayer));
    float ratingChangeOpponent = (K * ((1.0 - score) - expectedScoreOpponent));

    // �������ֵĵȼ���
    playerRating += ratingChangePlayer;
    opponentRating += ratingChangeOpponent;

	return ratingChangePlayer;
}


//
//����1d�ĵȼ���Ϊ1000
//ÿ�β�200��
//9d׼��2600��