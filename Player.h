#pragma once
#include "Entity.h"
#include "PlayerData.h"
#include "HealthComponent.h"
#include "TransformComponent.h"
#include "DirectionComponent.h"
#include "AnimationComponent.h"
#include "InputComponent.h"

/*必要なコンポーネント
・TransformComponent
・SpriteComponent
・AnimateComponent
*/

class Player :public Entity {
public:
	Player(PlayerData data);
	~Player() = default;

	void Update(const InputState* input, float deltaTime)override;
	void Draw()override;

	float GetX() {
		std::shared_ptr<TransformComponent> trans = GetComponent<TransformComponent>();
		return trans ? trans->x : 0;
	}

	float GetY() {
		std::shared_ptr<TransformComponent> trans = GetComponent<TransformComponent>();
		return trans ? trans->y : 0;
	}

	PlayerData SavePlayerData();
private:
	int mp;

	void LoadPlayerData(PlayerData data);
};