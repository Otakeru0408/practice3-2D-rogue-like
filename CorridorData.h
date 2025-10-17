#pragma once

//廊下の領域データを保持するクラス
class CorridorData {
public:
	int x, y;       // 部屋の左上座標
	float w, h;       // 部屋の幅と高さ

	CorridorData(int _x = 0, int _y = 0, float _w = 0, float _h = 0)
		: x(_x), y(_y), w(_w), h(_h) {
	}
};