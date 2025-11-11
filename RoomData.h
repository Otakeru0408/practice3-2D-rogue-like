// RoomData.h
#pragma once
#include <vector>
#include <memory>
#include "CorridorData.h"

class RoomData
{
public:
	int x, y;       // 部屋の左上座標
	float w, h;       // 部屋の幅と高さ
	int maxX, maxY;	//部屋のもともとの最大の位置
	int maxW, maxH;	//部屋のもともとの最大のサイズ
	int index;
	std::vector<std::shared_ptr<RoomData>> nextRooms;

	RoomData(int _x = 0, int _y = 0, float _w = 0, float _h = 0,
		int _mx = 0, int _my = 0, float _mw = 0, float _mh = 0)
		: x(_x), y(_y), w(_w), h(_h), maxX(_mx), maxY(_my), maxW(_mw), maxH(_mh), index(0) {
	}
};
