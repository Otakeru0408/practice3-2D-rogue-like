#include "GemStone.h"

GemStone::GemStone(InGameState* parent, Player* player, int image_handle, int id)
	:Entity(parent), imageScale(0.2f), myId(id) {
	auto trans = std::make_shared<TransformComponent>(this, 0, 0);
	AddComponent(trans);
	auto sprite = std::make_shared<SpriteRendererComponent>(this, image_handle, RenderLayer::BackgroundMid,
		trans.get(), imageScale);
	AddComponent(sprite);

	player_trans = player->GetComponent<TransformComponent>().get();
	sprite->player_trans = player_trans;

	//コライダーを設定する
	//TransformとSpriteRendererを割り当てる
	auto corridor = std::make_shared<CorridorComponent>(this, sprite->GetImageSize());
	corridor->player_trans = player->GetComponent<TransformComponent>().get();
	std::tie(corridor->player_width, corridor->player_height)
		= player->GetComponent<AnimationComponent>()->GetImageSize();
	AddComponent(corridor);

	corridor->OnHitEnter = [parent, this]() {
		parent->OnGemStoneHit(myId);
		};
}

void GemStone::Update(const InputState* input, float deltaTime) {
	Entity::Update(input, deltaTime);
}

void GemStone::Draw() {
	Entity::Draw();
}