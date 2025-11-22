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
	Player(PlayerData data, IGameState* panent);
	~Player() = default;

	void Update(const InputState* input, float deltaTime)override;
	void Draw()override;

	//X, Y, VX, VYを取得する関数
	float GetX() {
		return m_Trans ? m_Trans->x : 0;
	}

	float GetY() {
		return m_Trans ? m_Trans->y : 0;
	}

	float GetVX() {
		return m_Trans ? m_Trans->vx : 0;
	}

	float GetVY() {
		return m_Trans ? m_Trans->vy : 0;
	}

	//W, Hを取得する関数
	float GetW() {
		return w * scale;
	}

	float GetH() {
		return h * scale;
	}

	//X, Y, VX, VYを設定する関数
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

	void SetVX(float vx) {
		if (m_Trans) {
			m_Trans->vx = vx;
		}
	}

	void SetVY(float vy) {
		if (m_Trans) {
			m_Trans->vy = vy;
		}
	}


	PlayerData SavePlayerData();
private:
	int mp;
	int w;
	int h;
	float scale;

	void LoadPlayerData(PlayerData data);
	std::shared_ptr<TransformComponent> m_Trans;
};