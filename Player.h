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

	PlayerData SavePlayerData();
private:
	int mp;

	void LoadPlayerData(PlayerData data);
};