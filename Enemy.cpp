#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(IGameState* parent, Player* player, int id)
	:Entity(parent), imageScale(0.2f), moveSpeed(1.0f), myId(id)
{
	auto health = std::make_shared<HealthComponent>(this, 100);
	AddComponent(health);

	auto trans = std::make_shared<TransformComponent>(this, GameData::windowWidth / 2, GameData::windowHeight / 2);
	AddComponent(trans);
	my_trans = trans.get();

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
	player_trans = player->GetComponent<TransformComponent>().get();
	anim->player_trans = player_trans;
}

void Enemy::Update(const InputState* input, float deltaTime) {
	Entity::Update(input, deltaTime);

	//TODO : 移動処理をコンポーネントにするか検討
	float diff_x = player_trans->x - my_trans->x;
	float diff_y = player_trans->y - my_trans->y;

	float length = sqrt(diff_x * diff_x + diff_y * diff_y);
	my_trans->vx = (diff_x / length) * moveSpeed;
	my_trans->vy = (diff_y / length) * moveSpeed;
}

void Enemy::Draw() {
	Entity::Draw();
}

