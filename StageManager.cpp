// StageManager.cpp
#include "StageManager.h"
#include <DxLib.h>
#include <cstdlib>
#include <ctime>

StageManager::StageManager(int width, int height)
	: stageWidth(width), stageHeight(height), root(nullptr)
	, roomColor(GetColor(100, 100, 100)), pathColor(GetColor(0, 255, 0))
{
	std::srand((unsigned int)std::time(nullptr));
}

void StageManager::Init()
{
	// ルートノード作成
	root = new Node(0, 0, stageWidth * 10, stageHeight * 10);
	/*
	ノード自体は最初は箱を持たずに、関係性だけ。
	そこから２分木のように分かれていき、最後の葉っぱだけに箱を描画する
	*/

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
	//depthが0になるまで再帰処理
	if (depth <= 0) return node;

	//splitH = 縦に割るか、横に割るか 0 or 1
	//ここでランダムに決めるが、その後アスペクト比により調整が入る
	bool splitH = (std::rand() % 2 == 0);

	//そのnodeが横長ならそれを半分にするような処理
	/*
	例えば、一回目のSplitなら1280 × 720 = 1.77...となるので縦に割れる

	*/
	if (node->w > node->h && node->w / node->h >= 1.25f)
		splitH = false;	//縦割り
	else if (node->h > node->w && node->h / node->w >= 1.25f)
		splitH = true; //横割り

	//もし横割りなら
	if (splitH)
	{
		//横割りの縦の割合を決定する
		//まず縦に４分割して、4/1+(0 < rand < 半分の高さ)
		//つまり、4/1 ～ 4/3の間で割合を決めているということ
		int split = (int)(node->h / 4 + std::rand() % (node->h / 2));
		node->left = new Node(node->x, node->y, node->w, split);
		node->right = new Node(node->x, node->y + split, node->w, node->h - split);
	}
	//縦割りなら
	else
	{
		//同じく横方向の割合をランダムに決める
		int split = (int)(node->w / 4 + std::rand() % (node->w / 2));
		node->left = new Node(node->x, node->y, split, node->h);
		node->right = new Node(node->x + split, node->y, node->w - split, node->h);
	}

	//上下、あるいは左右に分割したので、それぞれにSplitをかける
	Split(node->left, depth - 1);
	Split(node->right, depth - 1);

	/*もし次のSplitでdepthが0になった場合、
	* node->leftまたはnode->rightそれぞれのleftとrightはnullptrになる
	*/
	return node;
}

void StageManager::CreateRoom(Node* node)
{
	//先ほどのSplitでRootNodeから分割した左右に向かって再帰をかける
	if (node->left || node->right)
	{
		if (node->left) CreateRoom(node->left);
		if (node->right) CreateRoom(node->right);

		// 子ども同士を接続
		ConnectChildren(node);
	}
	//もしrightもleftもnullptrであるならそこは葉である。
	//だからそこに部屋を生成する
	else
	{
		//そのノードの最大幅 - (0より大きく半分より小さい幅)と４で大きいほう
		int rw = 4 >= node->w - (std::rand() % (node->w / 2 + 1)) ? 4 : node->w - (std::rand() % (node->w / 2 + 1));
		int rh = 4 >= node->h - (std::rand() % (node->h / 2 + 1)) ? 4 : node->h - (std::rand() % (node->h / 2 + 1));

		//node->x,yは左上を示しているので、xが0 < x < 最大幅-実際幅の間にあるべき。
		int rx = node->x + std::rand() % (node->w - rw + 1);
		int ry = node->y + std::rand() % (node->h - rh + 1);


		//ここでRoomDataを作成し、その葉nodeに持たせる。
		node->room = RoomData(rx, ry, rw, rh);
		node->hasRoom = true;
	}
}

void StageManager::CollectRooms(Node* node)
{
	//またRootNodeから始まる
	if (node == nullptr) return;

	//もしleftもrightもないならそれは葉nodeであるから配列に追加
	if (node->left == nullptr && node->right == nullptr)
	{
		rooms.push_back(node->room);
	}
	//そうでないなら再帰をかける
	else
	{
		CollectRooms(node->left);
		CollectRooms(node->right);
	}
}

RoomData StageManager::GetRoom(Node* node)
{
	if (node == nullptr) return RoomData();
	if (node->hasRoom) return node->room;

	RoomData leftRoom = GetRoom(node->left);
	if (leftRoom.w != 0 && leftRoom.h != 0) return leftRoom;

	RoomData rightRoom = GetRoom(node->right);
	return rightRoom;
}

void StageManager::ConnectChildren(Node* node)
{
	//葉であればスキップ
	if (!node->left || !node->right) return;

	RoomData leftRoom = GetRoom(node->left);
	RoomData rightRoom = GetRoom(node->right);

	//また、自身のLeftやRightに部屋がない
	if (leftRoom.w == 0 || rightRoom.w == 0) return;

	// 中心座標
	int lx = leftRoom.x + leftRoom.w / 2;
	int ly = leftRoom.y + leftRoom.h / 2;
	int rx = rightRoom.x + rightRoom.w / 2;
	int ry = rightRoom.y + rightRoom.h / 2;

	// 通路はL字型にする（水平→垂直）
	corridors.emplace_back(lx, ly);
	corridors.emplace_back(rx, ly);
	corridors.emplace_back(rx, ry);
}


void StageManager::Draw()
{
	//プレイヤーの位置を取得
	int px = 0;
	int py = 0;

	if (m_player) {
		px = m_player->GetX();
		py = m_player->GetY();
	}

	int thick = 10;
	// 通路
	for (size_t i = 0; i < corridors.size() - 2; i += 3)
	{
		auto p1 = corridors[i];
		auto p2 = corridors[i + 1];
		auto p3 = corridors[i + 2];
		DrawLine(p1.first - px, p1.second - py, p2.first - px, p2.second - py, pathColor, thick);
		DrawLine(p2.first - px, p2.second - py, p3.first - px, p3.second - py, pathColor, thick);
	}

	//部屋
	for (auto& r : rooms)
	{
		DrawBox(r.x - px, r.y - py, r.x + r.w - px, r.y + r.h - py, roomColor, TRUE);
	}
}
