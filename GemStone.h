#pragma once
#include "Entity.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"

class GemStone :public Entity {
public:
	GemStone(IGameState* parent);
	~GemStone() = default;

	void Update(const InputState* input, float deltaTime)override;
	void Draw()override;
private:
};