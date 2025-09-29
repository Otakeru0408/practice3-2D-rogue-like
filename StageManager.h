// StageManager.h
#pragma once
#include <vector>
#include "RoomData.h"
#include "Player.h"

class StageManager
{
public:
	StageManager(int width, int height);
	void Init();    // BSPでダンジョンを生成
	void Draw();    // DXライブラリで描画

	std::shared_ptr<Player> m_player;

private:
	struct Node
	{
		int x, y, w, h;
		Node* left;
		Node* right;
		RoomData room;

		Node(int _x, int _y, int _w, int _h)
			: x(_x), y(_y), w(_w), h(_h), left(nullptr), right(nullptr) {
		}
	};

	int stageWidth, stageHeight;
	std::vector<RoomData> rooms;
	Node* root;

	Node* Split(Node* node, int depth);
	void CreateRoom(Node* node);
	void CollectRooms(Node* node);
};
