// StageManager.cpp
#include "StageManager.h"
#include <DxLib.h>
#include <cstdlib>
#include <ctime>

StageManager::StageManager(int width, int height)
	: stageWidth(width), stageHeight(height), wholeScale(3.0f), root(nullptr)
{
	std::srand((unsigned int)std::time(nullptr));
}

void StageManager::Init()
{
	//最初にスタートする部屋のインデックスを設定
	nowRoomIndex = 4;
	//各部屋にインデックスを持たせるための変数
	indexVal = 0;

	// ルートノード作成
	root = new Node(0, 0, stageWidth * wholeScale, stageHeight * wholeScale);

	// 再帰的に分割
	Split(root, 3); // 深さ4くらいまで分割

	// 部屋の生成
	CreateRoom(root);

	// 部屋リスト収集
	rooms.clear();
	CollectRooms(root);

	//各部屋の隣接リストを収集
	CollectNextRooms();

	//各部屋の隣接リストから通路を作成する
	ConnectRooms();

	//プレイヤーの初期位置を設定する
	m_player->SetX(rooms[nowRoomIndex]->x + rooms[nowRoomIndex]->w / 2);
	m_player->SetY(rooms[nowRoomIndex]->y + rooms[nowRoomIndex]->h / 2);
}

void StageManager::Update(const InputState* input) {
	//Enterで注目ルームを変更できる
	if (input->IsKeyDown(KEY_INPUT_RETURN)) {
		nowRoomIndex = (nowRoomIndex + 1) % rooms.size();
		ConnectRooms();
	}
	if (input->IsKeyDown(KEY_INPUT_2)) {
		displayMaxRoomSize = !displayMaxRoomSize;
	}

	//部屋や通路の壁の当たり判定
	HitCheck();

	//次の部屋に移動したかを検出する
	CheckNextRoom();
}

void StageManager::HitCheck() {
	float px = m_player->GetX();
	float py = m_player->GetY();
	float pw = m_player->GetW();
	float ph = m_player->GetH();
	float pvx = m_player->GetVX();
	float pvy = m_player->GetVY();

	//プレイヤーと全ての部屋、通路を調べて、プレイヤーがどれかのなかにいればtrueで、
	// 一つもおさまっているものがないならfalse
	for (int i = 0; i < rooms.size(); i++) {
		//もしどこかの部屋にいたらこれで確認できる.
		if (IsInsideRect(px - pw / 2 + pvx, py - ph / 2 + pvy, pw, ph,
			rooms[i]->x, rooms[i]->y, rooms[i]->w, rooms[i]->h)) {
			return;
		}

		//全部通過したらどこの部屋にもいないということ
		//通路を確認する
	}

	for (int i = 0; i < corridors.size(); i++) {
		//廊下にいるかどうかを確認する
		if (IsInsideRect(px - pw / 2 + pvx, py - ph / 2 + pvy, pw, ph,
			corridors[i]->x, corridors[i]->y, corridors[i]->w, corridors[i]->h)) {
			return;
		}
	}

	//全て通り抜けたということは全てのrectからはみ出しているということ
	m_player->SetVX(0);
	m_player->SetVY(0);
}

