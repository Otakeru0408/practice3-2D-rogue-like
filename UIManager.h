#pragma once
#include "IUIElement.h"

#include <vector>
#include <memory>

class UIManager {
private:
	std::vector<std::shared_ptr<IUIElement>> m_elements;
public:
	void AddElement(const std::shared_ptr<IUIElement>& element) {
		m_elements.push_back(element);
	}

	void Update() {
		for (auto& e : m_elements) {
			e->Update();
		}
	}

	void Draw() {
		for (auto& e : m_elements) {
			e->Draw();
		}
	}

	void Clear() {
		m_elements.clear();
	}
};
