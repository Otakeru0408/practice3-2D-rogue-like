#pragma once
#include "Component.h"

class TransformComponent : public Component {
public:
	float x, y;
	float vx, vy;

	TransformComponent(Entity* o, float startX, float startY)
		: Component(o), x(startX), y(startY), vx(0), vy(0) {
	}

	void Update(const InputState* input, float deltaTime) override {
		x += vx;
		y += vy;
	}
};
