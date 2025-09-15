#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct PlayerData {
	std::string name;
	int hp;
	int mp;
	float posX;
	float posY;

	// CSVŒ`Ž®‚É•ÏŠ·
	std::string ToCSV() const {
		std::ostringstream oss;
		oss << name << "," << hp << "," << mp << "," << posX << "," << posY;
		return oss.str();
	}

	// CSV‚©‚ç•œŒ³
	static PlayerData FromCSV(const std::string& line) {
		PlayerData data;
		std::istringstream ss(line);
		std::string cell;

		std::getline(ss, data.name, ',');  // string

		std::getline(ss, cell, ',');
		data.hp = std::stoi(cell);

		std::getline(ss, cell, ',');
		data.mp = std::stoi(cell);

		std::getline(ss, cell, ',');
		data.posX = std::stof(cell);

		std::getline(ss, cell, ',');
		data.posY = std::stof(cell);

		return data;
	}
};
