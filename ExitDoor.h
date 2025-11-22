#pragma once
#include "Entity.h"
#include "TransformComponent.h"
#include "DirectionComponent.h"
#include "SpriteRendererComponent.h"

class ExitDoor :public Entity {
private:
public:
	ExitDoor(IGameState* parent);
	~ExitDoor() = default;
	void Update(const InputState* input, float deltaTime)override;
	void Draw()override;
};