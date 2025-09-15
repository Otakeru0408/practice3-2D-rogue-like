#pragma once
#include "Component.h"
#include <memory>

class HealthComponent : public Component {
private:
	int currentHP;
	int maxHP;

public:
	// コンストラクタで最大HPを設定
	HealthComponent(Entity* _parent, int max) :Component(_parent), maxHP(max), currentHP(max) {}

	// ダメージを受ける
	void TakeDamage(int damage) {
		currentHP -= damage;
		if (currentHP <= 0) {
			currentHP = 0;
			// 死亡イベントを通知する処理
			// 例: owner->OnDeath();
		}
	}

	// 回復する
	void Heal(int amount) {
		currentHP += amount;
		if (currentHP > maxHP) {
			currentHP = maxHP;
		}
	}

	// HPがゼロかどうか
	bool IsDead() const {
		return currentHP <= 0;
	}

	int GetCurrentHP() const {
		return currentHP;
	}
	void SetCurrentHP(int hp) {
		currentHP = hp;
	}
	int GetMaxHP()const {
		return maxHP;
	}
};