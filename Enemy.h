#pragma once
#include "Entity.h"
#include "HealthComponent.h"
#include "TransformComponent.h"
#include "DirectionComponent.h"
#include "AnimationComponent.h"

class Player;

class Enemy :public Entity {
public:
	Enemy(IGameState* parent, Player* player);
	~Enemy() = default;

	void Update(const InputState* input, float deltaTime)override;
	void Draw()override;

	void SetPos(int x, int y) {
		auto trans = GetComponent<TransformComponent>();
		trans->x = x;
		trans->y = y;
	}

private:
	float imageScale;
};