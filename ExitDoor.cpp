#include "ExitDoor.h"

ExitDoor::ExitDoor(IGameState* parent, Player* m_player)
	:Entity(parent)
{
	int handle = LoadGraph("Data/ExitDoor.png");

	auto trans = std::make_shared<TransformComponent>(this, GameData::windowWidth, GameData::windowHeight);
	AddComponent(trans);
	AddComponent(std::make_shared<DirectionComponent>(this));
	auto spriteRenderer = std::make_shared<SpriteRendererComponent>(this, handle, RenderLayer::BackgroundMid,
		trans.get(), 0.2f);
	AddComponent(spriteRenderer);
	spriteRenderer->player_trans = m_player->GetComponent<TransformComponent>().get();
}

void ExitDoor::Update(const InputState* input, float deltaTime) {
	Entity::Update(input, deltaTime);
}

void ExitDoor::Draw() {
	Entity::Draw();
}