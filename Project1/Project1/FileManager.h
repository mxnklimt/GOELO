#pragma once
#include <fstream>
#include <vector>
#include "iostream"
#include "goplayer.h"
#include <iomanip> // 用于设置浮点数格式
#include <sstream> // 用于格式化输出
class FileManager {
public:
    static void revisePlayerData(const std::vector<ChessPlayer>& players, const std::string& filename,
    int number1,int number2)
    {
        std::ifstream inFile(filename);
        if (!inFile) {
            std::cerr << "无法打开文件: " << filename << std::endl;
            return;
        }

        // 读取文件的所有行
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(inFile, line)) {
            lines.push_back(line);
        }
        inFile.close();

        // 检查 number1 和 number2 是否超出范围
        if (number1 < 0 || number1 >= lines.size() || number2 < 0 || number2 >= lines.size()) {
            std::cerr << "错误: number1 或 number2 超出文件行数!" << std::endl;
            return;
        }

        // 正确格式化 float，确保小数位
        auto formatPlayerData = [](const ChessPlayer& player) {
            std::ostringstream oss;
            oss << player.name << "," << player.rating << ","
                << std::fixed << std::setprecision(2) << player.elo; // 确保 elo 保留两位小数
            return oss.str();
            };

        // 修改指定行
        lines[number1] = formatPlayerData(players[number1]);
        lines[number2] = formatPlayerData(players[number2]);

        // 重新写入文件
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "无法写入文件: " << filename << std::endl;
            return;
        }

        for (const auto& l : lines) {
            outFile << l << "\n";
        }

        std::cout << "文件修改成功!" << std::endl;
    }
    static void savePlayerData(const std::vector<ChessPlayer>& players, const std::string& filename) {
        //filename：这是要保存数据的目标文件名（文件路径）。
        //std::ios::app：这是打开文件时的模式标志，表示以追加模式打开文件
        //如果文件已经存在，新的数据会被追加到文件末尾，而不是覆盖文件原有的内容。
        std::ofstream outFile(filename, std::ios::app);
        if (!outFile.is_open()) {
            std::cerr << "Error opening file for writing." << std::endl;
            return;
        }
		//players包含里很多个ChessPlayer对象，我们需要将每个对象的数据写入文件。
        for (const auto& player : players) {
            outFile << player.name << "," << player.rating << "," << player.elo << "\n";
        }
        outFile.close();
    }

    static void loadPlayerData(std::vector<ChessPlayer>& players, const std::string& filename) {
        std::ifstream inFile(filename);
        std::string line;

        while (std::getline(inFile, line)) {
            size_t pos1 = line.find(",");
            size_t pos2 = line.rfind(",");

            if (pos1 == std::string::npos || pos2 == std::string::npos)
                continue; // Skip malformed lines
            //从起始位置到第一个逗号之间是name
            //std::string::substr 函数用于从字符串中提取子字符串
            std::string name = line.substr(0, pos1);
            int rating = std::stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
            float elo = std::stof(line.substr(pos2 + 1));

            players.emplace_back(name, rating, elo);
        }
        inFile.close();
    }
    static void saveGameData(const std::vector<ChessGame>& games, const std::string& filename) {
        // 打开文件，如果文件不存在，会自动创建；以追加模式打开文件。
        std::ofstream outFile(filename, std::ios::app);
        if (!outFile.is_open()) {
            std::cerr << "Error opening file for writing." << std::endl;
            return;
        }

        // 遍历所有的游戏记录，将每一条对局信息写入文件
        for (const auto& game : games) {
            outFile << game.player1.name << ","  // 第一个棋手的姓名
                << game.player2.name << ","  // 第二个棋手的姓名
                << game.result << ","
                <<game.elo_change
                <<"\n";     // 对局结果
        }

        outFile.close();
    }
    
    static void loadGameData(std::vector<ChessGame>& games, const std::string& filename)
    {
        std::ifstream inFile(filename);
        std::string line;

        while (std::getline(inFile, line)) {
            //依次找到每个逗号的位置
            size_t pos1 = line.find(",");
            size_t pos3 = line.rfind(",");
            size_t pos2 = line.rfind(",", pos3 - 1);

            if (pos1 == std::string::npos || pos2 == std::string::npos)
                continue; // Skip malformed lines

            std::string name1 = line.substr(0, pos1);
            std::string name2 = line.substr(pos1 + 1, pos2 - pos1 - 1);
            std::string result = line.substr(pos2 + 1, pos3 - pos2 - 1);
            float elo_change = std::stof(line.substr(pos3 + 1));

            //在players.txt中查找ChessPlayer对象的elo和rating
            std::ifstream FindFile("players.txt");
            std::string FindLine;

            ChessPlayer* player1 = nullptr; // 用于存储找到的玩家1
            ChessPlayer* player2 = nullptr; // 用于存储找到的玩家2

            while (std::getline(FindFile, FindLine))
            {
                size_t pos4 = FindLine.find(",");
                size_t pos5 = FindLine.rfind(",");
                std::string Name = FindLine.substr(0, pos4);
                if (Name == name1)
                {
                    player1 = new ChessPlayer(name1, std::stoi(FindLine.substr(pos4 + 1, pos5 - pos4 - 1)), std::stof(FindLine.substr(pos5 + 1)));
                }
                else if (Name == name2)
                {
                    player2 = new ChessPlayer(name2, std::stoi(FindLine.substr(pos4 + 1, pos5 - pos4 - 1)), std::stof(FindLine.substr(pos5 + 1)));
                }
                if (player1 != nullptr && player2 != nullptr)
                {
                    break;
                }
            }
            FindFile.close(); // 关闭FindFile，以便下次循环可以重新打开

            if (player1 != nullptr && player2 != nullptr)
            {
                games.emplace_back(*player1, *player2, result, elo_change);
            }

            //释放player1，2
            delete player1;
            delete player2;
        }

        inFile.close(); // 在循环结束后关闭inFile
    }
};


