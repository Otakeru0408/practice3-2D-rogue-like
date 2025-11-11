// StageManager.h
#pragma once
#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>
#include "RoomData.h"
#include "CorridorData.h"
#include "Player.h"
#include "InputState.h"

class StageManager
{
public:
	StageManager(int width, int height);
	void Init();    // BSPでダンジョンを生成
	void Update(const InputState* input);
	void Draw();    // DXライブラリで描画

	std::shared_ptr<Player> m_player;

private:
	struct Node
	{
		int x, y, w, h;
		Node* left;
		Node* right;
		std::shared_ptr<RoomData> room;

		Node(int _x, int _y, int _w, int _h)
			: x(_x), y(_y), w(_w), h(_h), left(nullptr), right(nullptr) {
		}
	};

	int stageWidth, stageHeight;
	std::vector<std::shared_ptr<RoomData>> rooms;
	Node* root;

	Node* Split(Node* node, int depth);
	void CreateRoom(Node* node);
	void CollectRooms(Node* node);
	void CollectNextRooms();
	void ConnectRooms();

	//部屋と通路の当たり判定をしている関数
	void HitCheck();
	void CheckNextRoom();

	int indexVal = 0;
	int nowRoomIndex = 0;
	int corridorWidth = 50;
	bool displayMaxRoomSize = false;
	float wholeScale;

	bool isRoomLeft = false;

	//横、または縦方向でp1+w1がp2+w2の範囲内にいるかどうか
	//True:範囲内にいる　False:はみ出している
	bool HitTest(float p1, float w1, float p2, float w2) {
		return p2 < p1 && p1 + w1 < p2 + w2;
	}

	bool IsInsideRect(float ax, float ay, float aw, float ah,
		float bx, float by, float bw, float bh)
	{
		return (ax >= bx &&
			ay >= by &&
			ax + aw <= bx + bw &&
			ay + ah <= by + bh);
	}


	//横または縦方向で、p1+w1がp2+w2と重なる部分があるかどうか
	//True:重なっている　False:かさなっていない
	bool OverTest(float p1, float w1, float p2, float w2) {
		float overlap1 = max(p1, p2);
		float overlap2 = min(p1 + w1, p2 + w2);
		return (overlap1 < overlap2);
	}
};
