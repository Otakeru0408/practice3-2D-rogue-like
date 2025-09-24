#pragma once
#include "Component.h"
#include "TransformComponent.h"

class InputComponent : public Component {
private:
	std::shared_ptr<TransformComponent> transform;
	float moveSpeed;

public:
	InputComponent(Entity* _parent, float speed)
		:Component(_parent), moveSpeed(speed) {
		transform = owner->GetComponent<TransformComponent>();
	}
	void Update(const InputState* input, float deltaTime) override {
		transform->vx = 0;
		transform->vy = 0;

		if (input->IsMouseStay(0)) {
			//キャラの位置とマウスの位置を計算
			VECTOR tPos = VGet(GameData::windowWidth / 2, GameData::windowHeight / 2, 0);
			VECTOR mPos = VGet(input->mouseX, input->mouseY, 0);
			VECTOR moveVec = VNorm(VSub(mPos, tPos));
			//それぞれのベクトルから移動量を計算
			transform->vx = moveVec.x * deltaTime * moveSpeed;
			transform->vy = moveVec.y * deltaTime * moveSpeed;
		}
	}
};