void StageManager::CheckNextRoom() {
	float px = m_player->GetX();
	float py = m_player->GetY();
	float pw = m_player->GetW();
	float ph = m_player->GetH();
	float pvx = m_player->GetVX();
	float pvy = m_player->GetVY();
	if (IsInsideRect(px - pw / 2 + pvx, py - ph / 2 + pvy, pw, ph,
		rooms[nowRoomIndex]->x, rooms[nowRoomIndex]->y,
		rooms[nowRoomIndex]->w, rooms[nowRoomIndex]->h)) {
		//nowRoomIndexの部屋にいるということ。
		return;
	}

	//ここにつくと、nowRoomIndexの部屋にいないということ
	for (int i = 0; i < rooms.size(); i++) {
		//もしどこかの部屋にいたらこれで確認できる.
		if (IsInsideRect(px - pw / 2 + pvx, py - ph / 2 + pvy, pw, ph,
			rooms[i]->x, rooms[i]->y, rooms[i]->w, rooms[i]->h)) {
			nowRoomIndex = i;
			ConnectRooms();
			return;
		}
	}
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
		int rw = randW > corridorWidth ? randW : corridorWidth;
		int rh = randH > corridorWidth ? randH : corridorWidth;
		int rx = node->x + std::rand() % (node->w - rw + 1);
		int ry = node->y + std::rand() % (node->h - rh + 1);

		node->room = std::make_shared<RoomData>(rx, ry, rw, rh, node->x, node->y, node->w, node->h);
		node->room->index = indexVal;
		indexVal++;
		//node->room = RoomData(node->x, node->y, node->w, node->h, node->x, node->y, node->w, node->h);
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
		main->nextRooms = std::vector<std::shared_ptr<RoomData>>();
		for (auto& other : rooms) {
			//参照のアドレスをみて自分自身かどうか判定する
			if (&main == &other)continue;

			// a の境界
			int aLeft = main->maxX;
			int aRight = main->maxX + main->maxW;
			int aTop = main->maxY;
			int aBottom = main->maxY + main->maxH;

			// b の境界
			int bLeft = other->maxX;
			int bRight = other->maxX + other->maxW;
			int bTop = other->maxY;
			int bBottom = other->maxY + other->maxH;

			// 左右で隣接（垂直に重なっている範囲がある）
			if (aRight == bLeft && !(aBottom <= bTop || aTop >= bBottom)) {
				main->nextRooms.emplace_back(other);
				continue;
			}
			if (bRight == aLeft && !(bBottom <= aTop || bTop >= aBottom)) {
				main->nextRooms.emplace_back(other);
				continue;
			}

			// 上下で隣接（水平に重なっている範囲がある）
			if (aBottom == bTop && !(aRight <= bLeft || aLeft >= bRight)) {
				main->nextRooms.emplace_back(other);
				continue;
			}
			if (bBottom == aTop && !(bRight <= aLeft || bLeft >= aRight)) {
				main->nextRooms.emplace_back(other);
				continue;
			}
		}
	}
}

void StageManager::ConnectRooms() {
	//全ての部屋に対して
	corridors.clear();
	//for (int i = 0; i < rooms.size(); i++) {
	//	//隣接する部屋に通路を掛けられるかチェック
	//	for (int j = i + 1; j < rooms.size(); j++) {
	//		//この部屋が隣接する部屋に含まれているか確認
	//		if (ContainsRoomPtr(rooms[i]->nextRooms, rooms[j])) {
	//			//含まれているなら通路を掛けられるかチェック
	//			CheckRoomOverlap(rooms[i], rooms[j]);
	//		}
	//	}
	//}
	for (auto& room : rooms[nowRoomIndex]->nextRooms) {
		CheckRoomOverlap(rooms[nowRoomIndex], room);
	}
}

