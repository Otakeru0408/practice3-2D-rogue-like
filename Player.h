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
		return m_Trans ? m_Trans->x : 0;
	}

	float GetY() {
		return m_Trans ? m_Trans->y : 0;
	}

	void SetX(float x) {
		if (m_Trans) {
			m_Trans->x = x;
		}
	}

	void SetY(float y) {
		if (m_Trans) {
			m_Trans->y = y;
		}
	}

	PlayerData SavePlayerData();
private:
	int mp;

	void LoadPlayerData(PlayerData data);
	std::shared_ptr<TransformComponent> m_Trans;
};