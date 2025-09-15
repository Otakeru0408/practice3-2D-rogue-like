#pragma once
#include "Component.h"
#include "TransformComponent.h"

enum class Direction { Down, Left, Right, Up };

class DirectionComponent : public Component {
private:
	std::shared_ptr<TransformComponent> transform;
public:
	Direction dir;

	DirectionComponent(Entity* e)
		:Component(e), dir(Direction::Down) {
		transform = owner->GetComponent<TransformComponent>();
	}

	void Update(const InputState* input, float deltaTime) override {
		if (!transform) return;

		if (fabs(transform->vx) > fabs(transform->vy)) {
			dir = (transform->vx > 0) ? Direction::Right : Direction::Left;
		}
		else if (fabs(transform->vy) > 0) {
			dir = (transform->vy > 0) ? Direction::Down : Direction::Up;
		}
	}

};
