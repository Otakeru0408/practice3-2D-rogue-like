#pragma once
#include <vector>
#include <memory>

#include "DxLib.h"
#include "Component.h"
#include "IGameState.h"
#include "InputState.h"
#include "GameData.h"

class Entity {
private:
	std::vector<std::shared_ptr<Component>> components;
	IGameState* parentScene;
	bool alive;

public:
	Entity(IGameState* parent) : alive(true), parentScene(parent) {}

	virtual void Update(const InputState* input, float deltaTime) {
		for (const auto& comp : components) {
			if (comp->GetActive())
				comp->Update(input, deltaTime);
		}
	}

	virtual void Draw() {
		for (const auto& comp : components) {
			if (comp->GetActive())
				comp->Draw();
		}
	}

	void AddComponent(std::shared_ptr<Component> component) {
		components.emplace_back(component);
	}

	template <typename T>
	std::shared_ptr<T> GetComponent() {
		for (const auto& comp : components) {
			std::shared_ptr<T> typedComp = std::dynamic_pointer_cast<T>(comp);
			if (typedComp) {
				return typedComp;
			}
		}

		return nullptr;
	}

	template <typename S>
	std::vector<std::shared_ptr<S>> GetComponents() {
		std::vector<std::shared_ptr<S>> comps;
		for (const auto& comp : components) {
			std::shared_ptr<S> typedComp = std::dynamic_pointer_cast<S>(comp);
			if (typedComp) {
				comps.emplace_back(typedComp);
			}
		}
		return comps;
	}

	template<typename T>
	void RemoveComponent(std::shared_ptr<T> component) {
		components.erase(
			std::remove_if(components.begin(), components.end(),
				[&](const std::shared_ptr<Component>& c) {
					return c == component;
				}),
			components.end()
		);
	}

	bool IsAlive() const { return alive; }
	void Kill() { alive = false; }
};
