#include "Player.h"

Player::Player(PlayerData data)
	:mp(0)
{
	LoadPlayerData(data);
	AddComponent(std::make_shared<DirectionComponent>(this));
	auto anim = std::make_shared<AnimationComponent>(this);

	//アニメーション画像をInputする
	{
		std::vector<int> animVec;
		//UP
		animVec = std::vector<int>();
		animVec.emplace_back(LoadGraph("Data/Mowarm-up1.png"));
		animVec.emplace_back(LoadGraph("Data/Mowarm-up2.png"));
		anim->AddAnimation(Direction::Up, animVec);
		//DOWN
		animVec = std::vector<int>();
		animVec.emplace_back(LoadGraph("Data/Mowarm-down1.png"));
		animVec.emplace_back(LoadGraph("Data/Mowarm-down2.png"));
		anim->AddAnimation(Direction::Down, animVec);
		//RIGHT
		animVec = std::vector<int>();
		animVec.emplace_back(LoadGraph("Data/Mowarm-right1.png"));
		animVec.emplace_back(LoadGraph("Data/Mowarm-right2.png"));
		anim->AddAnimation(Direction::Right, animVec);
		//LEFT
		animVec = std::vector<int>();
		animVec.emplace_back(LoadGraph("Data/Mowarm-left1.png"));
		animVec.emplace_back(LoadGraph("Data/Mowarm-left2.png"));
		anim->AddAnimation(Direction::Left, animVec);
	}
	AddComponent(anim);

	AddComponent(std::make_shared<InputComponent>(this, 100));
}


void Player::Update(const InputState* input, float deltaTime) {
	Entity::Update(input, deltaTime);
}

void Player::Draw() {
	SetFontSize(20);
	PlayerData data = SavePlayerData();
	/*DrawFormatString(10, 500, GetColor(0, 0, 0),
		"hp:%d,maxHp;%d,mp:%d,posX:%.2f,posY:%.2f",
		data.hp, data.maxHp, data.mp, data.posX, data.posY);*/
	Entity::Draw();
}

PlayerData Player::SavePlayerData() {
	//現在のComponentから必要な情報をまとめてPlayerDataを作成する
	PlayerData data;
	data.hp = GetComponent<HealthComponent>()->GetCurrentHP();
	data.maxHp = GetComponent<HealthComponent>()->GetMaxHP();
	data.mp = mp;
	data.posX = GetComponent<TransformComponent>()->x;
	data.posY = GetComponent<TransformComponent>()->y;

	return data;
}

void Player::LoadPlayerData(PlayerData data) {
	//PlayerDataからPlayerのComponentを作成する
	auto health = std::make_shared<HealthComponent>(this, data.maxHp);
	health->SetCurrentHP(data.hp);
	AddComponent(health);

	auto trans = std::make_shared<TransformComponent>(this, GameData::windowWidth / 2, GameData::windowHeight / 2);
	AddComponent(trans);

	mp = data.mp;
}