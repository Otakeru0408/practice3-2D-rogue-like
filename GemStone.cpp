#include "GemStone.h"

GemStone::GemStone(IGameState* parent, Player* player) :Entity(parent) {
	int handle = LoadGraph("Data/GemStone2.png");
	auto trans = std::make_shared<TransformComponent>(this, 0, 0);
	AddComponent(trans);
	auto sprite = std::make_shared<SpriteRendererComponent>(this, handle, RenderLayer::BackgroundMid,
		trans.get(), 0.2f);
	AddComponent(sprite);

	player_trans = player->GetComponent<TransformComponent>().get();
	sprite->player_trans = player_trans;

}

void GemStone::Update(const InputState* input, float deltaTime) {
	Entity::Update(input, deltaTime);
}

void GemStone::Draw() {
	Entity::Draw();
}