// RoomData.h
#pragma once
#include <vector>

class RoomData
{
public:
	int x, y;       // •”‰®‚Ì¶ãÀ•W
	float w, h;       // •”‰®‚Ì•‚Æ‚‚³

	RoomData(int _x = 0, int _y = 0, float _w = 0, float _h = 0)
		: x(_x), y(_y), w(_w), h(_h) {
	}
};
