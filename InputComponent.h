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
		if (input->IsKeyStay(KEY_INPUT_A)) {
			transform->vx -= moveSpeed * deltaTime;
		}
		if (input->IsKeyStay(KEY_INPUT_D)) {
			transform->vx += moveSpeed * deltaTime;
		}
		if (input->IsKeyStay(KEY_INPUT_W)) {
			transform->vy -= moveSpeed * deltaTime;
		}
		if (input->IsKeyStay(KEY_INPUT_S)) {
			transform->vy += moveSpeed * deltaTime;
		}
	}
};