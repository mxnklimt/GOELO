#pragma once
#include <fstream>
#include <vector>
#include "iostream"
#include "goplayer.h"
#include <iomanip> // �������ø�������ʽ
#include <sstream> // ���ڸ�ʽ�����
class FileManager {
public:
    static void revisePlayerData(const std::vector<ChessPlayer>& players, const std::string& filename,
    int number1,int number2)
    {
        std::ifstream inFile(filename);
        if (!inFile) {
            std::cerr << "�޷����ļ�: " << filename << std::endl;
            return;
        }

        // ��ȡ�ļ���������
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(inFile, line)) {
            lines.push_back(line);
        }
        inFile.close();

        // ��� number1 �� number2 �Ƿ񳬳���Χ
        if (number1 < 0 || number1 >= lines.size() || number2 < 0 || number2 >= lines.size()) {
            std::cerr << "����: number1 �� number2 �����ļ�����!" << std::endl;
            return;
        }

        // ��ȷ��ʽ�� float��ȷ��С��λ
        auto formatPlayerData = [](const ChessPlayer& player) {
            std::ostringstream oss;
            oss << player.name << "," << player.rating << ","
                << std::fixed << std::setprecision(2) << player.elo; // ȷ�� elo ������λС��
            return oss.str();
            };

        // �޸�ָ����
        lines[number1] = formatPlayerData(players[number1]);
        lines[number2] = formatPlayerData(players[number2]);

        // ����д���ļ�
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "�޷�д���ļ�: " << filename << std::endl;
            return;
        }

        for (const auto& l : lines) {
            outFile << l << "\n";
        }

        std::cout << "�ļ��޸ĳɹ�!" << std::endl;
    }
    static void savePlayerData(const std::vector<ChessPlayer>& players, const std::string& filename) {
        //filename������Ҫ�������ݵ�Ŀ���ļ������ļ�·������
        //std::ios::app�����Ǵ��ļ�ʱ��ģʽ��־����ʾ��׷��ģʽ���ļ�
        //����ļ��Ѿ����ڣ��µ����ݻᱻ׷�ӵ��ļ�ĩβ�������Ǹ����ļ�ԭ�е����ݡ�
        std::ofstream outFile(filename, std::ios::app);
        if (!outFile.is_open()) {
            std::cerr << "Error opening file for writing." << std::endl;
            return;
        }
		//players������ܶ��ChessPlayer����������Ҫ��ÿ�����������д���ļ���
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
            //����ʼλ�õ���һ������֮����name
            //std::string::substr �������ڴ��ַ�������ȡ���ַ���
            std::string name = line.substr(0, pos1);
            int rating = std::stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
            float elo = std::stof(line.substr(pos2 + 1));

            players.emplace_back(name, rating, elo);
        }
        inFile.close();
    }
    static void saveGameData(const std::vector<ChessGame>& games, const std::string& filename) {
        // ���ļ�������ļ������ڣ����Զ���������׷��ģʽ���ļ���
        std::ofstream outFile(filename, std::ios::app);
        if (!outFile.is_open()) {
            std::cerr << "Error opening file for writing." << std::endl;
            return;
        }

        // �������е���Ϸ��¼����ÿһ���Ծ���Ϣд���ļ�
        for (const auto& game : games) {
            outFile << game.player1.name << ","  // ��һ�����ֵ�����
                << game.player2.name << ","  // �ڶ������ֵ�����
                << game.result << ","
                <<game.elo_change
                <<"\n";     // �Ծֽ��
        }

        outFile.close();
    }
    
    static void loadGameData(std::vector<ChessGame>& games, const std::string& filename)
    {
        std::ifstream inFile(filename);
        std::string line;

        while (std::getline(inFile, line)) {
            //�����ҵ�ÿ�����ŵ�λ��
            size_t pos1 = line.find(",");
            size_t pos3 = line.rfind(",");
            size_t pos2 = line.rfind(",", pos3 - 1);

            if (pos1 == std::string::npos || pos2 == std::string::npos)
                continue; // Skip malformed lines

            std::string name1 = line.substr(0, pos1);
            std::string name2 = line.substr(pos1 + 1, pos2 - pos1 - 1);
            std::string result = line.substr(pos2 + 1, pos3 - pos2 - 1);
            float elo_change = std::stof(line.substr(pos3 + 1));

            //��players.txt�в���ChessPlayer�����elo��rating
            std::ifstream FindFile("players.txt");
            std::string FindLine;

            ChessPlayer* player1 = nullptr; // ���ڴ洢�ҵ������1
            ChessPlayer* player2 = nullptr; // ���ڴ洢�ҵ������2

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
            FindFile.close(); // �ر�FindFile���Ա��´�ѭ���������´�

            if (player1 != nullptr && player2 != nullptr)
            {
                games.emplace_back(*player1, *player2, result, elo_change);
            }

            //�ͷ�player1��2
            delete player1;
            delete player2;
        }

        inFile.close(); // ��ѭ��������ر�inFile
    }
};


