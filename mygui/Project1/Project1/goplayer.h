
#pragma once
#include <string>
class ChessPlayer {
public:
    std::string name;
    int rating;
    float elo;

    ChessPlayer(const std::string& name, int rating, float elo)
        : name(name), rating(rating), elo(elo) {
    }

    std::string to_string() const {
        return "Name: " + name + ", Rating: " + std::to_string(rating) + ", ELO: " + std::to_string(elo);
    }
};

class ChessGame {
public:
    ChessPlayer player1;
    ChessPlayer player2;
    std::string result;
    float elo_change;
    ChessGame(const ChessPlayer& player1, const ChessPlayer& player2, const std::string& result,const float elo_change)
        : player1(player1), player2(player2), result(result),elo_change(elo_change) {
    }
    std::string to_string() const {
        return player1.name + " vs " + player2.name + " - " + result+"ELOChange ="+ std::to_string(elo_change);
    }
};

