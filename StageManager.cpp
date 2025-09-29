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

	//各部屋の隣接リストを収集
	CollectNextRooms();

	//各部屋の隣接リストから通路を作成する
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

		//node->room = RoomData(rx, ry, rw, rh, node->x, node->y, node->w, node->h);
		node->room = RoomData(node->x, node->y, node->w, node->h, node->x, node->y, node->w, node->h);
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

void StageManager::CollectNextRooms() {

	//各部屋に対して隣接する部屋を探す
	for (auto& main : rooms) {
		main.nextRooms = std::vector<RoomData>();
		for (auto& other : rooms) {
			//参照のアドレスをみて自分自身かどうか判定する
			if (&main == &other)continue;

			// a の境界
			int aLeft = main.maxX;
			int aRight = main.maxX + main.maxW;
			int aTop = main.maxY;
			int aBottom = main.maxY + main.maxH;

			// b の境界
			int bLeft = other.maxX;
			int bRight = other.maxX + other.maxW;
			int bTop = other.maxY;
			int bBottom = other.maxY + other.maxH;

			// 左右で隣接（垂直に重なっている範囲がある）
			if (aRight == bLeft && !(aBottom <= bTop || aTop >= bBottom)) {
				main.nextRooms.emplace_back(other);
				continue;
			}
			if (bRight == aLeft && !(bBottom <= aTop || bTop >= aBottom)) {
				main.nextRooms.emplace_back(other);
				continue;
			}

			// 上下で隣接（水平に重なっている範囲がある）
			if (aBottom == bTop && !(aRight <= bLeft || aLeft >= bRight)) {
				main.nextRooms.emplace_back(other);
				continue;
			}
			if (bBottom == aTop && !(bRight <= aLeft || bLeft >= aRight)) {
				main.nextRooms.emplace_back(other);
				continue;
			}
		}
	}
}

void StageManager::ConnectRooms() {

}

void StageManager::Draw()
{
	int px = 0;
	int py = 0;

	if (m_player) {
		px = m_player->GetX() - GameData::windowWidth / 2;
		py = m_player->GetY() - GameData::windowHeight / 2;
	}

	int count = 0;
	for (auto& r : rooms)
	{
		DrawBox(r.x - px, r.y - py, r.x + r.w - px, r.y + r.h - py,
			GetColor(100, 100, 100), TRUE);
		DrawBox(r.x - px, r.y - py, r.x + r.w - px, r.y + r.h - py,
			GetColor(0, 0, 0), FALSE);

		if (count == nowRoomIndex) {
			DrawBox(r.x - px, r.y - py, r.x + r.w - px, r.y + r.h - py,
				GetColor(100, 100, 255), TRUE);
			DrawBox(r.x - px, r.y - py, r.x + r.w - px, r.y + r.h - py,
				GetColor(0, 0, 0), FALSE);
		}
		count++;
	}

	for (auto& r : rooms[nowRoomIndex].nextRooms) {
		DrawBox(r.x - px, r.y - py, r.x + r.w - px, r.y + r.h - py,
			GetColor(255, 100, 100), TRUE);
		DrawBox(r.x - px, r.y - py, r.x + r.w - px, r.y + r.h - py,
			GetColor(0, 0, 0), FALSE);
	}

	DrawFormatString(50, 50, GetColor(0, 255, 0), "NextNum:%d", rooms[nowRoomIndex].nextRooms.size());
}
