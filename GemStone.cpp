#include "GemStone.h"

GemStone::GemStone(IGameState* parent) :Entity(parent) {
	int handle = LoadGraph("Data/GemStone1.png");
	auto trans = std::make_shared<TransformComponent>(this, 0, 0);
	AddComponent(trans);
	auto sprite = std::make_shared<SpriteRendererComponent>(this, handle, RenderLayer::BackgroundMid,
		trans.get(), 0.01f);
	AddComponent(sprite);
}

void GemStone::Update(const InputState* input, float deltaTime) {
	Entity::Update(input, deltaTime);
}

void GemStone::Draw() {
	Entity::Draw();
}