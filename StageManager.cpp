// StageManager.cpp
#include "StageManager.h"
#include <DxLib.h>
#include <cstdlib>
#include <ctime>

StageManager::StageManager(int width, int height)
	: stageWidth(width), stageHeight(height), root(nullptr)
	, roomColor(GetColor(100, 100, 100)), pathColor(GetColor(0, 255, 0))
	, pathThick(50), wholeScale(4)
{
	std::srand((unsigned int)std::time(nullptr));
}

void StageManager::Init()
{
	// ルートノード作成
	root = new Node(0, 0, stageWidth * wholeScale, stageHeight * wholeScale);
	/*
	ノード自体は最初は箱を持たずに、関係性だけ。
	そこから２分木のように分かれていき、最後の葉っぱだけに箱を描画する
	*/

	// 再帰的に分割
	Split(root, 3); // 深さ4くらいまで分割

	// 部屋の生成
	CreateRoom(root);

	// 部屋リスト収集
	rooms.clear();
	CollectRooms(root);

	SetPlayerStartPos();
}

void StageManager::Update() {
	//部屋の当たり判定を設定する
	/*
	やること
	・プレイヤーがどこかの部屋にいるかを確認
	・部屋の中なら外に出さないようにする
	・部屋の外なら渡り廊下の領域から離れないようにする
	・当たり判定の関数　X軸に対して離れているなら0, Y軸に対して離れているなら1みたいにする
	*/
	switch (CanMove())
	{
	case 1:m_player->SetVX(0); break;
	case 2:m_player->SetVY(0); break;
	case 3:m_player->SetVX(0); m_player->SetVY(0); break;
	default:
		break;
	}
}

int StageManager::CanMove() {
	int nextX = m_player->GetX() + m_player->GetVX();
	int nextY = m_player->GetY() + m_player->GetVY();

	bool outX = (nextX - m_player->GetW() / 2 < nowRoom.x) || (nextX + m_player->GetW() / 2 > nowRoom.x + nowRoom.w);
	bool outY = (nextY - m_player->GetH() / 2 < nowRoom.y) || (nextY + m_player->GetH() / 2 > nowRoom.y + nowRoom.h);

	if (outX && outY) return 3; // 両方はみ出す
	if (outX) return 1;         // X方向はみ出し
	if (outY) return 2;         // Y方向はみ出し

	return 0; // 収まっている
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
		px = m_player->GetX() - GameData::windowWidth / 2;
		py = m_player->GetY() - GameData::windowHeight / 2;
	}

	// 通路
	for (size_t i = 0; i < corridors.size() - 2; i += 3)
	{
		auto p1 = corridors[i];
		auto p2 = corridors[i + 1];
		auto p3 = corridors[i + 2];
		DrawCorridor(p1, p2, px, py, pathThick * wholeScale, pathColor);
		DrawCorridor(p2, p3, px, py, pathThick * wholeScale, pathColor);
	}

	//部屋
	for (auto& r : rooms)
	{
		DrawBox(r.x - px, r.y - py, r.x + r.w - px, r.y + r.h - py, roomColor, TRUE);
	}

	DrawMiniMap();
}

void StageManager::SetPlayerStartPos() {
	RoomData data = rooms[0];
	m_player->SetX(data.x + data.w / 2);
	m_player->SetY(data.y + data.h / 2);
	nowRoom = data;
}

// 2点間の通路を描画する関数
void StageManager::DrawCorridor(const std::pair<int, int>& a, const std::pair<int, int>& b, int px, int py, int thick, int color) {
	if (a.first == b.first) {
		// 縦方向
		int x = a.first - px;
		int y1 = a.second - py;
		int y2 = b.second - py;
		if (y1 > y2) std::swap(y1, y2);
		DrawBox(x - thick / 2, y1 - thick / 2, x + thick / 2, y2 + thick / 2, color, TRUE);
	}
	else if (a.second == b.second) {
		// 横方向
		int y = a.second - py;
		int x1 = a.first - px;
		int x2 = b.first - px;
		if (x1 > x2) std::swap(x1, x2);
		DrawBox(x1 - thick / 2, y - thick / 2, x2 + thick / 2, y + thick / 2, color, TRUE);
	}
}

void StageManager::DrawMiniMap() {
	float nowScale = 0.05f;
	int startPosX = 10;
	int startPosY = 10;
	for (auto& r : rooms)
	{
		DrawBox(startPosX + r.x * nowScale, startPosY + r.y * nowScale,
			startPosX + (r.x + r.w) * nowScale, startPosY + (r.y + r.h) * nowScale,
			GetColor(0, 0, 0), FALSE);
	}
	DrawCircle(startPosX + m_player->GetX() * nowScale, startPosY + m_player->GetY() * nowScale,
		5, GetColor(255, 0, 0), TRUE);
}