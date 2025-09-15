#pragma once
#include "InputState.h"
#include "Entity.h"

class Entity;

class Component {
protected:
	Entity* owner;
	bool isActive;
public:
	Component(Entity* o) : owner(o) {}
	virtual ~Component() {}

	virtual void Update(const InputState* input, float deltaTime) {}
	virtual void Draw() {}

	bool GetActive() { return isActive; }
	void SetActive(bool b) { isActive = b; }
};
