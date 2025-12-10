#pragma once
#include "Entity.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "Player.h"

class GemStone :public Entity {
public:
	GemStone(IGameState* parent, Player* player, int image_handle);
	~GemStone() = default;

	void Update(const InputState* input, float deltaTime)override;
	void Draw()override;

	void SetPos(int _x, int _y) {
		TransformComponent* myTrans = GetComponent<TransformComponent>().get();
		myTrans->x = _x;
		myTrans->y = _y;
	}

	auto GetPos() {
		TransformComponent* myTrans = GetComponent<TransformComponent>().get();
		return std::make_tuple(myTrans->x, myTrans->y);
	}

	float GetImageScale() {
		return imageScale;
	}
private:
	TransformComponent* player_trans;
	float imageScale;
};