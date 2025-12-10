#pragma once
#include "IUIElement.h"
#include "StageManager.h"
#include "Player.h"
#include "ExitDoor.h"
#include <memory>
#include <tuple>

class UIMiniMap :public IUIElement {
public:
	//StageManager,ミニマップの左上座標(x,y)
	UIMiniMap(std::shared_ptr<StageManager> sm, std::shared_ptr<Player> player, int sx, int sy
		, std::vector < std::tuple<float, float>>& gem_pos_info)
		:m_stageManager(sm), m_player(player)
		, startX(sx), startY(sy), scale(0.1f), icon_width(100)
		, doorX(0), doorY(0), doorW(0), doorH(0), isDoorFound(false)
		, gem_pos_ptr(gem_pos_info)
	{

	}
	~UIMiniMap()override = default;

	void Update()override {
		IUIElement::Update();

		//ドア情報がなかったらスキップ
		if (doorX == 0 || doorY == 0)return;
		if (isDoorFound)return;

		//もしスクリーンにドアが映ったらミニマップにドアの位置を表示する
		//プレイヤーのワールド位置とドアのワールド位置の差がスクリーン画面の半分より小さかったら
		if (abs(m_player->GetX() - doorX) <= GameData::windowWidth / 2
			&& abs(m_player->GetY() - doorY) <= GameData::windowHeight / 2) {
			isDoorFound = true;
		}
	}

	void Draw()override {
		IUIElement::Draw();
		//角部屋を描画する
		for (std::shared_ptr<RoomData> data : m_stageManager->rooms) {
			DrawBox(data->x * scale, data->y * scale,
				(data->x + data->w) * scale, (data->y + data->h) * scale,
				GetColor(0, 0, 0), false);
		}
		//プレイヤーを描画する
		DrawBox((m_player->GetX() - icon_width / 2) * scale, (m_player->GetY() - icon_width / 2) * scale,
			(m_player->GetX() + icon_width / 2) * scale, (m_player->GetY() + icon_width / 2) * scale,
			GetColor(255, 0, 0), true);
		//もしドアが見つかったら表示するようにする
		if (isDoorFound) {
			DrawBox((doorX - icon_width / 2) * scale, (doorY - icon_width / 2) * scale,
				(doorX + icon_width / 2) * scale, (doorY + icon_width / 2) * scale,
				GetColor(100, 100, 255), true);
		}

		//宝の位置を表示する
		//TODO : 常に表示し続けるか検討
		for (auto& [x, y] : gem_pos_ptr) {
			DrawBox((x - icon_width / 2) * scale, (y - icon_width / 2) * scale,
				(x + icon_width / 2) * scale, (y + icon_width / 2) * scale,
				GetColor(0, 0, 0), true);
		}
	}

	void SetDoorPos(std::tuple<float, float, float, float> doorInfo) {
		std::tie(doorX, doorY, doorW, doorH) = doorInfo;
	}
private:
	std::shared_ptr<StageManager> m_stageManager;
	std::shared_ptr<Player> m_player;
	int startX, startY;
	float doorX, doorY, doorW, doorH;
	float scale;
	int icon_width;
	bool isDoorFound;
	std::vector<std::tuple<float, float>>& gem_pos_ptr;
};