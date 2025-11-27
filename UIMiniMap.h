#pragma once
#include "IUIElement.h"
#include "StageManager.h"
#include "Player.h"

class UIMiniMap :public IUIElement {
public:
	//StageManager,ミニマップの左上座標(x,y)
	UIMiniMap(std::shared_ptr<StageManager> sm, std::shared_ptr<Player> player, int sx, int sy)
		:m_stageManager(sm), m_player(player)
		, startX(sx), startY(sy), scale(0.1f), icon_width(100)
	{

	}
	~UIMiniMap()override = default;

	void Update()override {
		IUIElement::Update();
	}

	void Draw()override {
		IUIElement::Draw();
		for (std::shared_ptr<RoomData> data : m_stageManager->rooms) {
			DrawBox(data->x * scale, data->y * scale,
				(data->x + data->w) * scale, (data->y + data->h) * scale,
				GetColor(0, 0, 0), false);
		}
		DrawBox((m_player->GetX() - icon_width / 2) * scale, (m_player->GetY() - icon_width / 2) * scale,
			(m_player->GetX() + icon_width / 2) * scale, (m_player->GetY() + icon_width / 2) * scale,
			GetColor(255, 0, 0), true);
	}
private:
	std::shared_ptr<StageManager> m_stageManager;
	std::shared_ptr<Player> m_player;
	int startX, startY;
	float scale;
	int icon_width;
};