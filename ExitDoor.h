#pragma once
#include "Entity.h"
#include "TransformComponent.h"
#include "DirectionComponent.h"
#include "SpriteRendererComponent.h"
#include "CorriderComponent.h"
#include "AnimationComponent.h"
#include "Player.h"

class ExitDoor :public Entity {
private:
	TransformComponent* player_trans;
public:
	ExitDoor(IGameState* parent, Player* m_player);
	~ExitDoor() = default;
	void Update(const InputState* input, float deltaTime)override;
	void Draw()override;

	void SetPos(int x, int y) {
		auto trans = GetComponent<TransformComponent>();
		trans->x = x;
		trans->y = y;
	}
	std::tuple<float, float, float, float> GetDoorXYWH();
};