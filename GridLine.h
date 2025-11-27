#pragma once
#include "Entity.h"
#include "TransformComponent.h"

/*
Entity‚Ì•`‰æ‚ªPlayerˆË‘¶‚¾‚Á‚½‚è‚ÅŒ©‚Ã‚ç‚­‚È‚Á‚Ä‚«‚½‚Ì‚Å
”wŒi‚ÉƒOƒŠƒbƒh‚ð•~‚¢‚ÄÀ•W‚ðŠm”F‚µ‚½‚¢‚Æ‚¢‚¤–Ú“I
*/

class GridLine :public Entity {
public:
	GridLine(IGameState* parent, TransformComponent* trans)
		:Entity(parent), player_trans(trans)
	{

	}
	~GridLine() = default;

	void Update(const InputState* input, float deltaTime)override {

	}

	void Draw()override {
		int px = player_trans->x - GameData::windowWidth / 2;
		int py = player_trans->y - GameData::windowHeight / 2;
		for (int i = 0; i < 100; i++) {
			DrawLine(0 - px, 100 * i - py, 10000 - px, 100 * i - py, GetColor(0, 0, 0));
			DrawFormatString(300 - px, 100 * i - py, GetColor(0, 0, 0), "%d", 100 * i);
		}
		for (int i = 0; i < 100; i++) {
			DrawLine(100 * i - px, 0 - py, 100 * i - px, 10000 - py, GetColor(0, 0, 0));
			DrawFormatString(100 * i - px, 300 - py, GetColor(0, 0, 0), "%d", 100 * i);
		}
	}
private:
	TransformComponent* player_trans;
};