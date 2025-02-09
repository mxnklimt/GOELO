#pragma once
#include "goplayer.h"
#include "FileManager.h"
#include <vector>
#include "Imgui/imgui.h"
#include "ELO_Calculate.h"
class PlayerInputWindow {
public:
    void display(std::vector<ChessPlayer>& players) {
        static char name[128] = "";
        static int rating = 1500;
        static float elo = 1200.0f;

        if (ImGui::CollapsingHeader(u8"新棋手")) {
            ImGui::InputText(u8"姓名", name, IM_ARRAYSIZE(name));
            ImGui::InputInt(u8"棋力", &rating);
            ImGui::InputFloat(u8"ELO", &elo, 0.1f, 10.0f, "%.1f");

            if (ImGui::Button(u8"添加棋手")) {
                if (strlen(name) > 0 && rating > 0 && elo > 0.0f) {
                    players.emplace_back(name, rating, elo);
                    FileManager::savePlayerData(players, "players.txt");

                    // 清空输入框
                    memset(name, 0, sizeof(name));
                    rating = 1500;
                    elo = 1200.0f;
                }
                else {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), u8"请填写完整的信息！");
                }
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), u8"");
            }
        }
    }
    void display2(std::vector<ChessGame>& games, std::vector<ChessPlayer>& players_notclear)
    {
		static char name1[128] = u8"王恺愉";
		static char name2[128] = "";
		static char result[128] = "";
		static float elo_change = 0.0f;
		if (ImGui::CollapsingHeader(u8"新对局")) {
			ImGui::InputText(u8"姓名1", name1, IM_ARRAYSIZE(name1));
			ImGui::InputText(u8"姓名2", name2, IM_ARRAYSIZE(name2));
			ImGui::InputText(u8"结果", result, IM_ARRAYSIZE(result));
			//ImGui::InputFloat(u8"ELO变化", &elo_change, 0.1f, 10.0f, "%.1f");
           
			

			if (ImGui::Button(u8"添加对局")) {
                int number1 = -1;
                int number2 = -1;
				//std::cout << "players_notclear.size() = " << players_notclear.size() << std::endl;
                for (int i = 0; i < players_notclear.size(); i++)
                {
                    std::string str1 = name1;  // C 风格字符串转换为 std::string
                    std::string str2 = name2;
                    std::string player_name = players_notclear[i].name;

                    if (str1.compare(player_name) == 0)  // 使用 compare() 进行字符串比较
                    {
                        number1 = i;
                        //std::cout << "number1 = " << number1 << std::endl;
                    }
                    if (str2.compare(player_name) == 0)
                    {
                        number2 = i;
                        //std::cout << "number2 = " << number2 << std::endl;
                    }
                }


                // 确保 number1 和 number2 是有效索引
                if (number1 == -1 || number2 == -1) {
                    std::cerr << "错误: 未找到匹配的棋手姓名!" << std::endl;
                    return;
                }

                //std::stoi将std::string类型转化为数字
                elo_change=updateGoRating(players_notclear[number1].elo, players_notclear[number2].elo, std::stoi(result), 20);
				std::cout <<  "Player1的ELO等级分为" << players_notclear[number1].elo << std::endl;

                players_notclear[0].elo = 1850;
                players_notclear[1].elo = 1900;
                players_notclear[2].elo = 1950;
                players_notclear[3].elo = 2050;
                players_notclear[4].elo = 2100;
                players_notclear[5].elo = 2150;
                players_notclear[6].elo = 2250;
                players_notclear[7].elo = 2300;
                players_notclear[8].elo = 2350;
                players_notclear[9].elo = 2450;
                players_notclear[10].elo = 2500;
                players_notclear[11].elo = 2550;
       
				//现在需要将修改过的elo值同步到player.txt文件中
				//首先将players_notclear中的数据写入players.txt文件中
				FileManager::revisePlayerData(players_notclear, "players.txt",number1,number2);
				// 将对局信息写入文件
				// 使用 ChessGame 的构造函数创建一个新的 ChessGame 对象
				ChessGame game(players_notclear[number1], players_notclear[number2], result, elo_change);

                
				games.emplace_back(game);

				std::vector<ChessGame> games_last;
				games_last.emplace_back(game);
				FileManager::saveGameData(games_last,"games.txt");
                games_last.clear();

			}
		}
    }

};

