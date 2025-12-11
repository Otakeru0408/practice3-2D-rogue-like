#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(IGameState* parent, Player* player)
	:Entity(parent), imageScale(0.2f)
{
	auto health = std::make_shared<HealthComponent>(this, 100);
	AddComponent(health);

	auto trans = std::make_shared<TransformComponent>(this, GameData::windowWidth / 2, GameData::windowHeight / 2);
	AddComponent(trans);

	AddComponent(std::make_shared<DirectionComponent>(this));
	auto anim = std::make_shared<AnimationComponent>(this, imageScale);

	//アニメーション画像をInputする
	{
		std::vector<int> animVec;
		//UP
		animVec = std::vector<int>();
		animVec.emplace_back(LoadGraph("Data/Rogue-Enemy1.png"));
		animVec.emplace_back(LoadGraph("Data/Rogue-Enemy2.png"));
		anim->AddAnimation(Direction::Up, animVec);
		//DOWN
		animVec = std::vector<int>();
		animVec.emplace_back(LoadGraph("Data/Rogue-Enemy1.png"));
		animVec.emplace_back(LoadGraph("Data/Rogue-Enemy2.png"));
		anim->AddAnimation(Direction::Down, animVec);
		//RIGHT
		animVec = std::vector<int>();
		animVec.emplace_back(LoadGraph("Data/Rogue-Enemy1.png"));
		animVec.emplace_back(LoadGraph("Data/Rogue-Enemy2.png"));
		anim->AddAnimation(Direction::Right, animVec);
		//LEFT
		animVec = std::vector<int>();
		animVec.emplace_back(LoadGraph("Data/Rogue-Enemy1.png"));
		animVec.emplace_back(LoadGraph("Data/Rogue-Enemy2.png"));
		anim->AddAnimation(Direction::Left, animVec);

		//GetGraphSize(animVec[0], &w, &h);
	}
	AddComponent(anim);
	anim->SetAnimationMode(1);	//1を渡すと常に動き続ける
	anim->player_trans = player->GetComponent<TransformComponent>().get();
}

void Enemy::Update(const InputState* input, float deltaTime) {
	Entity::Update(input, deltaTime);
}

void Enemy::Draw() {
	Entity::Draw();
}

