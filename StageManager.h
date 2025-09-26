// StageManager.h
#pragma once
#include <vector>
#include<memory>
#include "RoomData.h"
#include "Player.h"

class StageManager
{
public:
	StageManager(int width, int height);
	void Init();    // BSPでダンジョンを生成
	void Update();
	void Draw();    // DXライブラリで描画

	std::shared_ptr<Player> m_player;

private:
	struct Node {
		int x, y, w, h;
		Node* left;
		Node* right;
		RoomData room;
		bool hasRoom;
		Node(int _x, int _y, int _w, int _h)
			: x(_x), y(_y), w(_w), h(_h), left(nullptr), right(nullptr), hasRoom(false) {
		}
	};

	std::vector<RoomData> rooms;
	std::vector<std::pair<int, int>> corridors; // 通路を線分として管理

	Node* root;

	Node* Split(Node* node, int depth);
	void CreateRoom(Node* node);
	void CollectRooms(Node* node);
	void ConnectChildren(Node* node);
	RoomData GetRoom(Node* node);

	//プレイヤーのスタート地点を決める
	void SetPlayerStartPos();

	int stageWidth, stageHeight;
	int roomColor;
	int pathColor;
};
