#include "ExitDoor.h"

ExitDoor::ExitDoor(IGameState* parent)
	:Entity(parent)
{
	int handle = LoadGraph("Data/ExitDoor.png");

	auto trans = std::make_shared<TransformComponent>(this, 0, 0);
	AddComponent(trans);
	AddComponent(std::make_shared<DirectionComponent>(this));
	AddComponent(std::make_shared<SpriteRendererComponent>(this, handle, RenderLayer::BackgroundMid,
		trans.get(), 0.3f));
}

void ExitDoor::Update(const InputState* input, float deltaTime) {
	Entity::Update(input, deltaTime);
}

void ExitDoor::Draw() {
	Entity::Draw();
}