void StageManager::Draw()
{
	int px = 0;
	int py = 0;

	if (m_player) {
		px = m_player->GetX() - GameData::windowWidth / 2;
		py = m_player->GetY() - GameData::windowHeight / 2;
	}

	//もし2が押されたら部屋の元サイズを表示する
	if (displayMaxRoomSize) {
		for (auto& r : rooms)
		{
			DrawBox(r->maxX - px, r->maxY - py, r->maxX + r->maxW - px, r->maxY + r->maxH - py,
				GetColor(100, 100, 100), TRUE);
			DrawBox(r->maxX - px, r->maxY - py, r->maxX + r->maxW - px, r->maxY + r->maxH - py,
				GetColor(0, 0, 0), FALSE);
		}
	}


	//通路を描画する
	for (auto& col : corridors) {
		DrawBox(col->x - px, col->y - py, col->x + col->w - px, col->y + col->h - py,
			GetColor(200, 200, 0), TRUE);
	}

	//部屋を描画する
	int count = 0;
	for (auto& r : rooms)
	{
		DrawBox(r->x - px, r->y - py, r->x + r->w - px, r->y + r->h - py,
			GetColor(100, 100, 100), TRUE);
		DrawBox(r->x - px, r->y - py, r->x + r->w - px, r->y + r->h - py,
			GetColor(0, 0, 0), FALSE);

		/*if (count == nowRoomIndex) {
			DrawBox(r->x - px, r->y - py, r->x + r->w - px, r->y + r->h - py,
				GetColor(100, 100, 255), TRUE);
			DrawBox(r->x - px, r->y - py, r->x + r->w - px, r->y + r->h - py,
				GetColor(0, 0, 0), FALSE);
		}
		count++;*/
	}

	///////////////////////以下はデバッグ用

	/*

	//nowRoomIndexの部屋を色付きで表示する
	for (auto& r : rooms[nowRoomIndex]->nextRooms) {
		DrawBox(r->x - px, r->y - py, r->x + r->w - px, r->y + r->h - py,
			GetColor(255, 100, 100), TRUE);
		DrawBox(r->x - px, r->y - py, r->x + r->w - px, r->y + r->h - py,
			GetColor(0, 0, 0), FALSE);
	}

	//DrawFormatString(50, 50, GetColor(0, 0, 0), "nowRoomIndex:%d", nowRoomIndex);
	DrawFormatString(50, 75, GetColor(0, 0, 0), "isRoomLeft:%d", isRoomLeft);

	DrawFormatString(50, 100, GetColor(0, 0, 0), "pX:%f, pY;%f, pW:%f, pH:%f", m_player->GetX(), m_player->GetY(), m_player->GetW(), m_player->GetH());
	DrawFormatString(50, 150, GetColor(0, 0, 0), "rX:%d, rY;%d, rW:%f, rH;%f", rooms[nowRoomIndex]->x, rooms[nowRoomIndex]->y,
		rooms[nowRoomIndex]->w, rooms[nowRoomIndex]->h);

		*/
}


// ----------------------------------------------------
// 2つの部屋がX軸またはY軸方向で重なっているかを判定する関数
// 重なりがcorridorWidth以上なら、1(X軸方向)または2(Y軸方向)を返す
// どちらでもない場合は0を返す
// ----------------------------------------------------
int StageManager::CheckRoomOverlap(const std::shared_ptr<RoomData>& a,
	const std::shared_ptr<RoomData>& b)
{
	if (!a || !b) return 0; // nullチェック

	// X軸方向の重なり
	int left = max(a->x, b->x);
	int right = min(a->x + a->w, b->x + b->w);
	int overlapX = right - left;

	// Y軸方向の重なり
	int top = max(a->y, b->y);
	int bottom = min(a->y + a->h, b->y + b->h);
	int overlapY = bottom - top;

	// X軸方向で十分重なっている
	if (overlapX > (corridorWidth * wholeScale)) {
		int lessY = (a->y + a->h / 2) <= (b->y + b->h / 2) ? (a->y + a->h / 2) : (b->y + b->h / 2);
		int moreY = (a->y + a->h / 2) > (b->y + b->h / 2) ? (a->y + a->h / 2) : (b->y + b->h / 2);
		corridors.emplace_back(
			std::make_shared<CorridorData>(
				left + overlapX / 2 - (corridorWidth * wholeScale) / 2, lessY,
				(corridorWidth * wholeScale), moreY - lessY
			)
		);
		return 1;
	}
	// Y軸方向で十分重なっている
	else if (overlapY > (corridorWidth * wholeScale)) {
		int lessX = (a->x + a->w / 2) <= (b->x + b->w / 2) ? (a->x + a->w / 2) : (b->x + b->w / 2);
		int moreX = (a->x + a->w / 2) > (b->x + b->w / 2) ? (a->x + a->w / 2) : (b->x + b->w / 2);
		corridors.emplace_back(
			std::make_shared<CorridorData>(
				lessX, top + overlapY / 2 - (corridorWidth * wholeScale) / 2,
				moreX - lessX, (corridorWidth * wholeScale)
			)
		);
		return 2;
	}

	// どちらでもない
	return 0;
}
