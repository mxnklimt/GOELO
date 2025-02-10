#pragma once
float calculateExpectedScore(int playerRating, int opponentRating);
float updateGoRating(float& playerRating, float& opponentRating, double score, int K = 20);