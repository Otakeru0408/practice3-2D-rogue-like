#include "ExitDoor.h"

ExitDoor::ExitDoor(IGameState* parent, Player* m_player)
	:Entity(parent)
{
	//ドアの画像をロード
	int handle = LoadGraph("Data/ExitDoor.png");

	auto trans = std::make_shared<TransformComponent>(this, GameData::windowWidth, GameData::windowHeight);
	AddComponent(trans);
	AddComponent(std::make_shared<DirectionComponent>(this));
	auto spriteRenderer = std::make_shared<SpriteRendererComponent>(this, handle, RenderLayer::BackgroundMid,
		trans.get(), 0.2f, GetColor(255, 0, 0));
	AddComponent(spriteRenderer);
	spriteRenderer->player_trans = m_player->GetComponent<TransformComponent>().get();

	//コライダーを設定する
	//TransformとSpriteRendererを割り当てる
	auto corridor = std::make_shared<CorridorComponent>(this, spriteRenderer->GetImageSize());
	corridor->player_trans = m_player->GetComponent<TransformComponent>().get();
	std::tie(corridor->player_width, corridor->player_height)
		= m_player->GetComponent<AnimationComponent>()->GetImageSize();
	AddComponent(corridor);

	corridor->OnHitEnter = [parent]() {
		parent->LoadNextScene();
		};
}

void ExitDoor::Update(const InputState* input, float deltaTime) {
	Entity::Update(input, deltaTime);
}

void ExitDoor::Draw() {
	Entity::Draw();
}

std::tuple<float, float, float, float>  ExitDoor::GetDoorXYWH() {
	TransformComponent* trans = GetComponent<TransformComponent>().get();
	SpriteRendererComponent* sprite = GetComponent<SpriteRendererComponent>().get();
	auto [w, h] = sprite->GetImageSize();
	return std::make_tuple(trans->x, trans->y, w, h);
}