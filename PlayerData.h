#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

//Playerのセーブ用構造体
struct PlayerData {
	int hp;
	int maxHp;
	int mp;
	float posX;
	float posY;

	PlayerData() {
		hp = 0;
		maxHp = 0;
		mp = 0;
		posX = 0;
		posY = 0;
	}

	// CSV形式に変換
	std::string ToCSV() const {
		std::ostringstream oss;
		oss << hp << "," << maxHp << "," << mp << "," << posX << "," << posY;
		return oss.str();
	}

	// CSVから復元
	static PlayerData FromCSV(const std::string& line) {
		PlayerData data;
		std::istringstream ss(line);
		std::string cell;

		std::getline(ss, cell, ',');
		data.hp = std::stoi(cell);

		std::getline(ss, cell, ',');
		data.maxHp = std::stoi(cell);

		std::getline(ss, cell, ',');
		data.mp = std::stoi(cell);

		std::getline(ss, cell, ',');
		data.posX = std::stof(cell);

		std::getline(ss, cell, ',');
		data.posY = std::stof(cell);

		return data;
	}
};
