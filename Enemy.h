#pragma once
#include "Entity.h"
#include "HealthComponent.h"
#include "TransformComponent.h"
#include "DirectionComponent.h"
#include "AnimationComponent.h"

#include <math.h>

class Player;

class Enemy :public Entity {
public:
	Enemy(IGameState* parent, Player* player, int id);
	~Enemy() = default;

	void Update(const InputState* input, float deltaTime)override;
	void Draw()override;

	void SetPos(int x, int y) {
		auto trans = GetComponent<TransformComponent>();
		trans->x = x;
		trans->y = y;
	}

	int GetMyId() { return myId; }
	auto GetPos() { return std::make_tuple(my_trans->x, my_trans->y); }
private:
	float imageScale;
	TransformComponent* player_trans;
	TransformComponent* my_trans;
	float moveSpeed;
	int myId;
};