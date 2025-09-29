// StageManager.cpp
#include "StageManager.h"
#include <DxLib.h>
#include <cstdlib>
#include <ctime>

StageManager::StageManager(int width, int height)
	: stageWidth(width), stageHeight(height), root(nullptr)
{
	std::srand((unsigned int)std::time(nullptr));
}

void StageManager::Init()
{
	// ルートノード作成
	root = new Node(0, 0, stageWidth, stageHeight);

	// 再帰的に分割
	Split(root, 4); // 深さ4くらいまで分割

	// 部屋の生成
	CreateRoom(root);

	// 部屋リスト収集
	rooms.clear();
	CollectRooms(root);
}

StageManager::Node* StageManager::Split(Node* node, int depth)
{
	if (depth <= 0) return node;

	bool splitH = (std::rand() % 2 == 0);

	if (node->w > node->h && node->w / node->h >= 1.25)
		splitH = false;
	else if (node->h > node->w && node->h / node->w >= 1.25)
		splitH = true;

	if (splitH)
	{
		int split = (int)(node->h / 4 + std::rand() % (node->h / 2));
		node->left = new Node(node->x, node->y, node->w, split);
		node->right = new Node(node->x, node->y + split, node->w, node->h - split);
	}
	else
	{
		int split = (int)(node->w / 4 + std::rand() % (node->w / 2));
		node->left = new Node(node->x, node->y, split, node->h);
		node->right = new Node(node->x + split, node->y, node->w - split, node->h);
	}

	Split(node->left, depth - 1);
	Split(node->right, depth - 1);

	return node;
}

void StageManager::CreateRoom(Node* node)
{
	if (node->left || node->right)
	{
		if (node->left) CreateRoom(node->left);
		if (node->right) CreateRoom(node->right);
	}
	else
	{
		/*int rw = std::max(4, node->w - (std::rand() % (node->w / 2 + 1)));
		int rh = std::max(4, node->h - (std::rand() % (node->h / 2 + 1)));*/
		int randW = node->w - (std::rand() % (node->w / 2 + 1));
		int randH = node->h - (std::rand() % (node->h / 2 + 1));
		int rw = randW > 4 ? randW : 4;
		int rh = randH > 4 ? randH : 4;
		int rx = node->x + std::rand() % (node->w - rw + 1);
		int ry = node->y + std::rand() % (node->h - rh + 1);

		node->room = RoomData(rx, ry, rw, rh);
	}
}

void StageManager::CollectRooms(Node* node)
{
	if (node == nullptr) return;

	if (node->left == nullptr && node->right == nullptr)
	{
		rooms.push_back(node->room);
	}
	else
	{
		CollectRooms(node->left);
		CollectRooms(node->right);
	}
}

void StageManager::Draw()
{
	for (auto& r : rooms)
	{
		DrawBox(r.x, r.y, r.x + r.w, r.y + r.h, GetColor(100, 100, 100), TRUE);
	}
}
